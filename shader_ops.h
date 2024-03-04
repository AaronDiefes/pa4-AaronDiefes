#ifndef SHADER_H
#define SHADER_H
#include "include/GShader.h"
#include "include/GPaint.h"
#include "include/GPixel.h"
#include "include/GBitmap.h" 
#include "include/GMatrix.h"
#include "include/GPoint.h"
#include "Edge.h"
#include <iostream>
#include "blend_functions.h"
#include "include/GColor.h"
#include "my_utils.h"

class MyBMShader:public GShader{
  public: 
    MyBMShader(const GBitmap& device, const GMatrix& mat) : fDevice(device), fMat(mat) {}

    bool isOpaque() override{
        return fDevice.isOpaque();
    }
    
    bool setContext(const GMatrix& ctm) override{
        GMatrix temp = ctm * fMat;

        if(auto inverted = temp.invert()){
          inv = *inverted;
          return true;
        }
        return false;
    }

    void shadeRow(int x, int y, int count, GPixel row[]) override{
      float x_prime = inv[0] * (x + 0.5f) + inv[2] * (y + 0.5f) + inv[4]; //x' = ax + cy + e
      float y_prime = inv[1] * (x + 0.5f) + inv[3] * (y + 0.5f) + inv[5]; //y' = bx + dy + f
      
      if(inv[1] == 0){ // <== if(b == 0), occurs when no rotation in inv
          float x = x_prime;
          int yCurr = clamp_y(GFloorToInt(y_prime));
          for(int i = 0; i < count; i++){
            int xCurr = GFloorToInt(x);
            xCurr = clamp_x(xCurr);

            row[i] = *(fDevice.getAddr(xCurr, yCurr));
            x += inv[0];
          }
      }

      else{
        for(int i = 0; i < count; i++){

          int xCurr = GFloorToInt(x_prime);
          int yCurr = GFloorToInt(y_prime);
          xCurr = clamp_x(xCurr);
          yCurr = clamp_y(yCurr);


          row[i] = *(fDevice.getAddr(xCurr, yCurr));
          
          x_prime += inv[0];
          y_prime += inv[1];
        }
      }
    }                
  

    int clamp_x(int x){
      if(x < 0){
        return 0;
      }
      if(x >= fDevice.width()){
        return fDevice.width() - 1;
      }
      return x;
    }

    int clamp_y(int y){
      if(y < 0){
        return 0;
      }
      if(y >= fDevice.height()){
        return fDevice.height() - 1;
      }
      return y;
    }
  private:
    const GBitmap fDevice;
    const GMatrix fMat;
    GMatrix inv;
};

class MyGradientShader:public GShader{
  public:
    MyGradientShader(GPoint p0, GPoint p1, const GColor colors[], int count) : p0(p0), p1(p1), count(count) {
      for (int i = 0; i < count ; i++){
        gradient_colors.push_back(colors[i]);
      }
    }

    bool isOpaque() override{

    }

    bool setContext(const GMatrix& ctm) override{
      GMatrix linear_transformation_matrix = GMatrix(
        p1.x - p0.x,    -(p1.y - p0.y),   p0.x,
        p1.y - p0.y,    p1.x - p0.x,      p0.y
      );
      
      GMatrix temp = ctm * linear_transformation_matrix;
      if(auto inverted = temp.invert()){
          inv = *inverted;
          return true;
      }
      return false;
    }

    void shadeRow(int x, int y, int c, GPixel row[]) override{

      float x_prime = (inv[0] * (x + 0.5f) + inv[2] * (y + 0.5f) + inv[4]) * (count - 1); //x' = ax + cy + e

      for(int i = 0; i < c; i++){
        // int xCurr = GFloorToInt(x_prime);
        // xCurr = xCurr.MyBMShader::clamp_x(xCurr);
        
        // float x_prime = (inv[0] * (x + 0.5f + i) + inv[2] * (y + 0.5f) + inv[4]) * (count - 1); //x' = ax + cy + e

        float currX = x_prime;
        if(currX < 0){
          currX = 0;
        }
        if(currX > count - 1){
          currX = count - 1;
        }
        int k = floor(currX);
        float d0 = currX - (float)k;
        assert(0 <= d0 <= 1);
        // float t = d0 * (count - 1);
        // if(k == count - 1){
        //   gradient_color = (1-d0)*gradient_colors[k];
        // }
       
        GColor gradient_color = (1-d0)*gradient_colors[k] + ((d0) * gradient_colors[k + 1]);

        
        row[i] = unpremult(gradient_color);
        //GPixel_PackARGB(unpremult(gradient_color.a), GRoundToInt(255*gradient_color.r*gradient_color.a), GRoundToInt(255*gradient_color.g*gradient_color.a), GRoundToInt(255*gradient_color.b*gradient_color.a));

        x_prime += inv[0]*(count - 1);

      }
      
    }

  private:
    GPoint p0;
    GPoint p1;
    int count;  
    std::vector<GColor> gradient_colors; 
    GMatrix inv;
};

std::unique_ptr<GShader> GCreateBitmapShader(const GBitmap& fDevice, const GMatrix& fMat){
        return std::unique_ptr<GShader>(new MyBMShader(fDevice, fMat));
}

std::unique_ptr<GShader> GCreateLinearGradient(GPoint p0, GPoint p1, const GColor colors[], int count){
        return std::unique_ptr<GShader>(new MyGradientShader(p0, p1, colors, count));
}



void shadeFillPolygon(GShader* sh, std::vector<Edge> edges, GBitmap fDevice, std::vector<GMatrix> ctm, BlendProc proc){
    int bot = edges[0].bottom;
    int t = edges[edges.size() - 1].top;

    Edge a = edges[edges.size() - 1];
    Edge b = edges[edges.size() - 2];

    //CHECK if edges is empty, if it is, don't draw
    for(int i = t; i <= bot; i++){
        
        //find left and right
        int aIntersect = GRoundToInt(a.eval(i));
        int bIntersect = GRoundToInt(b.eval(i));

        if(aIntersect > bIntersect){
            swap(aIntersect, bIntersect);
        }
        
        int pixels_in_row = bIntersect - aIntersect;
        
        GPixel row[pixels_in_row];
        if(aIntersect < fDevice.width()){
          sh->shadeRow(aIntersect, i, pixels_in_row, row);
          shadeBlendRow(aIntersect, i, pixels_in_row, proc, fDevice, row);
        }
         
        if(b.lastRow(i)){
            edges.pop_back();
            if(edges.empty()){
                break;
            } 
            if(edges.size() == 1){
                b = edges[edges.size()-1];
            }  
            else{
                b = edges[edges.size()-2];
            }         

        }
        if (a.lastRow(i)){
            edges.pop_back();
            if(edges.empty()){
                break;
            }
            a = edges[edges.size()-2];

        }
    }
}
#endif


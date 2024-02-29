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





class MyShader:public GShader{
  public: 
    MyShader(const GBitmap& device, const GMatrix& mat) : fDevice(device), fMat(mat) {}

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

std::unique_ptr<GShader> GCreateBitmapShader(const GBitmap& fDevice, const GMatrix& fMat){
        return std::unique_ptr<GShader>(new MyShader(fDevice, fMat));
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
        
        // GPixel* dst = fDevice.getAddr(aIntersect, i);

        // //if dst.a == 1
        // if (GPixel_GetA(*dst) == 255){
        //     if(proc == dst_over_mode){proc = clear_mode;}
        //     if(proc == src_out_mode){proc = clear_mode;}
        //     if(proc == dst_atop_mode){proc = dst_in_mode;}
        //     if(proc == xor_mode){proc = dst_out_mode;}
        // }
        // //if dst.a == 0
        // if(GPixel_GetA(*dst) == 0){
        //     if(proc == src_in_mode){proc = clear_mode;}
        //     if(proc == src_atop_mode){proc = dst_out_mode;}
        // }

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
            // std::cout<<"a bottom: "<<a.bottom<<std::endl;
            edges.pop_back();
            if(edges.empty()){
                break;
            }
            a = edges[edges.size()-2];

        }
    }
}
#endif


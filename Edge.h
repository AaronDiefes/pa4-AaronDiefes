#ifndef EDGE_H
#define EDGE_H

#include "include/GPoint.h"
#include <algorithm> 
#include <vector>
#include <utility>
#include "include/GBitmap.h"
#include <iostream>

class Edge {
public:
    int top, bottom;
    float m, x;
    int dir;
    float left_x;

//w-1 + w
    //(1-w)c1 + w*c2
    //w = p0.y > p1.y
    //c1 = -1, c2 = 1
    Edge(GPoint p0, GPoint p1, GBitmap fDevice) { 
       int w = p0.y > p1.y;
       dir = 2*w - 1;
       
       if (w) {
            std::swap(p0, p1);
        }
        top = GRoundToInt(p0.y);
        bottom = GRoundToInt(p1.y);
        left_x = p0.x;
        m = calculateSlope(p0, p1);

        // if (top == bottom) {
        //     return;
        // }

        float b = calculateB(p0, p1);
        x = b;
        // std::cout<<" x: "<<x<<std::endl; 

    }
   

    // Static member function to access the vector of edges
    static std::vector<Edge>& getEdges() {
        static std::vector<Edge> edges;
        return edges;
    }

    static void clip(GPoint p0, GPoint p1, GBitmap fDevice, std::vector<Edge> &edges){
        
        if (p0.y > p1.y) {
            std::swap(p0, p1);
        }
        // std::cout<<"Before: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;

        GPoint new_p0 = p0;
        GPoint new_p1 = p1;

        // Line segment is completely out of bounds
        if (p1.y < 0 || p0.y >= fDevice.height()) {
            return;
        }

        // Line segment top point is out of bounds
        if (p0.y < 0){
            new_p0 = {getTopX(p0, p1), 0};
        }
        
        // Line segment bottom point is out of bounds
        if (p1.y > fDevice.height()){
            new_p1 = {getBottomX(p0, p1, fDevice), float(fDevice.height())};
        }

        //reassign p0 and p1 with new x values
        p0 = new_p0;
        p1 = new_p1;
        
        int left = 0;
        int right = fDevice.width();
        
        if (p0.x > p1.x) {
            std::swap(p0, p1);
        }
        // std::cout<<"Before: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
        
        //IF edge is completely horizontally contained
        if(p0.x >= left && p1.x < right){
            Edge inEdge = Edge(p0, p1, fDevice);
            // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
            if(inEdge.top < inEdge.bottom){
                edges.push_back(inEdge);
            }
            return;
        }
        
        //line segment completely to the left 
        if (p1.x < left) {
            GPoint pp0 = {0, p0.y};
            GPoint pp1 = {0, p1.y};
            Edge leftEdge = Edge(pp0, pp1, fDevice);
            // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
            if(leftEdge.top < leftEdge.bottom){
                edges.push_back(leftEdge);
            }
            return;
        }

        //line segment completely to the right 
        if (p0.x >= right ) {
            GPoint pp0 = {fDevice.width(), p0.y};
            GPoint pp1 = {fDevice.width(), p1.y};

            Edge rightEdge = Edge(pp0, pp1, fDevice);
            // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
            if(rightEdge.top < rightEdge.bottom){
                edges.push_back(rightEdge);
            }
            return;
        }

        
        //line segment partially to the left 
        if (p0.x < left) {
            GPoint lc;                
            lc.x = 0;
            lc.y = getLeftY(p0, p1);
            GPoint clipped_p0 = {0, p0.y};
                
            Edge leftEdge1 = Edge(lc, clipped_p0, fDevice);
            // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
            if(leftEdge1.top < leftEdge1.bottom){
                edges.push_back(leftEdge1);
            }
                            
            if (p1.x >= right) {
                GPoint rc;
                rc.x = fDevice.width();
                rc.y = getRightY(p0, p1, fDevice);
                GPoint clipped_p1 = {fDevice.width() -1, p1.y};

                Edge rightEdge1 = Edge(rc, clipped_p1, fDevice);
                if(rightEdge1.top < rightEdge1.bottom){
                    edges.push_back(rightEdge1);
                }
                // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
                    


                //ADD EDGE between bounds
                
                Edge inEdge1 = Edge(lc, rc, fDevice);
                // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
                if(inEdge1.top < inEdge1.bottom){
                    edges.push_back(inEdge1);
                }                
                return;
                    
                // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
                //TODO: Delete duplicate check if causing problems
            }
            Edge inEdge2 = Edge(lc, p1, fDevice);
            if(inEdge2.top < inEdge2.bottom){
                edges.push_back(inEdge2);
            }
            return;
        }

        //line segment partially to the right 
        if (p1.x >= right) {
            GPoint rc;
            rc.x = fDevice.width();
            rc.y = getRightY(p0, p1, fDevice);
            GPoint clipped_p1 = {fDevice.width(), p1.y};

    
            Edge rightEdge2 = Edge(rc, clipped_p1, fDevice);
            if(rightEdge2.top < rightEdge2.bottom){
                edges.push_back(rightEdge2);
            }
            // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
                


            Edge inEdge3 = Edge(p0, rc, fDevice);
            // std::cout<<"After: p0.x: "<<p0.x<<" p0.y: "<<p0.y<<" p1.x: "<<p1.x<<" p1.y: "<<p1.y<<std::endl;
            if(inEdge3.top < inEdge3.bottom){
                edges.push_back(inEdge3);
            }          
            return;
        }


        return;
        }
    
    bool lastRow(int y){
        // std::cout<<y<<std::endl;
                
        // std::cout<<"bottom: "<<bottom<<std::endl;

        // assert(y >= top && y < bottom);
        return y == bottom - 1;
    }

    float eval(int y){
        return m * ((float)y + 0.5f) + x;
    }

    bool isValid(int y) const {
        //returns true if y is within our top and bottom, meaning
        //this Y is valid for computing our corresponding X.
        return top <= y && y <= bottom;
    }



private:
    static float getTopX(GPoint p0, GPoint p1){
        float ret = p0.x + ((p1.x - p0.x) * (0 - p0.y)) / (p1.y - p0.y);
        return ret;
    }

    static float getBottomX(GPoint p0, GPoint p1, GBitmap fDevice){
        float ret = p0.x + ((p1.x - p0.x) * (fDevice.height() - p0.y)) / (p1.y - p0.y);
        return ret;
    }
    
    static float getLeftY(GPoint p0, GPoint p1){
        float ret = p0.y + ((p1.y - p0.y) * (0 - p0.x)) / (p1.x - p0.x);
        return ret;
    }

    static float getRightY(GPoint p0, GPoint p1, GBitmap fDevice){
        float ret = p0.y + ((p1.y - p0.y) * (fDevice.width() - p0.x)) / (p1.x - p0.x);
        return ret;
    }

    float calculateSlope(GPoint p0, GPoint p1) {
        return (p1.x - p0.x) / (p1.y - p0.y);
    }

    float calculateB(GPoint p0, GPoint p1) {
        return p0.x - (m * p0.y);
    }
    
   
};
#endif
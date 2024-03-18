#include "include/GPath.h"
//acts on a GPath
//GPath has rectangle, what are bounds and orientation
//iterate over points
//this->moveTo
void GPath::addRect(const GRect& r, GPath::Direction dir) {
    this->moveTo(r.left, r.top);
    if(dir == kCW_Direction){
        this->lineTo(r.right, r.top);
        this->lineTo(r.right, r.bottom);
        this->lineTo(r.left, r.bottom);
        this->lineTo(r.left, r.top);
    }
    else{
        this->lineTo(r.left, r.bottom);
        this->lineTo(r.right, r.bottom);
        this->lineTo(r.right, r.top);
        this->lineTo(r.left, r.top);
    }
    return;
};
void GPath::addPolygon(const GPoint pts[], int count){
    this->moveTo(pts[0]);
    for(int i = 1; i < count; i++){
        this->lineTo(pts[i]);
    }
    return;
};
GRect GPath::bounds() const{
    if(fPts.size() == 0){
        return GRect{0, 0, 0, 0};
    }
    
    float minX = 0;
    float minY = 0;
    float maxX = fPts[0].x;
    float maxY = fPts[0].y;
    
    for(int i = 0; i < fPts.size(); i++){
        if(fPts[i].x < minX){
            minX = fPts[i].x;
        }
        if(fPts[i].x > maxX){
            maxX = fPts[i].x;
        }
        if(fPts[i].y < minY){
            minY = fPts[i].y;
        }
        if(fPts[i].y > maxY){
            maxY = fPts[i].y;
        }
    }
    return GRect{minX, minY, maxX, maxY};
};
void GPath::transform(const GMatrix& m){
   m.mapPoints(fPts.data(), fPts.data(), fPts.size());
   return;
};


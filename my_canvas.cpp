#include "include/GBitmap.h"
#include "include/GCanvas.h"
#include "include/GColor.h"
#include "include/GMath.h"
#include "include/GPath.h"
#include "include/GPixel.h"
#include "include/GPoint.h"
#include "include/GRandom.h"
#include "include/GRect.h"
#include "include/GTime.h"
#include "include/GTypes.h"
#include "include/GShader.h"
#include "starter_canvas.h"
#include "Edge.h"
#include "my_utils.h"
#include "blend_functions.h"
#include "shader_ops.h"
#include <iostream>
// #include "shader_ops.cpp"



/*
 *  Copyright 2024 <me>
 */


void MyCanvas::clear(const GColor& color) {
    
    GPixel pixel = unpremult(color);
    BlendProc proc = src_mode;
    for(int i = 0; i < fDevice.height(); i++){
        blitRow(0, i, fDevice.width(), proc, fDevice, pixel);
    }
}

void MyCanvas::drawRect(const GRect& rect, const GPaint& paint) {

    GPoint p1 = {rect.left, rect.top};
    GPoint p2 = {rect.right, rect.top};
    GPoint p3 = {rect.right, rect.bottom};
    GPoint p4 = {rect.left, rect.bottom};

    GPoint src[4] = {p1, p2, p3, p4};
    drawConvexPolygon(src, 4, paint);
};

void MyCanvas::drawConvexPolygon(const GPoint points[], int count, const GPaint& paint){
    
    if(count < 3){
        return;
    }
    
    GPoint dst[count];
    GMatrix mat = ctm[ctm.size() - 1];

    mat.mapPoints(dst, points, count);
    
    std::vector <Edge> edges;
    
    //create array of edges
    for(int i = 0; i < count; i++){
        int nextIndex = (i + 1) % count; 
        Edge::clip(dst[i], dst[nextIndex], fDevice, edges);
    }

    if (edges.size() < 2){
        return;
    }

    //sort by top value
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.top > b.top;
    });

    // for(int i = 0; i < edges.size(); i++){
    //     std::cout<<"edge top: "<<edges[i].top<<" edge bottom: "<<edges[i].bottom<<" edge slope: "<<edges[i].m<<" edge x: "<<edges[i].x<<std::endl;
    // }
    if(paint.getShader()){
        GShader* sh = paint.getShader();
        if(sh->setContext(ctm[ctm.size() - 1])){
            GBlendMode mode_type = paint.getBlendMode();
            BlendProc proc = gProcs[(int)mode_type];
            if(sh->isOpaque()){
                if(proc == src_over_mode){proc = src_mode;}
                if(proc == src_over_mode){proc = dst_mode;}
                if(proc == dst_out_mode){proc = clear_mode;}
                if(proc == src_atop_mode){proc = src_in_mode;}
                if(proc == xor_mode){proc = src_out_mode;}
            }
            shadeFillPolygon(sh, edges, fDevice, ctm, proc);
        }
    }
    else{
        GColor color = paint.getColor();
        GBlendMode mode_type = paint.getBlendMode();
        BlendProc proc = gProcs[(int)mode_type];

        if(color.a == 1){
            if(proc == src_over_mode){proc = src_mode;}
            if(proc == src_over_mode){proc = dst_mode;}
            if(proc == dst_out_mode){proc = clear_mode;}
            if(proc == src_atop_mode){proc = src_in_mode;}
            if(proc == xor_mode){proc = src_out_mode;}
        }
        if(color.a == 0){
            if(proc == src_mode){proc = clear_mode;}
            if(proc == src_over_mode){proc = dst_mode;}
            if(proc == dst_over_mode){proc = dst_mode;}
            if(proc == src_in_mode){proc = clear_mode;}
            if(proc == dst_in_mode){proc = clear_mode;}
            if(proc == src_out_mode){proc = clear_mode;}
            // if(proc == dst_out_mode){proc = clear_mode;}
            if(proc == src_atop_mode){proc = dst_mode;}
            // if(proc == dst_atop_mode){proc = clear_mode;}
            if(proc == xor_mode){proc = dst_mode;}
        }
        fillPolygon(edges, color, proc, fDevice);
    }
}



std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap& device) {
    return std::unique_ptr<GCanvas>(new MyCanvas(device));
}

void MyCanvas::save() {
    GMatrix top = ctm[ctm.size() - 1];
    ctm.push_back(top);
}

void MyCanvas::restore() {
    ctm.erase(ctm.end() -1);
}

void MyCanvas::concat(const GMatrix& matrix) {
    GMatrix& top = ctm.back();    
    top = GMatrix::Concat(top, matrix);
}

void MyCanvas::drawPath(const GPath& path, const GPaint&){
    GPath::Edger edger(path);
    //edger.next(put next point in here)
    //make the edges
    tempPoints = path.fPts;
    while( (edger.next(tempPoints)) != GPath::kDone) {
        edges.emplace_back(tempPoints[0], tempPoints[1]);
    }
}


std::string GDrawSomething(GCanvas* canvas, GISize dim) {
    // as fancy as you like
    // ...
    // canvas->clear(...);
    // canvas->fillRect(...);

    return "tears in rain";
}


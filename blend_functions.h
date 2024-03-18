// #include "blend_functions.h"
#ifndef BLEND_H
#define BLEND_H
#include "my_utils.h"
#include <algorithm>

//!<     0
GPixel clear_mode(GPixel src, GPixel dest){
    return GPixel_PackARGB(0, 0, 0, 0); 
}

//!<     S
GPixel src_mode(GPixel src, GPixel dest){ 
    return src; 
}

//!<     D
GPixel dst_mode(GPixel src, GPixel dest){
    return dest; 
}

//!<     S + (1 - Sa)*D
GPixel src_over_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);
            int sr = GPixel_GetR(src);
            int sg = GPixel_GetG(src);
            int sb = GPixel_GetB(src);

            int da = GPixel_GetA(dest);
            int dr = GPixel_GetR(dest);
            int dg = GPixel_GetG(dest);
            int db = GPixel_GetB(dest);

            
            int ba = sa + div255((255-sa)*da);
            int br = sr + div255((255-sa)*dr);
            int bg = sg + div255((255-sa)*dg);
            int bb = sb + div255((255-sa)*db);
            
            return GPixel_PackARGB(ba, br, bg, bb);
        }

// D + (1 - Da)*S
GPixel dst_over_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);
            int sr = GPixel_GetR(src);
            int sg = GPixel_GetG(src);
            int sb = GPixel_GetB(src);

            int da = GPixel_GetA(dest);
            int dr = GPixel_GetR(dest);
            int dg = GPixel_GetG(dest);
            int db = GPixel_GetB(dest);

            
            int ba = da + div255((255-da)*sa);
            int br = dr + div255((255-da)*sr);
            int bg = dg + div255((255-da)*sg);
            int bb = db + div255((255-da)*sb);
            
            return GPixel_PackARGB(ba, br, bg, bb);
        }

// Da * S
GPixel src_in_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);
            int sr = GPixel_GetR(src);
            int sg = GPixel_GetG(src);
            int sb = GPixel_GetB(src);

            int da = GPixel_GetA(dest);

            int ba = div255(da * sa);
            int br = div255(da * sr);
            int bg = div255(da * sg);
            int bb = div255(da * sb);

            return GPixel_PackARGB(ba, br, bg, bb);
        }

// Sa * D
GPixel dst_in_mode(GPixel src, GPixel dest){
            int da = GPixel_GetA(dest);
            int dr = GPixel_GetR(dest);
            int dg = GPixel_GetG(dest);
            int db = GPixel_GetB(dest);

            int sa = GPixel_GetA(src);
            
            int ba = div255(sa * da);
            int br = div255(sa * dr);
            int bg = div255(sa * dg);
            int bb = div255(sa * db);

            return GPixel_PackARGB(ba, br, bg, bb);
        }

// (1 - Da)*S
GPixel src_out_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);
            int sr = GPixel_GetR(src);
            int sg = GPixel_GetG(src);
            int sb = GPixel_GetB(src);

            int da = GPixel_GetA(dest);
            
            int ba = div255((255-da)*sa);
            int br = div255((255-da)*sr);
            int bg = div255((255-da)*sg);
            int bb = div255((255-da)*sb);
            
            return GPixel_PackARGB(ba, br, bg, bb);
        }

// (1 - Sa)*D
GPixel dst_out_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);

            int da = GPixel_GetA(dest);
            int dr = GPixel_GetR(dest);
            int dg = GPixel_GetG(dest);
            int db = GPixel_GetB(dest);

            int ba = div255((255-sa)*da);
            int br = div255((255-sa)*dr);
            int bg = div255((255-sa)*dg);
            int bb = div255((255-sa)*db);
            
            return GPixel_PackARGB(ba, br, bg, bb);
        }

// Da*S + (1 - Sa)*D
GPixel src_atop_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);
            int sr = GPixel_GetR(src);
            int sg = GPixel_GetG(src);
            int sb = GPixel_GetB(src);

            int da = GPixel_GetA(dest);
            int dr = GPixel_GetR(dest);
            int dg = GPixel_GetG(dest);
            int db = GPixel_GetB(dest);

            
            int ba = div255(da*sa + (255-sa)*da);
            int br = div255(da*sr + (255-sa)*dr);
            int bg = div255(da*sg + (255-sa)*dg);
            int bb = div255(da*sb + (255-sa)*db);
            
            return GPixel_PackARGB(ba, br, bg, bb);
        }

// Sa*D + (1 - Da)*S
GPixel dst_atop_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);
            int sr = GPixel_GetR(src);
            int sg = GPixel_GetG(src);
            int sb = GPixel_GetB(src);

            int da = GPixel_GetA(dest);
            int dr = GPixel_GetR(dest);
            int dg = GPixel_GetG(dest);
            int db = GPixel_GetB(dest);

            
            int ba = div255(sa*da + (255-da)*sa);
            int br = div255(sa*dr + (255-da)*sr);
            int bg = div255(sa*dg + (255-da)*sg);
            int bb = div255(sa*db + (255-da)*sb);
            
            return GPixel_PackARGB(ba, br, bg, bb);
        }

//(1 - Sa)*D + (1 - Da)*S
GPixel xor_mode(GPixel src, GPixel dest){
            int sa = GPixel_GetA(src);
            int sr = GPixel_GetR(src);
            int sg = GPixel_GetG(src);
            int sb = GPixel_GetB(src);

            int da = GPixel_GetA(dest);
            int dr = GPixel_GetR(dest);
            int dg = GPixel_GetG(dest);
            int db = GPixel_GetB(dest);
      
            int ba = div255((255-sa)*da) + div255((255-da)*sa);
            int br = div255((255-sa)*dr) + div255((255-da)*sr);
            int bg = div255((255-sa)*dg) + div255((255-da)*sg);
            int bb = div255((255-sa)*db) + div255((255-da)*sb);
            
            return GPixel_PackARGB(ba, br, bg, bb);
        }

typedef GPixel (*BlendProc)(GPixel, GPixel);
extern const BlendProc gProcs[12];

const BlendProc gProcs[] = {
    clear_mode,
    src_mode,
    dst_mode,
    src_over_mode,
    dst_over_mode,
    src_in_mode,
    dst_in_mode,
    src_out_mode,
    dst_out_mode,
    src_atop_mode,
    dst_atop_mode,
    xor_mode
};

template<typename Proc> void blitRow(int x, int y, int n, Proc blend, GBitmap fDevice, GPixel src){
    GPixel* dst = fDevice.getAddr(x, y);
    if(blend == clear_mode){
        for(int i = 0; i < n; i++){
            GPixel p = clear_mode(src, dst[i]); 
            dst[i] = p;
        }
    }
    else if(blend == src_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src; 
            dst[i] = p;
        }
    }
    else if(blend == dst_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_mode(src, dst[i]); 
            dst[i] = p;
        }
    }
    else if(blend == src_over_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_over_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_over_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_over_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == src_in_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_in_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_in_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_in_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == src_out_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_out_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_out_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_out_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == src_atop_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_atop_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_atop_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_atop_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == xor_mode){
        for(int i = 0; i < n; i++){
            GPixel p = xor_mode(src, dst[i]);
            dst[i] = p;
        }
    }
    // else{
    //     for(int i = 0; i < n; i++){
    //         GPixel p = blend(src, dst[i]);
    //         dst[i] = p;
    //     }
    // }
}
template<typename Proc> void shadeBlendRow(int x, int y, int n, Proc blend, GBitmap fDevice, GPixel src[]){
    GPixel* dst = fDevice.getAddr(x, y);
    if(blend == clear_mode){
        for(int i = 0; i < n; i++){
            GPixel p = clear_mode(src[i], dst[i]); 
            dst[i] = p;
        }
    }
    else if(blend == src_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src[i]; 
            dst[i] = p;
        }
    }
    else if(blend == dst_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_mode(src[i], dst[i]); 
            dst[i] = p;
        }
    }
    else if(blend == src_over_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_over_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_over_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_over_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == src_in_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_in_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_in_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_in_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == src_out_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_out_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_out_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_out_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == src_atop_mode){
        for(int i = 0; i < n; i++){
            GPixel p = src_atop_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == dst_atop_mode){
        for(int i = 0; i < n; i++){
            GPixel p = dst_atop_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else if(blend == xor_mode){
        for(int i = 0; i < n; i++){
            GPixel p = xor_mode(src[i], dst[i]);
            dst[i] = p;
        }
    }
    else{
        for(int i = 0; i < n; i++){
            GPixel p = blend(src[i], dst[i]);
            dst[i] = p;
        }
    }
}

void fillPolygon(std::vector<Edge> edges, GColor color, BlendProc proc, GBitmap fDevice){

    GPixel src = unpremult(color);
    int bot = edges[0].bottom;
    int t = edges[edges.size() - 1].top;

    Edge a = edges[edges.size() - 1];
    Edge b = edges[edges.size() - 2];
    
    for(int i = t; i <= bot; i++){
        
        //find left and right
        int aIntersect = GRoundToInt(a.eval(i));
        int bIntersect = GRoundToInt(b.eval(i));

        if(aIntersect > bIntersect){
            swap(aIntersect, bIntersect);
        }
        
        int pixels_in_row = bIntersect - aIntersect;
        //should not need this if statement (also in shaderFill)
        if(aIntersect >= fDevice.width()){
            aIntersect = fDevice.width() - 1;
        }
        

        blitRow(aIntersect, i, pixels_in_row, proc, fDevice, src);

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

        //are we still with second to last edge in edges
        if (a.lastRow(i)){
            // a = edges.back();
            edges.pop_back();
            if(edges.empty()){
                break;
            }
            a = edges[edges.size()-2];

        }
    }
}
BlendProc getBlendMode(BlendProc proc, GColor color){
    BlendProc new_proc;
    if(color.a == 1){
            if(proc == src_over_mode){new_proc = src_mode;}
            if(proc == src_over_mode){new_proc = dst_mode;}
            if(proc == dst_out_mode){new_proc = clear_mode;}
            if(proc == src_atop_mode){new_proc = src_in_mode;}
            if(proc == xor_mode){new_proc = src_out_mode;}
    }
    if(color.a == 0){
        if(proc == src_mode){new_proc = clear_mode;}
        if(proc == src_over_mode){new_proc = dst_mode;}
        if(proc == dst_over_mode){new_proc = dst_mode;}
        if(proc == src_in_mode){new_proc = clear_mode;}
        if(proc == dst_in_mode){new_proc = clear_mode;}
        if(proc == src_out_mode){new_proc = clear_mode;}
        // if(proc == dst_out_mode){new_proc = clear_mode;}
        if(proc == src_atop_mode){new_proc = dst_mode;}
        // if(proc == dst_atop_mode){new_proc = clear_mode;}
        if(proc == xor_mode){new_proc = dst_mode;}
    }
    return new_proc;
}
#endif

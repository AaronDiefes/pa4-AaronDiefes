#include "include/GBitmap.h"
#include "include/GCanvas.h"
#include "include/GColor.h"
#include "include/GMath.h"
#include "include/GPixel.h"
#include "include/GPoint.h"
#include "include/GRandom.h"
#include "include/GRect.h"
#include "include/GTime.h"
#include "include/GTypes.h"
#include "starter_canvas.h"
#include <iostream>
#include "Edge.h"

#ifndef myUtils
#define myUtils


using namespace std;
/*
 *  Copyright 2024 <me>
 */

// PA1
        GIRect intersect(const GRect rect, const GBitmap& device){
            return GIRect::LTRB(
        max(GRoundToInt(rect.left), 0),
        max(GRoundToInt(rect.top), 0),
        min(device.width(), GRoundToInt(rect.right)),
        min(device.height(), GRoundToInt(rect.bottom))
            );
        }
        
        static inline uint8_t div255(unsigned before){
            return (before + 128) * 257 >> 16;
        }

        GPixel unpremult(GColor color){
            int r = GRoundToInt(color.r * color.a * 255);
            int g = GRoundToInt(color.g * color.a * 255);
            int b = GRoundToInt(color.b * color.a * 255);
            int a = GRoundToInt(color.a * 255);
            return GPixel_PackARGB(a, r, g, b);
        }

#endif

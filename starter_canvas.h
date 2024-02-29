/*
 *  Copyright 2024 <me>
 */

#ifndef _g_starter_canvas_h_
#define _g_starter_canvas_h_

#include "include/GCanvas.h"
#include "include/GRect.h"
#include "include/GColor.h"
#include "include/GBitmap.h"

class MyCanvas : public GCanvas {
public:
    MyCanvas(const GBitmap& device) : fDevice(device), ctm({GMatrix()}) {}

    void clear(const GColor& color) override;
    void drawRect(const GRect& rect, const GPaint& paint) override;

    void drawConvexPolygon(const GPoint[], int count, const GPaint&) override;
    
    void save() override;
    void restore() override;
    void concat(const GMatrix&) override;
    void drawPath(const GPath& path, const GPaint&) override;

private:
    // Note: we store a copy of the bitmap
    const GBitmap fDevice;
    std::vector<GMatrix> ctm {};


    // Add whatever other fields you need
};

#endif
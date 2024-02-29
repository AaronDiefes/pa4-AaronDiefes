#ifndef PATH_H
#define PATH_H
#include "include/GPath.h"

class MyPath:public GPath{
    public:
        void addRect(const GRect& r, Direction dir);
        void addPolygon(const GPoint pts[], int count);
        GRect bounds() const;
        void transform(const GMatrix& m);
    private:
};
#endif
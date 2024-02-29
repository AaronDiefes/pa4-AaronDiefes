
#include "include/GMatrix.h"
#include "include/GPoint.h"

// #include <cmath>
#include <iostream>
#include <vector>

//TODO: all helpers are currently also defined in GCanvas

GMatrix::GMatrix(){
    fMat[0] = 1.0f;     fMat[2] = 0.0f;     fMat[4] = 0.0f;
    fMat[1] = 0.0f;     fMat[3] = 1.0f;     fMat[5] = 0.0f;
}



GMatrix GMatrix::Translate(float tx, float ty){
    GMatrix translate_matrix = GMatrix((float)1, (float)0, tx, (float)0, (float)1, ty);
    return translate_matrix;
}

GMatrix GMatrix::Scale(float sx, float sy){
    GMatrix scale_matrix = GMatrix(sx, (float)0, (float)0, (float)0, sy, (float)0);
    return scale_matrix;
}

GMatrix GMatrix::Rotate(float radians){
    GMatrix rotate_matrix = GMatrix(cos(radians), -sin(radians), 0, sin(radians), cos(radians), 0);
    return rotate_matrix;
}

GMatrix GMatrix::Concat(const GMatrix& a, const GMatrix& b){
    return GMatrix(
            (a[0] * b[0]) + (a[2] * b[1]), 
            (a[0] * b[2]) + (a[2] * b[3]),
            (a[0] * b[4]) + (a[2] * b[5]) + a[4], 
            (a[1] * b[0]) + (a[3] * b[1]),
            (a[1] * b[2]) + (a[3] * b[3]), 
            (a[1] * b[4]) + (a[3] * b[5]) + a[5]
    );
}

float determinant(const GMatrix& a) {
    return (a[0] * a[3]) - (a[1] * a[2]);
}

std::optional<GMatrix> GMatrix::invert() const {
    GMatrix curr = (*this);
    float det = determinant(curr);
    if (det == 0) {
        return {};
    }
    
    float invDet = 1.0f / det;

    return GMatrix(
        (curr[3]) * invDet,
        (-curr[2]) * invDet,
        (curr[2] * curr[5] - curr[3] * curr[4]) * invDet,
        (-curr[1]) * invDet,
        (curr[0]) * invDet,
        (curr[4] * curr[1] - curr[0] * curr[5]) * invDet
    );
}

void GMatrix::mapPoints(GPoint dst[], const GPoint src[], int count) const {
    GMatrix curr = (*this);
    for(int i = 0; i < count; ++i) {
        float mapped_x = (curr[0] * src[i].x) + (curr[2] * src[i].y) + curr[4];
        float mapped_y = (curr[1] * src[i].x) + (curr[3] * src[i].y) + curr[5];

        dst[i].x = mapped_x;
        dst[i].y = mapped_y;
    }
}

#pragma once

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "core.h"

using point = Vector::point_t;
 
const double eps = 1e-14;
 
point make_point(double x, double y) {
    point p = { x, y };
    return p;
}
 
void print_point(point p) {
    double x = p.x;
    double y = p.y;
    if (x == 0) {
        x = 0;
    }
    if (y == 0) {
        y = 0;
    }
    // printf("(%g, %g)", x, y);
}
 
double sq(double x) {
    return x * x;
}
 
bool within(double x1, double y1, double x2, double y2, double x, double y) {
    double d1 = sqrt(sq(x2 - x1) + sq(y2 - y1));    // distance between end-points
    double d2 = sqrt(sq(x - x1) + sq(y - y1));      // distance from point to one end
    double d3 = sqrt(sq(x2 - x) + sq(y2 - y));      // distance from point to other end
    double delta = d1 - d2 - d3;
    return fabs(delta) < eps;   // true if delta is less than a small tolerance
}
 
int rxy(double x1, double y1, double x2, double y2, double x, double y, bool segment) {
    if (!segment || within(x1, y1, x2, y2, x, y)) {
        print_point(make_point(x, y));
        return 1;
    } else {
        return 0;
    }
}
 
double fx(double A, double B, double C, double x) {
    return -(A * x + C) / B;
}
 
double fy(double A, double B, double C, double y) {
    return -(B * y + C) / A;
}
 
// Prints the intersection points (if any) of a circle, center 'cp' with radius 'r',
// and either an infinite line containing the points 'p1' and 'p2'
// or a segment drawn between those points.
bool intersects(point p1, point p2, point cp, double r, bool segment) {
    double x0 = cp.x, y0 = cp.y;
    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double A = y2 - y1;
    double B = x1 - x2;
    double C = x2 * y1 - x1 * y2;
    double a = sq(A) + sq(B);
    double b, c, d;
    bool bnz = true;
    int cnt = 0;
 
    if (fabs(B) >= eps) {
        // if B isn't zero or close to it
        b = 2 * (A * C + A * B * y0 - sq(B) * x0);
        c = sq(C) + 2 * B * C * y0 - sq(B) * (sq(r) - sq(x0) - sq(y0));
    } else {
        b = 2 * (B * C + A * B * x0 - sq(A) * y0);
        c = sq(C) + 2 * A * C * x0 - sq(A) * (sq(r) - sq(x0) - sq(y0));
        bnz = false;
    }
    d = sq(b) - 4 * a * c; // discriminant
    if (d < 0) {
        // line & circle don't intersect
        // printf("[]\n");
        return false;
    }
 
    if (d == 0) {
        // line is tangent to circle, so just one intersect at most
        if (bnz) {
            double x = -b / (2 * a);
            double y = fx(A, B, C, x);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
        } else {
            double y = -b / (2 * a);
            double x = fy(A, B, C, y);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
        }
    } else {
        // two intersects at most
        d = sqrt(d);
        if (bnz) {
            double x = (-b + d) / (2 * a);
            double y = fx(A, B, C, x);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
 
            x = (-b - d) / (2 * a);
            y = fx(A, B, C, x);
            cnt += rxy(x1, y1, x2, y2, x, y, segment);
        } else {
            double y = (-b + d) / (2 * a);
            double x = fy(A, B, C, y);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
 
            y = (-b - d) / (2 * a);
            x = fy(A, B, C, y);
            cnt += rxy(x1, y1, x2, y2, x, y, segment);
        }
    }
 
    // if (cnt <= 0) {
    //     printf("[]");
    // }

    return cnt > 0;
}
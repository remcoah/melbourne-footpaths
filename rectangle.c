/*------------------------------------------------------------------------------ 
..Project: assignment 2
  rectangle.c :  
         = the implementation of module rectangle of the project
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "rectangle.h"

/*------------ function definitions ------------------------------------------*/

// returns a rectangle in accordance to arguments
rectangle2d_t makeRectangle(long double top, long double right, long double bottom, long double left) {
    point2d_t bottomLeft = (point2d_t){.x = left, .y = bottom};
    point2d_t topRight = (point2d_t){.x = right, .y = top};
    return (rectangle2d_t){.bottomLeft = bottomLeft, .topRight = topRight};
}

// determines if 2 points are the same
int pointsAreEqual(point2d_t a, point2d_t b) {
    return (ldoublesAreEqual(a.x, b.x) && ldoublesAreEqual(a.y, b.y));
}

// finds the midpoint of a rectangle
point2d_t findMidPoint(rectangle2d_t r) {
    double midX = (r.bottomLeft.x + r.topRight.x)/2;
    double midY = (r.topRight.y + r.bottomLeft.y)/2;
    return (point2d_t){.x = midX, .y = midY};
}

// tests whether a given 2D point lies within the rectangle and returns 1 (TRUE)
// if it does
int inRectangle(rectangle2d_t r, point2d_t p) {
    int containsY, containsX;

    containsY = ((p.y >= r.bottomLeft.y) && (p.y <= r.topRight.y));
    containsX = ((p.x <= r.topRight.x) && (p.x >= r.bottomLeft.x));
    
    return (containsY && containsX);
}

// tests whether two given rectangles overlap and returns 1 (TRUE) if they do
int rectangleOverlap(rectangle2d_t a, rectangle2d_t b) {
    
    // If rectangle has no area
    if ((a.bottomLeft.x == a.topRight.x || a.bottomLeft.y == a.topRight.y || 
         b.bottomLeft.x == b.topRight.x || b.bottomLeft.y == b.topRight.y)) {
        return 0;
    }

    // If one rectangle is on left side of other
    if (a.bottomLeft.x > b.topRight.x || b.bottomLeft.x > a.topRight.x) return 0;
 
    // If one rectangle is above other
    if (a.bottomLeft.y > b.topRight.y || b.bottomLeft.y > a.topRight.y) return 0;

    return 1;
    
}
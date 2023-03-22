/*------------------------------------------------------------------------------
..Project: assignment 2
  rectangle.h :  
         = the interface of module rectangle of the project
------------------------------------------------------------------------------*/

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "data.h"

// define struct for 2D point
typedef struct point2d {
    long double x, y;
} point2d_t;

// define struct for 2d rectangle
typedef struct rectangle2d {
    point2d_t bottomLeft;
    point2d_t topRight;
} rectangle2d_t;

/*------------ function definitions ------------------------------------------*/

// returns a rectangle in accordance to arguments
rectangle2d_t makeRectangle(long double top, long double right, long double bottom, long double left);

// determines if 2 points are the same
int pointsAreEqual(point2d_t a, point2d_t b);

// finds the midpoint of a rectangle
point2d_t findMidPoint(rectangle2d_t r);

// tests whether a given 2D point lies within the rectangle and returns 1 (TRUE)
// if it does
int inRectangle(rectangle2d_t r, point2d_t p);

// tests whether two given rectangles overlap and returns 1 (TRUE) if they do
int rectangleOverlap(rectangle2d_t a, rectangle2d_t b);

#endif
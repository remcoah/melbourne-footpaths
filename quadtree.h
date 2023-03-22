/*------------------------------------------------------------------------------
..Project: assignment 2
  quadtree.h :  
         = the interface of module pr quadtree of the project
------------------------------------------------------------------------------*/

#ifndef _QUADTREE_H_
#define _QUADTREE_H_
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "array.h"
#include "data.h"
#include "rectangle.h"

#define MAX_ROW_LENGTH 512 // maximum number of characters in a csv row

// numerical values of the different rectangle quadrants
typedef enum quadrants{SW, NW, NE, SE, ROOT}quadrant;
typedef enum nodeTypes{WHITE, GREY, BLACK}nodeType;

// define struct for data point
typedef struct dataPoint {
    array_t *footpaths;
    point2d_t point;
} dataPoint_t;

// define struct for quadtree node
typedef struct quadTreeNode quadTreeNode_t;
struct quadTreeNode {
    dataPoint_t *dataPoint;
    rectangle2d_t rectangle;
    quadTreeNode_t *children[4]; // representing the 4 directions of child nodes
    nodeType typeOfNode;
};

/*------------ function definitions ------------------------------------------*/

// makes empty root quad tree node, given rectangle
quadTreeNode_t* makeQuadTreeNode(rectangle2d_t rectangle);

// frees a quad tree node and all its children
void freeQuadTreeNode(quadTreeNode_t* q);

// inserts data into quadtree
void insertDataToQT(quadTreeNode_t* tree, data_t* data);

// creates & returns an empty data point
dataPoint_t *dataPointCreate(void);

// frees a data point
void freeDataPoint(dataPoint_t* data);

// reads data from input file, returns pointer to the root node
quadTreeNode_t* readInputFrom(char* fileName, quadTreeNode_t* root);

// given a rectangle and point, returns the quadrant of the rectangle that the 
// point lies in
quadrant determineQuadrant(rectangle2d_t r, point2d_t p);

// adds a datapoint given with its 2D coordinates to the quadtree
void addPoint(quadTreeNode_t* root, data_t* data, point2d_t point);

// subdivides a quadtree node
quadTreeNode_t *quadTreeSubDivide(quadTreeNode_t* root);

// tests whether a datapoint given by its 2D coordinate lies within the
// root node and returns the datapoint if it does and NULL otherwise
dataPoint_t* searchPoint(quadTreeNode_t* root, point2d_t point);

// takes a 2D rectangle as argument and returns all datapoints in the PR
// quadtree whose coordinates lie within the query rectangle
void rangeQuery(rectangle2d_t query, quadTreeNode_t* root, array_t* out);

// prints the path to find a given point in a root
// assumes that the point is in the root
void printPath(quadTreeNode_t* root, point2d_t point, FILE* out);

// prints quadrant to out as string
void printQuadrantTo(quadrant quad, FILE* out);


#endif
/*------------------------------------------------------------------------------ 
..Project: assignment 2
  quadtree.c :  
         = the implementation of module pr quadtree of the project
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "rectangle.h"
#include "array.h"
#include "quadtree.h"

/*------------ function definitions ------------------------------------------*/

// makes empty root quad tree node, given rectangle
quadTreeNode_t* makeQuadTreeNode(rectangle2d_t rectangle) {
    quadTreeNode_t *out = malloc(sizeof(*out));
    assert(out);

    out->rectangle = rectangle;
    out->dataPoint = NULL;

    for(int direction = SW; direction <=SE; direction++) {
        out->children[direction] = NULL;
    }

    out->typeOfNode = WHITE;
    return out;
}

// frees a quad tree node and all its children
void freeQuadTreeNode(quadTreeNode_t* q) {
    assert(q);

    // free all children if they exist
    for(int direction = SW; direction <= SE; direction++) {
        if(q->children[direction]) freeQuadTreeNode(q->children[direction]);
    }

    freeDataPoint(q->dataPoint);
    free(q);
    q = NULL;
}

// inserts data into quadtree
void insertDataToQT(quadTreeNode_t* tree, data_t* data) {
    // make start and end data point
    point2d_t start = (point2d_t){.x = data->start_lon, .y = data->start_lat};
    point2d_t end = (point2d_t){.x = data->end_lon, .y = data->end_lat};

    // end will have duplicate of data
    data_t* dataDup = malloc(sizeof(*dataDup));
    assert(dataDup);
    memcpy(dataDup, data, sizeof(*dataDup));
    dataDup->address = strdup(data->address);
    dataDup->clue_sa = strdup(data->clue_sa);
    dataDup->asset_type = strdup(data->asset_type);
    dataDup->segside = strdup(data->segside);

    // insert both points to tree
    addPoint(tree, data, start);
    addPoint(tree, dataDup, end);
}

// creates & returns an empty data point
dataPoint_t *dataPointCreate(void) {
    dataPoint_t *data = malloc(sizeof(*data));
	assert(data);
    data->footpaths = arrayCreate();
    return data;
}

// frees a data point
void freeDataPoint(dataPoint_t* data) {
    if (data) arrayFree(data->footpaths);
    free(data);
    data = NULL;
}

// reads data from input file, returns pointer to the root node
quadTreeNode_t* readInputFrom(char* fileName, quadTreeNode_t* root) {
    FILE* file = fopen(fileName, "r");
    assert(file!=NULL);
    char line[MAX_ROW_LENGTH];

    // read header
    fgets(line, MAX_ROW_LENGTH, file);

    // read records
    while(fgets(line, MAX_ROW_LENGTH, file) != NULL) {
        data_t* record = read_row(line);
        insertDataToQT(root, record);
    }

    fclose(file);
    return root;
}

// given a rectangle and point, returns the quadrant of the rectangle that the 
// point lies in
quadrant determineQuadrant(rectangle2d_t r, point2d_t p) {
    assert(inRectangle(r, p));

    quadrant out;
    point2d_t middle = findMidPoint(r);
    
    // determine if p is N
    if(p.y > middle.y) {
        // if West
        if (p.x < middle.x){
            out = NW;

        // p is East
        } else out = NE;

    // p is S
    } else {
        // if East
        if (p.x > middle.x) {
            out = SE;
        
        // p is West
        } else out = SW;
    }

    return out;
}

// adds a datapoint given with its 2D coordinates to the quadtree
void addPoint(quadTreeNode_t* root, data_t* data, point2d_t point) {

    // base case, empty (white) node
    if (root->typeOfNode == WHITE) {
        root->dataPoint = dataPointCreate();
        root->dataPoint->point = point;
        sortedArrayInsert(root->dataPoint->footpaths, data);
        root->typeOfNode = BLACK;
        return;

    // add to data point if same points and data already exists
    // (black node, same data)
    } else if ((root->typeOfNode == BLACK) && (pointsAreEqual(root->dataPoint->point, point))) {
        sortedArrayInsert(root->dataPoint->footpaths, data);
        return;

    // root node not empty
    } else if (root->children[SW] == NULL) root = quadTreeSubDivide(root);
    
    addPoint(root->children[determineQuadrant(root->rectangle, point)], data, point);
}

// subdivides a quadtree node
quadTreeNode_t *quadTreeSubDivide(quadTreeNode_t* root) {
    point2d_t mid = findMidPoint(root->rectangle);
    rectangle2d_t r = root->rectangle;   // for convenience

    // south west
    rectangle2d_t southwest = makeRectangle(mid.y, mid.x, r.bottomLeft.y, r.bottomLeft.x);
    root->children[SW] = makeQuadTreeNode(southwest);

    // north west
    rectangle2d_t northwest = makeRectangle(r.topRight.y, mid.x,mid.y, r.bottomLeft.x);
    root->children[NW] = makeQuadTreeNode(northwest);

    // north east
    rectangle2d_t northeast = makeRectangle(r.topRight.y, r.topRight.x, mid.y, mid.x);
    root->children[NE] = makeQuadTreeNode(northeast);
    
    // south east
    rectangle2d_t southeast = makeRectangle(mid.y, r.topRight.x,r.bottomLeft.y, mid.x);
    root->children[SE] = makeQuadTreeNode(southeast);
    
    // insert current data in root to proper quadrant
    quadrant newQuad = determineQuadrant(r, root->dataPoint->point);
    addPoint(root->children[newQuad], root->dataPoint->footpaths->A[0], root->dataPoint->point);
    root->dataPoint->footpaths->A[0] = NULL;

    for(int i=1; i<root->dataPoint->footpaths->n; i++) {
        addPoint(root->children[newQuad], root->dataPoint->footpaths->A[i], root->dataPoint->point);
        root->dataPoint->footpaths->A[i] = NULL;
    }

    // convert node to grey node
    root->dataPoint->footpaths->n = 0;
    if (root->dataPoint->footpaths->A) free(root->dataPoint->footpaths->A);
    if (root->dataPoint->footpaths) free(root->dataPoint->footpaths);
    if (root->dataPoint) free(root->dataPoint);
    root->dataPoint = NULL;
    root->typeOfNode = GREY;

    return root;
}

// tests whether a datapoint given by its 2D coordinate lies within the
// root node and returns the datapoint if it does and NULL otherwise
dataPoint_t* searchPoint(quadTreeNode_t* root, point2d_t point) {

    // base case (bottom most node)
    if (root->children[SW] == NULL) {
        
        // quadrant where point should be is empty, point not in tree
        if(root->dataPoint == NULL) return NULL;

        // point in tree
        if(pointsAreEqual(root->dataPoint->point, point)) return root->dataPoint;

        return NULL;

    // recursive case (node has children)
    } else {
        return searchPoint(root->children[determineQuadrant(root->rectangle, point)], point);
    }
}

// takes a 2D rectangle as argument and returns all datapoints in the PR
// quadtree whose coordinates lie within the query rectangle
void rangeQuery(rectangle2d_t query, quadTreeNode_t* root, array_t* out) {

    // base case (root has no children, and quadrant overlaps with rectangle)
    if (root->children[SW] == NULL) {

        // add all data records at datapoint to out array if they lie in range
        if ((root->dataPoint) && (inRectangle(query, root->dataPoint->point))) {
            for(int i = 0; i < root->dataPoint->footpaths->n; i++) {
                data_t *data = root->dataPoint->footpaths->A[i]; // convenience

                // have not visited data yet
                if (!arrayBinarySearch(out, data)) {
                    sortedArrayInsert(out, data);
                }
            }
        }

    // recursive case (node has children, check all of them)
    } else {
        for(int quad = SW; quad <= SE; quad++) {
            if (rectangleOverlap(query, root->children[quad]->rectangle)) {
                if (root->children[quad]->dataPoint || root->children[quad]->typeOfNode == GREY){
                     printQuadrantTo(quad, stdout);
                }
                rangeQuery(query, root->children[quad], out);
            }
        }
    }
}

// prints the path to find a given point in a root to out
// assumes that the root has the point in data point
void printPath(quadTreeNode_t* root, point2d_t point, FILE* out) {
    quadTreeNode_t *temp = root;

    while(temp->children[SW]) {
        quadrant quad = determineQuadrant(temp->rectangle, point);
        printQuadrantTo(quad, out);
        temp = temp->children[quad];
    }

    fprintf(out, "\n");

}

// prints quadrant to out as string
void printQuadrantTo(quadrant quad, FILE* out) {
    if (quad == NW) fprintf(out, " NW");
    if (quad == SW) fprintf(out, " SW");
    if (quad == NE) fprintf(out, " NE");
    if (quad == SE) fprintf(out, " SE");
}
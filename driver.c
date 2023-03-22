/*------------------------------------------------------------------------------ 
..Project: assignment 2
  Author: Remco Holstege 1217370

  This project implements a quad tree data structure that can store 
  data regarding polygon data on Melbourne's footpaths
  
  executible programs contains 3 arguments:
    1. the stage (either 3 or 4)
        stage 3 finds data records found at the given point
        stage 4 finds all data records found in the given rectangle
    2. the filename of the datafile
    3. the filename of the output file
    4. the x coordinate (longitude) of the left-most point of the root node area
    5. the y coordinate (latitude) of the bottom-most point of the root node area
    6. the x coordinate (longitude) of the right-most point of the root node area
    7. the y coordinate (latitude) of the top-most point of the root node area

  results are outputted to the output file, containing all information
  of the data record

  a summarised output is outputted to stdout

  driver.c : 
             = the main program of the project 
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "quadtree.h"
#include "rectangle.h"

// order of input arguments
typedef enum commandArgs{PROGRAM, STAGE, INPUT, OUTPUT, 
                         LEFT, BOTTOM, RIGHT, TOP}argument;

/*------------ function prototypes -------------------------------------------*/
void implement(quadTreeNode_t* rootNode, FILE *in, FILE *out, FILE *info, char stage);
void stageThreeSearch(point2d_t point, quadTreeNode_t* root, FILE* info, FILE* out);
void stageFourSearch(rectangle2d_t rectangle, quadTreeNode_t* root, FILE* info, FILE* out);

/*------------ function definitions ------------------------------------------*/

int main(int argc, char *argv[]) {
    // set input specs
    FILE *xtndedOutFile = fopen(argv[OUTPUT], "w");
    assert(xtndedOutFile);
    char stage = *argv[STAGE];

    // make tree rectangle based on input
    long double l, b, r, t;
    l = strtold(argv[LEFT], NULL);
    b = strtold(argv[BOTTOM], NULL);
    r = strtold(argv[RIGHT], NULL);
    t = strtold(argv[TOP], NULL);
    rectangle2d_t baseRectangle = makeRectangle(t, r, b, l);

    // make quad tree and add input data
    quadTreeNode_t *rootNode = makeQuadTreeNode(baseRectangle);
    rootNode = readInputFrom(argv[INPUT], rootNode);

    // implement assignment stages
    implement(rootNode, stdin, stdout, xtndedOutFile, stage);

    // clean up
    freeQuadTreeNode(rootNode);
    fclose(xtndedOutFile);
    return EXIT_SUCCESS;
}

// implements commands from "in" and the commandline, also prints shortened
// output to "out" as well as an extended output based on commandline
void implement(quadTreeNode_t* rootNode, FILE *in, FILE *out, FILE *info, char stage) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // perform each command
    while((read = getline(&line, &len, in)) != -1) {
        line[strcspn(line, "\n")] = 0;
        fprintf(out, "%s -->", line);
        fprintf(info, "%s\n", line);
        
        if(stage == '3') {
            long double lon;
            long double lat;
            sscanf(line, "%Lf %Lf", &lon, &lat);

            point2d_t point = (point2d_t){.x = lon, .y = lat};

            stageThreeSearch(point, rootNode, info, out);

        } else if(stage == '4') {
            long double left;
            long double bottom;
            long double right;
            long double top;
            sscanf(line, "%Lf %Lf %Lf %Lf", &left, &bottom, &right, &top);

            rectangle2d_t range = makeRectangle(top, right, bottom, left);

            stageFourSearch(range, rootNode, info, out);
        }
    }

    free(line);
}

// implements stage three search to find any data matches with the same start or
// end point, prints the quadrant path to "out" and data to "info"
void stageThreeSearch(point2d_t point, quadTreeNode_t* root, FILE* info, FILE* out) {
    
    printPath(root, point, out);
    dataPoint_t* dataPoint = searchPoint(root, point);

    // print all data stored in dataPoint
    if(dataPoint) {
        printArrayTo(dataPoint->footpaths, info);
    }

}

// implements stage four search to find any data matches within the rectangle
// prints all the data to "info" and all quadrants checked to out 
void stageFourSearch(rectangle2d_t rectangle, quadTreeNode_t* root, FILE* info, FILE* out) {
    array_t* records = arrayCreate();
    rangeQuery(rectangle, root, records);

    printf("\n");
    printArrayTo(records, info);

    // free array, but not records
    if(records->A) free(records->A);
    if(records) free(records);
}
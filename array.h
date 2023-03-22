/*-------------------------------------------------------------- 
..Project: assignment 2
  array.h : 
          = the interface of module array of the project
modified from workshop 3.9
----------------------------------------------------------------*/

#ifndef _ARRAY_H_
#define _ARRAY_H_
#include "data.h"

#define INIT_SIZE 2     // initial size of arrays 

typedef struct array {
	data_t **A;      // use as array of data_t *, ie. data_t *A[]
	int size;           // current array size
	int n;              // current number of elements
} array_t;

// creates & returns an empty array
array_t *arrayCreate();

// free memory used by array "arr"
void arrayFree(array_t *arr);

// makes sure that array's size exceeds its number of elements
//    ie. there is space for adding new element - from workshop 3.9
void arrayEnableInsert(array_t *arr);

// inserts data "s" into array "arr", ensuring "arr" is sorted 
void sortedArrayInsert(array_t *arr, data_t *s);

// finds index of data struct with the closest grade1in value in a sorted array
int nearest_grade1in(double d, array_t* arr);

// determines which index in array is closer to target value
// assumes ascending order
int get_closest_index(array_t* arr, int index1, int index2, double target);

// inserts data "s" into end of array "arr" (does not sort)
void arrayAppend(array_t *arr, data_t *s);

// prints all contents of array to out
void printArrayTo(array_t* arr, FILE *out);

// searches for data with "id" in sorted array "arr"
// returns the pointer to the found data, NULL if not found
data_t *arrayBinarySearch(array_t *arr, data_t* data);

#endif
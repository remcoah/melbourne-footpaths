/*-------------------------------------------------------------- 
..Project: assignment 2
  array.c : 
          = the implementation of module array of the project
modified from workshop 3.9
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "array.h"
#include "data.h"

// creates & returns an empty array - from workshop 3.9
array_t *arrayCreate() {
	array_t *arr = malloc(sizeof(*arr));
	assert(arr);
	int size = INIT_SIZE;      // "size" used for convenience
	arr->size = size;
	arr->A = malloc(size * sizeof(*(arr->A)));
	assert(arr->A);
	arr->n = 0;
	return arr;
}

// free memory used by array "arr" - from workshop 3.9
void arrayFree(array_t *arr) {
	for (int i = 0; i < arr->n; i++) {
		if(arr->A[i]) free_data(arr->A[i]);
	}
	free(arr->A);
	free(arr);
    arr = NULL;
}

// makes sure that array's size exceeds its number of elements
//    ie. there is space for adding new element - from workshop 3.9
void arrayEnableInsert(array_t *arr) {
	if (arr->n == arr->size) {
		arr->size <<= 1;       // same as arr->size *= 2;
		arr->A= realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

// inserts data "s" into sorted array "arr", ensuring "arr" remain sorted 
// modified from workshop 3.9
void sortedArrayInsert(array_t *arr, data_t *s) {
	int i;
	arrayEnableInsert(arr);

	// starting from the end, shift all elements > s one position to the right
	for (i = arr->n - 1; i >= 0 && cmpfunc(s, arr->A[i]) < 0; i-- ) {
		arr->A[i + 1] = arr->A[i];
	}
    
	// now "s" should be in A[i+1]
	arr->A[i + 1] = s;
	arr->n++;
}

// prints all contents of array to out
void printArrayTo(array_t* arr, FILE *out) {
    for(int i=0; i<arr->n; i++) {
        print_data_to(arr->A[i], out);
    }
}

// searches for data with "id" in sorted array "arr"
// returns the pointer to the found data, NULL if not found  - from workshop 3.9
data_t *arrayBinarySearch(array_t *arr, data_t* data) {

	data_t **A = arr->A;    // for convenience 
	int mid, lo = 0, hi = arr->n - 1;
	while (lo <= hi) {
		mid = (lo + hi)/2;
		int cmp = cmpfunc(data, A[mid]);
		if (cmp == 0) return A[mid];
		if (cmp < 0) {
			hi = mid - 1;
		} else {
			lo = mid + 1;
		}
	}
	return NULL;
}
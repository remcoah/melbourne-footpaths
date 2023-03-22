/*------------------------------------------------------------------------------
..Project: assignment 2
  data.h :  
         = the interface of module data of the project
------------------------------------------------------------------------------*/

#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define MAX_FIELD 128   // maximum number of characters for a single field
#define EPSILON 1e-12 // maximum difference between 2 float numbers to be considered equal

// define struct for street data
typedef struct data {  // stores street data from input
    int footpath_id, mcc_id, mccid_int, status_id, street_id, street_group;
    double deltaz, distance, grade1in, rlmax, rlmin, start_lat, start_lon, end_lat, end_lon;
    char *address, *clue_sa, *asset_type, *segside;
} data_t;


/*------------ function definitions ------------------------------------------*/

// determines if two doubles are considered to be equal returns 1 (TRUE) or 
// 0 (FALSE)
int ldoublesAreEqual(long double a, long double b);

// reads a csv row and creates a struct containing the proper information
// returns a pointer to the data struct
data_t* read_row(char* row);

// makes and allocates memory for an empty data struct
data_t* make_empty_data(void);

// frees data struct
void free_data(data_t* data);

// replaces commas within string columns of a csv row
void replace_inner_commas(char* row);

// formats a string cell to proper form by inserting commas and removing quotes
void format_string_cell(char* string);

// prints "data" to "output_file" formatted as per assignment specs
void print_data_to(data_t* data, FILE* output_file);

// compare function to sort based on footpath
int cmpfunc(data_t *dataA, data_t *dataB);

// returns footpathid for data record
int data_get_footpathid(data_t* data);

#endif
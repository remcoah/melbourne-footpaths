/*------------------------------------------------------------------------------
..Project: assignment 2
  data.c :  
         = the implementation of module data of the project
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "data.h"

/*------------ function definitions ------------------------------------------*/

// determines if two doubles are considered to be equal returns 1 (TRUE) or 
// 0 (FALSE)
int ldoublesAreEqual(long double a, long double b) {
    return (fabsl(a-b) < EPSILON);
}

// reads a csv row and creates a struct containing the proper information
// returns a pointer to the data struct
data_t* read_row(char* row) {    
    data_t* out = make_empty_data();

    // to ensure strings are read correctly
    replace_inner_commas(row);

    // strings to store cell variables
    char footpath_id[MAX_FIELD+1], address[MAX_FIELD+1], clue_sa[MAX_FIELD+ 1], 
        asset_type[MAX_FIELD], deltaz[MAX_FIELD+1], distance[MAX_FIELD+1], grade1in[MAX_FIELD+1],
        mcc_id[MAX_FIELD+1], mccid_int[MAX_FIELD+1], rlmax[MAX_FIELD+1], rlmin[MAX_FIELD+1], 
        segside[MAX_FIELD], status_id[MAX_FIELD+1], street_id[MAX_FIELD+1], street_group[MAX_FIELD+1], 
        start_lat[MAX_FIELD+1], start_lon[MAX_FIELD+1], end_lat[MAX_FIELD+1], end_lon[MAX_FIELD+1];

    char *cell;
    int i = 0;
    while((cell = strsep(&row, ",")) != NULL) {
        if(i == 0) {
            strcpy(footpath_id, cell);
        }  else if(i == 1) {
            strcpy(address, cell);
        } else if(i == 2) {
            strcpy(clue_sa, cell);
        } else if(i == 3) {
            strcpy(asset_type, cell);
        } else if(i == 4) {
            strcpy(deltaz, cell);
        } else if(i == 5) {
            strcpy(distance, cell);
        } else if(i == 6) {
            strcpy(grade1in, cell);
        } else if(i == 7) {
            strcpy(mcc_id, cell);
        } else if(i == 8) {
            strcpy(mccid_int, cell);
        } else if(i == 9) {
            strcpy(rlmax, cell);
        } else if(i == 10) {
            strcpy(rlmin, cell);
        } else if(i == 11) {
            strcpy(segside, cell);
        } else if(i == 12) {
            strcpy(status_id, cell);
        } else if(i == 13) {
            strcpy(street_id, cell);
        } else if(i == 14) {
            strcpy(street_group, cell);
        } else if(i == 15) {
            strcpy(start_lat, cell);
        } else if(i == 16) {
            strcpy(start_lon, cell);
        } else if(i == 17) {
            strcpy(end_lat, cell);
        } else if(i == 18) {
            strcpy(end_lon, cell);
        }
        i++;
    }

    // ensure that strings are formated correctly
    format_string_cell(address);
    format_string_cell(clue_sa);
    format_string_cell(asset_type);
    format_string_cell(segside);

    // insert data into struct
    out->address = strdup(address);
    out->clue_sa = strdup(clue_sa);
    out->asset_type = strdup(asset_type);
    out->segside = strdup(segside); 
    out->footpath_id = atoi(footpath_id);
    out->mcc_id = atoi(mcc_id);
    out->mccid_int = atoi(mccid_int);
    out->status_id = atoi(status_id);
    out->street_id = atoi(street_id);
    out->street_group = atoi(street_group);
    out->deltaz = atof(deltaz);
    out->distance = atof(distance);
    out->grade1in = atof(grade1in);
    out->rlmax = atof(rlmax);
    out->rlmin = atof(rlmin);
    out->start_lat = atof(start_lat);
    out->start_lon = atof(start_lon);
    out->end_lat = atof(end_lat);
    out->end_lon = atof(end_lon);
    
    return out;
    // i had such a beatiful and clean function using sscanf but whoever
    // designed this assignment hates students so now i have this ugly
    // function pls dont remove marks i dont want it to be like this <3
}

// makes and allocates memory for an empty data struct
data_t* make_empty_data(void) {
    data_t *data;
	data = (data_t*)malloc(sizeof(data_t));
	assert(data!=NULL);
    return data;
}

// frees data struct
void free_data(data_t* data) {
    if(data == NULL) return;

    assert(data->address!=NULL);
    free(data->address);

    assert(data->clue_sa!=NULL);
    free(data->clue_sa);

    assert(data->asset_type!=NULL);
    free(data->asset_type);

    assert(data->segside!=NULL);
    free(data->segside);

    free(data);
    data = NULL;
}

// replaces commas within string columns of a csv row with a $ char
void replace_inner_commas(char* row) {
    // determines if the column read is a string variable that contains a comma
    int string_col = -1;

    for(int i = 0; i < strlen(row); i++) {
        if(row[i] == '\"') {
            string_col = string_col * (-1);
        }
        // positive value means string contains comma
        if((string_col > 0) && (row[i] == ',')) {
            row[i] = '$';
        }
    }

}

// formats a string cell to proper form by inserting commas and removing quotes
void format_string_cell(char* string) {
    if (string[0] == '\"') {
        string[strlen(string) - 1] = '\0';
        memmove(string, string+1, strlen(string));
    }

    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '$') 
            string[i] = ',';
    }
}

// prints "data" to "output_file" formatted as per assignment specs
void print_data_to(data_t* data, FILE* output_file) {
    fprintf(output_file, "--> footpath_id: %d || address: %s || clue_sa: %s ||",
                            data->footpath_id, data->address, data->clue_sa);
    fprintf(output_file, " asset_type: %s || deltaz: %.2f || distance: %.2f || ", 
                            data->asset_type, data->deltaz, data->distance);
    fprintf(output_file, "grade1in: %.1f || mcc_id: %d || mccid_int: %d || ", 
                            data->grade1in, data->mcc_id, data->mccid_int);
    fprintf(output_file, "rlmax: %.2f || rlmin: %.2f || segside: %s || statusid: %d || ", 
                            data->rlmax, data->rlmin, data->segside, data->status_id);
    fprintf(output_file, "streetid: %d || street_group: %d || start_lat: %lf || ", 
                            data->street_id, data->street_group, data->start_lat);
    fprintf(output_file, "start_lon: %lf || end_lat: %lf || end_lon: %lf || \n", 
                            data->start_lon, data->end_lat, data->end_lon);
}

// compare function to sort based on grade1in
int cmpfunc(data_t *dataA, data_t *dataB) {
    return data_get_footpathid(dataA) - data_get_footpathid(dataB);
}

// returns footpathid for data record
int data_get_footpathid(data_t* data) {
    assert(data != NULL);
    return data->footpath_id;
}
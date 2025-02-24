#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "csv_functions.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-f | -r | -h] <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[argc - 1];
    //printf("file: %s\n", filename);

    // initialize variables for header parsing
    int field_index = -1;
    char header[MAX_FIELDS][MAX_LINE_LENGTH];
    int num_fields = 0;
    
    parse_header(filename, header, &num_fields);

    // checks if -h flag is present
    bool h_flag = false;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            h_flag = true;
            break;
        }
    }

    // process the arguments
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            count_fields(filename);
        } else if (strcmp(argv[i], "-r") == 0) {
            count_records(filename);
        } else if (strcmp(argv[i], "-min") == 0) {  // if -min and there is an argument after it
            if (h_flag) {
                field_index = parse_field_name(argv[i + 1], header, &num_fields);
            } else {
                field_index = atoi(argv[i + 1]);
            }

            if (field_index == -1) {
                fprintf(stderr, "Error: Field '%s' not found in header\n", argv[i + 1]);
                return EXIT_FAILURE;
            }
            min_field(field_index, filename);
        } else if (strcmp(argv[i], "-max") == 0) {
            if (h_flag) {
                field_index = parse_field_name(argv[i + 1], header, &num_fields);
            } else {
                field_index = atoi(argv[i + 1]);
            }

            if (field_index == -1) {
                fprintf(stderr, "Error: Field '%s' not found in header\n", argv[i + 1]);
                return EXIT_FAILURE;
            }
            max_field(field_index, filename);
        } else if (strcmp(argv[i], "-mean") == 0) { // do we need this:  && i + 1 < argc
            if (h_flag) {
                field_index = parse_field_name(argv[i + 1], header, &num_fields);
            } else {
                field_index = atoi(argv[i + 1]);
            }

            if (field_index == -1) {
                fprintf(stderr, "Error: Field '%s' not found in header\n", argv[i + 1]);
                return EXIT_FAILURE;
            }
            mean_field(field_index, filename);
        } else if (strcmp(argv[i], "-records") == 0){
	  if (h_flag){
	    field_index = parse_field_name(argv[i + 1], header, &num_fields);
	  } else {
	    field_index = atoi(argv[i + 1]);
	  }
	  if (field_index == -1) {
	    fprintf(stderr, "Error: Field '%s' not found in header\n", argv[i+1]);
	    return EXIT_FAILURE;
	  }
	  record_check(filename, field_index, argv[i + 2]);
	}
        // added functionality for the records funtion in the same manner as min/max/mean
    }

    return EXIT_SUCCESS;
}

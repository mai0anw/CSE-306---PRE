#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h> // Used for INT_MAX, and INT_MIN

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 8

// Function Prototypes
void count_fields(const char *filename);
void count_records(const char *filename);
char** split_by_commas(const char *input, int *num_fields);
char* process_quoted_field(char *field);
int min_field(int field_index, const char *filename);
int max_field(int field_index, const char *filename);
double mean_field(int field_index, const char *filename);
void parse_header(const char *filename, char header[MAX_FIELDS][MAX_LINE_LENGTH], int *num_fields);
int parse_field_name(const char *field_name, char header[MAX_FIELDS][MAX_LINE_LENGTH], int *num_fields);

#endif // CSV_H
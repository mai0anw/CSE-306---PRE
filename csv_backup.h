#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


void count_fields(const char*);
void count_records(const char* , bool a);
char process_quoted_field(char *field);

int min_field(int field_index, const char *filename, int has_header);
int max_field(int field_index, const char *filename, int has_header);

double mean_field(int field_index, const char *filename, int has_header);

void parse_header(const char *filename, char header[MAX_FIELDS][MAX_LINE_LENGTH], int *num_fields);

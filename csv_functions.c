#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h> // Used for INT_MAX, and INT_MIN

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 8

void count_fields(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file)) {
        int count = 1;
        for (char *ptr = line; *ptr; ptr++) {
            if (*ptr == ',') count++;
        }
        printf("%d\n", count);
    }
    fclose(file);
}

void count_records(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        count++;
    }
    printf("%d\n", count);
    fclose(file);
}

char** split_by_commas(const char *input, int *num_fields) {
    int capacity = 10;
    char **fields = malloc(capacity * sizeof(char*));
    int field_count = 0;
    
    const char *start = input;
    int i = 0;
    bool in_quotes = false;
    
    for (i = 0; input[i] != '\0'; i++) {
        // toggle in_quotes when encountering a quote
        if (input[i] == '"') {
            in_quotes = !in_quotes;
        }

        // split on comma when not inside quotes
        if (input[i] == ',' && !in_quotes) {
            // allocate space for the new field (substring from start to i)
            int length = i - (start - input);
            fields[field_count] = malloc(length + 1);
            strncpy(fields[field_count], start, length);
            fields[field_count][length] = '\0';  // null-terminate the string
            
            field_count++;
            if (field_count >= capacity) {
                // resize the fields array if needed
                capacity *= 2;
                fields = realloc(fields, capacity * sizeof(char*));
            }
            
            // move start to the next character after the comma
            start = input + i + 1;
        }
    }

    // handles the last field (after the last comma or at the end of the string)
    if (start != input + i) {
        fields[field_count] = malloc(i - (start - input) + 1);
        strncpy(fields[field_count], start, i - (start - input));
        fields[field_count][i - (start - input)] = '\0';
        field_count++;
    }

    *num_fields = field_count;
    return fields;
}

char* process_quoted_field(char *field) {
    int len = strlen(field);

    // if the field has quotes around it at the beginning and end
    if (field[0] == '"' && field[len - 1] == '"') {
        field[len - 1] = '\0';  // repalces the closing quote
        char *new_field = malloc(len - 1);  // Allocate new space for the processed string
        
        // & is used to get the address
        strcpy(new_field, &field[1]); //start from [1], and copy everything else
        return new_field;
    }
    // else if not quoted, return as is
    // printf("processed: %s\n", field);
    return field;

}

// updated MIN
int min_field(int field_index, const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];     // buffer

    // sum divided by count is the mean --> sum/count = mean
    int min_value = INT_MAX;        // min value to neg inf
    int valid_int = 0; 
    int num_fields = 0;  // Initialize num_fields here

    // skips the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: File is empty or unable to read header\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // needs to read the csv lines from the input
    while (fgets(line, sizeof(line), file)) {  // reads lines
        char **fields = split_by_commas(line, &num_fields);  // Split the line into fields

        if (num_fields == 0) {
            continue;  // Skip empty lines
        }

        // Process each field
        for (int curr_column = 0; curr_column < num_fields; curr_column++) {
            char *field = fields[curr_column];  // Current field

            // check for quotes
            char *processed_field = process_quoted_field(field);

            // if the current column is the right field index
            if (curr_column == field_index) {
                int curr_value;

                // sscanf turns "100" (string) -> 100 (int) and store that into curr_value
                if (sscanf(processed_field, "%d", &curr_value) == 1) {
                    // after successful conversion, determine the min value
                    if(curr_value < min_value) {
                        min_value = curr_value;  // keep track of the min value
                    }
                    // valid integers have been found
                    valid_int = 1;
                } 

                // if a new string was made for quoted fields, free it after usage
                if (processed_field != field) {
                    free(processed_field);
                }
            }
        }

        // Free the memory allocated for fields after processing the line
        for (int i = 0; i < num_fields; i++) {
            free(fields[i]);
        }
        free(fields);
    }

    fclose(file);

    // If no valid numeric data was found
    if (valid_int == 0) {
        fprintf(stderr, "Error: No valid numeric data in the specified field\n");
        return EXIT_FAILURE;
    }

    // Calculate and return the mean
    
    //printf("Min_value: %d\n", min_value);
    printf("%d\n", min_value);
    return min_value;
}


// updated MAX
int max_field(int field_index, const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];     // buffer

    // sum divided by count is the mean --> sum/count = mean
    int max_value = INT_MIN;        // min value to neg inf
    int valid_int = 0; 
    int num_fields = 0;  // Initialize num_fields here

    // skips the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: File is empty or unable to read header\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // needs to read the csv lines from the input
    while (fgets(line, sizeof(line), file)) {  // reads lines
        char **fields = split_by_commas(line, &num_fields);  // Split the line into fields

        if (num_fields == 0) {
            continue;  // Skip empty lines
        }

        // process each field
        for (int curr_column = 0; curr_column < num_fields; curr_column++) {
            char *field = fields[curr_column];  // Current field

            // check for quotes
            char *processed_field = process_quoted_field(field);

            // if the current column is the right field index
            if (curr_column == field_index) {
                int curr_value;

                // sscanf turns "100" (string) -> 100 (int) and store that into curr_value
                if (sscanf(processed_field, "%d", &curr_value) == 1) {
                    // after successful conversion, determine the min value
                    if(curr_value > max_value) {
                        max_value = curr_value;  // keep track of the min value
                    }
                    // valid integers have been found
                    valid_int = 1;
                } 

                // if a new string was made for quoted fields, free it after usage
                if (processed_field != field) {
                    free(processed_field);
                }
            }
        }

        // free the memory allocated for fields after processing the line
        for (int i = 0; i < num_fields; i++) {
            free(fields[i]);
        }
        free(fields);
    }

    fclose(file);

    // if no valid numeric data was found
    if (valid_int == 0) {
        fprintf(stderr, "Error: No valid numeric data in the specified field\n");
        return EXIT_FAILURE;
    }

    // calculate and return the mean
    
    //printf("Max_value: %d\n", max_value);
    printf("%d\n", max_value);
    return max_value;
}

// updated MEAN
double mean_field(int field_index, const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];     // buffer

    // sum divided by count is the mean --> sum/count = mean
    double sum = 0;
    int count = 0;
    int num_fields = 0;  // Initialize num_fields here

    // skips the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: File is empty or unable to read header\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // needs to read the csv lines from the input
    while (fgets(line, sizeof(line), file)) {  // reads lines
        char **fields = split_by_commas(line, &num_fields);  // Split the line into fields

        if (num_fields == 0) {
            continue;  // Skip empty lines
        }

        // Process each field
        for (int curr_column = 0; curr_column < num_fields; curr_column++) {
            char *field = fields[curr_column];  // Current field

            // check for quotes
            char *processed_field = process_quoted_field(field);

            // if the current column is the right field index
            if (curr_column == field_index) {
                double curr_value;

                // sscanf turns "100.5" (string) -> 100.5 (double) and store that into curr_value
                if (sscanf(processed_field, "%lf", &curr_value) == 1) {
                    sum += curr_value;  // accumulate the sum
                    count++;             // increment the count
                }

                // if a new string was made for quoted fields, free it after usage
                if (processed_field != field) {
                    free(processed_field);
                }
            }
        }

        // free the memory allocated for fields after processing the line
        for (int i = 0; i < num_fields; i++) {
            free(fields[i]);
        }
        free(fields);
    }

    fclose(file);

    // if no valid numeric data was found
    if (count == 0) {
        fprintf(stderr, "Error: No valid numeric data in the specified field\n");
        return EXIT_FAILURE;
    }

    // calculate and return the mean
    double mean = sum / count;
    //printf("Sum: %f\n", sum);
    //printf("Count: %d\n", count);
    //printf("Mean: %.2f\n", mean);
    printf("%.2f\n", mean);
    return mean;
}


void parse_header(const char *filename, char header[MAX_FIELDS][MAX_LINE_LENGTH], int *num_fields) {
    // Open the CSV file for reading
    FILE *file = fopen(filename, "r");
    if (!file) { // Check if file opening failed
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH]; // Buffer to store the first line (header row)

    // Read the first line (header row) from the file
    if (fgets(line, sizeof(line), file)) {
        *num_fields = 0; // Initialize the number of fields to 0

        // Tokenize the first line using "," and "\n" as delimiters
        char *field = strtok(line, ",\n");
        
        while (field) { // Loop through each field in the header row
            // Copy the field into the header array (avoiding buffer overflow)
            strncpy(header[*num_fields], field, MAX_LINE_LENGTH);
            
            (*num_fields)++; // Increment field count
            
            // Get the next field
            field = strtok(NULL, ",\n");
        }
    }

    fclose(file); // Close the file
}


// parses the field name to convert it into a index int used in the other min/max/mean/record functions. used when the -h flag is present
int parse_field_name(const char *field_name, char header[MAX_FIELDS][MAX_LINE_LENGTH], int *num_fields) {
    for (int i = 0; i < *num_fields; i++) {
        if (strcmp(header[i], field_name) == 0) {
            //printf("INDEX: %d\n", i);
            return i;  // Return the index of the matching field
        }
    }
    return -1;  // Field not found
}

void record_check(const char *filename, int idx, char *value){

    FILE *file = fopen(filename, "r"); //pointer to open and read file
    if (!file){
      perror("Error opening file");
      exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH]; //buffer that stores the header row
    //char *value = val;
    //int curr_col = 0;
    int num_fields = 0;

    //works
    //printf("%s", value);
    //printf("%d", idx);
    // skips the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: File is empty or unable to read header\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)){

        char **fields = split_by_commas(line, &num_fields);

	//printf("%s", line);
	//should split the line by commas when it is not in quotes
	//char *line_copy[MAX_LINE_LENGTH] = strdup(line);
      //strcpy(line_copy, line);


        for (int curr_column = 0; curr_column < num_fields; curr_column++){
            char *field = fields[curr_column];
	    //printf("%s", field);

            char *processed_field = process_quoted_field(field);
	     
	    //printf("%d\n", processed_field[7]);

            if (curr_column == idx) {
	      //printf("%d", processed_field[curr_column]);
	      //if (strcmp(processed_field, value) == 0){
	      //printf("%s\n", );
	      //}

	      char curr_value[MAX_LINE_LENGTH];

	      if(sscanf(processed_field, "%s", curr_value) == 1){
		if(strcmp(curr_value, value) ==0){
		  printf("%s\n", line);
		}
	      }

	      
	      //printf("%s", line);

	      if (processed_field != field){
		free(processed_field);
	      }

            }


        }

        for (int i = 0; i < num_fields; i++) {
            free(fields[i]);
        }
        free(fields);

        //char *field;
        //int curr_col = 0;



        //field = strtok(line, ",\n"); //split each field
        //test
        //printf("%s", field);

        //while (field != NULL){
            //char *process_field = process_quoted_field(field);
            //processes the field and gets rid of the quotations

            //if (curr_col == idx && strcmp(process_field, val) == 0){
                //printf("%s", line_copy); //print line if it matches
            //}

            //if(process_field != field){
            //free(process_field);
                //}

            //curr_col ++;
            //field = strtok(NULL, ",\n");


        //}

        //free(line_copy);

    }

    //return; 
    fclose(file);

}
// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         fprintf(stderr, "Usage: %s [-f | -r | -h] <filename>\n", argv[0]);
//         return EXIT_FAILURE;
//     }

//     const char *filename = argv[argc - 1];
//     //printf("file: %s\n", filename);

//     // initialize variables for header parsing
//     int field_index = -1;
//     char header[MAX_FIELDS][MAX_LINE_LENGTH];
//     int num_fields = 0;
    
//     parse_header(filename, header, &num_fields);

//     // checks if -h flag is present
//     bool h_flag = false;
//     for (int i = 1; i < argc - 1; i++) {
//         if (strcmp(argv[i], "-h") == 0) {
//             h_flag = true;
//             break;
//         }
//     }

//     // process the arguments
//     for (int i = 1; i < argc - 1; i++) {
//         if (strcmp(argv[i], "-f") == 0) {
//             count_fields(filename);
//         } else if (strcmp(argv[i], "-r") == 0) {
//             count_records(filename);
//         } else if (strcmp(argv[i], "-min") == 0) {  // if -min and there is an argument after it
//             if (h_flag) {
//                 field_index = parse_field_name(argv[i + 1], header, &num_fields);
//             } else {
//                 field_index = atoi(argv[i + 1]);
//             }

//             if (field_index == -1) {
//                 fprintf(stderr, "Error: Field '%s' not found in header\n", argv[i + 1]);
//                 return EXIT_FAILURE;
//             }
//             min_field(field_index, filename);
//         } else if (strcmp(argv[i], "-max") == 0) {
//             if (h_flag) {
//                 field_index = parse_field_name(argv[i + 1], header, &num_fields);
//             } else {
//                 field_index = atoi(argv[i + 1]);
//             }

//             if (field_index == -1) {
//                 fprintf(stderr, "Error: Field '%s' not found in header\n", argv[i + 1]);
//                 return EXIT_FAILURE;
//             }
//             max_field(field_index, filename);
//         } else if (strcmp(argv[i], "-mean") == 0) { // do we need this:  && i + 1 < argc
//             if (h_flag) {
//                 field_index = parse_field_name(argv[i + 1], header, &num_fields);
//             } else {
//                 field_index = atoi(argv[i + 1]);
//             }

//             if (field_index == -1) {
//                 fprintf(stderr, "Error: Field '%s' not found in header\n", argv[i + 1]);
//                 return EXIT_FAILURE;
//             }
//             mean_field(field_index, filename);
//         }
//         // added functionality for the records funtion in the same manner as min/max/mean
//     }

//     return EXIT_SUCCESS;
// }


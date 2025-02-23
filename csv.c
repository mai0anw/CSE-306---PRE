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
    return field;

}

int min_field(int field_index, const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return EXIT_FAILURE;
    }
    
    
    char line[MAX_LINE_LENGTH];     // buffer
    int min_value = INT_MAX;        // min value to pos inf
    int valid_int = 0;              // flag to determine if the data is valid


    // skips the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: File is empty or unable to read header\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // needs to read the csv lines from the input
    while (fgets(line, sizeof(line), file)){       // reads lines
        char *field;        // pointer to store each field
        int curr_column = 0;     // holds the current column value
        int curr_value;          // stores the converted integer value

        // split the field by comma or newline
        field = strtok(line, ",\n");

        // field is not empty
        while (field != NULL){      //reads fields
            // check for quotes
            char *processed_field = process_quoted_field(field);

            // if the current colum is the right field index
            if (curr_column == field_index) {
                // sscanf turns "100" (string) -> 100 (int) and store that into the addy at value
                // sscanf == 1, means that it successfully converted
                if (sscanf(processed_field, "%d", &curr_value) == 1) {
                    // after successful conversion, determine the min value
                    if(curr_value < min_value) {
                        min_value = curr_value;  // keep track of the min value
                    }
                    // valid integers have been found
                    valid_int = 1;
                } 

                // if a new string was made for quoted fields, we need to free it after usage
                if (processed_field != field) {
                    free(processed_field);
                }
            
            // since the target value has been found and min is updated, exit from inner loop
            break;
            }

            curr_column++;
            field = strtok(NULL, ",\n");    //updates to the next field
        }
    }
    fclose(file);

    // no valid integers read to calculate min with
    if(valid_int == 0){
        fprintf(stderr, "Error: No valid numeric data in the specified field\n");
        return EXIT_FAILURE;
    }

    printf("%d\n", min_value);
    return min_value;
}

int max_field(int field_index, const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return EXIT_FAILURE;
    }
    
    char line[MAX_LINE_LENGTH];     // buffer
    int max_value = INT_MIN;        // max value to neg inf
    int valid_int = 0;              // flag to determine if the data is valid

    // skips the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: File is empty or unable to read header\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // needs to read the csv lines from the input
    while (fgets(line, sizeof(line), file)){       // reads lines
        char *field;        // pointer to store each field
        int curr_column = 0;     // holds the current column value
        int curr_value;          // stores the converted integer value

        // split the field by comma or newline
        field = strtok(line, ",\n");

        // field is not empty
        while (field != NULL){      //reads fields
            // check for quotes
            char *processed_field = process_quoted_field(field);

            // if the current colum is the right field index
            if (curr_column == field_index) {
                // sscanf turns "100" (string) -> 100 (int) and store that into the addy at value
                // sscanf == 1, means that it successfully converted
                if (sscanf(processed_field, "%d", &curr_value) == 1) {
                    // after successful conversion, determine the max value
                    if(curr_value > max_value) {
                        max_value = curr_value;  // keep track of the max value
                    }
                    // valid integers have been found
                    valid_int = 1;
                }

                // if a new string was made for quoted fields, we need to free it after usage
                if (processed_field != field) {
                    free(processed_field);
                }
            // since the target value has been found and max is updated, exit from inner loop
            break;
            }

            curr_column++;
            field = strtok(NULL, ",\n");    //updates to the next field
        }
    }

    // no valid integers read to calculate max with
    if(valid_int == 0){
        fprintf(stderr, "Error: No valid numeric data in the specified field\n");
        return EXIT_FAILURE;
    }

    printf("%d\n", max_value);
    return max_value;

}

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

    // skips the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: File is empty or unable to read header\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // needs to read the csv lines from the input
    while (fgets(line, sizeof(line), file)){       // reads lines
        char *field;        // pointer to store each field
        int curr_column = 0;     // holds the current column value
        double curr_value;          // stores the converted integer value

        // split the field by comma or newline
        field = strtok(line, ",\n");

        // field is not empty
        while (field != NULL){      //reads fields
            // check for quotes
            char *processed_field = process_quoted_field(field);

            // if the current colum is the right field index
            if (curr_column == field_index) {
                // sscanf turns "100" (string) -> 100 (int) and store that into the addy at value
                // sscanf == 1, means that it successfully converted
                if (sscanf(processed_field, "%lf", &curr_value) == 1) {
                    // after successful conversion, accumulate both sum and count
                    printf("%f\n", sum);
                    printf("%d\n", count);
                    sum += curr_value;  
                    count++;
                }

                // if a new string was made for quoted fields, we need to free it after usage
                if (processed_field != field) {
                    free(processed_field);
                }
            
            // since the target value has been found and min is updated, exit from inner loop
            break;
            }

            curr_column++;
            field = strtok(NULL, ",\n");    //updates to the next field
        }
    }

    // if not a valid mean (meaning no integers), return EXIT_FAILURE?
    if (sum == 0){
        fprintf(stderr, "Error: No valid numeric data in the specified field\n");
        return EXIT_FAILURE;
    }

    //otherwise returns the means
    double mean = sum/count;
    printf("%f\n", sum);
    printf("%d\n", count);
    printf("%f\n", mean);
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

int parse_field_name(const char *field_name, char header[MAX_FIELDS][MAX_LINE_LENGTH], int *num_fields) {
    for (int i = 0; i < *num_fields; i++) {
        if (strcmp(header[i], field_name) == 0) {
            return i;  // Return the index of the matching field
        }
    }
    return -1;  // Field not found
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        fprintf(stderr, "Usage: %s [-f | -r] <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[argc - 1];

    int field_index = -1;
    char *field_name = NULL;
    char header[MAX_FIELDS][MAX_LINE_LENGTH];
    int num_fields = 0;

    parse_header(filename, header, &num_fields);
    
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            count_fields(filename);
        } else if (strcmp(argv[i], "-r") == 0) {
            count_records(filename);
        } else if (strcmp(argv[i], "-h") == 0) {

            if (strcmp(argv[i + 1], "-min") == 0) {
                field_name = argv[i + 2];  // use the field name provided after -min
                field_index = parse_field_name(field_name, header, &num_fields);

                if (field_index == -1) {
                    fprintf(stderr, "Error: Field '%s' not found in header\n", field_name);
                    return EXIT_FAILURE;
                }
                
                min_field(field_index, filename);
            }

            else if (strcmp(argv[i + 1], "-max") == 0) {
                field_name = argv[i + 2];  // use the field name provided after -min
                field_index = parse_field_name(field_name, header, &num_fields);

                if (field_index == -1) {
                    fprintf(stderr, "Error: Field '%s' not found in header\n", field_name);
                    return EXIT_FAILURE;
                }
                
                max_field(field_index, filename);
            }

            else if (strcmp(argv[i + 1], "-mean") == 0) {
                field_name = argv[i + 2];  // use the field name provided after -min
                field_index = parse_field_name(field_name, header, &num_fields);

                if (field_index == -1) {
                    fprintf(stderr, "Error: Field '%s' not found in header\n", field_name);
                    return EXIT_FAILURE;
                }
                
                mean_field(field_index, filename);
            }
        } 
        // hello
        // else if (strcmp(argv[i], "-min") == 0) {
        //     int conversion = atoi(argv[i + 1]);
        //     min_field(conversion, filename);
        // } else if (strcmp(argv[i], "-max") == 0) {
        //     int conversion = atoi(argv[i + 1]);
        //     max_field(conversion, filename);
        // } else if (strcmp(argv[i], "-mean") == 0) {
        //     int conversion = atoi(argv[i + 1]);
        //     mean_field(conversion, filename);
        // } else {
        //     return EXIT_FAILURE;
        // }
    }

    
    return EXIT_SUCCESS;
}



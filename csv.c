#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void count_records(const char *filename, bool has_header) {
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
    if (has_header) count--;
    printf("%d\n", count);
    fclose(file);
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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-f | -r] <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    bool has_header = false;
    const char *filename = argv[argc - 1];
    char header[MAX_FIELDS][MAX_LINE_LENGTH];
    int num_fields = 0;
    
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            has_header = true;
            parse_header(filename, header, &num_fields);
        } else if (strcmp(argv[i], "-f") == 0) {
            count_fields(filename);
        } else if (strcmp(argv[i], "-r") == 0) {
            count_records(filename, has_header);
        }
    }
    
    return EXIT_SUCCESS;
}

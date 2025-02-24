#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include "csv_functions.h"


// tests not needed for PRE

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 8

void runTest_min_field(int field_index, const char *filename, int expected) {
    int actual = min_field(field_index, filename);
    cr_assert_eq(actual, expected, "min_field(\"%s\") should have produced \"%d\", but returned \"%d\" instead.\n", filename, expected, actual);

}

void runTest_max_field(int field_index, const char *filename, int expected) {
    int actual = max_field(field_index, filename);
    cr_assert_eq(actual, expected, "max_field(\"%s\") should have produced \"%d\", but returned \"%d\" instead.\n", filename, expected, actual);

}

void runTest_mean_field(int field_index, const char *filename, double expected) {
    double actual = mean_field(field_index, filename);
    cr_assert_float_eq(actual, expected, 0.01, "mean_field(\"%s\") should have produced \"%.2f\", but returned \"%.2f\" instead.\n", filename, expected, actual);

}

// tests the min field
Test(test_min, test00) { runTest_min_field(0, "05020004-eng.csv", 1998); }
Test(test_min, test01) { runTest_min_field(7, "05020004-eng.csv", 0); }

// tests the max field
Test(test_max, test02) { runTest_max_field(0, "05020004-eng.csv", 2004); }
Test(test_max, test03) { runTest_max_field(7, "05020004-eng.csv", 100); }

// tests the mean field
Test(test_mean, test04) { runTest_mean_field(7, "05020004-eng.csv", 10.26); }
Test(test_mean, test05) { runTest_mean_field(0, "05020004-eng.csv", 2001.00); }

// tests all three at the same time
Test(test_min_max_mean, test06) {
    runTest_min_field(0, "05020004-eng.csv", 1998);
    runTest_max_field(0, "05020004-eng.csv", 2004);  // Replace 2023 with expected max value
    runTest_mean_field(7, "05020004-eng.csv", 10.26);  // Replace 7.0 with expected mean value
}





// tests to check parsing a field name, needs to make sure this works for the min, max, mean, and record functions
void runTest_parse_field_name(char header[MAX_FIELDS][MAX_LINE_LENGTH], int *num_fields) {
    // set up a mock header for the test
    strcpy(header[0], "Name");
    strcpy(header[1], "Age");
    strcpy(header[2], "City");
    *num_fields = 3;  // set the number of fields in the header
}

// tests for [0]
Test(test_parse_field_name, test07) {
    char header[MAX_FIELDS][MAX_LINE_LENGTH];
    int num_fields;
    
    // Initialize header with test data
    runTest_parse_field_name(header, &num_fields);
    
    // test for a valid field name that exists
    const char *field_name = "Name";
    int result = parse_field_name(field_name, header, &num_fields);
    cr_assert_eq(result, 0, "Expected index 0, but got %d", result);
}

// tests for [1]
Test(test_parse_field_name, test08) {
    char header[MAX_FIELDS][MAX_LINE_LENGTH];
    int num_fields;
    
    // Initialize header with test data
    runTest_parse_field_name(header, &num_fields);
    
    // test for a valid field name that exists
    const char *field_name = "Age";
    int result = parse_field_name(field_name, header, &num_fields);
    cr_assert_eq(result, 1, "Expected index 1, but got %d", result);
}

// tests for [2]
Test(test_parse_field_name, test09) {
    char header[MAX_FIELDS][MAX_LINE_LENGTH];
    int num_fields;
    
    // Initialize header with test data
    runTest_parse_field_name(header, &num_fields);
    
    // test for a valid field name that exists
    const char *field_name = "City";
    int result = parse_field_name(field_name, header, &num_fields);
    cr_assert_eq(result, 2, "Expected index 2, but got %d", result);
}
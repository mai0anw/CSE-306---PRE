#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include "csv.h"

void redirect_stdout(void) {
    cr_redirect_stdout();
}

Test(sample_test, test_name) {
    printf("Test is running...\n");
    cr_assert_eq(1, 1, "Should pass");
}


// Test for "csv -f 05020004-eng.csv"
Test(csv_tests, test_field_count, .init = redirect_stdout) {
    // Run csv program and redirect output to a temporary file
    int result = system("./csv -f 05020004-eng.csv > output.txt");
    
    // Ensure the program executed successfully
    cr_assert_eq(result, 0, "csv command failed to execute");

    // Open the output file and check its contents
    FILE *file = fopen("output.txt", "r");
    cr_assert_not_null(file, "Failed to open output.txt");

    char buffer[128];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    // Expected output (assuming the first row has 8 fields)
    cr_assert_str_eq(buffer, "8\n", "Incorrect number of fields returned");
}

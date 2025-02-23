#include <criterion/criterion.h>
#include "csv.h" // Include the header of your csv.c file


void runTest_count_fields(const char *filename, int expected) {
    int actual = count_fields(filename);
    cr_assert_eq( actual, expected, "count_fields(\"%s\") should have produced \"%d\", but returned \"%d\" instead.\n", filename, expected, actual);

}

Test(PRE, test00) { runTest_count_fields("05020004-eng.csv", 8); }
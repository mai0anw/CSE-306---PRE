#include <criterion/criterion.h>
#include "csv.h" // Include the header of your csv.c file


void runTest_min_field(int field_index, const char *filename, int expected) {
    int actual = min_field(field_index, filename);
    cr_assert_eq(actual, expected, "count_fields(\"%s\") should have produced \"%d\", but returned \"%d\" instead.\n", filename, expected, actual);

}

Test(PRE, test00) { runTest_min_field(0, "05020004-eng.csv", 8); }
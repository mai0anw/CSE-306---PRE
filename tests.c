#include <criterion/criterion.h>
#include "csv.h" // Include the header of your csv.c file

Test(min_field, valid_field) {
    // Test min_field with a valid field
    int result = min_field(2, "data.csv");
    cr_assert_eq(result, expected_min_value, "The minimum value should be correct.");
}

Test(mean_field, valid_field) {
    // Test mean_field with a valid field
    double result = mean_field(3, "data.csv");
    cr_assert_eq(result, expected_mean_value, "The mean value should be correct.");
}

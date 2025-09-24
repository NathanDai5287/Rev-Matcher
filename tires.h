#include <math.h>

const double wheel_diameter_in = 17;
const double sidewall_height_mm = 92.25;
const double sidewall_height_in = sidewall_height_mm / 25.4;
const double total_diameter_in = wheel_diameter_in + 2 * sidewall_height_in;
const double circumference_in = M_PI * total_diameter_in;
const double circumference_mi = circumference_in / 5280 / 12;

#include <float.h>
#include "tires.h"

const double final = 2.866;
const double ratios[] = {NAN, 5.087, 2.991, 2.035, 1.594, 1.286, 1};

const double match[] = {NAN, NAN, 1.70077, 1.46978, 1.27666, 1.2395, 1.286};
const double drop[] = {NAN, 0.58797, 0.68037, 0.78329, 0.80678, 0.7776};

static int get_closest_index(double target, const double arr[], int n) {
	int best_i = -1;
	double best = DBL_MAX;
	for (int i = 0; i < n; i++) {
		if (isnan(arr[i])) continue;

		double distance = fabs(arr[i] - target);
		if (distance < best) {
			best = distance;
			best_i = i;
		}
	}

	return best_i;
}

int get_current_gear(double mph, double rpm) {
	double wheel_rph = mph / circumference_mi;
	double wheel_rpm = wheel_rph / 60;
	double transmisstion_rpm = wheel_rpm * final;
	double ratio = rpm / transmisstion_rpm;

	int gear = get_closest_index(ratio, ratios, 7);

	return gear;
}

double get_current_speed(int gear, double rpm) {
	double transmission_rpm = rpm / ratios[gear];
	double wheel_rpm = transmission_rpm / final;
	double wheel_rph = wheel_rpm * 60;
	double wheel_mph = wheel_rph * circumference_mi;

	return wheel_mph;
}

double get_rev_match(int current_gear, double rpm) {
	return rpm * match[current_gear];
}

double get_rev_drop(int current_gear, double rpm) {
	return rpm * drop[current_gear];
}

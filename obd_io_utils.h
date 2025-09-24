#include <stdio.h>
#include <unistd.h>
#include <string.h>

const double ERROR = -1.0;

double get_rpm(int* fd) {
	const char* LIVE_RPM = "010C\r";
	write(*fd, LIVE_RPM, strlen(LIVE_RPM));

	char buf[128];
	int n = read(*fd, buf, sizeof(buf) - 1);
	if (n <= 0) return ERROR;
	buf[n] = 0;

	unsigned int a, b;
	int response = sscanf(buf, "%*x %*x %x %x", &a, &b);
	printf("Raw RPM response: %s\n", buf);
	if (response != 2) return ERROR;

	double rpm = ((a << 8) | b) / 4.0;
	return rpm;
}

double get_vehicle_speed(int* fd) {
	const char* LIVE_SPEED = "010D\r";
	write(*fd, LIVE_SPEED, strlen(LIVE_SPEED));

	char buf[128];
	int n = read(*fd, buf, sizeof(buf) - 1);
	if (n <= 0) return ERROR;
	buf[n] = 0;

	unsigned int a;
	int response = sscanf(buf, "%*x %*x %x", &a);
	printf("Raw SPEED response: %s\n", buf);
	if (response != 1) return ERROR;

	double kph = (double) a;
	double mph = kph / 1.609344;

	return mph;
}

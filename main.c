#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>

#include "obd_io_utils.h"
#include "gear_utils.h"

int main() {
	int fd = open("/dev/tty.usbserial", O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	struct termios tty;
	memset(&tty, 0, sizeof(tty));
	tcgetattr(fd, &tty);

	cfsetispeed(&tty, B38400);
	cfsetospeed(&tty, B38400);

	tty.c_cflag = CS8 | CREAD | CLOCAL;
	tty.c_iflag = 0;
	tty.c_oflag = 0;
	tty.c_lflag = 0;

	tcsetattr(fd, TCSANOW, &tty);

	// initialize ELM327 adapter
	const char* reset_adapter = "ATZ\r";
	const char* disable_echo = "ATE0\r";

	write(fd, reset_adapter, strlen(reset_adapter));
	sleep(1);
	write(fd, disable_echo, strlen(disable_echo));
	sleep(1);

	// get rpm
	while (true) {
		double rpm = get_rpm(&fd);
		double mph = get_vehicle_speed(&fd);

		int gear = get_current_gear(mph, rpm);
		int upshift_gear = gear + 1;
		int downshift_gear = gear - 1;

		double upshift_rpm = get_rev_drop(gear, rpm);
		double downshift_rpm = get_rev_match(gear, rpm);

		printf("%d - %d\n", gear, (int) round(rpm));

		printf("%d ↑ %d: %d (-%d, -%d%%)\n",
			gear, upshift_gear,
			(int) round(upshift_rpm),
			(int) round(rpm - upshift_rpm),
			(int) round(((rpm - upshift_rpm) / rpm) * 100)
		);

		printf("%d ↓ %d: %d (+%d, +%d%%)\n",
			gear, downshift_gear,
			(int) round(downshift_rpm),
			(int) round(downshift_rpm - rpm),
			(int) round(((downshift_rpm - rpm) / rpm) * 100)
		);

		usleep(200000);
	}

	close(fd);
	return 0;
}

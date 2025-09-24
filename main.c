#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>

#include "obd_io_utils.h"
#include "gear_utils.h"

// ELM327 initialization commands
static const char* const ELM327_RESET = "ATZ\r";
static const char* const ELM327_DISABLE_ECHO = "ATE0\r";
static const char* const ELM327_DISABLE_LINEFEEDS = "ATL0\r";
static const char* const ELM327_DISABLE_SPACES = "ATS0\r";
static const char* const ELM327_DISABLE_HEADERS = "ATH0\r";
static const char* const ELM327_SET_AUTO_PROTOCOL = "ATSP0\r";

int main() {
	int fd = open("/dev/tty.usbserial-D3A15F7V", O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	struct termios tty;
	memset(&tty, 0, sizeof(tty));
	tcgetattr(fd, &tty);

	const int baud_rate = B115200;
	cfsetispeed(&tty, baud_rate);
	cfsetospeed(&tty, baud_rate);

	tty.c_cflag = CS8 | CREAD | CLOCAL;
	tty.c_iflag = 0;
	tty.c_oflag = 0;
	tty.c_lflag = 0;

	tcsetattr(fd, TCSANOW, &tty);

	// initialize ELM327 adapter
	const char* init_commands[] = {
		ELM327_RESET,
		ELM327_DISABLE_ECHO,
		ELM327_DISABLE_LINEFEEDS,
		ELM327_DISABLE_SPACES,
		ELM327_DISABLE_HEADERS,
		ELM327_SET_AUTO_PROTOCOL
	};

	for (int i = 0; i < 6; i++) {
		const char* command = init_commands[i];
		write(fd, command, strlen(command));
		usleep(200000);
	}

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

		fflush(stdout);
		usleep(200000);
	}

	close(fd);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "log.h"

#define DEVICE_PATH "/dev/ttyACM0"
#define DEVICE_BAUD B9600
#define END_CHAR '\n'

void send_ring_msg(int arduino_fd, int ntimes)
{
	char out_buffer[100];
	snprintf(out_buffer, sizeof(out_buffer) - 1, "RINGN %d\n", ntimes);
	write(arduino_fd, out_buffer, strlen(out_buffer));
	fprintf(log_fp, "Sent data to arduino: %s\n", out_buffer);
}

int arduino_init(const char *path, struct termios *old_config, struct termios *new_config)
{
	int fd = open(path, O_RDWR | O_NOCTTY);
//	int fd = open(DEVICE_PATH, O_RDWR);


	if(fd == -1)
	{
		fprintf(log_fp, "Arduino not connected (could not open %s), exiting...\n", path);
		exit(EXIT_FAILURE);
	}

	fprintf(log_fp, "Arduino fd opened as %i\n", fd);
	tcgetattr(fd, old_config);
	new_config = old_config;
	cfsetispeed(new_config, DEVICE_BAUD);
	cfsetospeed(new_config, DEVICE_BAUD);

	new_config->c_cflag &= ~PARENB;
	new_config->c_cflag &= ~CSTOPB;
	new_config->c_cflag &= ~CSIZE;
	new_config->c_cflag |= CS8;
	/* Canonical mode */
	//new_config->c_lflag |= ICANON;
	new_config->c_lflag |= ICANON;
	/* commit the serial port settings */
	tcsetattr(fd, TCSANOW, new_config);
	return fd;
}

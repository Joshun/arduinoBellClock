#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "arduino.h"

#define HOUR_SLEEP 60*60

int initial_wait(void);
int hourly_wait(int current_hour);
void ring_bell(int fd, int hour);
int custom_wait(int current_hour, int divisor, int ard_fd);

enum OP_MODES { OP_HOURLY, OP_HALF_HOURLY, OP_QUARTER_HOURLY };

#define OP_MODE OP_QUARTER_HOURLY

int main(int argc, char **argv)
{
	int current_hour = 0;
	
	struct termios old_ard_config, new_ard_config;
	int ard_fd = arduino_init(&old_ard_config, &new_ard_config);
	
	current_hour = initial_wait();
	ring_bell(ard_fd, current_hour);
	
	while(1) {
		if( OP_MODE == OP_HALF_HOURLY ) {
			current_hour = custom_wait(current_hour, 2, ard_fd);
		}
		else if( OP_MODE == OP_QUARTER_HOURLY ) {
			current_hour = custom_wait(current_hour, 4, ard_fd);
		}
		else {
			current_hour = hourly_wait(current_hour);
		}
		ring_bell(ard_fd, current_hour);
	}	
	
	return 0;
}

int initial_wait(void)
{
	time_t current_time;
	time_t future_time;
	time_t timediff;
	struct tm *current_time_struct = NULL;
	struct tm future_time_struct;
	
	current_time = time(NULL);
	current_time_struct = localtime(&current_time);
	future_time_struct = *current_time_struct;
	
	future_time_struct.tm_sec = 0;
	future_time_struct.tm_min = 0;
	
	if( future_time_struct.tm_hour == 23 ) {
		future_time_struct.tm_hour = 0;
	}
	else {
		future_time_struct.tm_hour += 1;
	}
	
	future_time = mktime(&future_time_struct);
	timediff = difftime(future_time, current_time);
	printf("Initial wait: need to sleep for %ld secs (%ld minutes)\n", timediff, timediff / 60);
	sleep(timediff);
		
	return future_time_struct.tm_hour;
}

int hourly_wait(int current_hour)
{
	printf("Hourly wait: need to sleep for %d secs (%d minutes)\n", HOUR_SLEEP, HOUR_SLEEP / 60);
	sleep(HOUR_SLEEP);
	if( current_hour == 23 ) {
		return 0;
	}
	else {
		return current_hour + 1;
	}
}

int custom_wait(int current_hour, int divisor, int ard_fd)
{
	int wait_time = HOUR_SLEEP / divisor;
	
	int i;
	for(i=0; i<(divisor - 1); i++) {
		printf("Custom wait (cycle %d): need to sleep for %d secs (%d minutes)\n", i, wait_time, wait_time / 60);
		sleep(wait_time);
		ring_bell(ard_fd, 1);
	}
	sleep(wait_time);
	if( current_hour == 23 ) {
		return 0;
	}
	else {
		return current_hour + 1;
	}
}

void ring_bell(int ard_fd, int hour)
{
	int dings;
	if( hour == 0 )
		dings = 12;
	else if( hour > 12 )
		dings = hour - 12;
	else
		dings = hour;
	send_ring_msg(ard_fd, dings);
}

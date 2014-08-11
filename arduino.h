#include <termios.h>

void send_ring_msg(int arduino_fd, int ntimes);
int arduino_init(const char *path, struct termios *old_config, struct termios *new_config);

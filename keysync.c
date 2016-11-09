/*
 * Toy code to interface with the iBIZ KeySync serial keyboard for Pocket PC.
 * (C)2016 Mike Bourgeous (see LICENSE).
 */
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

static const char key_chars[128] = {
	// 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
	'a', 's', 'd', 'f', 'h', 'g', 'z', 'x', 'c', 'v',   0, 'b', 'q', 'w', 'e', 'r', // 0x00-0x0f
	'y', 't', '1', '2', '3', '4', '6', '5', '=', '9', '7', '-', '8', '0', ']', 'o', // 0x10-0x1f
	'u', '[', 'i', 'p', '\n','l', 'j', '\'','k', ';', '\\',',', '/', 'n', 'm', '.', // 0x20-0x2f
	'\t',' ', '`', '\b', // 0x30-0x33
	/* Shift, ctrl, etc. - 0x34..0x3f */
	/* Arrows - 0x7b..0x7e */
};

// Opens a serial port for completely raw I/O at 9600/8N1
int open_port(const char *port)
{
	struct termios terminfo;
	int fd;

	fd = open(port, O_RDONLY);
	if(fd == -1) {
		perror("open");
		return -1;
	}

	// Set terminal and line settings (echo, stop bits, etc.)
	if(tcgetattr(fd, &terminfo)) {
		perror("Error getting current port settings");
		close(fd);
		return -1;
	}

	terminfo.c_iflag = 0;
	terminfo.c_oflag = 0;
	terminfo.c_cflag = CS8 | CLOCAL | CREAD;
	terminfo.c_lflag = 0;

	memset(terminfo.c_cc, _POSIX_VDISABLE, sizeof(terminfo.c_cc));
	terminfo.c_cc[VMIN] = 1; // minimum 1 byte reads

	cfsetispeed(&terminfo, B9600);
	cfsetospeed(&terminfo, B9600);

	if(tcsetattr(fd, TCSANOW, &terminfo)) {
		perror("Error setting port settings");
		close(fd);
		return -1;
	}

	return fd;
}

int main(int argc, char *argv[])
{
	int fd;
	uint8_t c;
	ssize_t len;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s tty\n", argv[0]);
		fprintf(stderr, "Example: %s /dev/ttyUSB0\n", argv[0]);
		return -1;
	}

	fd = open_port(argv[1]);
	if(fd == -1) {
		return -1;
	}

	do {
		len = read(fd, &c, 1);
		if(len > 0 && (c & 0x80) && key_chars[c & 0x7f] != 0) {
			// Print character keypress events
			putchar(key_chars[c & 0x7f]);
			fflush(stdout);
		} else if(len < 0) {
			perror("read");
		}
	} while(len > 0);

	if(close(fd)) {
		perror("close");
		return -1;
	}

	return 0;
}

/*
 * Toy code to interface with the iBIZ KeySync serial keyboard for Pocket PC.
 * (C)2016 Mike Bourgeous (see LICENSE).
 */
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

static const char key_chars[128] = {
	// 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
	'a', 's', 'd', 'f', 'h', 'g', 'z', 'x', 'c', 'v',   0, 'b', 'q', 'w', 'e', 'r', // 0x00-0x0f
	'y', 't', '1', '2', '3', '4', '6', '5', '=', '9', '7', '-', '8', '0', ']', 'o', // 0x10-0x1f
	'u', '[', 'i', 'p', '\n','l', 'j', '\'','k', ';', '\\',',', '/', 'n', 'm', '.', // 0x20-0x2f
	'\t',' ', '`', // 0x30-0x32
	/* Shift, backspace, etc. - 0x33..0x3f */
	/* Arrows - 0x7b..0x7e */
};

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

	fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror("open");
		return -1;
	}

	do {
		len = read(fd, &c, 1);
		//printf("Read 0x%zd bytes: 0x%02x\n", len, c);
		if(len > 0 && (c & 0x80) && key_chars[c & 0x7f] != 0) {
			// Print character keypress events
			putchar(key_chars[c & 0x7f]);
			fflush(stdout);
		}
	} while(len > 0);

	if(close(fd)) {
		perror("close");
		return -1;
	}

	return 0;
}

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>

#define ADDRESS 0x378

void displayBits(unsigned value)
{
	unsigned c;

	/* Определить displayMask и сдвинуть влево на 7 бит */
	unsigned displayMask = 1 << 7;

	/* Цикл по битам */
	for (c = 1; c <= 8; c++)
	{
		putchar(value & displayMask ? '1' : '0');
		value <<= 1; /* сдвинуть value влево на 1 */
	}
	putchar('\n');
}

int main(int argc, char **argv)
{
	int fd;
	int param;
	char buff;

	if (argc > 2) {
		printf("Usage: %s [ 0 - 255 ]\n", argv[0]);
		exit(1);
	}

	if ((fd = open("/dev/port", O_RDWR)) < 0)
	{
		printf("Note: you must be superuser to run this program.\n");
		exit(1);
	}
	
	if (argc == 1)
	{
		lseek(fd, ADDRESS, SEEK_SET);
		read(fd, &buff, 1);

		printf("Current value: ");
		displayBits(buff);
	}
	else
	{
		param = atoi(argv[1]);

		if ((param < 0) || (param > 255)) {
			printf("Usage: %s [ 0 - 255 ]\n", argv[0]);
		}
		else
		{
			lseek(fd, ADDRESS, SEEK_SET);
			read(fd, &buff, 1);
			printf("Old value:     ");
			displayBits(buff);

			lseek(fd, ADDRESS, SEEK_SET);
			write(fd, &param, 1);
			
			lseek(fd, ADDRESS, SEEK_SET);
			read(fd, &buff, 1);
			printf("New value:     ");
			displayBits(buff);
		}
	
	}
	close(fd);

	return 0;
}

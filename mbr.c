#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct partition {
    unsigned char active;
    unsigned char start_head;
    unsigned char start_sector;
    unsigned char start_cylinder;
    unsigned char type;
    unsigned char end_head;
    unsigned char end_sector;
    unsigned char end_cylinder;
    unsigned int offset;
    unsigned int sectors_num;
};

#pragma pack(push, 1)
struct mbr {
    unsigned char loader[446];
    struct partition part[4];
    unsigned char sign[2];
};
#pragma pack(pop)

int main(int argc, char **argv) {
    int hdd;
    struct mbr record;
    int i;
    int e = EOF;

    if (argc != 2) {
	printf("Usage: %s <device>\n", argv[0]);
	return 1;
    }

    if ((hdd = open(argv[1], O_RDONLY)) != EOF) {
	read(hdd, &record, sizeof(record));

	if (record.sign[0] != 0x55 || record.sign[1] != 0xAA) {
	    printf("MBR signature error!\n");
	    return 1;
	}

	for (i = 0; (i < 4) && record.part[i].type; i++) {
	    if (record.part[i].type == 0x5)
		e = i;
	
	    printf("%s%d %s %0.2x %u %u\n",
		argv[1],
		i + 1,
		record.part[i].active == 0x80 ? "*" : " ",
		record.part[i].type,
		record.part[i].offset,
		record.part[i].sectors_num + record.part[i].offset - 1
	    );
	}

	if (e != EOF) {
	    struct mbr ext;
	    lseek64(hdd, (off64_t) (record.part[e].offset) * 512, SEEK_SET);
	    read(hdd, &ext, sizeof(ext));

	    if (ext.sign[0] != 0x55 || ext.sign[1] != 0xAA) {
	        printf("EBR signature error!\n");
	        return 1;
	    }

	    printf("%s%d %s %0.2x %u %u\n",
		argv[1],
		i + 1,
		record.part[i].active == 0x80 ? "*" : " ",
		ext.part[0].type,
		ext.part[0].offset + record.part[e].offset,
		ext.part[0].sectors_num + ext.part[0].offset + record.part[e].offset - 1
	    );
	}

	close(hdd);
    }

    return 0;
}

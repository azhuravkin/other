#ifndef _PACK_H
#define _PACH_H

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>

struct file {
	off_t begin;
	off_t size;
	char name[64];
};

#endif

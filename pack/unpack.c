#include "pack.h"

void usage(char *cmd) {
	fprintf(stderr, "Usage:\t%s <dir> <num> [filename]\n\n", cmd);
	exit(EXIT_FAILURE);
}

void create(char *file, struct file *f, int pack) {
	char buffer[8192];
	int fp;
	size_t num;
	size_t tail;
	size_t len;

	unlink(file);

	if ((fp = open(file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == EOF)
		exit(EXIT_FAILURE);

	for (tail = f->size; tail > 0; tail -= len) {
		if (tail < sizeof(buffer))
			num = tail;
		else
			num = sizeof(buffer);

		len = read(pack, buffer, num);
		write(fp, buffer, len);
	}

	close(fp);
}

int main(int argc, char **argv) {
	struct stat st;
	DIR *d;
	char idx_name[128];
	char idx_path[128];
	char pack_name[128];
	char pack_path[128];
	char file_path[128];
	struct file f;
	int idx;
	int pack;

	if (argc < 3 || argc > 4)
		usage(argv[0]);

	if (stat(argv[1], &st) || !S_ISDIR(st.st_mode))
		usage(argv[0]);

	if ((d = opendir(argv[1])) == NULL)
		usage(argv[0]);

	snprintf(idx_name, sizeof(idx_name), "%s.idx", argv[2]);
	snprintf(pack_name, sizeof(pack_name), "%s.pack", argv[2]);
	snprintf(idx_path, sizeof(idx_path), "%s/%s", argv[1], idx_name);
	snprintf(pack_path, sizeof(idx_path), "%s/%s", argv[1], pack_name);

	if ((idx = open(idx_path, O_RDONLY)) == EOF)
		usage(argv[0]);

	if ((pack = open(pack_path, O_RDONLY)) == EOF)
		usage(argv[0]);

	while (read(idx, &f, sizeof(f))) {
		snprintf(file_path, sizeof(file_path), "%s/%s", argv[1], f.name);

		if (argc == 4) {
		    if (!strcmp(f.name, argv[3])) {
			lseek(pack, f.begin, SEEK_SET);
			create(file_path, &f, pack);
			break;
		    }
		} else
		    create(file_path, &f, pack);
	}

	close(idx);
	close(pack);

	return 0;
}

#include "pack.h"

void usage(char *cmd) {
	fprintf(stderr, "Usage:\t%s <dir> <num>\n", cmd);
	exit(EXIT_FAILURE);
}

void append(char *file, int pack) {
	char buffer[8192];
	int fp;
	size_t num;

	if ((fp = open(file, O_RDONLY)) == EOF)
		exit(EXIT_FAILURE);

	while ((num = read(fp, buffer, sizeof(buffer))))
		write(pack, buffer, num);

	close(fp);
}

int main(int argc, char **argv) {
	struct stat st;
	struct dirent *de;
	DIR *d;
	char idx_name[128];
	char idx_path[128];
	char pack_name[128];
	char pack_path[128];
	char file_path[128];
	struct file f;
	int idx;
	int pack;

	if (argc != 3)
		usage(argv[0]);

	if (stat(argv[1], &st) || !S_ISDIR(st.st_mode))
		usage(argv[0]);

	if ((d = opendir(argv[1])) == NULL)
		usage(argv[0]);

	snprintf(idx_name, sizeof(idx_name), "%s.idx", argv[2]);
	snprintf(pack_name, sizeof(pack_name), "%s.pack", argv[2]);
	snprintf(idx_path, sizeof(idx_path), "%s/%s", argv[1], idx_name);
	snprintf(pack_path, sizeof(idx_path), "%s/%s", argv[1], pack_name);

	unlink(idx_path);

	if ((idx = open(idx_path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == EOF)
		usage(argv[0]);

	unlink(pack_path);

	if ((pack = open(pack_path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == EOF)
		usage(argv[0]);

	memset(&f, '\0', sizeof(f));

	while ((de = readdir(d))) {
		if (!strcmp(de->d_name, idx_name) || !strcmp(de->d_name, pack_name))
			continue;

		snprintf(file_path, sizeof(file_path), "%s/%s", argv[1], de->d_name);
		stat(file_path, &st);

		if (!S_ISREG(st.st_mode))
			continue;

		snprintf(f.name, sizeof(f.name), "%s", de->d_name);
		f.size = st.st_size;

		write(idx, &f, sizeof(f));
		f.begin += f.size;

		append(file_path, pack);
	}

	close(idx);
	close(pack);

	return 0;
}

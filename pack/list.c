#include "pack.h"

void usage(char *cmd) {
	fprintf(stderr, "Usage:\t%s <dir> <num>\n\n", cmd);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	struct stat st;
	char idx_name[128];
	char idx_path[128];
	struct file f;
	int idx;
	int i;

	if (argc != 3)
		usage(argv[0]);

	if (stat(argv[1], &st) || !S_ISDIR(st.st_mode))
		usage(argv[0]);

	snprintf(idx_name, sizeof(idx_name), "%s.idx", argv[2]);
	snprintf(idx_path, sizeof(idx_path), "%s/%s", argv[1], idx_name);

	if ((idx = open(idx_path, O_RDONLY)) == EOF)
		usage(argv[0]);

	for (i = 1; read(idx, &f, sizeof(f)); i++)
		printf("%d) %lu %s\n", i, f.size, f.name);

	close(idx);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct iface {
    char ifname[8];
    int in;
    int out;
};

struct entry {
    int date;
    int in;
    int out;
    int ifnum;
    struct iface i[0];
};

struct entry *addif(struct entry *e, char *ifname, int in, int out)
{
    struct iface *i;

    i = calloc(1, sizeof(struct iface));
    strcpy(i->ifname, ifname);
    i->in = in;
    i->out = out;

    e = realloc(e, sizeof(struct entry) + sizeof(struct iface) * (e->ifnum + 1));

    memcpy(&e->i[e->ifnum++], i, sizeof(struct iface));

    free(i);

    return e;
}

void print(const char *file)
{
    FILE *fp;
    struct entry e;
    struct iface iface;
    int i;
    
    if ((fp = fopen(file, "r")) == NULL)
	return;

    while (fread(&e, sizeof(struct entry), 1, fp)) {
	printf("date: %d in: %d out: %d\n", e.date, e.in, e.out);
    
    	for (i = 0; i < e.ifnum; i++) {
    	    fread(&iface, sizeof(struct iface), 1, fp);
	    printf("\tiface: %s, in: %d, out: %d\n", iface.ifname, iface.in, iface.out);
	}

	printf("\n");
    }

    fclose(fp);
}

int main(void) {
    struct entry *e;
    char *file = "file";

    e = malloc(sizeof(struct entry));
    memset(e, 0, sizeof(struct entry));
    e->date = 20100401;
    e->in = 100;
    e->out = 200;

    e = addif(e, "ppp0", 10, 20);
    e = addif(e, "ppp1", 11, 21);
    e = addif(e, "ppp2", 12, 22);

    FILE *fp = fopen(file, "w");
    fwrite(e, sizeof(struct entry) + sizeof(struct iface) * e->ifnum, 1, fp);

    free(e);

    e = malloc(sizeof(struct entry));
    memset(e, 0, sizeof(struct entry));
    e->date = 20100402;
    e->in = 101;
    e->out = 201;

    e = addif(e, "tun0", 30, 40);
    e = addif(e, "tun1", 31, 41);

    fwrite(e, sizeof(struct entry) + sizeof(struct iface) * e->ifnum, 1, fp);
    fclose(fp);

    free(e);

    print(file);

    return 0;
}

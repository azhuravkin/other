#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/route.h>
#include <arpa/inet.h>

struct route {
    uint32_t net;
    uint32_t mask;
    int metric;
    char dev[16];
};

static int cmp(const void *p1, const void *p2) {
    struct route *r1 = (struct route *) p1;
    struct route *r2 = (struct route *) p2;

    if (r1->mask < r2->mask)
	return 1;
    else if (r1->mask > r2->mask)
	return -1;
    if (r1->metric > r2->metric)
	return 1;
    else if (r1->metric < r2->metric)
	return -1;
    return 0;
}

int main(int argc, char **argv) {
    char buff[1024];
    FILE *fp;
    int i;
    int num = 0;
    struct route *routes = NULL;
    struct sockaddr_in addr;

    if (argc != 2) {
	return 1;
    }

    inet_aton(argv[1], &addr.sin_addr);

    if ((fp = fopen("/proc/net/route", "r"))) {
	while (fgets(buff, sizeof(buff), fp)) {
	    char dev[16];
	    uint32_t net;
	    uint32_t mask;
	    int iflags, metric;
	
	    if ((sscanf(buff, "%16s %X %*128s %X %*d %*d %d %X %*d %*d %*d\n",
		    dev, &net, &iflags, &metric, &mask) == 5) && (iflags & RTF_UP)) {
		routes = realloc(routes, sizeof(struct route) * (num + 1));
		routes[num].net = net;
		routes[num].mask = mask;
		routes[num].metric = metric;
		strcpy(routes[num].dev, dev);
		num++;
	    }
	}
	fclose(fp);
    }

    qsort(routes, num, sizeof(struct route), cmp);

    for (i = 0; i < num; i++) {
	if ((addr.sin_addr.s_addr & routes[i].mask) == routes[i].net) {
	    printf("%s\n", routes[i].dev);
	    break;
	}
    }

    if (routes) {
	free(routes);
    }

    return 0;
}

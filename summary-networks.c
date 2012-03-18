#include <stdio.h>
#include <stdlib.h>

struct network {
    unsigned int net, mask;
    struct network *next;
};

struct network *head = NULL;
int networks_num = 0;

unsigned int aton(unsigned char net[4]) {
    return ((net[0] << 24) + (net[1] << 16) + (net[2] << 8) + net[3]);
}

unsigned int mask_to_prefix(unsigned int mask) {
    int i;

    for (i = 0; i < 32; i++)
	if (((1 << (31 - i)) & mask) == 0)
	    return i;

    return 32;
}

unsigned int prefix_to_mask(unsigned int prefix) {
    return (0xFFFFFFFF << (32 - prefix));
}

void print_list(void) {
    struct network *cur;

    for (cur = head; cur; cur = cur->next)
	printf("%hhu.%hhu.%hhu.%hhu/%u\n",
	    (cur->net & 0xFF000000) >> 24,
	    (cur->net & 0xFF0000) >> 16,
	    (cur->net & 0xFF00) >> 8,
	    (cur->net & 0xFF),
	    mask_to_prefix(cur->mask));
}

int cmp(const void *p1, const void *p2) {
    const struct network **e1 = (const struct network **) p1;
    const struct network **e2 = (const struct network **) p2;

    if ((*e1)->mask > (*e2)->mask) return -1;
    if ((*e1)->mask < (*e2)->mask) return 1;

    if ((*e1)->net < (*e2)->net) return -1;
    if ((*e1)->net > (*e2)->net) return 1;

    return 0;
}

void sort(void) {
    struct network *routes[networks_num];
    struct network *cur;
    int i = 0;

    if (networks_num < 2)
	return;

    for (cur = head; cur; cur = cur->next)
	routes[i++] = cur;

    qsort(routes, networks_num, sizeof(struct network *), cmp);

    head = routes[0];

    for (i = 0; i < networks_num - 1; i++)
	routes[i]->next = routes[i + 1];

    routes[i]->next = NULL;
}

int match(struct network *cur, struct network *next) {
    if ((cur->net & next->mask) == (next->net & next->mask))
	return 1;

    return 0;
}

void into(void) {
    int del = 0;
    struct network *cur, *next, *prev, *tmp;

    for (cur = head; cur; cur = next) {
	next = cur->next;
	for (tmp = cur->next; tmp; tmp = tmp->next) {
	    if (match(cur, tmp)) {
		// Delete route
		if (cur == head)
		    head = next;
		else
		    prev->next = next;

		free(cur);
		del = 1;
		networks_num--;
		break;
	    }
	}

	if (del)
	    del = 0;
	else
	    prev = cur;
    }
}

int glue(void) {
    struct network *cur, *next;
    unsigned int mask;
    int glued = 0;

    for (cur = head; cur; cur = next) {
	next = cur->next;

	if (next && cur->mask == next->mask) {
	    mask = prefix_to_mask(mask_to_prefix(cur->mask) - 1);

	    if ((cur->net & mask) == (next->net & mask)) {
		cur->mask = mask;
		// Delete route
		cur->next = next->next;
		free(next);
		next = cur->next;
		networks_num--;
		glued = 1;
	    }
	}
    }

    return glued;
}

void free_list(void) {
    struct network *cur, *next;

    for (cur = head; cur; cur = next) {
	next = cur->next;
	free(cur);
    }
    networks_num = 0;
}

int main(void) {
    struct network *cur, *prev;
    char buffer[128];
    int num;

    while (fgets(buffer, sizeof(buffer), stdin)) {
	unsigned char net[4];
	unsigned int prefix;

	num = sscanf(buffer, "%hhu.%hhu.%hhu.%hhu/%u\n",
	    &net[0], &net[1], &net[2], &net[3], &prefix);
	
	if (num != 5) continue;

	cur = malloc(sizeof(struct network));

	cur->net = aton(net);
	cur->mask = prefix_to_mask(prefix);
	cur->next = NULL;

	if (!head)
	    head = cur;
	else
	    prev->next = cur;

	prev = cur;
	networks_num++;
    }

    sort();
    into();

    while (glue()) sort();

    print_list();
    free_list();

    return 0;
}

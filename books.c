#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct book {
    char name[128];
    float cost;
    unsigned year;
};

struct book *books = NULL;
int num = 0;

void delete_book(int ind)
{
    struct book *new;
    int i, j;

    new = (struct book *) malloc(sizeof(struct book) * (num - 1));

    for (i = j = 0; i < num; i++) {
	if (i == ind - 1)
	    continue;
	new[j] = books[i];
	j++;
    }

    free(books);
    books = new;
    num--;
}

int name_cmp(const void *p1, const void *p2)
{
    const struct book *e1 = (const struct book *) p1;
    const struct book *e2 = (const struct book *) p2;

    return strcmp(e1->name, e2->name);
}

int cost_cmp(const void *p1, const void *p2)
{
    const struct book *e1 = (const struct book *) p1;
    const struct book *e2 = (const struct book *) p2;

    if (e1->cost < e2->cost) return -1;
    if (e1->cost > e2->cost) return 1;

    return 0;
}

int year_cmp(const void *p1, const void *p2)
{
    const struct book *e1 = (const struct book *) p1;
    const struct book *e2 = (const struct book *) p2;

    if (e1->year < e2->year) return -1;
    if (e1->year > e2->year) return 1;

    return 0;
}

int main(void)
{
    char c = '\0';
    int i;
    char file[128];
    FILE *fp;

    do {
	switch (c) {
	    case '\0':
		break;
	    case '1':
		if (books)
		    books = (struct book *) realloc(books, sizeof(struct book) * (num + 1));
		else
		    books = (struct book *) calloc(1, sizeof(struct book));

		printf("Enter Name: ");
		scanf("%128s", books[num].name);

		printf("Enter Cost: ");
		scanf("%f", &books[num].cost);

		printf("Enter Year: ");
		scanf("%u", &books[num].year);

		num++;
		break;
	    case '2':
		if (num)
		    printf("\n%-4s%-20s%-12s%s\n-----------------------------------------\n", "No", "Name", "Cost", "Year");
		for (i = 0; i < num; i++)
		    printf("%-4d%-20s%-12.2f%u\n", i + 1, books[i].name, books[i].cost, books[i].year);
		break;
	    case '3':
		printf("Sorted by name\n");
		qsort(books, num, sizeof(struct book), name_cmp);
		break;
	    case '4':
		printf("Sorted by cost\n");
		qsort(books, num, sizeof(struct book), cost_cmp);
		break;
	    case '5':
		printf("Sorted by year\n");
		qsort(books, num, sizeof(struct book), year_cmp);
		break;
	    case '6':
		printf("Enter file name: ");
		scanf("%128s", file);

		if ((fp = fopen(file, "w")) == NULL)
		    printf("Error opening file %s\n", file);

		if (num)
		    fprintf(fp, "%-4s%-20s%-12s%s\r\n-----------------------------------------\r\n", "No", "Name", "Cost", "Year");

		for (i = 0; i < num; i++)
		    fprintf(fp, "%-4d%-20s%-12.2f%u\r\n", i + 1, books[i].name, books[i].cost, books[i].year);

		fclose(fp);
		printf("Saved list to %s\n", file);
		break;
	    case '7':
		printf("Enter number book for deleting: ");
		scanf("%d", &i);

		if (i > num)
		    printf("Invalid number %d\n", i);
		else
		    delete_book(i);
		break;
	    case '\r':
	    case '\n':
		continue;
	    default:
		printf("Invalid input %c\n\n", c);
		break;
	}

	printf("\n1 - Input new book\n");
	printf("2 - Print all books\n");
	printf("3 - Sort books by name\n");
	printf("4 - Sort books by cost\n");
	printf("5 - Sort books by year\n");
	printf("6 - Save list to file\n");
	printf("7 - Delete book\n");
	printf("9 - Exit\n\n");
	printf("> ");
    } while ((c = getchar()) != '9');

    if (books)
	free(books);

    return 0;
}

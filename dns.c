#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void) {
	struct addrinfo *a;
	char *host = "tut.by";

	getaddrinfo(host, NULL, NULL, &a);

	struct sockaddr_in *saddr = (struct sockaddr_in *) a->ai_addr;
	printf("hostname: %s\n", inet_ntoa(saddr->sin_addr));

	return 0;
}

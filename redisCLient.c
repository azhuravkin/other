#include <stdio.h>
#include <hiredis/hiredis.h>

char *redisStatus[] = { "", "String", "Array", "Integer", "Nil", "Status", "Error" };

int main(int argc, char **argv) {
    int i;
    redisReply *reply;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s \"<command>\"\n", argv[0]);
	return 1;
    }

    redisContext *ctx = redisConnect("127.0.0.1", 6379);
    reply = redisCommand(ctx, argv[1]);

    if (reply) {
	printf("Type: %s\n\n", redisStatus[reply->type]);

	switch (reply->type) {
	case REDIS_REPLY_ERROR:
	    puts(reply->str);
	    break;
	case REDIS_REPLY_INTEGER:
	    printf("%ld\n", reply->integer);
	    break;
	case REDIS_REPLY_STRING:
	    puts(reply->str);
	    break;
	case REDIS_REPLY_ARRAY:
	    for (i = 0; i < reply->elements; i++)
		puts(reply->element[i]->str);
	    break;
	}
    }

    freeReplyObject(reply);
    redisFree(ctx);

    return 0;
}

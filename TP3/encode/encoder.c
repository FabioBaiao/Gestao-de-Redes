#include <stdio.h>
#include <arpa/inet.h>
#include <PRIM_set_request.h>
#include <PRIM_get_request.h>
#include <PRIM_get_next.h>
#include <PRIM_get_bulk.h>

typedef struct tuple {
	char* prim;
	char** args;
	int argc;
} *TUPLE;

TUPLE lineHandler(char* line) {
	TUPLE res = calloc(1, sizeof(struct tuple));
	int size = 20, i = 0;
	res->args = (char**) malloc(size * sizeof(char *));
	res->prim = strtok(line, " ");
	char* token = strtok(NULL, " ");
	do {
		res->args[i] = strdup(token);
		token = strtok(NULL, " ");
		i += 1;
		if (i%20 == 0 && token != NULL) {
			size += 10;
			res->args = realloc(res->args, size);
		}
	} while(token != NULL);
	res->argc = i;

	return res;
}

TUPLE argvHandler(char* argv[], int n) {
	TUPLE res = calloc(1, sizeof(struct tuple));
	res->prim = argv[0];
	res->argc = n-1;
	res->args = (char **) malloc((n-1) * sizeof(char *));
	int i;
	for (i = 0; i < n-1; i++)
		res->args[i] = argv[i+1];

	return res;
}

RES parsePrim(long reqID, char* line, char* argv[], int n) {
	TUPLE res;

	if (line != NULL)
		res = lineHandler(line);
	else
		res = argvHandler(argv, n);

	if (!strcmp(res->prim, "set-request")){
		/**	ARGS
			≃> requestID  c_str  version  [data_type  value  oid]
		*/
		return setReqHandler(reqID, res->args[0], res->args[1], res->args+2, res->argc-2);
	}
	if (!strcmp(res->prim, "get-request")){
		/** ARGS
			=> requestID  c_str  version  [oid.0]
		*/
		//return getReqHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "get-next-request")){
		/** ARGS
			=> requestID  c_str  version  [oid]
		*/
		//return getNextHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "get-bulk-request")){
		/** ARGS
			=> requestID  c_str  version  non-repeaters  max-rep  [oid]
		*/
		//return getBulkHandler(reqID, res->args, res->argc);
	}
/*	if (!strcmp(res->prim, "response")){
		/** ARGS
			=> requestID  errIndex  errStat  [(instance, type, value)]
		//
		return responseHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "trap-notification")){
		/** ARGS
			=> requestID  c_str  version  uptime  trap-oid  [(instance, type, value)]
		//
		return trapHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "inform")){
		/** ARGS
			=> requestID  c_str  version  uptime  trap-oid  [(instance, type, value)]
		//
		return informHandler(reqID, res->args, res->argc);
	}*/
}

int main(int argc, char** argv) {

	long requestID = 0;

	struct sockaddr_in addr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	socklen_t sock_size;

	if (argc > 1 && !strcmp(argv[1], "socket")) {
		addr.sin_family = AF_INET;
		addr.sin_port = htons(9999);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		sock_size = sizeof(addr);
	}

	char* line = calloc(255, sizeof(char));

	while(fgets(line, 255, stdin) != NULL) {
		RES res = parsePrim(requestID++, line, NULL, -1);

		if (argc > 1 && !strcmp(argv[1], "socket")) {
			int sent =
				sendto(sock, res->buff, res->size, 0, (struct sockaddr *)&addr, sock_size);
		}
		else {
			printf("%ld\n", res->size);
			fwrite(res->buff, sizeof(uint8_t), res->size, stdout);
			printf("\n");
		}
	}

	return 0;
}

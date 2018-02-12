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
	int size = 5, i = 0;
	res->args = (char**) malloc(size * sizeof(char *));
	res->prim = strtok(line, " ");
	char* token = strtok(NULL, " ");
	do {
		res->args[i] = strdup(token);
		token = strtok(NULL, " ");
		i += 1;
		if (i%5 == 0 && token != NULL) {
			size += 5;
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

	if (!strcmp(res->prim, "snmpset")){
		/**	ARGS
			≃> requestID  data_type  value  oid  c_str  version
		*/
		return setReqHandler(reqID, res->args[0], res->args[1], res->args+2);
	}
/*	if (!strcmp(res->prim, "snmpget")){
		/** ARGS
			=> requestID  c_str  version  [oid.0]
		//
		return getReqHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "snmpgetnext")){
		/** ARGS
			=> requestID  c_str  version  [oid]
		//
		return getNextHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "snmpbulkget")){
		/** ARGS
			=> requestID  c_str  version  non-repeaters  max-rep  [oid]
		//
		return getBulkHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "response")){
		/** ARGS
			=> requestID  errIndex  errStat  [(instance, type, value)]
		//
		return responseHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "snmptrap")){
		/** ARGS
			=> requestID  c_str  version  uptime  trap-oid  [(instance, type, value)]
		//
		return trapHandler(reqID, res->args, res->argc);
	}
	if (!strcmp(res->prim, "snmpinform")){
		/** ARGS
			=> requestID  c_str  version  uptime  trap-oid  [(instance, type, value)]
		//
		return informHandler(reqID, res->args, res->argc);
	}*/
}

int main(int argc, char** argv) {

	long requestID = 0;

	if (argc > 2) {
		/** INPUT FROM THE CMD LINE
			=> ./encoder  PRIMITIVE  [DATA_TYPE]  [VALUE]  OID  C_STR  VERSION
			EXAMPLE
			=> ./encoder  set-request  integer  7  1.3.6.1.2.1.25.2.3.1.4  public  -v2c
		*/
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(9999);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		int sock = socket(AF_INET, SOCK_DGRAM, 0);
		socklen_t sock_size = sizeof(addr);

		RES res = parsePrim(requestID, NULL, argv+1, argc-1);
		int sent =
			sendto(sock, res->buff, res->size, 0, (struct sockaddr *)&addr, sock_size);
	}
	else
		if (argc == 2) {
			/** INPUT FROM FILE
				=> ./encoder FILE_NAME
			*/
			printf("=> Looking for the file to read...\n");
			FILE *fp = fopen(*(argv+1), "r");

			if (fp != NULL) {
				printf("=> File successfully opened!\n");
				FILE *out = fopen("SNMP_output.txt", "w");
				char* line = (char*) malloc(255);

				while (!feof(fp)) {
					if (fgets(line, 255, fp) != NULL){
						RES res = parsePrim(requestID, line, NULL, -1);
						fwrite(res->buff, sizeof(uint8_t), res->size, out);
					}
				}

				free(line);
				fclose(fp);
				fclose(out);
				printf("Encoding finished! Output:\n");
			    system("cat SNMP_output.txt");
			}
			else
				printf("File couldn't be found or opened :(\n");
		}
		else
			printf("=> Not enough arguments were given.\n");

	return 0;
}

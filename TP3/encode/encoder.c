#include <stdio.h>
#include <arpa/inet.h>
#include <PRIM_set_request.h>

uint8_t* parsePrim(long reqID, char* line, char* argv[]) {
	char* prim;
	char* args[5];
	if (line != NULL) {
		prim = strtok(line, " ");
		args[0] = strtok(NULL, " ");
		args[1] = strtok(NULL, " ");
		args[2] = strtok(NULL, " ");
		args[3] = strtok(NULL, " ");
		args[4] = strtok(NULL, " ");
	}
	else {
		prim = argv[0];
		args[0] = argv[1];
		args[1] = argv[2];
		args[2] = argv[3];
		args[3] = argv[4];
		args[4] = argv[5];
	}

	if (!strcmp(prim, "set-request")){
		/**	ARGS
			≃> requestID  data_type  value  [oid,  c_str,  version]
		*/

		return simple_setRequest(reqID, args[0], args[1], args+2);
	}
	if (!strcmp(prim, "get-request")){
		/** ... */
	}
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

		uint8_t* buff = parsePrim(requestID, NULL, argv+1);
		int sent =
			sendto(sock, buff, 1024, 0, (struct sockaddr *)&addr, sock_size);
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
					if (fgets(line, 255, fp) != NULL)
						fprintf(out, "%s\n", parsePrim(requestID, line, NULL));
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

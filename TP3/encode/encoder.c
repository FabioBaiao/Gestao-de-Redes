#include <stdio.h>
#include <PRIM_set_request.h>

void parsePrim(long reqID, char* line, char* argv[]) {
	char* prim;
	char* args[5];
	if (line != NULL) {
		prim = strtok(*line, " ");
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

		simple_setRequest(reqID, args[0], args[1], args+2);
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
		*/
		parsePrim(requestID, NULL, argv+1);		
	}

	if (argc == 2) {
		/** INPUT FROM FILE
			=> ./encoder FILE_NAME
		*/
		printf("=> Looking for the file to read...");
		FILE *fp = fopen(*(argv+1), "r");
		
		if (fp != NULL) {
			printf("=> File successfully opened!\n");
			char* line = (char*) malloc(255);

			while (!feof(fp)) {
				if (fgets(line, 255, fp) != NULL)
					parsePrim(requestID, &line, NULL);
			}
			
			free(line);
		}
		else
			printf("File couldn't be found or opened :(\n");
	}
	else
		printf("=> Not enough arguments were given.\n");

	return 0;
}

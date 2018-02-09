#include <stdio.h>
#include <PRIM_set_request.h>

int main(int argc, char** argv) {
	
	long requestID = 0;
	
	if (argc > 2) {
		/** INPUT FROM THE CMD LINE
			=> ./encoder  PRIMITIVE  [DATA_TYPE]  [VALUE]  OID  C_STR  VERSION
		*/
		char* prim = *(argv+1);

		if (!strcmp(prim, "set-request")){
			/**	ARGS
				≃> requestID  data_type  value  [oid,  c_str,  version]
			*/
			simple_setRequest(requestID, *(argv+2), *(argv+3), argv+4); 
		}
		if (!strcmp(prim, "get-request")){
			/** ... */
		}
	}

	if (argc == 2) {
		/** INPUT FROM FILE
			=> ./encoder FILE_NAME
		*/
		printf("=> Looking for the file to read...");
		FILE *fp = fopen(argv[1], "r");
		
		if (fp != NULL) {
			printf("=> File successfully opened!\n");
			char* buf = (char*) malloc(255);
			fgets(buf, 255, fp);
			char* token = strtok(buf, " ");

			free(buf);
		}
		else
			printf("File couldn't be found or opened :(\n");
	}

	else
		printf("=> Not enough arguments were given.\n");

	return 0;
}

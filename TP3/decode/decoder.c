#include <decodeAPI.h>

void printDecode(char** result, int size) {
  int i = 0;
  printf("C_STR: %s\n", result[i++]);
  printf("VERSION: %s\n", result[i++]);
  char* primitive = result[i];
  printf("PRIMITIVE: %s\n", result[i++]);
  printf("REQUEST ID: %s\n", result[i++]);
  if (!strcmp(primitive, "set-request")) {
    printf("DATA TYPE: %s\n", result[i++]);
    printf("VALUE: %s\n", result[i++]);
    printf("OID: %s\n", result[i++]);
  }
  else if (!strcmp(primitive, "get-bulk-request")) {
    printf("NON REPEATERS: %s\n", result[i++]);
    printf("MAX REPETITIONS: %s\n", result[i++]);
    while (i < size)
      printf("OID: %s\n", result[i++]);
  }
  else {
    while (i < size)
      printf("OID: %s\n", result[i++]);
  }

  printf("\n");
}

void readFromSocket() {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9999);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  socklen_t udp_socket_size = sizeof(addr);
  bind(sock, (struct sockaddr *)&addr, udp_socket_size);

  while (1) {
    size_t buffer_size = 1024;
    uint8_t *buffer = calloc(1024, sizeof(uint8_t));

    int recv = recvfrom(sock, buffer, buffer_size, 0, (struct sockaddr *)&addr, &udp_socket_size);

    char** result = calloc(8, sizeof(char*));
    int size = decode_snmp(buffer, recv, result);
    printDecode(result, size);
  }
}

int main(int argc, char const *argv[]) {

  if (argc > 1 && !strcmp(argv[1], "socket")) {
    readFromSocket();
  }
  else {
    size_t buffer_size = 1024;
    uint8_t *buffer = calloc(buffer_size, sizeof(uint8_t));
    int size;
    while (scanf("%d\n", &size) > 0) {
      int recv = fread(buffer, sizeof(uint8_t), size, stdin);

      char** result = calloc(8, sizeof(char*));
      size = decode_snmp(buffer, recv, result);
      printDecode(result, size);

      getchar();
    }
  }
  return 0;
}

#include <Message.h>
#include <PDUs.h>

void printOid(uint8_t* buf, int size) {
  printf("%d", buf[0]);
  for (size_t i = 1; i < size; i++) {
    printf(".%d", buf[i]);
  }
}

void decode_simple_syntax(SimpleSyntax_t* simple) {
  printf("DATA TYPE: ");
  switch (simple->present) {
    case SimpleSyntax_PR_integer_value:
      printf("integer\n");
      printf("VALUE: %ld\n", simple->choice.integer_value);
      break;
    case SimpleSyntax_PR_string_value:
      printf("string\n");
      printf("VALUE: %s\n", (char *) simple->choice.string_value.buf);
      break;
    case SimpleSyntax_PR_objectID_value:
      printf("objectID\n");
      printf("VALUE: ");
      printOid(simple->choice.objectID_value.buf, simple->choice.objectID_value.size);
      printf("\n");
  }
}

void decode_obj_syntax(ObjectSyntax_t* object_syntax) {
  switch (object_syntax->present) {
    case ObjectSyntax_PR_simple:
      decode_simple_syntax(&object_syntax->choice.simple);
      break;
  }
}

void decode_obj_name(ObjectName_t* object_name) {
  printf("OID: ");
  printOid(object_name->buf, object_name->size);
  printf("\n");
}

void decode_var_bind(VarBind_t* var_bind) {
  switch (var_bind->choice.present) {
    case choice_PR_value:
      decode_obj_syntax(&var_bind->choice.choice.value);
      break;
  }
  decode_obj_name(&var_bind->name);
}

void decode_var_bindings(VarBindList_t var_bindings) {
  for (int i = 0; i < var_bindings.list.count; i++) {
    decode_var_bind(var_bindings.list.array[i]);
  }
}

void decode_pdus(PDUs_t* pdu) {
  printf("PRIMITIVE: ");
  switch (pdu->present) {
    case PDUs_PR_set_request:
      printf("set-request\n");
      decode_var_bindings(pdu->choice.set_request.variable_bindings);
      break;

    default:
      break;
  }
}

void decode_message(Message_t* message) {
  PDUs_t* pdu = 0;
  asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_PDUs, (void**)&pdu, message->data.buf, message->data.size);

  decode_pdus(pdu);

  printf("C_STR: %s\n", (char *) message->community.buf);
  printf("VERSION: %d\n", message->version);
}

void decode_snmp(uint8_t buffer_final[], size_t buffer_final_size) {
  Message_t* message = 0;
  asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_Message, (void **)&message, buffer_final, buffer_final_size);

  //xer_fprint(stdout, &asn_DEF_Message, message);
  decode_message(message);
}

int main(int argc, char const *argv[]) {
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

    decode_snmp(buffer, recv);
    printf("\n");
  }
  return 0;
}

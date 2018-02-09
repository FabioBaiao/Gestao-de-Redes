#include <Message.h>
#include <PDUs.h>

void decode_var_bind(VarBind_t* var_bind) {
  
}

void decode_var_bindings(VarBindList_t var_bindings) {
  for (int i = 0; i < var_bindings.list.count; i++) {
    decode_var_bind(var_bindings.list.array[i]);
  }
}

void decode_pdus(PDUs_t pdu) {
  switch (pdu->present) {
    case PDUs_PR_set_request:
      decode_var_bindings(pdu->choice.set_request.variable_bindings);
      break;

    default:
  }
}

void decode_message(Message_t* message) {
  PDUs_t* pdu = 0;
  asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_PDUs, (void**)&pdu, message->data.buf, message->data.size);

  decode_pdus(pdu);

  VarBindList_t var_bindings = pdu->choice.set_request.variable_bindings;
  int var_list_size = var_bindings.list.count;
  VarBind_t* var_bind = var_bindings.list.array[0];
}

void decode_snmp(uint8_t buffer_final[], size_t buffer_final_size) {
  Message_t* message = 0;
  asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_Message, (void **)&message, buffer_final, buffer_final_size);

  xer_fprint(stdout, &asn_DEF_Message, message);
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
    uint8_t *buffer = calloc(1, 1024 * sizeof(uint8_t));
    int recv = recvfrom(sock, buffer, buffer_size, 0, (struct sockaddr *)&addr, &udp_socket_size);

    decode_snmp(buffer, recv);
  }
  return 0;
}

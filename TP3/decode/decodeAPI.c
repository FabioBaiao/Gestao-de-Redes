#include <decodeAPI.h>

// retorna string com oid
char* printOid(uint8_t* buf, int size) {
  char* result = calloc(32, sizeof(char));
  char aux[8];
  sprintf(result, "%d", buf[0]);
  for (size_t i = 1; i < size; i++) {
    sprintf(aux, ".%d", buf[i]);
    strcat(result, aux);
  }
  return result;
}

// retorna array com duas strings: DATA TYPE e VALUE
char** decode_simple_syntax(SimpleSyntax_t* simple) {
  char** result = calloc(2, sizeof(char*));
  switch (simple->present) {
    case SimpleSyntax_PR_integer_value:
      result[0] = strdup("integer");
      result[1] = calloc(8, sizeof(char));
      sprintf(result[1], "%ld", simple->choice.integer_value);
      break;
    case SimpleSyntax_PR_string_value:
      result[0] = strdup("string");
      result[1] = strdup((char *) simple->choice.string_value.buf);
      break;
    case SimpleSyntax_PR_objectID_value:
      result[0] = strdup("objectID");
      result[1] = printOid(simple->choice.objectID_value.buf, simple->choice.objectID_value.size);
      break;
  }
  return result;
}

// retorna array com duas strings: DATA TYPE e VALUE
char** decode_application_syntax(ApplicationSyntax_t* app) {
  char** result = calloc(2, sizeof(char*));
  switch (app->present) {
      case ApplicationSyntax_PR_ipAddress_value:
        result[0] = strdup("IpAddress");
        result[1] = strdup((char *) app->choice.ipAddress_value.buf);
        break;
      case ApplicationSyntax_PR_counter_value:
        result[0] = strdup("Counter32");
        result[1] = calloc(8, sizeof(char));
        sprintf(result[1], "%ld", app->choice.counter_value);
        break;
      case ApplicationSyntax_PR_timeticks_value:
        result[0] = strdup("TimeTicks");
        result[1] = calloc(16, sizeof(char));
        sprintf(result[1], "%ld", app->choice.timeticks_value);
        break;
      case ApplicationSyntax_PR_arbitrary_value:
        result[0] = strdup("Opaque");
        result[1] = strdup((char *) app->choice.arbitrary_value.buf);
        break;
      case ApplicationSyntax_PR_big_counter_value:
        result[0] = strdup("Counter64");
        result[1] = calloc(16, sizeof(char));
        unsigned long l;
        asn_INTEGER2ulong(&app->choice.big_counter_value, &l);
        sprintf(result[1], "%ld", l);
        break;
      case ApplicationSyntax_PR_unsigned_integer_value:
        result[0] = strdup("Unsigned32");
        result[1] = calloc(8, sizeof(char));
        sprintf(result[1], "%ld", app->choice.unsigned_integer_value);
        break;
  }
  return result;
}

// retorna array com duas strings: DATA TYPE e VALUE
char** decode_obj_syntax(ObjectSyntax_t* object_syntax) {
  switch (object_syntax->present) {
    case ObjectSyntax_PR_simple:
      return decode_simple_syntax(&object_syntax->choice.simple);
      break;
    case ObjectSyntax_PR_application_wide:
      return decode_application_syntax(&object_syntax->choice.application_wide);
      break;
  }
}

char* decode_obj_name(ObjectName_t* object_name) {
  return printOid(object_name->buf, object_name->size);
}

// retorna numero de posicoes preenchidas no array result
int decode_var_bind(VarBind_t* var_bind, char** result) {
  char** aux = NULL;
  switch (var_bind->choice.present) {
    case choice_PR_value:
      aux = decode_obj_syntax(&var_bind->choice.choice.value);
      result[0] = aux[0];
      result[1] = aux[1];
      break;
    default:
      break;
  }
  char* obj_name = decode_obj_name(&var_bind->name);;
  if (aux == NULL) {
    result[0] = strdup(obj_name);
    return 1;
  }
  else {
    result[2] = strdup(obj_name);
    return 3;
  }
}

// retorna numero de posicoes preenchidas no array result
int decode_var_bindings(VarBindList_t var_bindings, char** result) {
  int count = 0;
  for (int i = 0; i < var_bindings.list.count; i++) {
    int res = decode_var_bind(var_bindings.list.array[i], result);
    result += res;
    count += res;
  }
  return count;
}

// retorna numero de posicoes preenchidas no array result
int decode_pdus(PDUs_t* pdu, char** result) {
  int size;
  switch (pdu->present) {
    case PDUs_PR_set_request:
      result[0] = strdup("set-request");
      result[1] = calloc(16, sizeof(char));
      sprintf(result[1], "%ld", pdu->choice.set_request.request_id);
      size = decode_var_bindings(pdu->choice.set_request.variable_bindings, result+2);
      return size+2;
      break;
    case PDUs_PR_get_request:
      result[0] = strdup("get-request");
      result[1] = calloc(16, sizeof(char));
      sprintf(result[1], "%ld", pdu->choice.get_request.request_id);
      size = decode_var_bindings(pdu->choice.get_request.variable_bindings, result+2);
      return size+2;
      break;
    case PDUs_PR_get_next_request:
      result[0] = strdup("get-next-request");
      result[1] = calloc(16, sizeof(char));
      sprintf(result[1], "%ld", pdu->choice.get_next_request.request_id);
      size = decode_var_bindings(pdu->choice.get_next_request.variable_bindings, result+2);
      return size+2;
      break;
    case PDUs_PR_get_bulk_request:
      result[0] = strdup("get-bulk-request");
      result[1] = calloc(16, sizeof(char));
      sprintf(result[1], "%ld", pdu->choice.get_bulk_request.request_id);
      result[2] = calloc(16, sizeof(char));
      sprintf(result[2], "%ld", pdu->choice.get_bulk_request.non_repeaters);
      result[3] = calloc(16, sizeof(char));
      sprintf(result[3], "%ld", pdu->choice.get_bulk_request.max_repetitions);
      size = decode_var_bindings(pdu->choice.get_next_request.variable_bindings, result+4);
      return size+4;
    default:
      break;
  }
  return 0;
}

// retorna numero de posicoes preenchidas no array result
int decode_message(Message_t* message, char** result) {
  PDUs_t* pdu = 0;
  asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_PDUs, (void**)&pdu, message->data.buf, message->data.size);

  result[0] = strdup((char *) message->community.buf);
  result[1] = calloc(8, sizeof(char*));
  sprintf(result[1], "%ld", message->version);

  int size = decode_pdus(pdu, result+2);

  return size+2;
}

int decode_snmp(uint8_t buffer_final[], size_t buffer_final_size, char** result) {
  Message_t* message = 0;
  asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_Message, (void **)&message, buffer_final, buffer_final_size);

  //xer_fprint(stdout, &asn_DEF_Message, message);
  return decode_message(message, result);
}

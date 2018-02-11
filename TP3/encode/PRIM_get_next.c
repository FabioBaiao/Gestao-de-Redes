#include <PRIM_get_next.h>

uint8_t* buildMsg_getNext(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v) {
	ANY_t* data;
	data = calloc(1, sizeof(ANY_t));
	data->buf = buf;
	data->size = ret.encoded;

	Message_t* message;
	message = calloc(1, sizeof(Message_t));
	OCTET_STRING_t* os = calloc(1, sizeof(OCTET_STRING_t));
	OCTET_STRING_fromBuf(os, cs, -1);
	message->version = v;
	message->community = *os;
	message->data = *data;

	uint8_t* buf_final = calloc(1024, sizeof(uint8_t));
	asn_enc_rval_t ret_final =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_Message, message, buf_final, 1024);

	//xer_fprint(stdout, &asn_DEF_Message, message);
	return buf_final;
}

uint8_t* buildPDU_getNext(VarBindList_t* varlist, long reqID, char* cs, long v) {
	GetNextRequest_PDU_t* getNextRequestPDU;
	getNextRequestPDU = calloc(1, sizeof(GetNextRequest_PDU_t));
	getNextRequestPDU->request_id = reqID;
	getNextRequestPDU->error_index = 0;
	getNextRequestPDU->error_status = 0;
	getNextRequestPDU->variable_bindings = *varlist;

	PDUs_t *pdu;
	pdu = calloc(1, sizeof(PDUs_t));
	pdu->present = PDUs_PR_get_next_request;
	pdu->choice.get_next_request = *getNextRequestPDU;

	size_t buf_size = 1024;
	uint8_t* buf = calloc(buf_size, sizeof(uint8_t));
	asn_enc_rval_t ret =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_PDUs, pdu, buf, buf_size);

	return buildMsg_getNext(buf, ret, cs, v);
}

uint8_t* varBinding_getNext(long reqID, ObjectName_t* names[], char* cs, long v) {
	VarBind_t* var_bind[3];
	VarBindList_t* varlist;
	varlist = calloc(1, sizeof(VarBindList_t));
	int i = 0;

	do {
		var_bind[i] = calloc(1, sizeof(VarBind_t));
		var_bind[i]->name = *names[i];
		var_bind[i]->choice.present = choice_PR_unSpecified;
		var_bind[i]->choice.choice.unSpecified = -1;
		ASN_SEQUENCE_ADD(&varlist->list, var_bind[i]);
		i++;
	} while (names[i] != NULL);

	return buildPDU_getNext(varlist, reqID, cs, v);
}

uint8_t* getNextHandler(long reqID, char* args[], int n) {
	char *token, *oid_str, *c_str = args[0];
	long v = atol(args[1]);
	int j, i;
	ObjectName_t** oids = (ObjectName_t**) malloc(n * sizeof(ObjectName_t*));
	for (i = 0; i < n; i++)
		oids[i] = NULL;
	size_t oid_size = 16;
	uint8_t* oid;
	i = 0;

	do {
		oid_str = args[i+2];
		oids[i] = calloc(1, sizeof(ObjectName_t));
		oid = calloc(oid_size, sizeof(uint8_t));
		j = 0;
		token = strtok(oid_str, ".");

		do {
			oid[j++] = atoi(token);
			token = strtok(NULL, ".");
		}
		while (token != NULL);

		oids[i]->buf = oid;
		oids[i]->size = j;
		i++;
	} while(i+2 < n);

	return varBinding_getNext(reqID, oids, c_str, v);
}

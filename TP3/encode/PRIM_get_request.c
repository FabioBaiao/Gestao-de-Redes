#include <PRIM_get_request.h>
#include <encodeAPI.h>

RES buildPDU_getReq(VarBindList_t* varlist, long reqID, char* cs, long v) {
	GetRequest_PDU_t* getRequestPDU;
	getRequestPDU = calloc(1, sizeof(GetRequest_PDU_t));
	getRequestPDU->request_id = reqID;
	getRequestPDU->error_index = 0;
	getRequestPDU->error_status = 0;
	getRequestPDU->variable_bindings = *varlist;

	PDUs_t *pdu;
	pdu = calloc(1, sizeof(PDUs_t));
	pdu->present = PDUs_PR_get_request;
	pdu->choice.get_request = *getRequestPDU;

	size_t buf_size = 1024;
	uint8_t* buf = calloc(buf_size, sizeof(uint8_t));
	asn_enc_rval_t ret =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_PDUs, pdu, buf, buf_size);

	return buildMsg(buf, ret, cs, v);
}

RES varBinding_getReq(long reqID, ObjectName_t* names[], char* cs, long v) {
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

	return buildPDU_getReq(varlist, reqID, cs, v);
}

RES getReqHandler(long reqID, char* args[], int n) {
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

	return varBinding_getReq(reqID, oids, c_str, v);
}

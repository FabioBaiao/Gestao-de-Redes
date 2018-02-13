#include <PRIM_get_bulk.h>

RES buildPDU_getBulk(VarBindList_t* varlist, long reqID, char* cs, long v, long nr, long maxr) {
	GetBulkRequest_PDU_t* getBulkRequestPDU;
	getBulkRequestPDU = calloc(1, sizeof(GetBulkRequest_PDU_t));
	getBulkRequestPDU->request_id = reqID;
	getBulkRequestPDU->non_repeaters = nr;
	getBulkRequestPDU->max_repetitions = maxr;
	getBulkRequestPDU->variable_bindings = *varlist;

	PDUs_t *pdu;
	pdu = calloc(1, sizeof(PDUs_t));
	pdu->present = PDUs_PR_get_bulk_request;
	pdu->choice.get_bulk_request = *getBulkRequestPDU;

	size_t buf_size = 1024;
	uint8_t* buf = calloc(buf_size, sizeof(uint8_t));
	asn_enc_rval_t ret =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_PDUs, pdu, buf, buf_size);

	return buildMsg(buf, ret, cs, v);
}

RES varBinding_getBulk(long reqID, ObjectName_t* names[], char* cs, long v, long nr, long maxr) {
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

	return buildPDU_getBulk(varlist, reqID, cs, v, nr, maxr);
}

RES getBulkHandler(long reqID, char* args[], int n) {
	char *token, *oid_str, *c_str = args[0];
	long v = atol(args[1]);
	int noRep = atol(args[2]);
	int maxRep = atol(args[3]);
	int j, i;
	ObjectName_t** oids = (ObjectName_t**) malloc(n * sizeof(ObjectName_t*));
	for (i = 0; i < n; i++)
		oids[i] = NULL;
	size_t oid_size = 16;
	uint8_t* oid;
	i = 0;

	do {
		oid_str = args[i+4];
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
	} while(i+4 < n);

	return varBinding_getBulk(reqID, oids, c_str, v, noRep, maxRep);
}

#include <PRIM_set_request.h>
/* Including external dependencies */
#include <asn_application.h>
#include <asn_internal.h>
#include <OCTET_STRING.h>
#include <OBJECT_IDENTIFIER.h>
#include <IpAddress.h>
#include <Counter32.h>
#include <Counter64.h>
#include <Opaque.h>
#include <Unsigned32.h>
#include <TimeTicks.h>
#include <asn_codecs_prim.h>
#include <PDUs.h>
#include <SetRequest-PDU.h>
#include <VarBind.h>
#include <ANY.h>
#include <Message.h>

void buildMsg(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v) {
	ANY_t* data;
	data = calloc(1, sizeof(ANY_t));
	data->buf = buf;
	data->size = ret.encoded;

	Message_t* message;
	message = calloc(1, sizeof(Message_t));
	message->version = v;
	OCTET_STRING_t* os = calloc(1, sizeof(OCTET_STRING_t));
	OCTET_STRING_fromBuf(os, cs, -1);
	message->community = *os;
	message->data = *data;

	uint8_t* buf_final[64];
	asn_enc_rval_t ret_final =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_Message, message, buf_final, sizeof(buf_final));

	xer_fprint(stdout, &asn_DEF_Message, message);
}

void buildPDU(VarBindList_t* varlist, long reqID, char** tail) {
	SetRequest_PDU_t* setRequestPDU;
	setRequestPDU = calloc(1, sizeof(SetRequest_PDU_t));
	setRequestPDU->request_id = reqID;
	setRequestPDU->error_index = 0;
	setRequestPDU->error_status = 0;
	setRequestPDU->variable_bindings = *varlist;

	PDUs_t *pdu;
	pdu = calloc(1, sizeof(PDUs_t));
	pdu->present = PDUs_PR_set_request;
	pdu->choice.set_request = *setRequestPDU;

	uint8_t buf[32];
	asn_enc_rval_t ret =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_PDUs, pdu, buf, sizeof(buf));

	buildMsg(buf, ret, *tail, atol(*(tail+1)));
}

void varBinding(long reqID, ObjectSyntax_t* syntax, ObjectName_t* name, char** tail) {
	VarBind_t* var_bind;
	var_bind = calloc(1, sizeof(VarBind_t));
	var_bind->name = *name;
	var_bind->choice.present = choice_PR_value;
	var_bind->choice.choice.value = *syntax;

	VarBindList_t* varlist;
	varlist = calloc(1, sizeof(VarBindList_t));
	/*
	 * Add another structure into the set by its pointer.
	 * RETURN VALUES:
	 * 0 for success and -1/errno for failure.
	 */
	int r = ASN_SEQUENCE_ADD(&varlist->list, var_bind);
	if (!r) buildPDU(varlist, reqID, tail+1);
	else printf("Error when adding structure into the set."); //retirar o printf daqui
}

void varsObject(long reqID, SimpleSyntax_t* simple, char* oid, char** tail) {
	ObjectSyntax_t* object_syntax;
	object_syntax = calloc(1, sizeof(ObjectSyntax_t));
	object_syntax->present = ObjectSyntax_PR_simple;
	object_syntax->choice.simple = *simple;

	ObjectName_t* object_name;
	object_name = calloc(1, sizeof(ObjectName_t));
	object_name->buf = (uint8_t *) oid;
	object_name->size = strlen(oid);

	varBinding(reqID, object_syntax, object_name, tail);
}

void simple_setRequest(long reqID, char* type, char* val, char** tail){
	SimpleSyntax_t* simple;
	simple = calloc(1, sizeof(SimpleSyntax_t));

	if (!strcmp(type, "integer")) {
		simple->present = SimpleSyntax_PR_integer_value;
		simple->choice.integer_value = atol(val);
		varsObject(reqID, simple, *tail, tail+1);
	}
	else if (!strcmp(type, "string")) {
		simple->present = SimpleSyntax_PR_string_value;
		OCTET_STRING_t* s = calloc(1, sizeof(OCTET_STRING_t));
		if (!OCTET_STRING_fromString(s, val))
			simple->choice.string_value = *s;
		varsObject(reqID, simple, *tail, tail+1);
	}
	else if (!strcmp(type, "objectID")) {
		simple->present = SimpleSyntax_PR_objectID_value;
		OBJECT_IDENTIFIER_t* obj = calloc(1, sizeof(OBJECT_IDENTIFIER_t));
		obj->buf = (uint8_t*) val;
		obj->size = sizeof(val);
		simple->choice.objectID_value = *obj;
		varsObject(reqID, simple, *tail, tail+1);
	}
	else app_setRequest(reqID, type, val, tail);
}

void app_setRequest(long reqID, char* type, char* val, char** tail){
	if (!strcmp(type, "IpAddress_t"));
		//((IpAddress_t) val);
	if (!strcmp(type, "Counter32_t"));
		//((Counter32_t) val);
	if (!strcmp(type, "TimeTicks_t"));
		//((TimeTicks_t) val);
	if (!strcmp(type, "Opaque_t"));
		//((Opaque_t) val);
	if (!strcmp(type, "Counter64_t"));
		//((Counter64_t) val);
	if (!strcmp(type, "Unsigned32_t"));
		//((Unsigned32_t) val);
}
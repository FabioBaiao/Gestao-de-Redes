#include <PRIM_response.h>

RES buildPDU_response(VarBindList_t* varlist, long reqID, long ei, long es) {
	Response_PDU_t* responsePDU;
	responsePDU = calloc(1, sizeof(Response_PDU_t));
	responsePDU->request_id = reqID;
	responsePDU->error_index = ei;
	responsePDU->error_status = es;
	responsePDU->variable_bindings = *varlist;

	PDUs_t *pdu;
	pdu = calloc(1, sizeof(PDUs_t));
	pdu->present = PDUs_PR_get_next_request;
	pdu->choice.get_next_request = *responsePDU;

	size_t buf_size = 1024;
	uint8_t* buf = calloc(buf_size, sizeof(uint8_t));
	asn_enc_rval_t ret =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_PDUs, pdu, buf, buf_size);

	return buildMsg(buf, ret, "public", 2);
}

VarBind_t* varBinding_response(ObjectSyntax_t* syntax, ObjectName_t* name) {
	VarBind_t* var_bind;
	var_bind = calloc(1, sizeof(VarBind_t));
	var_bind->name = *name;
	var_bind->choice.present = choice_PR_value;
	var_bind->choice.choice.value = *syntax;

	return var_bind;
}

VarBind_t* varsObject_response(SimpleSyntax_t* simple, ApplicationSyntax_t* app, char* oid_str) {
	ObjectSyntax_t* object_syntax;
	object_syntax = calloc(1, sizeof(ObjectSyntax_t));
	if (simple != NULL) {
		object_syntax->present = ObjectSyntax_PR_simple;
		object_syntax->choice.simple = *simple;
	}
	else {
		object_syntax->present = ObjectSyntax_PR_application_wide;
		object_syntax->choice.application_wide = *app;
	}

	ObjectName_t* object_name;
	object_name = calloc(1, sizeof(ObjectName_t));
	//=============================================
	size_t oid_size = 16;
	uint8_t* oid = calloc(oid_size, sizeof(uint8_t));
	int i = 0;
	char* token = strtok(oid_str, ".");
	do {
		oid[i++] = atoi(token);
		token = strtok(NULL, ".");
	}
	while (token != NULL);
	//=============================================
	object_name->buf = oid;
	object_name->size = i;
	return varBinding_response(object_syntax, object_name);
}

VarBind_t* simple_response(char* oid, char* type, char* val){
	SimpleSyntax_t* simple;
	simple = calloc(1, sizeof(SimpleSyntax_t));

	if (!strcmp(type, "integer")) {
		simple->present = SimpleSyntax_PR_integer_value;
		simple->choice.integer_value = atol(val);
		return varsObject_response(simple, NULL, oid);
	}
	else if (!strcmp(type, "string")) {
		simple->present = SimpleSyntax_PR_string_value;
		OCTET_STRING_t* s = calloc(1, sizeof(OCTET_STRING_t));
		if (!OCTET_STRING_fromString(s, val))
			simple->choice.string_value = *s;
		return varsObject_response(simple, NULL, oid);
	}
	else if (!strcmp(type, "objectID")) {
		simple->present = SimpleSyntax_PR_objectID_value;
		OBJECT_IDENTIFIER_t* obj = calloc(1, sizeof(OBJECT_IDENTIFIER_t));
		//=============================================
		size_t oid_size = 16;
		uint8_t* oid_buf = calloc(oid_size, sizeof(uint8_t));
		int i = 0;
		char* token = strtok(val, ".");
		do {
			oid_buf[i++] = atoi(token);
			token = strtok(NULL, ".");
		}
		while (token != NULL);
		//=============================================
		obj->buf = oid_buf;
		obj->size = i;
		simple->choice.objectID_value = *obj;
		return varsObject_response(simple, NULL, oid);
	}
	else return app_response(type, val, oid);
}

VarBind_t* app_response(char* oid, char* type, char* val){
	ApplicationSyntax_t* app;
	app = calloc(1, sizeof(ApplicationSyntax_t));

	if (!strcmp(type, "IpAddress")) {
		app->present = ApplicationSyntax_PR_ipAddress_value;
		OCTET_STRING_t* ip = calloc(1, sizeof(OCTET_STRING_t));
		OCTET_STRING_fromBuf(ip, val, -1);
		app->choice.ipAddress_value = (IpAddress_t) *ip;
		return varsObject_response(NULL, app, oid);
	}
	else if (!strcmp(type, "Counter32")) {
		app->present = ApplicationSyntax_PR_counter_value;
		app->choice.counter_value = atol(val);
		return varsObject_response(NULL, app, oid);
	}
	else if (!strcmp(type, "TimeTicks")) {
		app->present = ApplicationSyntax_PR_timeticks_value;
		app->choice.timeticks_value = atol(val);
		return varsObject_response(NULL, app, oid);
	}
	else if (!strcmp(type, "Opaque")) {
		app->present = ApplicationSyntax_PR_arbitrary_value;
		OCTET_STRING_t* s = calloc(1, sizeof(OCTET_STRING_t));
		if (!OCTET_STRING_fromString(s, val))
			app->choice.arbitrary_value = *s;
		return varsObject_response(NULL, app, oid);	
	}
	else if (!strcmp(type, "Counter64")) {
		app->present = ApplicationSyntax_PR_big_counter_value;
		INTEGER_t* num = calloc(1, sizeof(INTEGER_t));
		asn_ulong2INTEGER(num, atol(val));
		app->choice.big_counter_value = *num;
		return varsObject_response(NULL, app, oid);
	}
	else if (!strcmp(type, "Unsigned32")) {
		app->present = ApplicationSyntax_PR_unsigned_integer_value;
		app->choice.unsigned_integer_value = atol(val);
		return varsObject_response(NULL, app, oid);
	}
	else return other_response(oid, type, val);
}

VarBind_t* other_response(char* oid, char* type, char* val) {
	VarBind_t* var_bind;
	var_bind = calloc(1, sizeof(VarBind_t));

	ObjectName_t* object_name;
	object_name = calloc(1, sizeof(ObjectName_t));
	//=============================================
	size_t oid_size = 16;
	uint8_t* oid_buf = calloc(oid_size, sizeof(uint8_t));
	int i = 0;
	char* token = strtok(oid, ".");
	do {
		oid_buf[i++] = atoi(token);
		token = strtok(NULL, ".");
	}
	while (token != NULL);
	//=============================================
	object_name->buf = oid_buf;
	object_name->size = i;

	if (!strcmp(type, "noSuchObject")) {
		var_bind->name = *object_name;
		var_bind->choice.present = choice_PR_noSuchObject;
		var_bind->choice.choice.noSuchObject = -1;
	}

	if (!strcmp(type, "noSuchInstance")) {
		var_bind->name = *object_name;
		var_bind->choice.present = choice_PR_noSuchInstance;
		var_bind->choice.choice.noSuchInstance = -1;
	}

	if (!strcmp(type, "endOfMibView")) {
		var_bind->name = *object_name;
		var_bind->choice.present = choice_PR_endOfMibView;
		var_bind->choice.choice.endOfMibView = -1;
	}

	return var_bind;
}


RES responseHandler(long reqID, char* ei, char* es, char* tail[], int size) {
	VarBindList_t* varlist;
	varlist = calloc(1, sizeof(VarBindList_t));

	int i;
	for (i = 0; i < size; i+=3)	 {
		VarBind_t* vb = simple_response(tail[i], tail[i+1], tail[i+2]);
		ASN_SEQUENCE_ADD(&varlist->list, vb);
	}

	return buildPDU_response(varlist, reqID, atol(ei), atol(es));
}

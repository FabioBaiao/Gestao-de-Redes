#ifndef PRIM_set_request
#define PRIM_set_request

#include <PDUs.h>
#include <Message.h>

typedef struct result {
	uint8_t* buff;
	size_t size ;
} *RES;

RES buildMsg_setReq(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);
RES buildPDU_setReq(VarBindList_t* varlist, long reqID, char* cs, char* v);
VarBind_t* varBinding_setReq(ObjectSyntax_t* syntax, ObjectName_t* name);
VarBind_t* varsObject(SimpleSyntax_t* simple, ApplicationSyntax_t* app, char* oid_str);
VarBind_t* simple_setRequest(char* oid, char* type, char* val);
VarBind_t* app_setRequest(char* oid, char* type, char* val);
RES setReqHandler(long reqID, char* cs, char* v, char** tail, int size);


#endif
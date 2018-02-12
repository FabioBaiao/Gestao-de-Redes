#ifndef encodeAPI
#define encodeAPI

#include <PDUs.h>
#include <Message.h>

typedef struct result {
	uint8_t* buff;
	size_t size ;
} *RES;

RES setReqHandler(long reqID, char* type, char* val, char** tail);
RES app_setRequest(long reqID, char* type, char* val, char** tail);
RES varsObject(long reqID, SimpleSyntax_t* simple, ApplicationSyntax_t* app, char* oid, char** tail);
RES varBinding(long reqID, ObjectSyntax_t* syntax, ObjectName_t* name, char** tail);
RES buildPDU(VarBindList_t* varlist, long reqID, char** tail);
RES buildMsg(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);

#endif
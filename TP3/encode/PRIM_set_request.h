#ifndef PRIM_set_request
#define PRIM_set_request

#include <stdio.h>
#include <asn_codecs.h>
#include <ObjectSyntax.h>
#include <SimpleSyntax.h>
#include <ApplicationSyntax.h>
#include <ObjectName.h>
#include <VarBindList.h>
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

typedef struct result {
	uint8_t* buff;
	size_t size ;
} *RES;

RES setReqHandler(long reqID, char* type, char* val, char** tail);
RES app_setRequest(long reqID, char* type, char* val, char** tail);
RES varsObject(long reqID, SimpleSyntax_t* simple, ApplicationSyntax_t* app, char* oid, char** tail);
RES varBinding_setReq(long reqID, ObjectSyntax_t* syntax, ObjectName_t* name, char** tail);
RES buildPDU_setReq(VarBindList_t* varlist, long reqID, char** tail);
RES buildMsg_setReq(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);

#endif
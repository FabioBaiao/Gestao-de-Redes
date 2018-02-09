#ifndef PRIM_set_request
#define PRIM_set_request

#include <stdio.h>
#include <asn_codecs.h>
#include <ObjectSyntax.h>
#include <SimpleSyntax.h>
#include <ApplicationSyntax.h>
#include <ObjectName.h>
#include <VarBindList.h>

void simple_setRequest(long reqID, char* type, char* val, char** tail);
void app_setRequest(long reqID, char* type, char* val, char** tail);
void varsObject(long reqID, SimpleSyntax_t* simple, char* oid, char** tail);
void varBinding(long reqID, ObjectSyntax_t* syntax, ObjectName_t* name, char** tail);
void buildPDU(VarBindList_t* varlist, long reqID, char** tail);
void buildMsg(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);

#endif
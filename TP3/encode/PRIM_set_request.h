#ifndef PRIM_set_request
#define PRIM_set_request

#include <stdio.h>
#include <asn_codecs.h>
#include <ObjectSyntax.h>
#include <SimpleSyntax.h>
#include <ApplicationSyntax.h>
#include <ObjectName.h>
#include <VarBindList.h>

uint8_t* simple_setRequest(long reqID, char* type, char* val, char** tail);
uint8_t* app_setRequest(long reqID, char* type, char* val, char** tail);
uint8_t* varsObject(long reqID, SimpleSyntax_t* simple, ApplicationSyntax_t* app, char* oid, char** tail);
uint8_t* varBinding_setReq(long reqID, ObjectSyntax_t* syntax, ObjectName_t* name, char** tail);
uint8_t* buildPDU_setReq(VarBindList_t* varlist, long reqID, char** tail);
uint8_t* buildMsg_setReq(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);

#endif
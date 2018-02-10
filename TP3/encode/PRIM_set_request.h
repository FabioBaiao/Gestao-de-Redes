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
uint8_t* varsObject(long reqID, SimpleSyntax_t* simple, char* oid, char** tail);
uint8_t* varBinding(long reqID, ObjectSyntax_t* syntax, ObjectName_t* name, char** tail);
uint8_t* buildPDU(VarBindList_t* varlist, long reqID, char** tail);
uint8_t* buildMsg(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);

#endif
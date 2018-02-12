#ifndef PRIM_response
#define PRIM_response

#include <stdio.h>
#include <asn_codecs.h>
#include <ObjectName.h>
#include <VarBindList.h>
#include <asn_application.h>
#include <asn_internal.h>
#include <OCTET_STRING.h>
#include <asn_codecs_prim.h>
#include <PDUs.h>
#include <Response-PDU.h>
#include <VarBind.h>
#include <ANY.h>
#include <Message.h>

uint8_t* buildMsg_response(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);
uint8_t* buildPDU_response(VarBindList_t* varlist, long reqID, char* cs, long v);
uint8_t* varBinding_response(long reqID, ObjectName_t* names[], char* cs, long v);
uint8_t* responseHandler(long reqID, char* args[], int n);

#endif
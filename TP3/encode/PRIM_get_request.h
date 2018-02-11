#ifndef PRIM_get_request
#define PRIM_get_request

#include <stdio.h>
#include <asn_codecs.h>
#include <ObjectName.h>
#include <VarBindList.h>

uint8_t* buildMsg_getReq(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);
uint8_t* buildPDU_getReq(VarBindList_t* varlist, long reqID, char* cs, long v);
uint8_t* varBinding_getReq(long reqID, ObjectName_t* names[], char* cs, long v);
uint8_t* getReqHandler(long reqID, char* args[]);

#endif
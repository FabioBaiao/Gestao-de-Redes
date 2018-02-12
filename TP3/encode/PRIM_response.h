#ifndef PRIM_response
#define PRIM_response

#include <PDUs.h>
#include <Message.h>

RES buildMsg_response(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);
RES buildPDU_response(VarBindList_t* varlist, long reqID, char* cs, long v);
RES varBinding_response(long reqID, ObjectName_t* names[], char* cs, long v);
RES responseHandler(long reqID, char* args[], int n);

#endif
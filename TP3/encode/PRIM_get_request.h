#ifndef PRIM_get_request
#define PRIM_get_request

#include <encodeAPI.h>

RES buildPDU_getReq(VarBindList_t* varlist, long reqID, char* cs, long v);
RES varBinding_getReq(long reqID, ObjectName_t* names[], char* cs, long v);
RES getReqHandler(long reqID, char* args[], int n);

#endif
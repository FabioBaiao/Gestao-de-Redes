#ifndef PRIM_get_next
#define PRIM_get_next

#include <encodeAPI.h>

RES buildPDU_getNext(VarBindList_t* varlist, long reqID, char* cs, long v);
RES varBinding_getNext(long reqID, ObjectName_t* names[], char* cs, long v);
RES getNextHandler(long reqID, char* args[], int n);

#endif
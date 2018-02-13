#ifndef PRIM_get_bulk
#define PRIM_get_bulk

#include <encodeAPI.h>

RES buildPDU_getBulk(VarBindList_t* varlist, long reqID, char* cs, long v, long nr, long maxr);
RES varBinding_getBulk(long reqID, ObjectName_t* names[], char* cs, long v, long nr, long maxr);
RES getBulkHandler(long reqID, char* args[], int n);

#endif
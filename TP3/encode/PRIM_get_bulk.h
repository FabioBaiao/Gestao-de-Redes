#ifndef PRIM_get_bulk
#define PRIM_get_bulk

#include <PRIM_set_request.h>

#include <stdio.h>
#include <asn_codecs.h>
#include <ObjectName.h>
#include <VarBindList.h>
#include <asn_application.h>
#include <asn_internal.h>
#include <OCTET_STRING.h>
#include <asn_codecs_prim.h>
#include <PDUs.h>
#include <GetBulkRequest-PDU.h>
#include <VarBind.h>
#include <ANY.h>
#include <Message.h>


RES buildMsg_getBulk(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);
RES buildPDU_getBulk(VarBindList_t* varlist, long reqID, char* cs, long v);
RES varBinding_getBulk(long reqID, ObjectName_t* names[], char* cs, long v);
RES getBulkHandler(long reqID, char* args[], int n);

#endif
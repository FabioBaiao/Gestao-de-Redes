#ifndef PRIM_get_next
#define PRIM_get_next

#include <stdio.h>
#include <asn_codecs.h>
#include <ObjectName.h>
#include <VarBindList.h>
#include <asn_application.h>
#include <asn_internal.h>
#include <OCTET_STRING.h>
#include <asn_codecs_prim.h>
#include <PDUs.h>
#include <GetNextRequest-PDU.h>
#include <VarBind.h>
#include <ANY.h>
#include <Message.h>

uint8_t* buildMsg_getNext(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);
uint8_t* buildPDU_getNext(VarBindList_t* varlist, long reqID, char* cs, long v);
uint8_t* varBinding_getNext(long reqID, ObjectName_t* names[], char* cs, long v);
uint8_t* getNextHandler(long reqID, char* args[]);

#endif
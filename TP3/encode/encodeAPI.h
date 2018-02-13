#ifndef encodeAPI
#define encodeAPI

#include <PDUs.h>
#include <Message.h>

typedef struct result {
	uint8_t* buff;
	size_t size ;
} *RES;

typedef struct tuple {
	char* prim;
	char** args;
	int argc;
} *TUPLE;

RES buildMsg(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v);

#endif
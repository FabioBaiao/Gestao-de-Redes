#include <encodeAPI.h>

RES buildMsg(uint8_t* buf, asn_enc_rval_t ret, char* cs, long v) {
	ANY_t* data;
	data = calloc(1, sizeof(ANY_t));
	data->buf = buf;
	data->size = ret.encoded;

	Message_t* message;
	message = calloc(1, sizeof(Message_t));
	OCTET_STRING_t* os = calloc(1, sizeof(OCTET_STRING_t));
	OCTET_STRING_fromBuf(os, cs, -1);
	message->version = v;
	message->community = *os;
	message->data = *data;

	uint8_t* buf_final = calloc(1024, sizeof(uint8_t));
	asn_enc_rval_t ret_final =
		asn_encode_to_buffer(0, ATS_BER, &asn_DEF_Message, message, buf_final, 1024);

	RES res = calloc(1, sizeof(struct result));
	res->buff = buf_final;
	res->size = ret_final.encoded;

	return res;
}
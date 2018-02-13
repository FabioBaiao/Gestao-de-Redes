#ifndef PRIM_get_request
#define PRIM_get_request

#include <encodeAPI.h>

RES buildPDU_getReq(VarBindList_t* varlist, long reqID, char* cs, long v);
RES varBinding_getReq(long reqID, ObjectName_t* names[], char* cs, long v);

/**
* reqID: indice do request, definido pela aplicacao
* args: array de strings com o seguinte formato: [community string, version, oid, oid, ...]
* n: tamanho do array tail
* RETORNA um RES que contem a codificacao e o numero de bytes respetivo
*/
RES getReqHandler(long reqID, char* args[], int n);

#endif

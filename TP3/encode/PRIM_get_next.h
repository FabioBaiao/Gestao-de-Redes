#ifndef PRIM_get_next
#define PRIM_get_next

#include <encodeAPI.h>

RES buildPDU_getNext(VarBindList_t* varlist, long reqID, char* cs, long v);
RES varBinding_getNext(long reqID, ObjectName_t* names[], char* cs, long v);

/**
* reqID: indice do request, definido pela aplicacao
* args: array de strings com o seguinte formato: [community string, version, oid, oid, ...]
* n: tamanho do array tail
* RETORNA um RES que contem a codificacao e o numero de bytes respetivo
*/
RES getNextHandler(long reqID, char* args[], int n);

#endif

#ifndef PRIM_get_bulk
#define PRIM_get_bulk

#include <encodeAPI.h>

RES buildPDU_getBulk(VarBindList_t* varlist, long reqID, char* cs, long v, long nr, long maxr);
RES varBinding_getBulk(long reqID, ObjectName_t* names[], char* cs, long v, long nr, long maxr);

/**
* reqID: indice do request, definido pela aplicacao
* args: array de strings com o seguinte formato: [community string, version, non_repeaters, max_repetitions, oid, oid, ...]
* n: tamanho do array tail
* RETORNA um RES que contem a codificacao e o numero de bytes respetivo
*/
RES getBulkHandler(long reqID, char* args[], int n);

#endif

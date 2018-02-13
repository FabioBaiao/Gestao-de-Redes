#ifndef PRIM_set_request
#define PRIM_set_request

#include <encodeAPI.h>

RES buildPDU_setReq(VarBindList_t* varlist, long reqID, char* cs, char* v);
VarBind_t* varBinding_setReq(ObjectSyntax_t* syntax, ObjectName_t* name);
VarBind_t* varsObject(SimpleSyntax_t* simple, ApplicationSyntax_t* app, char* oid_str);
VarBind_t* simple_setRequest(char* oid, char* type, char* val);
VarBind_t* app_setRequest(char* oid, char* type, char* val);

/**
* reqID: indice do request, definido pela aplicacao
* cs: community string
* v: versao
* tail: array de strings com o seguinte formato: [oid, type, value, oid, type, value, ...]
* size: tamanho do array tail
* RETORNA um RES que contem a codificacao e o numero de bytes respetivo
*/
RES setReqHandler(long reqID, char* cs, char* v, char** tail, int size);


#endif

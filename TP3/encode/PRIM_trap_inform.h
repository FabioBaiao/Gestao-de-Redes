#ifndef PRIM_trap_inform
#define PRIM_trap_inform

#include <PDUs.h>
#include <Message.h>
#include <encodeAPI.h>

RES buildPDU_trap(VarBindList_t* varlist, int trap, long reqID, char* cs, long v);
VarBind_t* varBinding_trap(ObjectSyntax_t* syntax, ObjectName_t* name);

/**
* reqID: indice do request, definido pela aplicacao
* trap: 0 -> inform; 1 -> trap;
* cs: community string
* v: versao
* tail: array de strings com o seguinte formato: [oid, type, value, oid, type, value, ...]
* size: tamanho do array tail
* RETORNA um RES que contem a codificacao e o numero de bytes respetivo
*/
RES trapHandler(long reqID, int trap, char* ei, char* es, char* argv[], int n);

VarBind_t* varsObject_trap(SimpleSyntax_t* simple, ApplicationSyntax_t* app, char* oid_str);
VarBind_t* simple_trap(char* oid, char* type, char* val);
VarBind_t* app_trap(char* oid, char* type, char* val);

#endif

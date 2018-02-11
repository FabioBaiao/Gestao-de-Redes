#include <Message.h>
#include <PDUs.h>

char* printOid(uint8_t*, int);
char** decode_simple_syntax(SimpleSyntax_t*);
char** decode_application_syntax(ApplicationSyntax_t*);
char** decode_obj_syntax(ObjectSyntax_t*);
int decode_var_bind(VarBind_t*, char**);
int decode_var_bindings(VarBindList_t, char**);
int decode_pdus(PDUs_t*, char**);
int decode_message(Message_t*, char**);
int decode_snmp(uint8_t*, size_t, char**);

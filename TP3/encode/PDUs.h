/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "SNMPv2-PDU"
 * 	found in "snmpv2c.asn1"
 */

#ifndef	_PDUs_H_
#define	_PDUs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GetRequest-PDU.h"
#include "GetNextRequest-PDU.h"
#include "GetBulkRequest-PDU.h"
#include "Response-PDU.h"
#include "SetRequest-PDU.h"
#include "InformRequest-PDU.h"
#include "SNMPv2-Trap-PDU.h"
#include "Report-PDU.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PDUs_PR {
	PDUs_PR_NOTHING,	/* No components present */
	PDUs_PR_get_request,
	PDUs_PR_get_next_request,
	PDUs_PR_get_bulk_request,
	PDUs_PR_response,
	PDUs_PR_set_request,
	PDUs_PR_inform_request,
	PDUs_PR_snmpV2_trap,
	PDUs_PR_report
} PDUs_PR;

/* PDUs */
typedef struct PDUs {
	PDUs_PR present;
	union PDUs_u {
		GetRequest_PDU_t	 get_request;
		GetNextRequest_PDU_t	 get_next_request;
		GetBulkRequest_PDU_t	 get_bulk_request;
		Response_PDU_t	 response;
		SetRequest_PDU_t	 set_request;
		InformRequest_PDU_t	 inform_request;
		SNMPv2_Trap_PDU_t	 snmpV2_trap;
		Report_PDU_t	 report;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDUs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PDUs;

#ifdef __cplusplus
}
#endif

#endif	/* _PDUs_H_ */
#include <asn_internal.h>

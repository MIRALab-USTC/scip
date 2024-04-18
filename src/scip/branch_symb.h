#ifndef __SCIP_BRANCH_SYMB_H__
#define __SCIP_BRANCH_SYMB_H__


#include "scip/def.h"
#include "scip/type_retcode.h"
#include "scip/type_scip.h"
#include "scip/type_var.h"

#ifdef __cplusplus
extern "C" {
#endif

EXTERN
SCIP_RETCODE SCIPincludeBranchruleSymb(
   SCIP*                 scip,
   const char*           loadpath
   );

SCIP_RETCODE SCIPincludeBranchruleSymbDebug(SCIP* scip);

EXTERN
SCIP_Real SCIPGetBranchDecisionTime(SCIP* scip);

EXTERN
SCIP_RETCODE getSymbBestCands(SCIP* scip, SCIP_BRANCHRULE* branchrule, SCIP_VAR ***goodcands_ptr, int* ngoodcands);


#ifdef __cplusplus
}
#endif

#endif

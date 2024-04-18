#ifndef __SCIP_BRANCH_SYMB_LOCAL_H__
#define __SCIP_BRANCH_SYMB_LOCAL_H__

#include "blockmemshell/memory.h"
#include "scip/pub_branch.h"
#include "scip/pub_message.h"
#include "scip/pub_misc.h"
#include "scip/pub_misc_sort.h"
#include "scip/pub_tree.h"
#include "scip/pub_var.h"
#include "scip/scip_branch.h"
#include "scip/scip_mem.h"
#include "scip/scip_message.h"
#include "scip/scip_numerics.h"
#include "scip/scip_param.h"
#include "scip/scip_randnumgen.h"
#include "scip/scip_sol.h"
#include "scip/scip_tree.h"
#include "scip/scip_var.h"
#include <string.h>
#include <dlfcn.h>
#include "scip/branch_symb.h"

#ifdef __cplusplus
extern "C" {
#endif

// typedef SCIP_RETCODE (*BranchExecLpFunc) (SCIP* scip, SCIP_BRANCHRULE* branchrule, SCIP_Bool allowaddcons, SCIP_RESULT* result);
// typedef SCIP_RETCODE (*BranchInitSolFunc) (SCIP* scip, SCIP_BRANCHRULE* branchrule);

typedef SCIP_RETCODE (*AllocAndGetSymbGoodCandsFunc) (SCIP* scip, SCIP_BRANCHRULE* branchrule, SCIP_VAR*** goodcands_ptr, int* ngoodcands);

typedef struct
{
   float *type_0, *type_1, *type_2, *type_3, *coefs, *coefs_pos, *coefs_neg, *nnzrs, *root_cdeg_mean, *root_cdeg_var, *root_cdeg_min, *root_cdeg_max, *root_pcoefs_count, *root_pcoefs_mean, *root_pcoefs_var, *root_pcoefs_min, *root_pcoefs_max, *root_ncoefs_count, *root_ncoefs_mean, *root_ncoefs_var, *root_ncoefs_min, *root_ncoefs_max;
} SYMB_ROOTINFO;

typedef struct
{
   int goodcandslen;
   int* candsindices;
   float* scoreslist;
} SYMB_TEMPINFO;

/** branching rule data */
struct SCIP_BranchruleData
{
   void*             dl;
   SCIP_Bool         copy;
   SYMB_ROOTINFO rootinfo;
   SYMB_TEMPINFO tempinfo;
   SCIP_Bool inited;
   SCIP_Real decisiontime;
   SCIP_Real bestratio;
   AllocAndGetSymbGoodCandsFunc allocandgetsymbgoodcandsfunc;
};


#ifdef __cplusplus
}
#endif

#endif

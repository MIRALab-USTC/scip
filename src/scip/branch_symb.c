#include "scip/branch_symb_local.h"


#define BRANCHRULE_NAME          "symb"
#define BRANCHRULE_DESC          "branching on symb values"
#define BRANCHRULE_PRIORITY      100000
#define BRANCHRULE_MAXDEPTH      -1
#define BRANCHRULE_MAXBOUNDDIST  1.0
#define DEFAULT_COPY TRUE
#define DEFAULT_BESTRATIO 0.01

SCIP_DECL_BRANCHEXECLP(branchExeclpSymb);
// SCIP_RETCODE allocAndGetSymbGoodCands(SCIP* scip, SCIP_BRANCHRULE* branchrule, SCIP_VAR*** goodcands_ptr, int* ngoodcands);

static
SCIP_DECL_BRANCHFREE(branchFreeSymb)
{  SCIP_BRANCHRULEDATA* branchruledata;

   branchruledata = SCIPbranchruleGetData(branchrule);
   assert(branchruledata != NULL);

   if(branchruledata->dl != NULL) dlclose(branchruledata->dl);

   SCIPfreeBlockMemory(scip, &branchruledata);
   SCIPbranchruleSetData(branchrule, NULL);

   return SCIP_OKAY;
}

static
void InitRootInfoToNull(SYMB_ROOTINFO* rootinfo)
{
   (rootinfo->type_0)=NULL;
   (rootinfo->type_1)=NULL;
   (rootinfo->type_2)=NULL;
   (rootinfo->type_3)=NULL;
   (rootinfo->coefs)=NULL;
   (rootinfo->coefs_pos)=NULL;
   (rootinfo->coefs_neg)=NULL;
   (rootinfo->nnzrs)=NULL;
   (rootinfo->root_cdeg_mean)=NULL;
   (rootinfo->root_cdeg_var)=NULL;
   (rootinfo->root_cdeg_min)=NULL;
   (rootinfo->root_cdeg_max)=NULL;
   (rootinfo->root_pcoefs_count)=NULL;
   (rootinfo->root_pcoefs_mean)=NULL;
   (rootinfo->root_pcoefs_var)=NULL;
   (rootinfo->root_pcoefs_min)=NULL;
   (rootinfo->root_pcoefs_max)=NULL;
   (rootinfo->root_ncoefs_count)=NULL;
   (rootinfo->root_ncoefs_mean)=NULL;
   (rootinfo->root_ncoefs_var)=NULL;
   (rootinfo->root_ncoefs_min)=NULL;
   (rootinfo->root_ncoefs_max)=NULL;
}

static
SCIP_DECL_BRANCHINITSOL(branchInitsolSymb)
{
   SCIP_BRANCHRULEDATA* branchruledata;
   branchruledata = SCIPbranchruleGetData(branchrule);
   assert(branchruledata != NULL);

   branchruledata->inited = FALSE;
   branchruledata->decisiontime = 0.;
   InitRootInfoToNull(&(branchruledata->rootinfo));

   branchruledata->tempinfo.goodcandslen = 0;
   branchruledata->tempinfo.candsindices = NULL;
   branchruledata->tempinfo.scoreslist = NULL;

   assert( *((float**) (&(branchruledata->rootinfo) + 1) - 1) == NULL );

    return SCIP_OKAY;
}

static
SCIP_DECL_BRANCHEXITSOL(branchExitsolSymb)
{
   SCIP_BRANCHRULEDATA* branchruledata;
   branchruledata = SCIPbranchruleGetData(branchrule);
   assert(branchruledata != NULL);
   SYMB_ROOTINFO* rootinfo = &(branchruledata->rootinfo);

   if(branchruledata->inited)
   {
      free(rootinfo->type_0);
      free(rootinfo->type_1);
      free(rootinfo->type_2);
      free(rootinfo->type_3);
      free(rootinfo->coefs);
      free(rootinfo->coefs_pos);
      free(rootinfo->coefs_neg);
      free(rootinfo->nnzrs);
      free(rootinfo->root_cdeg_mean);
      free(rootinfo->root_cdeg_var);
      free(rootinfo->root_cdeg_min);
      free(rootinfo->root_cdeg_max);
      free(rootinfo->root_pcoefs_count);
      free(rootinfo->root_pcoefs_mean);
      free(rootinfo->root_pcoefs_var);
      free(rootinfo->root_pcoefs_min);
      free(rootinfo->root_pcoefs_max);
      free(rootinfo->root_ncoefs_count);
      free(rootinfo->root_ncoefs_mean);
      free(rootinfo->root_ncoefs_var);
      free(rootinfo->root_ncoefs_min);
      free(rootinfo->root_ncoefs_max);

      free(branchruledata->tempinfo.candsindices);
      free(branchruledata->tempinfo.scoreslist);

      branchruledata->inited = FALSE;
      InitRootInfoToNull(&(branchruledata->rootinfo));

      branchruledata->tempinfo.goodcandslen = 0;
      branchruledata->tempinfo.candsindices = NULL;
      branchruledata->tempinfo.scoreslist = NULL;
   }

   return SCIP_OKAY;
}

static
SCIP_DECL_BRANCHCOPY(branchCopySymb)
{
   assert(scip != NULL);
   assert(branchrule != NULL);
   assert(strcmp(SCIPbranchruleGetName(branchrule), BRANCHRULE_NAME) == 0);

   SCIP_BRANCHRULEDATA* branchruledata;
   branchruledata = SCIPbranchruleGetData(branchrule);
   assert(branchruledata != NULL);

   if(branchruledata->copy)
   {
      SCIP_CALL(SCIPincludeBranchruleSymbwithPtrs(scip, NULL, SCIPbranchruleGetExeLpFunc(branchrule), branchruledata->allocandgetsymbgoodcandsfunc));
   }

   return SCIP_OKAY;
}


SCIP_RETCODE getSymbBestCands(SCIP* scip, SCIP_BRANCHRULE* branchrule, SCIP_VAR ***goodcands_ptr, int* ngoodcands)
{
   SCIP_BRANCHRULEDATA* branchruledata;
   branchruledata = SCIPbranchruleGetData(branchrule);

   SCIP_CALL( branchruledata->allocandgetsymbgoodcandsfunc(scip, branchrule, goodcands_ptr, ngoodcands) );
   return SCIP_OKAY;
}


// static
// SCIP_DECL_BRANCHEXECLP(branchExeclpHybridSymbDebug)
// {
//    SCIP_VAR **goodcands_ptr;
//    int ngoodcands;
//    SCIP_CALL( getSymbBestCands(scip, branchrule, &goodcands_ptr, &ngoodcands) );
//    assert( (ngoodcands > 0 && goodcands_ptr != NULL));

//    SCIP_CALL( SCIPbranchVar(scip, goodcands_ptr[0], NULL, NULL, NULL) );

//    free(goodcands_ptr);
//    *result = SCIP_BRANCHED;
//    return SCIP_OKAY;
// }

SCIP_RETCODE SCIPincludeBranchruleSymbwithPtrs(
   SCIP*                 scip,
   void*                 dl,
   BranchExecLpFunc   execlpfunc,
   AllocAndGetSymbGoodCandsFunc allocandgetsymbgoodcandsfunc
   )
{
   SCIP_BRANCHRULEDATA* branchruledata;
   SCIP_BRANCHRULE* branchrule;
   SCIP_CALL( SCIPallocBlockMemory(scip, &branchruledata) );
   branchruledata->dl = dl;
   branchruledata->allocandgetsymbgoodcandsfunc = allocandgetsymbgoodcandsfunc;
   assert(allocandgetsymbgoodcandsfunc != NULL);
   // branchruledata->inited = FALSE;
   // InitRootInfoToNull(&(branchruledata->rootinfo));
   // assert( *((float**) (&(branchruledata->rootinfo) + 1) - 1) == NULL );


   SCIP_CALL( SCIPincludeBranchruleBasic(scip, &branchrule, BRANCHRULE_NAME, BRANCHRULE_DESC, BRANCHRULE_PRIORITY,
         BRANCHRULE_MAXDEPTH, BRANCHRULE_MAXBOUNDDIST, branchruledata) );
   assert(branchrule != NULL);


   SCIP_CALL( SCIPsetBranchruleCopy(scip, branchrule, branchCopySymb) );
   SCIP_CALL( SCIPsetBranchruleFree(scip, branchrule, branchFreeSymb) );
   SCIP_CALL( SCIPsetBranchruleExitsol(scip, branchrule, branchExitsolSymb) );

   assert(execlpfunc != NULL);
   SCIP_CALL( SCIPsetBranchruleExecLp(scip, branchrule, execlpfunc) );
   SCIP_CALL( SCIPsetBranchruleInitsol(scip, branchrule, branchInitsolSymb) );

   SCIP_CALL( SCIPaddBoolParam(scip,
         "branching/symb/copy",
         "should symb brancher be copied?",
         &branchruledata->copy, TRUE, DEFAULT_COPY, NULL, NULL) );

   SCIP_CALL( SCIPaddRealParam(scip,
         "branching/symb/bestratio",
         "the ratio to obtain candidates recommend by symb",
         &branchruledata->bestratio, TRUE, DEFAULT_BESTRATIO, 0., 1.,  NULL, NULL) );

   return SCIP_OKAY;
}

SCIP_RETCODE SCIPincludeBranchruleSymb(
   SCIP*                 scip,
   const char*           loadpath
   )
{
   void* dl = dlopen(loadpath, RTLD_NOW | RTLD_DEEPBIND);
   if(!dl)
   {
      SCIPerrorMessage("cannot load the library from the path %s", loadpath);
      return SCIP_ERROR;
   }
   BranchExecLpFunc execlpfunc = (BranchExecLpFunc) (dlsym(dl, "branchExeclpSymb"));
   if(!execlpfunc)
   {
      SCIPerrorMessage("cannot load the function branchExeclpSymb from the library");
      return SCIP_ERROR;
   }

   AllocAndGetSymbGoodCandsFunc allocandgetsymbgoodcandsfunc = (AllocAndGetSymbGoodCandsFunc) (dlsym(dl, "allocAndGetSymbGoodCands"));
   if(!allocandgetsymbgoodcandsfunc)
   {
      SCIPerrorMessage("cannot load the function allocandgetsymbgoodcandsfunc from the library");
      return SCIP_ERROR;
   }

   // BranchInitSolFunc initsolfunc = (BranchInitSolFunc) (dlsym(dl, "branchInitsolSymb"));
   // if(!initsolfunc)
   // {
   //    SCIPerrorMessage("cannot load the function branchInitsolSymb from the library");
   //    return SCIP_ERROR;
   // }

   SCIP_CALL(SCIPincludeBranchruleSymbwithPtrs(scip, dl, execlpfunc, allocandgetsymbgoodcandsfunc));

   return SCIP_OKAY;
}

// SCIP_RETCODE SCIPincludeBranchruleSymbDebug(SCIP* scip)
// {
//    SCIP_CALL(SCIPincludeBranchruleSymbwithPtrs(scip, NULL, branchExeclpSymb, allocAndGetSymbGoodCands));
// }

SCIP_Real SCIPGetBranchDecisionTime(SCIP* scip)
{
   SCIP_BRANCHRULEDATA* branchruledata;
   SCIP_BRANCHRULE* branchrule;

   assert(scip != NULL);

   branchrule = SCIPfindBranchrule(scip, BRANCHRULE_NAME);
   branchruledata = SCIPbranchruleGetData(branchrule);

   return branchruledata->decisiontime;
}


// SCIP_RETCODE SCIPincludeBranchruleHybridSymbDebug(SCIP* scip)
// {
//    SCIP_CALL(SCIPincludeBranchruleSymbwithPtrs(scip, NULL, branchExeclpHybridSymbDebug, allocAndGetSymbGoodCands));
// }

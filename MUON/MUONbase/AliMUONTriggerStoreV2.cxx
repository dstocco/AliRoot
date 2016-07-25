/**************************************************************************
* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
*                                                                        *
* Author: The ALICE Off-line Project.                                    *
* Contributors are mentioned in the code where appropriate.              *
*                                                                        *
* Permission to use, copy, modify and distribute this software and its   *
* documentation strictly for non-commercial purposes is hereby granted   *
* without fee, provided that the above copyright notice appears in all   *
* copies and that both the copyright notice and this permission notice   *
* appear in the supporting documentation. The authors make no claims     *
* about the suitability of this software for any purpose. It is          *
* provided "as is" without express or implied warranty.                  *
**************************************************************************/


#include "AliMUONTriggerStoreV2.h"

//#include "AliMUONGlobalTrigger.h"
//#include "AliMUONLocalTrigger.h"
//#include "AliMUONRegionalTrigger.h"
#include "AliMUONTreeManager.h"
#include <TTree.h>

/// \cond CLASSIMP
ClassImp(AliMUONTriggerStoreV2)
/// \endcond

//_____________________________________________________________________________
AliMUONTriggerStoreV2::AliMUONTriggerStoreV2() : AliMUONTriggerStoreV1()
{
  /// ctor
//  fLocal->SetName("AliMUONRecoLocalTrigger");
//  fRegional->SetName("AliMUONRecoRegionalTrigger");
//  fGlobal->SetName("AliMUONRecoGlobalTrigger");
//  fEmptyLocal->SetName("AliMUONRecoLocalTrigger");
}

//_____________________________________________________________________________
AliMUONTriggerStoreV2::~AliMUONTriggerStoreV2()
{
  /// dtor
}

//_____________________________________________________________________________
Bool_t
AliMUONTriggerStoreV2::Connect(TTree& tree, Bool_t alone) const
{
  /// Connect this store to the tree
  AliMUONTreeManager tman;
  Bool_t ok(kTRUE);
  
  Bool_t isMaking = ( tree.GetBranch("MUONLocalTrigRec") == 0 );
  
  if ( isMaking ) 
  {
    ok = ok && tman.MakeBranch(tree,ClassName(),"TClonesArray",
                               "MUONLocalTrigRec",LocalPtr());
    ok = ok && tman.MakeBranch(tree,ClassName(),"TClonesArray",
                               "MUONRegionalTrigRec",RegionalPtr());
    ok = ok && tman.MakeBranch(tree,ClassName(),"TClonesArray",
                               "MUONGlobalTrigRec",GlobalPtr());
  }
  else
  {
    if ( alone ) tman.UpdateBranchStatuses(tree,"TrigRec");
    ok = ok && tman.SetAddress(tree,"MUONLocalTrigRec",LocalPtr());
    ok = ok && tman.SetAddress(tree,"MUONRegionalTrigRec",RegionalPtr());
    ok = ok && tman.SetAddress(tree,"MUONGlobalTrigRec",GlobalPtr());
  }
  return ok;
}

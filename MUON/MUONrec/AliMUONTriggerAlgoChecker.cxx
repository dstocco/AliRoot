/**************************************************************************
 * Copyright(c) 1998-2007, ALICE Experiment at CERN, All rights reserved. *
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

#include "AliMUONTriggerAlgoChecker.h"

#include "TObjArray.h"
#include "TString.h"

#include "AliLog.h"

#include "AliMUONConstants.h"
#include "AliMUONVTriggerStore.h"
#include "AliMUONLocalTrigger.h"
#include "AliMpDDLStore.h"
#include "AliMpLocalBoard.h"

/// \cond CLASSIMP
ClassImp(AliMUONTriggerAlgoChecker) // Class implementation in ROOT context
/// \endcond


//________________________________________________________________________
AliMUONTriggerAlgoChecker::AliMUONTriggerAlgoChecker() :
TObject()
{
  /// Default Ctor.
}

//________________________________________________________________________
AliMUONTriggerAlgoChecker::~AliMUONTriggerAlgoChecker()
{
  //
  /// Destructor
  //
}

//____________________________________________________________________________
Int_t AliMUONTriggerAlgoChecker::CompareLocalResponse ( AliMUONVTriggerStore& triggersFromRaw,
                                                        AliMUONVTriggerStore& triggersRecomputed ) const
{
  /// Compare re-computed response (from bit pattern) with the raw response

  // Sort triggers

  Int_t nBoardsWithError = 0;

  UInt_t boardErrors = 0;

  TIter next(triggersRecomputed.CreateLocalIterator());
  AliMUONLocalTrigger* recoTrigger = 0x0;
  while ( ( recoTrigger = static_cast<AliMUONLocalTrigger*>(next()) ) )
  {
    Int_t iboard = recoTrigger->LoCircuit();

    AliMUONLocalTrigger* rawTrigger = triggersFromRaw.FindLocal(iboard);

    AliMpLocalBoard* localBoardMp = AliMpDDLStore::Instance()->GetLocalBoard(iboard); // get local board object for switch value

    Bool_t yCopyErr = kFALSE;
    if ( ( recoTrigger->GetY1Pattern() != rawTrigger->GetY1Pattern() ) ||
         ( recoTrigger->GetY2Pattern() != rawTrigger->GetY2Pattern() ) ) {
      yCopyErr = kTRUE;
    }
    if ( ( recoTrigger->GetY3Pattern() != rawTrigger->GetY3Pattern() ) ||
         ( recoTrigger->GetY4Pattern() != rawTrigger->GetY4Pattern() ) ) {
      yCopyErr = kTRUE;

      for ( Int_t ineigh=-1; ineigh<=1; ineigh +=2 ) {
        Int_t switchBit = ( ineigh == -1 ) ? AliMpLocalBoard::kOR1 : AliMpLocalBoard::kOR0;
        if ( ! localBoardMp->GetSwitch(switchBit) ) continue;
        for ( Int_t itype=0; itype<2; itype++ ) {
          AliMUONVTriggerStore* currStore = ( itype == 0 ) ? &triggersRecomputed : &triggersFromRaw;
          AliMUONLocalTrigger* copyNeighbour = static_cast<AliMUONLocalTrigger*>(currStore->FindLocal(iboard+ineigh));
          if ( copyNeighbour ) copyNeighbour->SetAlgoErrors(copyNeighbour->GetAlgoErrors()|AliMUONLocalTrigger::kErrYcopyNeighbour);
        }
      }
    }

    TString debugString = Form("Local board %i", iboard);

    boardErrors = 0;
    if ( yCopyErr ) boardErrors |= AliMUONLocalTrigger::kErrYcopy;

    if ( recoTrigger->LoStripX() != rawTrigger->LoStripX() ) {
      boardErrors |= AliMUONLocalTrigger::kErrXpos;
      debugString += Form("  errXpos (%i, %i)", recoTrigger->LoStripX(), rawTrigger->LoStripX());
    }

    if ( recoTrigger->GetDeviation() != rawTrigger->GetDeviation() ) {
      boardErrors |= AliMUONLocalTrigger::kErrXdev;
      debugString += Form("  errXdev (%i, %i)", recoTrigger->GetDeviation(), rawTrigger->GetDeviation());
    }

    if ( recoTrigger->GetLoDecision() != rawTrigger->GetLoDecision() ) {
      boardErrors |= AliMUONLocalTrigger::kErrLoDecision;
      debugString += Form("  errDecision (%i, %i)", recoTrigger->GetLoDecision(), rawTrigger->GetLoDecision());
    }

    if ( recoTrigger->LoLpt() != rawTrigger->LoLpt() ) {
      boardErrors |= AliMUONLocalTrigger::kErrLoLpt;
      debugString += Form("  errLpt (%i, %i)", recoTrigger->LoLpt(), rawTrigger->LoLpt());
    }

    if ( recoTrigger->LoHpt() != rawTrigger->LoHpt() ) {
      boardErrors |= AliMUONLocalTrigger::kErrLoHpt;
      debugString += Form("  errHpt (%i, %i)", recoTrigger->LoHpt(), rawTrigger->LoHpt());
    }

    if ( recoTrigger->LoStripY() != rawTrigger->LoStripY() ) {
      boardErrors |= AliMUONLocalTrigger::kErrYpos;
      debugString += Form("  errYpos (%i, %i)", recoTrigger->LoStripY(), rawTrigger->LoStripY());
    }

    if ( recoTrigger->LoTrigY() != rawTrigger->LoTrigY()  ) {
      boardErrors |= AliMUONLocalTrigger::kErrYtrig;
      debugString += Form("  errYtrig (%i, %i)", recoTrigger->LoTrigY(), rawTrigger->LoTrigY());
    }

    if ( boardErrors ) {
      nBoardsWithError++;
      recoTrigger->SetAlgoErrors(recoTrigger->GetAlgoErrors()|boardErrors);
      rawTrigger->SetAlgoErrors(rawTrigger->GetAlgoErrors()|boardErrors);
      debugString += Form("  errCode = 0x%x\n",recoTrigger->GetAlgoErrors());
      AliDebug(1,debugString.Data());
    }
  } // loop on local boards

  return  nBoardsWithError;

}

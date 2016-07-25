#ifndef ALIMUONTRIGGERALGOCHECKER_h
#define ALIMUONTRIGGERALGOCHECKER_h

/// \class AliMUONTriggerAlgoChecker
/// \brief Recompute the muon trigger response and compare with input
///
/// The class recompute the muon trigger response and compares with input
///
/// \author Diego Stocco <dstocco@cern.ch>, Subatech
/// \date Jul 5, 2016

#include "TObject.h"

class AliMUONVTriggerStore;

class AliMUONTriggerAlgoChecker : public TObject {

public:
  AliMUONTriggerAlgoChecker();
  virtual ~AliMUONTriggerAlgoChecker();

  Int_t CompareLocalResponse ( AliMUONVTriggerStore& triggersFromRaw, AliMUONVTriggerStore& triggersRecomputed ) const;

private:

  /// \cond CLASSIMP
  ClassDef(AliMUONTriggerAlgoChecker, 0); // Trigger algorithm checker
  /// \endcond
};

#endif

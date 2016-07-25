#ifndef ALIMUONTRIGGERSTOREV2_H
#define ALIMUONTRIGGERSTOREV2_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
* See cxx source for full Copyright notice                               */

// $Id$

/// \ingroup base
/// \class AliMUONTriggerStoreV2
/// \brief Implementation of AliMUONVTriggerStore
///
/// This is actually equal to AliMUONTriggerStoreV1, but allows to store the
/// objects in the RecPoints with a different name. It is used to store
/// the trigger response re-computed offline
///
/// \author Diego Stocco <dstocco@cern.ch>, Subatech
/// \date Jul 5, 2016

#include "AliMUONTriggerStoreV1.h"

class AliMUONTriggerStoreV2 : public AliMUONTriggerStoreV1
{
public:
  AliMUONTriggerStoreV2();
  virtual ~AliMUONTriggerStoreV2();

  virtual Bool_t Connect(TTree& tree, Bool_t alone=kTRUE) const;

  ClassDef(AliMUONTriggerStoreV2,1) // Implementation of AliMUONVTriggerStore
};

#endif

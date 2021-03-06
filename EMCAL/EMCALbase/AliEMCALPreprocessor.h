#ifndef ALIEMCALPREPROCESSOR_H
#define ALIEMCALPREPROCESSOR_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////////////
///  
/// \class AliEMCALPreprocessor
/// \ingroup EMCALbase
/// \brief Process data in shuttle to feed OCDB  
///
/// EMCAL Preprocessor class. It runs by Shuttle at the end of the run,
/// calculates stuff to be posted in OCDB
///
/// Adapted from AliPHOSPreprocessor.
///
/// \author: Boris Polichtchouk, (IHEP) (For PHOS)
/// \author Gustavo Conesa Balbastre <Gustavo.Conesa.Balbastre@cern.ch>, LPSC-IN2P3-CNRS, copy from PHOS
/// \author David Silvermyr (ORNL), May 2008, update based on TPC code
/// \author Markus Fasel (ORNL), November 2015, adding DCAL STU
///
////////////////////////////////////////////////////////////////////////////////

#include "AliPreprocessor.h"

class AliDCSValue;
class AliEMCALSensorTempArray;
class TEnv;

class AliEMCALPreprocessor : public AliPreprocessor 
{

 public:
  
  AliEMCALPreprocessor(); //! ctor
  AliEMCALPreprocessor(AliShuttleInterface* shuttle); //! overloaded ctor
  AliEMCALPreprocessor(const AliEMCALPreprocessor &); //! copy ctor
  AliEMCALPreprocessor& operator = (const  AliEMCALPreprocessor &source); //! assignment operator
  virtual ~AliEMCALPreprocessor();//! dtor

 protected:

  virtual void Initialize(Int_t run, UInt_t startTime, UInt_t endTime);
  virtual UInt_t Process(TMap* dcsAliasMap);
  
  UInt_t  MapTemperature  (TMap* dcsAliasMap);
  UInt_t  MapTriggerConfig(TMap* dcsAliasMap);
 
  UInt_t  ExtractPedestals(Int_t sourceFXS);
  UInt_t  ExtractSignal   (Int_t sourceFXS);

  AliDCSValue *ReadDCSValue(const TMap *values, const char *valname);

 private:

  TEnv                     *fConfEnv;   ///< Preprocessor configuration map
  AliEMCALSensorTempArray  *fTemp;      ///< CDB class for temperature sensors
  Bool_t                    fConfigOK;  ///< Identify succesful reading of OCDB Config
    
  /// \cond CLASSIMP
  ClassDef(AliEMCALPreprocessor,1);
  /// \endcond

};

#endif //ALIEMCALPREPROCESSOR_H

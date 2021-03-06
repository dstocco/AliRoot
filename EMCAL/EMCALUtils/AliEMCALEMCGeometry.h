#ifndef ALIEMCALEMCGEOMETRY_H
#define ALIEMCALEMCGEOMETRY_H
/* Copyright(c) 1998-2004, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//_________________________________________________________________________
/// \class AliEMCALEMCGeometry
/// \ingroup EMCALUtils
/// \brief EMCal geometry SM base, singleton
///
/// Geometry class  for EMCAL : singleton
/// EMCAL consists of layers of scintillator and lead
/// with scintillator fiber arranged as "shish-kebab" skewers
/// Places the the Barrel Geometry of The EMCAL at Midrapidity
/// between 80 and 180(or 190) degrees of Phi and
/// -0.7 to 0.7 in eta
///
///     EMCAL geometry tree:
///     EMCAL -> superModule -> module -> tower(cell)
///     Indexes
///     absId -> nSupMod     -> nModule -> (nIphi,nIeta)
///
///   Name choices:
///   EMCAL_PDC06 (geometry used for PDC06 simulations, kept for backward compatibility)
///      = equivalent to SHISH_77_TRD1_2X2_FINAL_110DEG in old notation
///   EMCAL_COMPLETE (geometry for expected complete detector)
///      = equivalent to SHISH_77_TRD1_2X2_FINAL_110DEG scTh=0.176 pbTh=0.144
///          in old notation
///
///   EMCAL_FIRSTYEAR - geometry for December 2009 to December 2010 run period
///                     with four Super Modules
///
///   Adding V1 (EMCAL_FIRSTYEARV1, EMCAL_COMPLETEV1) - geometry from December 2009 ;
///                1. Fixed bug for positions of modules inside SM
///                   (first module has tilt 0.75 degree);
///                2. Added Al front plate (width 1 cm) and 2 paper sheets per sampling
///                   layer (additional 0.2 mm)
///                   The sizes have updated with last information from production
///                   drawing (end of October 2010).
///                3. COMPLETEV1 contains now only 10 SM for runs for year 2011
///                4. COMPLETE12SMV1 contains 12 SM for runs from year 2012 and on
///                5. COMPLETE12SMV1_DCAL contains 12 SM and 6 DCal SM -- not for standard user mode
///                6. COMPLETE12SMV1_DCAL_DEV contains 12 SM and 10 DCal SM -- not for standard user mode
///                7. COMPLETE12SMV1_DCAL_8SM contains 12 SM and 6 DCal SM and 2 extentions
///
///   EMCAL_WSUC (Wayne State test stand)
///      = no definite equivalent in old notation, was only used by
///          Aleksei, but kept for testing purposes
///
///   etc.
///
/// \author Sahal Yacoob (LBL / UCT)
/// \author Yves Schutz (SUBATECH)
/// \author Jennifer Klay (LBL)
/// \author Aleksei Pavlinov (WSU)
/// \author Magali Estienne (SUBATECH)
/// \author M.L. Wang CCNU Wuhan & Subatech. Adapted for DCAL
//_________________________________________________________________________

// --- ROOT system ---
#include <TMath.h>
#include <TArrayD.h>
#include <TNamed.h>
class TString ;
class TObjArray;
class Riostream;

// --- AliRoot header files ---
class AliEMCALEMCGeometry;
class AliEMCALShishKebabTrd1Module;

class AliEMCALEMCGeometry : public TNamed 
{
public:
  
  /// possible SM Type
  enum fEMCSMType { kNotExistent = -1, kEMCAL_Standard = 0, kEMCAL_Half = 1, kEMCAL_3rd = 2, kDCAL_Standard = 3, kDCAL_Ext= 4 }; 
  
  AliEMCALEMCGeometry(); // default ctor only for internal usage (singleton)
  
  AliEMCALEMCGeometry(const AliEMCALEMCGeometry& geom);
  
  AliEMCALEMCGeometry(const Text_t* name, const Text_t* title,
                      const Text_t* mcname="", const Text_t* mctitle="");

  virtual ~AliEMCALEMCGeometry(void); 

  /// Assignement operator requested by coding convention but not needed
  AliEMCALEMCGeometry & operator = (const AliEMCALEMCGeometry  & /*rvalue*/) 
  {
    Fatal("operator =", "not implemented");
    return *this;
  };

  //////////
  // General
  //
  
  Bool_t IsInitialized(void) const { return fgInit ; }
  static const Char_t* GetDefaultGeometryName() { return fgkDefaultGeometryName ; }
  void   PrintGeometry();        //*MENU*  
  
  void   Init(const Text_t* mcname="", const Text_t* mctitle=""); 
  void   CheckAdditionalOptions();        //
  void   DefineSamplingFraction(const Text_t* mcname="", const Text_t* mctitle="");

  //////////////////////////////////////
  // Return EMCAL geometrical parameters
  //
  
  TString GetGeoName()               const { return fGeoName;}
  
  const Int_t * GetEMCSystem()       const { return fEMCSMSystem ; } 
        Int_t * GetEMCSystem()             { return fEMCSMSystem ; } // Why? GCB
  
  const Char_t* GetNameOfEMCALEnvelope() const { const Char_t* env = "XEN1"; return env ;}
  
  Float_t GetArm1PhiMin()            const { return fArm1PhiMin ; }
  Float_t GetArm1PhiMax()            const { return fArm1PhiMax ; }
  Float_t GetArm1EtaMin()            const { return fArm1EtaMin ; }
  Float_t GetArm1EtaMax()            const { return fArm1EtaMax ; }
  Float_t GetIPDistance()            const { return fIPDistance ; }
  
  Float_t GetEnvelop(Int_t index)    const { return fEnvelop[index] ; }
  Float_t GetShellThickness()        const { return fShellThickness ; }
  Float_t GetZLength()               const { return fZLength    ; }
 
  Float_t GetDCALInnerEdge()         const { return fDCALInnerEdge ; }
  Float_t GetDCALPhiMin()            const { return fDCALPhiMin ; }
  Float_t GetDCALPhiMax()            const { return fDCALPhiMax ; }
  Float_t GetDCALInnerExtandedEta()  const { return fDCALInnerExtandedEta ; }
  Float_t GetEMCALPhiMax()           const { return fEMCALPhiMax ; }
  Float_t GetDCALStandardPhiMax()    const { return fDCALStandardPhiMax ; }
  
  Int_t   GetNECLayers()             const { return fNECLayers ; }
  Int_t   GetNZ()                    const { return fNZ ; }
  Int_t   GetNEta()                  const { return fNZ ; }
  Int_t   GetNPhi()                  const { return fNPhi ; }
  
  Float_t GetECPbRadThick()          const { return fECPbRadThickness ; }
  Float_t GetECScintThick()          const { return fECScintThick ; }
  Float_t GetSampling()              const { return fSampling ; }
  
  Int_t   GetNumberOfSuperModules()  const { return fNumberOfSuperModules ; }
  Float_t GetPhiGapForSuperModules() const { return fPhiGapForSM ; }
  Float_t GetPhiModuleSize()         const { return fPhiModuleSize ; }
  Float_t GetEtaModuleSize()         const { return fEtaModuleSize ; }
  
  Float_t GetFrontSteelStrip()       const { return fFrontSteelStrip   ; }
  Float_t GetLateralSteelStrip()     const { return fLateralSteelStrip ; }
  Float_t GetPassiveScintThick()     const { return fPassiveScintThick ; }
  
  Float_t GetPhiTileSize()           const { return fPhiTileSize ; }
  Float_t GetEtaTileSize()           const { return fEtaTileSize ; }
  
  Float_t GetPhiSuperModule()        const { return fPhiSuperModule;}
  Int_t   GetNPhiSuperModule()       const { return fNPhiSuperModule;}
  
  Int_t   GetNPHIdiv()               const { return fNPHIdiv ; }
  Int_t   GetNETAdiv()               const { return fNETAdiv ; }
  Int_t   GetNCells()                const { return fNCells  ; }
  Float_t GetLongModuleSize()        const { return fLongModuleSize ; }
 
  Float_t GetTrd1Angle()             const { return fTrd1Angle          ; }
  Float_t Get2Trd1Dx2()              const { return f2Trd1Dx2           ; }
  Float_t GetEtaMaxOfTRD1()          const { return fEtaMaxOfTRD1       ; }
  Float_t GetTrd1AlFrontThick()      const { return fTrd1AlFrontThick   ; }
  Float_t GetTrd1BondPaperThick()    const { return fTrd1BondPaperThick ; }
  // --
  Int_t   GetNCellsInSupMod()        const { return fNCellsInSupMod ; }
  Int_t   GetNCellsInModule()        const { return fNCellsInModule ; }
  Int_t   GetKey110DEG()             const { return fKey110DEG      ; }
  Int_t   GetnSupModInDCAL()         const { return fnSupModInDCAL  ; }
  
  Int_t   GetILOSS()                 const { return fILOSS ; }
  Int_t   GetIHADR()                 const { return fIHADR ; }  

  // --
  Float_t  GetDeltaEta()             const { return (fArm1EtaMax-fArm1EtaMin)/ ((Float_t)fNZ)   ; }
  Float_t  GetDeltaPhi()             const { return (fArm1PhiMax-fArm1PhiMin)/ ((Float_t)fNPhi) ; }
  Int_t    GetNTowers()              const { return fNPhi * fNZ ; }
  //
  Double_t GetPhiCenterOfSM   (Int_t nsupmod) const;
  Double_t GetPhiCenterOfSMSec(Int_t nsupmod) const;
  Float_t  GetSuperModulesPar (Int_t ipar)    const { return fParSM[ipar];}
  Int_t    GetSMType(Int_t nSupMod)  const { if( nSupMod > GetNumberOfSuperModules() ) return kNotExistent;
                                             return fEMCSMSystem[nSupMod];                                 }
  //
  Bool_t   GetPhiBoundariesOfSM   (Int_t nSupMod, Double_t &phiMin, Double_t &phiMax) const;
  Bool_t   GetPhiBoundariesOfSMGap(Int_t nPhiSec, Double_t &phiMin, Double_t &phiMax) const;
  
  //
  static int ParseString(const TString &topt, TObjArray &Opt) ; 

  ///////////////////////////////
  //Geometry data member setters
  //
  void SetNZ(Int_t nz)           { fNZ= nz; 
                                   printf("SetNZ: Number of modules in Z set to %d"    , fNZ) ; }
  void SetNPhi(Int_t nphi)       { fNPhi= nphi; 
                                   printf("SetNPhi: Number of modules in Phi set to %d", fNPhi) ; }
  void SetSampling(Float_t samp) { fSampling = samp; 
                                   printf("SetSampling: Sampling factor set to %f"     , fSampling) ; }

  ///////////////////
  // useful utilities
  //
  /// \return theta in radians for a given pseudorapidity
  Float_t AngleFromEta(Float_t eta) const { 
    return 2.0*TMath::ATan(TMath::Exp(-eta));
  }
  
  /// \return z in for a given pseudorapidity and r=sqrt(x*x+y*y).
  Float_t ZFromEtaR(Float_t r,Float_t eta) const { 
    return r/TMath::Tan(AngleFromEta(eta));
  }

  //////////////////////////////////////////////////
  // Obsolete?
  Float_t GetSteelFrontThickness() const { return fSteelFrontThick;}
  //////////////////////////////////////////////////

  static const Char_t* fgkDefaultGeometryName; ///< Default name of geometry
  static Bool_t  fgInit;                 ///< Tells if geometry has been succesfully set up.

private:

  // Member data
  
  TString fGeoName;                      ///< geometry name

  TObjArray *fArrayOpts;                 //!<! array of geometry options
  const char *fkAdditionalOpts[6];       //!<! some additional options for the geometry type and name
  int  fNAdditionalOpts;                 //!<! size of additional options parameter

  Float_t fECPbRadThickness;             ///< cm, Thickness of the Pb radiators
  Float_t fECScintThick;                 ///< cm, Thickness of the scintillators
  Int_t   fNECLayers;                    ///< number of scintillator layers
  
  Float_t fArm1PhiMin; 			             ///< Minimum angular position of EMCAL in Phi (degrees)
  Float_t fArm1PhiMax;			             ///< Maximum angular position of EMCAL in Phi (degrees)
  Float_t fArm1EtaMin;			             ///< Minimum pseudorapidity position of EMCAL in Eta
  Float_t fArm1EtaMax; 			             ///< Maximum pseudorapidity position of EMCAL in Eta
  
  // Geometry Parameters
  Float_t fEnvelop[3];                   ///< the GEANT TUB for the detector 
  Float_t fIPDistance;                   ///< Radial Distance of the inner surface of the EMCAL
  Float_t fShellThickness;               ///< Total thickness in (x,y) direction
  Float_t fZLength;                      ///< Total length in z direction
  Float_t fDCALInnerEdge;                ///< Inner edge for DCAL
  Float_t fDCALPhiMin;	                 ///< Minimum angular position of DCAL in Phi (degrees)
  Float_t fDCALPhiMax;	                 ///< Maximum angular position of DCAL in Phi (degrees)
  Float_t fEMCALPhiMax;                  ///< Maximum angular position of EMCAL in Phi (degrees)
  Float_t fDCALStandardPhiMax;           ///< special edge for the case that DCAL contian extension
  Float_t fDCALInnerExtandedEta;         ///< DCAL inner edge in Eta (with some extension)
  Int_t   fNZ;                           ///< Number of Towers in the Z direction
  Int_t   fNPhi;                         ///< Number of Towers in the PHI direction
  Float_t fSampling;                     ///< Sampling factor

  // Shish-kebab option - 23-aug-04 by PAI; COMPACT, TWIST, TRD1 and TRD2
  Int_t   fNumberOfSuperModules;         ///< default is 12 = 6 * 2
  
  /// geometry structure 
  Int_t  *fEMCSMSystem;                  //[fNumberOfSuperModules] 
  
  Float_t fFrontSteelStrip;              ///< 13-may-05
  Float_t fLateralSteelStrip;            ///< 13-may-05
  Float_t fPassiveScintThick;            ///< 13-may-05
  
  Float_t fPhiModuleSize;                ///< Phi -> X 
  Float_t fEtaModuleSize;                ///< Eta -> Y
  Float_t fPhiTileSize;                  ///< Size of phi tile
  Float_t fEtaTileSize;                  ///< Size of eta tile
  
  Float_t fLongModuleSize;               ///< Size of long module
  Float_t fPhiSuperModule;               ///< Phi of normal supermodule (20, in degree)
  Int_t   fNPhiSuperModule;              ///< 9 - number supermodule in phi direction
  
  Int_t   fNPHIdiv;                      ///< number phi divizion of module
  Int_t   fNETAdiv;                      ///< number eta divizion of module
  //
  Int_t   fNCells;                       ///< number of cells in calo
  Int_t   fNCellsInSupMod;               ///< number cell in super module
  Int_t   fNCellsInModule;               ///< number cell in module)

  // TRD1 options - 30-sep-04
  Float_t fTrd1Angle;                    ///< angle in x-z plane (in degree) 
  Float_t f2Trd1Dx2;                     ///< 2*dx2 for TRD1
  Float_t fPhiGapForSM;                  ///< Gap betweeen supermodules in phi direction
  Int_t   fKey110DEG;                    ///< for calculation abs cell id; 19-oct-05 
  Int_t   fnSupModInDCAL;                ///< for calculation abs cell id;
  TArrayD fPhiBoundariesOfSM;            ///< phi boundaries of SM in rad; size is fNumberOfSuperModules;
  TArrayD fPhiCentersOfSM;               ///< phi of centers of SM; size is fNumberOfSuperModules/2
  TArrayD fPhiCentersOfSMSec;            ///< phi of centers of section where SM lies; size is fNumberOfSuperModules/2
  Float_t fEtaMaxOfTRD1;                 ///< max eta in case of TRD1 geometry (see AliEMCALShishKebabTrd1Module)
  
  // Oct 26,2010
  Float_t fTrd1AlFrontThick;             ///< Thickness of the Al front plate  
  Float_t fTrd1BondPaperThick;           ///< Thickness of the Bond Paper sheet  
  
  // Local Coordinates of SM
  TArrayD fCentersOfCellsEtaDir;         ///< size fNZ*fNETAdiv (for TRD1 only) (eta or z in SM, in cm)
  TArrayD fCentersOfCellsXDir;           ///< size fNZ*fNETAdiv (for TRD1 only) (       x in SM, in cm)
  TArrayD fCentersOfCellsPhiDir;         ///< size fNPhi*fNPHIdiv (for TRD1 only) (phi or y in SM, in cm)
  //
  TArrayD fEtaCentersOfCells;            ///< [fNZ*fNETAdiv*fNPhi*fNPHIdiv], positive direction (eta>0); eta depend from phi position; 
  TArrayD fPhiCentersOfCells;            ///< [fNPhi*fNPHIdiv] from center of SM (-10. < phi < +10.)
  
  // Move from AliEMCALv0 - Feb 19, 2006
  TList   *fShishKebabTrd1Modules;       //!<! list of modules
  
  // Local coordinates of SM for TRD1
  Float_t fParSM[3];                     ///< SM sizes as in GEANT (TRD1)

  Int_t   fILOSS;                        ///< Options for Geant (MIP business) - will call in AliEMCAL
  Int_t   fIHADR;                        ///< Options for Geant (MIP business) - will call in AliEMCAL

  Float_t fSteelFrontThick;              ///< Thickness of the front stell face of the support box - 9-sep-04; obsolete?
  
  /// \cond CLASSIMP
  ClassDef(AliEMCALEMCGeometry, 3) ;
  /// \endcond

};

#endif // AliEMCALEMCGEOMETRY_H

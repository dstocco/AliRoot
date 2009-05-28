#include "TClass.h"
#include "TMethod.h"
#include "TMethodCall.h"
#include "TMethodArg.h"
#include "TFile.h"
#include "TList.h"
#include "TH1.h"
#include "TF1.h"
#include "TObjArray.h"
#include "TDirectory.h"
#include "TTreeStream.h"

#include "AliLog.h"
#include "AliAnalysisTask.h"

#include "AliTRDrecoTask.h"

ClassImp(AliTRDrecoTask)
FILE* AliTRDrecoTask::fgFile = 0x0;
//_______________________________________________________
AliTRDrecoTask::AliTRDrecoTask(const char *name, const char *title)
  : AliAnalysisTask(name, title)
  ,fNRefFigures(0)
  ,fDebugLevel(0)
  ,fPlotFuncList(0x0)
  ,fContainer(0x0)
  ,fTracks(0x0)
  ,fTrack(0x0)
  ,fMC(0x0)
  ,fESD(0x0)
  ,fDebugStream(0x0)
{
  DefineInput(0, TObjArray::Class());
  DefineOutput(0, TObjArray::Class());
}

//_______________________________________________________
AliTRDrecoTask::~AliTRDrecoTask() 
{
  printf(" %s (%s)\n", GetName(), GetTitle());
  if(fDebugStream){ 
    delete fDebugStream;
    fDebugStream = 0x0;
  }

  if(fPlotFuncList){
    fPlotFuncList->Delete();
    delete fPlotFuncList;
    fPlotFuncList = 0x0;
  }
  
  if(fContainer){
    if(fContainer->IsOwner()) fContainer->Delete();
    delete fContainer;
    fContainer = 0x0;
  }

  if(fgFile){
    fflush(fgFile);
    fclose(fgFile);
    fgFile = 0x0;
  }
}

//_______________________________________________________
void AliTRDrecoTask::ConnectInputData(Option_t *)
{
  //
  // Connect input data
  //

  fTracks = dynamic_cast<TObjArray *>(GetInputData(0));
}

//_______________________________________________________
void AliTRDrecoTask::Exec(Option_t *)
{
  if(!fPlotFuncList){
    AliWarning("No functor list defined for the reference plots");
    return;
  }
  if(!fTracks) return;
  if(!fTracks->GetEntriesFast()) return;
  
  AliTRDtrackInfo *trackInfo = 0x0;
  TIter plotIter(fPlotFuncList);
  TObjArrayIter trackIter(fTracks);
  while((trackInfo = dynamic_cast<AliTRDtrackInfo*>(trackIter()))){
    fTrack = trackInfo->GetTrack();
    fMC    = trackInfo->GetMCinfo();
    fESD   = trackInfo->GetESDinfo();

    TMethodCall *plot = 0x0;
    plotIter.Reset();
    while((plot=dynamic_cast<TMethodCall*>(plotIter()))){
      plot->Execute(this);
    }
  }
  PostData(0, fContainer);
}

//_______________________________________________________
Bool_t AliTRDrecoTask::GetRefFigure(Int_t /*ifig*/)
{
  AliWarning("Retrieving reference figures not implemented.");
  return kFALSE;
}

//_______________________________________________________
Bool_t AliTRDrecoTask::PutTrendValue(Char_t *name, Double_t val, Double_t err)
{
  if(!fgFile){
    fgFile = fopen("TRD.Performance.txt", "at");
  }
  fprintf(fgFile, "%s_%s %f %f\n", GetName(), name, val, err);
  return kTRUE;
}

//_______________________________________________________
void AliTRDrecoTask::InitFunctorList()
{
  TClass *c = this->IsA();

  TMethod *m = 0x0;
  TIter methIter(c->GetListOfMethods());
  while((m=dynamic_cast<TMethod*>(methIter()))){
    TString name(m->GetName());
    if(!name.BeginsWith("Plot")) continue;
    if(!fPlotFuncList) fPlotFuncList = new TList();
    fPlotFuncList->AddLast(new TMethodCall(c, (const char*)name, ""));
  }
}

//_______________________________________________________
Bool_t AliTRDrecoTask::Load(const Char_t *filename)
{
  if(!TFile::Open(filename)){
    AliWarning(Form("Couldn't open file %s.", filename));
    return kFALSE;
  }
  TObjArray *o = 0x0;
  if(!(o = (TObjArray*)gFile->Get(GetName()))){
    AliWarning("Missing histogram container.");
    return kFALSE;
  }
  fContainer = (TObjArray*)o->Clone(GetName());
  gFile->Close();
  return kTRUE;
}

//________________________________________________________
Bool_t AliTRDrecoTask::Save(TObjArray *results){
  //
  // Store the output graphs in a ROOT file
  // Input TObject array will not be written as Key to the file,
  // only content itself
  //

  TDirectory *cwd = gDirectory;
  if(!TFile::Open(Form("TRD.Result%s.root", GetName()), "RECREATE")) return kFALSE;

  TIterator *iter = results->MakeIterator();
  TObject *inObject = 0x0, *outObject = 0x0;
  while((inObject = iter->Next())){
    outObject = inObject->Clone();
    outObject->Write(0x0, TObject::kSingleKey);
  }
  delete iter;
  gFile->Close(); delete gFile;
  cwd->cd(); 
  return kTRUE;
}

//_______________________________________________________
Bool_t AliTRDrecoTask::PostProcess()
{
  AliWarning("Post processing of reference histograms not implemented.");
  return kFALSE;
}

//_______________________________________________________
void AliTRDrecoTask::SetDebugLevel(Int_t level)
{
  fDebugLevel = level;
  if(fDebugLevel>=1){
    TDirectory *savedir = gDirectory;
    fDebugStream = new TTreeSRedirector(Form("TRD.DBG%s.root", GetName()));
    savedir->cd();
  }
}

//________________________________________________________
void AliTRDrecoTask::Adjust(TF1 *f, TH1 *h)
{
// Helper function to avoid duplication of code
// Make first guesses on the fit parameters

  // find the intial parameters of the fit !! (thanks George)
  Int_t nbinsy = Int_t(.5*h->GetNbinsX());
  Double_t sum = 0.;
  for(Int_t jbin=nbinsy-4; jbin<=nbinsy+4; jbin++) sum+=h->GetBinContent(jbin); sum/=9.;
  f->SetParLimits(0, 0., 3.*sum);
  f->SetParameter(0, .9*sum);

  f->SetParLimits(1, -.2, .2);
  f->SetParameter(1, -0.1);

  f->SetParLimits(2, 0., 4.e-1);
  f->SetParameter(2, 2.e-2);
  if(f->GetNpar() <= 4) return;

  f->SetParLimits(3, 0., sum);
  f->SetParameter(3, .1*sum);

  f->SetParLimits(4, -.3, .3);
  f->SetParameter(4, 0.);

  f->SetParLimits(5, 0., 1.e2);
  f->SetParameter(5, 2.e-1);
}

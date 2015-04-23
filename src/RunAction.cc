//run action implementation

#include "RunAction.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "TString.h"
#include <time.h>
#include <cmath>
#include <iostream>
#include <string>


RunAction::RunAction(std::shared_ptr<SimConfiguration> simConf) :
  G4UserRunAction(),
  simConf_(simConf)
{
}

RunAction::~RunAction(){
}

G4Run* RunAction::GenerateRun(){
  theRun_ = new Run(simConf_);
  return theRun_;
}

void RunAction::BeginOfRunAction(const G4Run*){
  simConf_->update();
  theRun_->initializeFile();
}

void RunAction::EndOfRunAction(const G4Run*){
  theRun_->closeFile();
}

Run::Run(std::shared_ptr<SimConfiguration> simConf):
  simConf_(simConf)
{
}

Run::~Run(){
}

void Run::initializeFile(){
  std::string fileName = Form("outDelta%.2f%s_x0_%.0f_y0_%.0f_n_%.2f.root",
			      simConf_->gen.delta*100,
			      "%",
			      simConf_->gen.x0,
			      simConf_->gen.y0,
			      simConf_->det.fieldIndex);
  file_ = new TFile(fileName.c_str(), "recreate");
}

void Run::beginNewEvent(G4int eventID){
  yGraph_ = new TGraph();
  yGraph_->SetName(Form("yGraph%i", eventID));
  yGraph_->SetTitle(Form("yGraph%i", eventID));
  xGraph_ = new TGraph();
  xGraph_->SetName(Form("xGraph%i", eventID));
  xGraph_->SetTitle(Form("xGraph%i", eventID));
  thetaGraph_ = new TGraph();
  thetaGraph_->SetName(Form("thetaGraph%i", eventID));
  thetaGraph_->SetTitle(Form("thetaGraph%i", eventID));
  zeroCrossings_ = 0;
}

void Run::recordEndOfEvent(){
  yGraph_->Write();
  xGraph_->Write();
  thetaGraph_->Write();
}

void Run::recordPlaneCrossing(G4int planeNo, G4double t, const G4ThreeVector& position){
  if(planeNo == 0){
    zeroCrossings_ +=1;
  }
  G4double theta = 360 * deg * ( static_cast<double>(planeNo) / simConf_->det.nPlanes + zeroCrossings_);
  G4ThreeVector rho(position.x(), position.y(), 0);
  G4double x = rho.mag() - 7.112*m;
  G4double y = position.z();
  yGraph_->SetPoint(yGraph_->GetN(), theta, y);
  xGraph_->SetPoint(xGraph_->GetN(), theta, x);
  thetaGraph_->SetPoint(thetaGraph_->GetN(), t, theta);
}

void Run::closeFile(){
  file_->Write();
  file_->Close();
}

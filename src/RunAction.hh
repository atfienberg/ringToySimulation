//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm2/include/RunAction.hh
/// \brief Definition of the RunAction class
//
// $Id: RunAction.hh 78550 2014-01-07 09:43:41Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include "G4Run.hh"
#include "SimConfiguration.hh"
#include "TGraph.h"
#include "TFile.h"

#include <string>
#include <memory>

class Run : public G4Run{
public:
  Run(std::shared_ptr<SimConfiguration> simConf);
  virtual ~Run();
  
  void initializeFile();
  void beginNewEvent(G4int eventID);
  void recordEndOfEvent();
  void closeFile();

  void recordPlaneCrossing(G4int planeNo, G4double t, const G4ThreeVector& position);

private:
  TFile* file_;
  TGraph* yGraph_;
  TGraph* xGraph_;
  TGraph* thetaGraph_;
  std::shared_ptr<SimConfiguration> simConf_;
  G4int zeroCrossings_;
};

class RunAction : public G4UserRunAction
{
public:
  
  RunAction(std::shared_ptr<SimConfiguration> simConf);
  virtual ~RunAction();

  virtual G4Run* GenerateRun();  
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  Run* theRun_;
  std::shared_ptr<SimConfiguration> simConf_;
};



  
  



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


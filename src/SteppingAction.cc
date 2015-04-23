#include "SteppingAction.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include <iostream>

SteppingAction::SteppingAction():
  G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction(){
}

void SteppingAction::UserSteppingAction(const G4Step* step){
  /*  std::cout << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << std::endl;
  std::cout << step->GetPostStepPoint()->GetGlobalTime() / nanosecond << std::endl;
  std::cout << step->GetPreStepPoint()->GetPosition().z() << std::endl;
  std::cout << step->GetPostStepPoint()->GetPosition().z() << std::endl;
  std::cout << step->GetPreStepPoint()->GetTotalEnergy() << std::endl;;
  std::cin.ignore();*/
  G4StepPoint* pStep = step->GetPostStepPoint();
  G4VPhysicalVolume* pStepPV = pStep->GetPhysicalVolume();
  if(pStepPV){
    if(pStepPV->GetName()=="tPlane"){
      if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ringRegion"){
	Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	run->recordPlaneCrossing(pStepPV->GetCopyNo(), 
				 pStep->GetGlobalTime(), 
				 pStep->GetPosition());
      }
    }
  }
  if(pStep->GetGlobalTime() > 10 * microsecond){
    step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

#include "SteppingAction.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
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
  G4VPhysicalVolume* pStepPV = step->GetPostStepPoint()->GetPhysicalVolume();
  if(pStepPV){
    if(pStepPV->GetName()=="tPlane"){
      if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ringRegion"){
	//	std::cout << "tick" <<std:: endl;
      }
    }
  }
  if(step->GetPostStepPoint()->GetGlobalTime() > 30 * microsecond){
    step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

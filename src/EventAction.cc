#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "RunAction.hh"

EventAction::EventAction():
  G4UserEventAction()
{
}

EventAction::~EventAction(){
}

void EventAction::BeginOfEventAction(const G4Event* evt){
  Run* run =  
    static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  
  run->beginNewEvent( evt->GetEventID() );
  
  if( evt->GetEventID() % 100 == 0){
    std::cout << "Processed Event " << evt->GetEventID() << std::endl;
  }
}

void EventAction::EndOfEventAction(const G4Event*){
  Run* run =  
    static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  
  run->recordEndOfEvent();
}

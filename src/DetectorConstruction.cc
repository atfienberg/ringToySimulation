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
// $Id: DetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Torus.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4ParticleTable.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include <iostream>
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  const double RINGRADIUS = 7.112*m;
  const double TUBERADIUS = 50*mm;

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*RINGRADIUS;
  G4double world_sizeZ  = 1.2*TUBERADIUS;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       world_sizeXY, world_sizeXY, world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // RING 
  //
  G4Torus* solidRingRegion = new G4Torus("ringRegion", 
					 0, TUBERADIUS+1*mm, RINGRADIUS, 0, 360*degree);
  G4LogicalVolume* logicRingRegion = 
    new G4LogicalVolume(solidRingRegion,
			world_mat,
			"ringRegion");
  new G4PVPlacement(0,
		    G4ThreeVector(),
		    logicRingRegion,
		    "ringRegion",
		    logicWorld,
		    false,
		    0,
		    checkOverlaps);

  G4Material* ringMaterial = nist->FindOrBuildMaterial("G4_Al");
  G4Torus* solidRing =    
    new G4Torus("ring",                    //its name
	      TUBERADIUS, TUBERADIUS+1*mm, RINGRADIUS, 0, 360*degree);
      
  G4LogicalVolume* logicRing =                         
    new G4LogicalVolume(solidRing,            //its solid
                        ringMaterial,             //its material
                        "ring");         //its name

  G4VisAttributes* ringAttr = new G4VisAttributes(G4Colour::White());
  ringAttr->SetForceWireframe(true);
  ringAttr->SetVisibility(true);
  logicRing->SetVisAttributes(ringAttr);

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicRing,                //its logical volume
                    "ring",              //its name
                    logicRingRegion,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  //tracking planes
  G4int nPlanes = 45;
  G4double planeWidth = .01*mm;
  G4Tubs* solidPlane = new G4Tubs("tPlane", 
				  0, 
				  45,
				  planeWidth,
				  0,
				  360*deg);
  G4LogicalVolume* logicPlane =
    new G4LogicalVolume(solidPlane, 
			world_mat,
			"tPlane");
  G4VisAttributes* planeAttr = new G4VisAttributes(G4Colour::Magenta());
  planeAttr->SetVisibility(true);
  planeAttr->SetForceSolid(true);
  logicPlane->SetVisAttributes(planeAttr);
  
  //placement test
  for(int i = 0; i < nPlanes; ++i){
    G4double theta = i * 360.0 * deg / nPlanes;
    
    G4RotationMatrix rm = G4RotationMatrix();
    rm.rotateY(90*deg);
    rm.rotateZ(-theta);
    G4ThreeVector planePos(RINGRADIUS*std::sin(theta), RINGRADIUS*std::cos(theta), 0);
    new G4PVPlacement(G4Transform3D(rm, planePos),
		      logicPlane,
		      "tPlane",
		      logicRingRegion,
		      false,
		      i,
		      checkOverlaps);
  }

  //make field
  RingField* theField = new RingField();
  G4EqMagElectricField* eqn = new G4EqMagElectricField(theField);
  G4int nvar = 8;
  G4MagIntegratorStepper* stepper = new G4ClassicalRK4(eqn, nvar);
  //G4FieldManager* manager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4FieldManager* manager = new G4FieldManager();
  manager->SetDetectorField(theField);
  G4double minStep = 0.01*mm;
  G4MagInt_Driver* driver = new G4MagInt_Driver(minStep, 
						stepper, 
						stepper->GetNumberOfVariables());
  G4ChordFinder* cFinder = new G4ChordFinder(driver);
  manager->SetChordFinder(cFinder);

  manager->SetMinimumEpsilonStep(1e-7);
  manager->SetMaximumEpsilonStep(1e-6);
  manager->SetDeltaOneStep(0.01*mm);
  manager->SetDeltaIntersection(0.01*mm);
  logicRingRegion->SetFieldManager(manager, true);

  //
  //always return the physical World
  //
  return physWorld;
}

RingField::~RingField(){
}

RingField::RingField() 
  :
  G4ElectroMagneticField(),
  r0(7.112*m)
{
  const double n = 0.18;
  G4double muM = G4ParticleTable::GetParticleTable()->FindParticle("mu+")->GetPDGMass();
  B0 = muM*29.3*std::sqrt(1-1/(29.3*29.3))/(r0*c_light);
  quadK = n*c_light*std::sqrt(1-1/(29.3*29.3))*B0/r0;
}

void RingField::GetFieldValue(const G4double Point[4], G4double* Bfield) const{
  Bfield[0] = 0;
  Bfield[1] = 0;
  Bfield[2] = B0;
  
  //quad fields
  G4ThreeVector rho(Point[0], Point[1], 0);
  G4double magRho = rho.mag();
  G4ThreeVector rhoHat = rho/magRho;
  G4double x = magRho - r0;
  G4ThreeVector xField = quadK * x * rhoHat;
  Bfield[3] = xField.x();
  Bfield[4] = xField.y();
  Bfield[5] = -1*quadK * Point[2];
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

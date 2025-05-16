#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVParameterised.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"

#include "detector.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}

	virtual G4VPhysicalVolume *Construct();
private:


	G4Box *solidWorld, *solidRadiator, *solidDetector;
	// add a cylinder for the scinntillator
	G4Tubs *solidScintillator;
	//G4LogicalVolume *logicWorld, *logicRadiator,*logicDetector, *logicScintillator;
	G4LogicalVolume *logicWorld, *logicRadiator, *logicScintillator;
	G4VPhysicalVolume *physWorld, *physRadiator, *physDetector, *physScintillator;

	G4Material *SiO2, *H2O, *Aerogel, *worldMat, *NaI;
	G4Element *C, *Na, *I;

	void DefineMaterials();

	// Whenever and whatever you construct put them here below.
	void ConstructCherenkov();
	void ConstructScintillator();

	virtual void ConstructSDandField();

	G4GenericMessenger *fMessenger;

	G4LogicalVolume *logicDetector;
	G4LogicalVolume *fScoringVolume; // logical volume for scoring

	G4int nCols, nRows; // number of columns and rows of the photosensors

	G4double xWorld, yWorld, zWorld; // dimensions of the world volume

	G4bool isCherenkov, isScintillator; // flags to check if the user wants to use cherenkov or scintillator

};

#endif

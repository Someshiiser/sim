#ifndef DETECTOR_HH
#define DETECTOR_HH
#include "G4VSensitiveDetector.hh" 
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
    public:
        MySensitiveDetector(G4String);
        ~MySensitiveDetector();

        private:
            //to register the hits
            virtual G4bool ProcessHits(G4Step *, G4TouchableHistory*);

        G4PhysicsOrderedFreeVector *quEff;
    
};
#endif
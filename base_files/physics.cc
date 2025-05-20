//it will tell the detector the interacting physics.

#include "physics.hh"

//create the constructor
MyPhysicsList::MyPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics()); //electromagnetc physics
    RegisterPhysics (new G4OpticalPhysics()); //optical physics

    // for the radioacytive decay
    RegisterPhysics (new G4DecayPhysics());
    RegisterPhysics (new G4RadioactiveDecayPhysics());
    
    // Register other physics processes here if needed
}

MyPhysicsList::~MyPhysicsList()
{}

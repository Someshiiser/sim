//it will tell teh detector the interacting physics.

#include "physics.hh"

//create the constructor
MyPhysicsList::MyPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics()); //electromagnetc physics
    RegisterPhysics (new G4OpticalPhysics()); //optical physics

}
MyPhysicsList::~MyPhysicsList()
{}

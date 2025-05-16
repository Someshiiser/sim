#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"  // for the units like GeV
#include "G4ParticleTable.hh"  // 

//for radioactive decay
#include "G4IonTable.hh"      // for the ion table
#include "G4Geantino.hh"      // for the geantino

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
    public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*); //it is one clas to be put as argument in it

    private:
    G4ParticleGun *fParticleGun; // particle gun by the fparticle gun

};

#endif
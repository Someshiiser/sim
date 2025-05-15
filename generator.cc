#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); //(number of particles per event=1=one primary vertex per particle )
    
    // was earlier at below now at top to be able to be overwritten later by macro.
    G4ParticleTable *particleTable = G4ParticleTable :: GetParticleTable();
    G4String particleName = "proton";
    G4ParticleDefinition *particle =particleTable -> FindParticle("proton");

    G4ThreeVector pos(0.,0.,0.); //create particle at the origin
    G4ThreeVector mom(0.,0.,1.); //momentum along the z direction

    fParticleGun -> SetParticlePosition(pos);
    fParticleGun -> SetParticleMomentumDirection(mom);
    fParticleGun -> SetParticleMomentum(100.*GeV); //magnitude of the momentum high energy = 100GeV
    fParticleGun -> SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) //to abtain inforamtion or use as input parameter for primary vertex
{
    // below can be done but disadvantage is that they are generated at start of the run, and whenever we would run the macro it will overwrite the previous one. 
    // So we will put them in the constructor, so they only will be read at the start of the run. And later overwriiten by the macro file.
    //G4ParticleTable *particleTable = G4ParticleTable :: GetParticleTable();
    //G4String particleName = "proton";
    //G4ParticleDefinition *particle =particleTable -> FindParticle("proton");

    //G4ThreeVector pos(0.,0.,0.); //create particle at the origin
    //G4ThreeVector mom(0.,0.,1.); //momentum along the z direction

    //fParticleGun -> SetParticlePosition(pos);
    //fParticleGun -> SetParticleMomentumDirection(mom);
    //fParticleGun -> SetParticleMomentum(100.*GeV); //magnitude of the momentum high energy = 100GeV
    //fParticleGun -> SetParticleDefinition(particle);

    fParticleGun -> GeneratePrimaryVertex(anEvent);
}
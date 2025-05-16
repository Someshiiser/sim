#include "construction.hh"
 
 
MyDetectorConstruction:: MyDetectorConstruction()  
{
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector construction");
    fMessenger->DeclareProperty("nCols", nCols, "Number of columns"); //nCols is the number of photosensors in the coulmn.
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows"); //nRows is the number of photosensors in the row.

    nCols = 100; // default value
    nRows = 100; // default value
    // we can also set the default value in the constructor.
    DefineMaterials(); 
    
} 
MyDetectorConstruction::~MyDetectorConstruction() 
{} 

void MyDetectorConstruction::DefineMaterials()
{
    // This method is used to define the materials used in the simulation.
    // It is not mandatory to use this method, but it is a good practice to
    // define the materials in a separate method for better organization.
    // The materials are defined using the G4NistManager class, which is a
    // singleton class that provides access to the NIST material database.
    // The materials are defined using the G4Material class, which is a
    // class that represents a material in Geant4.
    G4NistManager *nist = G4NistManager:: Instance(); 
   
    SiO2 =new G4Material("SiO2", 2.201*g/cm3, 2); // (name, density, number of componenets)
    SiO2 -> AddElement(nist-> FindOrBuildElement("Si"),1);
    SiO2 -> AddElement(nist-> FindOrBuildElement("O"),2);

    H2O =new G4Material("H2O", 1.*g/cm3, 2); // (name, density, number of componenets)
    H2O -> AddElement(nist-> FindOrBuildElement("H"),2);
    H2O -> AddElement(nist-> FindOrBuildElement("O"),1);

    C = nist-> FindOrBuildElement("C"); // add carbon

    Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel -> AddMaterial(SiO2, 62.5*perCent);
    Aerogel -> AddMaterial(H2O, 37.4*perCent);
    Aerogel -> AddElement(C, 0.1*perCent);

    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9}; // blue and red light nanometer.
    G4double rindexAerogel[2] = {1.1, 1.1};
    G4double rindexWorld[2] = {1.0, 1.0};

    G4MaterialPropertiesTable *mptAerogel= new G4MaterialPropertiesTable();
    mptAerogel -> AddProperty("RINDEX", energy, rindexAerogel, 2); //RINDEX is important, momentum, refractive index, two copies

    G4MaterialPropertiesTable *mptWorld= new G4MaterialPropertiesTable();
    mptWorld -> AddProperty("RINDEX", energy, rindexWorld, 2);

    Aerogel -> SetMaterialPropertiesTable(mptAerogel);
    worldMat -> SetMaterialPropertiesTable(mptWorld);

}
 
G4VPhysicalVolume *MyDetectorConstruction::Construct() 
{ 
 
//G4NistManager *nist = G4NistManager:: Instance(); 
// we are using the NIST manager to get the materials.
//making aero gel.
//G4Material *SiO2 =new G4Material("SiO2", 2.201*g/cm3, 2); // (name, density, number of componenets)
//SiO2 -> AddElement(nist-> FindOrBuildElement("Si"),1);
//SiO2 -> AddElement(nist-> FindOrBuildElement("O"),2);

//G4Material *H2O =new G4Material("H2O", 1.*g/cm3, 2); // (name, density, number of componenets)
//H2O -> AddElement(nist-> FindOrBuildElement("H"),2);
//H2O -> AddElement(nist-> FindOrBuildElement("O"),1);

//G4Element *C = nist-> FindOrBuildElement("C"); // add carbon

//G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
//Aerogel -> AddMaterial(SiO2, 62.5*perCent);
//Aerogel -> AddMaterial(H2O, 37.4*perCent);
//Aerogel -> AddElement(C, 0.1*perCent);

//G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9}; // blue and red light nanometer.
//G4double rindexAerogel[2] = {1.1, 1.1};
//G4double rindexWorld[2] = {1.0, 1.0};
// add this refractive index to the material
//G4MaterialPropertiesTable *mptAerogel= new G4MaterialPropertiesTable();
//mptAerogel -> AddProperty("RINDEX", energy, rindexAerogel, 2); //RINDEX is important, momentum, refractive index, two copies
//Aerogel -> SetMaterialPropertiesTable(mptAerogel); // add the property to the Areogel
// now we got the cherenkov light nut only till the material of aerogel it is proppogating. So now we want it to come out.
// to do so define refractive index for the world material
//G4Material *worldMat =nist->FindOrBuildMaterial("G4_AIR"); 
//G4MaterialPropertiesTable *mptWorld= new G4MaterialPropertiesTable();
//mptWorld -> AddProperty("RINDEX", energy, rindexWorld, 2);
//worldMat -> SetMaterialPropertiesTable(mptWorld);

G4double xWorld = 0.5*m;
G4double yWorld = 0.5*m;
G4double zWorld = 0.5*m;
// we have to define the world volume, which is the volume in which we will put all the other volumes.


solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); // (name, x, y, z)
  
logicWorld = new G4LogicalVolume (solidWorld, worldMat, "logicWorld"); 
 
physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //(rotation=0 ,origin placing, logical volume to work in, name, ,booelan=no=false,,for checking overlap)  

solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");

fScoringVolume = logicRadiator; // we have to define the scoring volume, which is the volume in which we will put all the other volumes.

physRadiator= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
//adding the photosensors, that is detectors.
    solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.001*m);
    //to create logical volume for the detector, we have to considere the fact the sensitive volume that we will defint later will have to refer to this volume, so we have to define it in class definition directly.
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, " logicDetector");
    
    //To hide the detectors, they are invisible. Thats it.
    G4VisAttributes* invisible = new G4VisAttributes();
    invisible->SetVisibility(false);
    logicDetector->SetVisAttributes(invisible);
    
    // now we have create physical instanses for the detector, for which we will need to array of sensitive detectors. to do create two for loops.
    for(G4int i=0; i< nRows;i++)
    {
        for(G4int j=0; j< nCols;j++)
        {
            physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m +(i+0.5)*m/nRows, -0.5*m +(j+0.5)*m/nCols, 0.49*m), logicDetector, "physDetector", logicWorld, false, j+i*nCols, true);
        }
    }
    // to detect them we create a new class by making two new files. detector.cc/.hh
return physWorld;
} 

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("Sensitive Detector");
    logicDetector -> SetSensitiveDetector(sensDet);
}
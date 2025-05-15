#include "construction.hh"
 
 
MyDetectorConstruction:: MyDetectorConstruction()  
{} 
MyDetectorConstruction::~MyDetectorConstruction() 
{} 
 
G4VPhysicalVolume *MyDetectorConstruction::Construct() 
{ 
 
G4NistManager *nist = G4NistManager:: Instance(); 



//making aero gel.
G4Material *SiO2 =new G4Material("SiO2", 2.201*g/cm3, 2); // (name, density, number of componenets)
SiO2 -> AddElement(nist-> FindOrBuildElement("Si"),1);
SiO2 -> AddElement(nist-> FindOrBuildElement("O"),2);

G4Material *H2O =new G4Material("H2O", 1.*g/cm3, 2); // (name, density, number of componenets)
H2O -> AddElement(nist-> FindOrBuildElement("H"),2);
H2O -> AddElement(nist-> FindOrBuildElement("O"),1);

G4Element *C = nist-> FindOrBuildElement("C"); // add carbon

G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
Aerogel -> AddMaterial(SiO2, 62.5*perCent);
Aerogel -> AddMaterial(H2O, 37.4*perCent);
Aerogel -> AddElement(C, 0.1*perCent);

G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9}; // blue and red light nanometer.
G4double rindexAerogel[2] = {1.1, 1.1};
G4double rindexWorld[2] = {1.0, 1.0};
// add this refractive index to the material
G4MaterialPropertiesTable *mptAerogel= new G4MaterialPropertiesTable();
mptAerogel -> AddProperty("RINDEX", energy, rindexAerogel, 2); //RINDEX is important, momentum, refractive index, two copies
Aerogel -> SetMaterialPropertiesTable(mptAerogel); // add the property to the Areogel
// now we got the cherenkov light nut only till the material of aerogel it is proppogating. So now we want it to come out.
// to do so define refractive index for the world material
G4Material *worldMat =nist->FindOrBuildMaterial("G4_AIR"); 
G4MaterialPropertiesTable *mptWorld= new G4MaterialPropertiesTable();
mptWorld -> AddProperty("RINDEX", energy, rindexWorld, 2);
worldMat -> SetMaterialPropertiesTable(mptWorld);



G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m); 
  
G4LogicalVolume *logicWorld = new G4LogicalVolume (solidWorld, worldMat, "logicWorld"); 
 
G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //(rotation=0 ,origin placing, logical volume to work in, name, ,booelan=no=false,,for checking overlap)  

G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
G4VPhysicalVolume *physRadiator= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
//adding the photosensors, that is detectors.
    G4Box *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.001*m);
    //to create logical volume for the detector, we have to considere the fact the sensitive volume that we will defint later will have to refer to this volume, so we have to define it in class definition directly.
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, " logicDetector");
    
    //To hide the detectors, they are invisible. Thats it.
    G4VisAttributes* invisible = new G4VisAttributes();
    invisible->SetVisibility(false);
    logicDetector->SetVisAttributes(invisible);
    
    // now we have create physical instanses for the detector, for which we will need to array of sensitive detectors. to do create two for loops.
    for(G4int i=0; i<100;i++)
    {
        for(G4int j=0; j<100;j++)
        {
            G4VPhysicalVolume *physDetecor = new G4PVPlacement(0, G4ThreeVector(-0.5*m +(i+0.5)*m/100, -0.5*m +(j+0.5)*m/100, 0.49*m), logicDetector, "physDetector", logicWorld, false, j+i*100, true);
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
#include "construction.hh"
 
 
MyDetectorConstruction:: MyDetectorConstruction()  
{
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector construction");
    fMessenger->DeclareProperty("nCols", nCols, "Number of columns"); //nCols is the number of photosensors in the coulmn.
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows"); //nRows is the number of photosensors in the row.
    fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");
    fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator setup");
    fMessenger->DeclareProperty("tof", isTOF, "Construct Time Of flight");
    fMessenger->DeclareProperty("atmosphere", isAtmosphere, "Construct Atmosphere");

    nCols = 10; // default value
    nRows = 10; // default value
    // we can also set the default value in the constructor.
    DefineMaterials(); 

    //xWorld = 5*m; // good for the isTOF.
    //yWorld = 5*m;
    //zWorld = 5*m;
    xWorld = 40*km; // good for the isAtmosphere.
    yWorld = 40*km;
    zWorld = 20*km;

    isCherenkov = false; // default value
    isScintillator = false; // default value
    isTOF = false; // default value
    isAtmosphere = true; // default value

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
    G4double rindexNaI[2] = {1.78, 1.78}; // for the soduim iodide scintillator.
    G4double reflectivity[2] = {1.0, 1.0}; // as we need all photons to reflect back.

    G4MaterialPropertiesTable *mptAerogel= new G4MaterialPropertiesTable();
    mptAerogel -> AddProperty("RINDEX", energy, rindexAerogel, 2); //RINDEX is important, momentum, refractive index, two copies

    G4MaterialPropertiesTable *mptWorld= new G4MaterialPropertiesTable();
    mptWorld -> AddProperty("RINDEX", energy, rindexWorld, 2);

    Aerogel -> SetMaterialPropertiesTable(mptAerogel);
    worldMat -> SetMaterialPropertiesTable(mptWorld);

    Na = nist -> FindOrBuildElement("Na"),
    I = nist -> FindOrBuildElement("I");
    NaI = new G4Material("NaI", 3.67*g/cm3, 2);
    NaI -> AddElement(Na, 1);
    NaI -> AddElement(I, 1);

    G4double fraction[2] = {1.0, 1.0}; // all wavelenths with the same intensity.

    G4MaterialPropertiesTable *mptNaI= new G4MaterialPropertiesTable();
    mptNaI -> AddProperty("RINDEX", energy, rindexNaI, 2); //RINDEX is important, momentum, refractive index, two copies
    mptNaI -> AddProperty("FASTCOMPONENT", energy, fraction, 2); // fast component is the scintillation light, which is emitted when the scintillator is excited by a charged particle.
    mptNaI -> AddConstProperty("SCINTILLATIONYIELD", 38./keV); // scintillation yield is the number of photons emitted per MeV of energy deposited in the scintillator.
    mptNaI -> AddConstProperty("FASTTIMECONSTANT", 250*ns); // fast time constant is the time it takes for the scintillation light to reach its maximum intensity.
    mptNaI -> AddConstProperty("YIELDRATIO", 1.0); // yield ratio is the ratio of the number of photons emitted in the fast component to the number of photons emitted in the slow component.
    mptNaI -> AddConstProperty("RESOLUTIONSCALE", 1.0); // resolution scale is the ratio of the energy deposited in the scintillator to the energy deposited in the photodetector.

    NaI -> SetMaterialPropertiesTable(mptNaI);

    // for the mirror surface,..
    mirrorSurface = new G4OpticalSurface("mirrorSurface");
    
    mirrorSurface -> SetType(dielectric_metal); // this is the type of the surface.
    mirrorSurface -> SetFinish(ground); // this is the finish of the surface.
    mirrorSurface -> SetModel(unified); // this is the model of the surface.

    // for this we create another property table, as wee need to create a reflective coating..
    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();

    mptMirror -> AddProperty("REFLECTIVITY", energy, reflectivity, 2); // reflectivity is the ratio of the number of photons reflected by the surface to the number of photons incident on the surface.

    mirrorSurface -> SetMaterialPropertiesTable(mptMirror); // we need to apply this to our scintillator, we go to our logicScintillator.

    // for the atmosphere here.
    G4double density0 = 1.29*kg/m3;
    G4double aN = 14.01*g/mole; //nitrogen
    G4double aO = 16.00*g/mole; //oxygen

    N = new G4Element("Nitrogen", "N", 7, aN); // proton = 7
    O = new G4Element("Oxygen", "O", 8, aO); // proton = 8

    G4double f = 3; // dof for the gas
    G4double R = 8.3144626181532; // universal gas constant
    G4double g0 = 9.81; // gravity
    G4double kappa = (f+2)/f; // adiabatic coefficient
    G4double T = 293.15; // temperature in K of the room
    G4double M = (0.3*aO + 0.7*aN)/1000; // average molar mass

    for(G4int i=0; i<10; i++)
    {
        std::stringstream stri;
        stri << i;
        G4double h = 40e3/10; // height of the atmosphere in m
        G4double density = density0*pow((1-(kappa)/kappa*M*g0*h/(R*T)), (1/(kappa-1))); // density of the atmosphere at height h
        Air[i] = new G4Material("G4_Air"+stri.str(), density, 2); // (name, density, number of componenets)
        Air[i] -> AddElement(N, 70*perCent); // 70% nitrogen
        Air[i] -> AddElement(O, 30*perCent); // 30% oxygen

    }


}
 
void MyDetectorConstruction::ConstructCherenkov() 
{
    solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
    logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");

    //fScoringVolume = logicRadiator; // we have to define the scoring volume, which is the volume in which we will put all the other volumes.

    physRadiator= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
    //adding the photosensors, that is detectors.
    solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.001*m);
    //to create logical volume for the detector, we have to considere the fact the sensitive volume that we will defint later will have to refer to this volume, so we have to define it in class definition directly.
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
    
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
}

void MyDetectorConstruction::ConstructScintillator() 
{
    //solidScintillator = new G4Tubs("solidScintillator", 10*cm, 20*cm, 30*cm, 0*deg, 360*deg);
    solidScintillator = new G4Box("solidScintillator", 5*cm, 5*cm, 6*cm); // spatial resolution depends upon the size of the scintillaors.
    logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicScintillator");
    
    G4LogicalSkinSurface *skinSurface = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface); // one can also use the logicScintillator instead the logicWorld.
    // we want to attach photodetector on top of scintillator, so we do as..
    solidDetector = new G4Box("solidDetector", 1*cm, 5*cm, 6*cm);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
    
    fScoringVolume = logicScintillator; // we have to define the scoring volume, which is the volume in which we will put all the other volumes.
    
    // for the array of the scintillators we cerate two for loops and fill full world volume.
    for (G4int i=0; i<6; i++)
    {   
        for (G4int j=0; j<16 ;j++)
        {

            // to bring in the right transformation, we create rotation in combination with the translation.
            G4Rotate3D rotZ(j*22.5*deg, G4ThreeVector(0., 0., 1. ));
            G4Translate3D transXScint(G4ThreeVector(5./tan(22.5/2*deg)*cm + 5.*cm, 0*cm, -40*cm + i*15*cm));

            G4Translate3D transXDet(G4ThreeVector(5./tan(22.5/2*deg)*cm + 6.*cm +5*cm, 0*cm, -40*cm + i*15*cm));

            G4Transform3D transformScint = (rotZ) * (transXScint); // this is the transformation, product of rotationa nd tanslation. first traanslation then rotation.
            G4Transform3D transformDet = (rotZ) * (transXDet);
            
            //physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintillator, "physScintialltor", logicWorld, false, 0, true); // it creates one scintillator.
            physScintillator = new G4PVPlacement(transformScint, logicScintillator, "physScintialltor", logicWorld, false, 0, true);
            physDetector = new G4PVPlacement(transformDet, logicDetector, "physDetector", logicWorld, false, 0, true);
        
        }
    } // this creates ring of 16 scintillators and then 16*6=96 scintillators in total.
    
}

void MyDetectorConstruction::ConstructTOF()
{
    solidDetector = new G4Box("solidDetector", 1*m, 1*m, 0.1*m);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    physDetector = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, -4*m), logicDetector, "physDetector", logicWorld, false, 0, true);
    physDetector = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 3*m), logicDetector, "physDetector", logicWorld, false, 1, true);

}

void MyDetectorConstruction::ConstructAtmosphere()
{
    solidAtmosphere = new G4Box("solidAtmosphere", xWorld, yWorld, zWorld/10); // (name, x, y, z)

    for(G4int i = 0; i < 10; i++)
    {
        logicAtmosphere[i] = new G4LogicalVolume(solidAtmosphere, Air[i], "logicAtmosphere"); // we need to create 10 layers of atmosphere.
        physAtmosphere[i] = new G4PVPlacement(0, G4ThreeVector(0., 0., zWorld/10*2*i - zWorld + zWorld/10.), logicAtmosphere[i], "physAtmosphere", logicWorld, false, i, true);

    }

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

// gone to the constructor.
//G4double xWorld = 0.5*m;
//G4double yWorld = 0.5*m;
//G4double zWorld = 0.5*m;
// we have to define the world volume, which is the volume in which we will put all the other volumes.


solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); // (name, x, y, z)
  
logicWorld = new G4LogicalVolume (solidWorld, worldMat, "logicWorld"); 
 
physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //(rotation=0 ,origin placing, logical volume to work in, name, ,booelan=no=false,,for checking overlap)  

if(isCherenkov) 
    ConstructCherenkov(); // if the user wants to use cherenkov, we call the function to construct it.
if(isScintillator)
    ConstructScintillator();
if(isTOF)
    ConstructTOF(); // if the user wants to use time of flight, we call the function to construct it.
if(isAtmosphere)
    ConstructAtmosphere(); // if the user wants to use atmosphere, we call the function to construct it.

// all below is gone to the cherenkov.
//solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
//logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
//
//fScoringVolume = logicRadiator; // we have to define the scoring volume, which is the volume in which we will put all the other volumes.
//
//physRadiator= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);
////adding the photosensors, that is detectors.
//    solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.001*m);
//    //to create logical volume for the detector, we have to considere the fact the sensitive volume that we will defint later will have to refer to this volume, so we have to define it in class definition directly.
//    logicDetector = new G4LogicalVolume(solidDetector, worldMat, " logicDetector");
//    
//    //To hide the detectors, they are invisible. Thats it.
//    G4VisAttributes* invisible = new G4VisAttributes();
//    invisible->SetVisibility(false);
//    logicDetector->SetVisAttributes(invisible);
//    
//    // now we have create physical instanses for the detector, for which we will need to array of sensitive detectors. to do create two for loops.
//    for(G4int i=0; i< nRows;i++)
//    {
//        for(G4int j=0; j< nCols;j++)
//        {
//            physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m +(i+0.5)*m/nRows, -0.5*m +(j+0.5)*m/nCols, 0.49*m), logicDetector, "physDetector", logicWorld, false, j+i*nCols, true);
//        }
//    }
//    // to detect them we create a new class by making two new files. detector.cc/.hh

return physWorld;
} 

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("Sensitive Detector");
    //logicDetector -> SetSensitiveDetector(sensDet);

    //if(isCherenkov)

    // below one cause problems, so we have to comment it for the isAtmosphere.
    //if(logicDetector != NULL)
        //logicDetector -> SetSensitiveDetector(sensDet);

    
}
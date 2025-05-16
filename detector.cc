#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name ):
G4VSensitiveDetector{name}
{
    quEff = new G4PhysicsOrderedFreeVector();

    // we have read our data file and put it in here
    std::ifstream datafile;
    datafile.open("eff.dat");

    while(1)
    {
        G4double wlen, queff;
        datafile >> wlen >> queff;
        if(datafile.eof()) break;
        G4cout << wlen << " " << queff << G4endl;
        quEff->InsertValues(wlen, queff/100); // we need to divide by 100 to get the efficiency in percentage
    }
    datafile.close();
    quEff->SetSpline(false); // we do need interpolation in bettween, so we aare using linear interpolation here. So use small distances in order to do so.
}

MySensitiveDetector:: ~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
    // to register the information the photon position
    //frist is the track
    G4Track *track = aStep->GetTrack();

    // to avoid the different values, like entering from the side.
    track -> SetTrackStatus(fStopAndKill); //it kills the photon when detected, no further propogation
    


    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    //in real experimnets we do not have the real position of the photons, we only have the positions of the detectors and from them we reconstruct the position.
    //G4cout << "Photon position" << posPhoton <<G4endl; // this will now give the positions at the detector/

    // We need the wavelength of the photon from the momentum of the photon.    
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();
    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03; //in nm , it is absolute value of the momentum.


    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();
    //G4cout <<"Copy Number:" <<copyNo <<G4endl; //it give the copy number
    //but we dont need the copy we need its position.
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    
    // below is done in order to hide the output in the terminal. And 
    
    #ifndef G4MULTITHREADED
        // commenting it out to make it faster.
        //G4cout << "Detector Position :" <<posDetector<<G4endl;
        //G4cout << "Photonwavelength:" <<wlen<<G4endl;
    #endif
    //Now we have the information we needed. To put it into the rootfile we proceed as.
    
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man -> FillNtupleIColumn(0, 0, evt);
    man -> FillNtupleDColumn(0, 1, posPhoton[0]); 
    man -> FillNtupleDColumn(0, 2, posPhoton[1]);
    man -> FillNtupleDColumn(0, 3, posPhoton[2]);
    man -> FillNtupleDColumn(0, 4, wlen);
    man -> AddNtupleRow(0);
    
    if(G4UniformRand() < quEff->Value(wlen))
    {
    man -> FillNtupleIColumn(1, 0, evt);
    man -> FillNtupleDColumn(1, 1, posDetector[0]); 
    man -> FillNtupleDColumn(1, 2, posDetector[1]);
    man -> FillNtupleDColumn(1, 3, posDetector[2]);
    man -> AddNtupleRow(1);
    }
    //it will add the row to the ntuple. The first column is the event number, the second is the x position, the third is the y position and the fourth is the z position.
    //G4cout << "Event Number:" << evt << G4endl;
    //G4cout << "X Position:" << posDetector[0] << G4endl;
    //G4cout << "Y Position:" << posDetector[1] << G4endl;
    //G4cout << "Z Position:" << posDetector[2] << G4endl;
    //G4cout << "Photon position" << posPhoton <<G4endl; // this will now give the positions at the detector.
    //G4cout << "Photon position" << posPhoton[0] <<G4endl; // this will now give the positions at the detector.
    //G4cout << "Photon position" << posPhoton[1] <<G4endl; // this will now give the positions at the detector.
    //G4cout << "Photon position" << posPhoton[2] <<G4endl; // this will now give the positions at the detector.    

    
    return true;
}


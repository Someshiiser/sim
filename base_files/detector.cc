#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name ):
G4VSensitiveDetector{name}
{}

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

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();
    //G4cout <<"Copy Number:" <<copyNo <<G4endl; //it give the copy number
    //but we dont need the copy we need its position.
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    G4cout << "Detector Position :" <<posDetector<<G4endl;
    
    //Now we have the information we needed. To put it into the rootfile we proceed as.
    
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man -> FillNtupleIColumn(0, evt);
    man -> FillNtupleDColumn(1, posDetector[0]); 
    man -> FillNtupleDColumn(2, posDetector[1]);
    man -> FillNtupleDColumn(3, posDetector[2]);
    man -> AddNtupleRow(0);
    //it will add the row to the ntuple. The first column is the event number, the second is the x position, the third is the y position and the fourth is the z position.
    //G4cout << "Event Number:" << evt << G4endl;
    //G4cout << "X Position:" << posDetector[0] << G4endl;
    //G4cout << "Y Position:" << posDetector[1] << G4endl;
    //G4cout << "Z Position:" << posDetector[2] << G4endl;
    //G4cout << "Photon position" << posPhoton <<G4endl; // this will now give the positions at the detector.
    //G4cout << "Photon position" << posPhoton[0] <<G4endl; // this will now give the positions at the detector.
    //G4cout << "Photon position" << posPhoton[1] <<G4endl; // this will now give the positions at the detector.
    //G4cout << "Photon position" << posPhoton[2] <<G4endl; // this will now give the positions at the detector.    
}


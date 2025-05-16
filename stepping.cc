#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
{
    fEventAction = eventAction;
}
MySteppingAction::~MySteppingAction()
{}
void MySteppingAction::UserSteppingAction(const G4Step* step)
{
    G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    const MyDetectorConstruction* detectorConstruction =
        static_cast<const MyDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume* fScoringVolume = detectorConstruction->GetScoringVolume();

    if (volume != fScoringVolume) return;
    // If we are here, it means that the step is in the scoring volume.
    // we have to get the energy deposited in the step.
    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edep);
}
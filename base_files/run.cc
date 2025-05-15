#include "run.hh"

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man ->OpenFile("output.root");
    // create tuple inside the output file.

    man ->CreateNtuple("Hits", "Hits");
    man ->CreateNtupleIColumn("fEvent");
    man ->CreateNtupleDColumn("fX");
    man ->CreateNtupleDColumn("fY");
    man ->CreateNtupleDColumn("fZ");
    man ->FinishNtuple(0);
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man ->Write();
    man ->CloseFile();
}


// If we need the output file to be created in the begining of the run then put it in the constructor.
// If we need the output file to be created at the end of the run then put it in the destructor.
// If we need the output file to be created at the end of the event then put it in the end of the event action.
// if we want output being new for each run then put it the begining of the action.
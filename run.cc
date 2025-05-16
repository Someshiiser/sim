#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    //this is the monte carlo data not the real one
    man ->CreateNtuple("Photons", "Photons");
    man ->CreateNtupleIColumn("fEvent");
    man ->CreateNtupleDColumn("fX");
    man ->CreateNtupleDColumn("fY");
    man ->CreateNtupleDColumn("fZ");
    man ->CreateNtupleDColumn("fWlen"); //wavelength
    man ->FinishNtuple(0);
   
    // this is the real data not the monte carlo one.
    man ->CreateNtuple("Hits", "Hits");
    man ->CreateNtupleIColumn("fEvent");
    man ->CreateNtupleDColumn("fX");
    man ->CreateNtupleDColumn("fY");
    man ->CreateNtupleDColumn("fZ");
    man ->FinishNtuple(1);

    man ->CreateNtuple("Scoring", "Scoring");
    man ->CreateNtupleDColumn("fEdep"); // energy deposited.
    man ->FinishNtuple(2);


}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    // right now the below one creates only one file and deletes it when we run again.
    // so we want to create a new file for each run. To do so we will insert the run number in the file name.
    G4int runID = run ->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    man ->OpenFile("output"+strRunID.str()+".root");
    // create tuple inside the output file.
    
    // we also dont want to creat the Ntuples each time at the start of the each run. So wee put below in the constructor. 
    //man ->CreateNtuple("Hits", "Hits");
    //man ->CreateNtupleIColumn("fEvent");
    //man ->CreateNtupleDColumn("fX");
    //man ->CreateNtupleDColumn("fY");
    //man ->CreateNtupleDColumn("fZ");
    //man ->FinishNtuple(0);
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
#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "g4root.hh"
#include "run.hh"

class MyEventAction : public G4UserEventAction
{
    public:
        MyEventAction(MyRunAction* );
        ~MyEventAction();

        // we have to create the event action and then we have to create the run action.
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
    
        void AddEdep(G4double edep)
        {
            fEdep += edep;
        }

    private:
        G4double fEdep;


};

#endif
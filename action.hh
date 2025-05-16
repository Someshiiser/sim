#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"
#include "run.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
    public:
    MyActionInitialization();
    ~MyActionInitialization();

    virtual void Build() const;  //runs our particle gun and computes the stepping and so on.
    virtual void BuildForMaster() const;

};

#endif
#include "action.hh"

MyActionInitialization :: MyActionInitialization()
{}

MyActionInitialization :: ~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);
}
// we now have to create a particle gun, so we create two new files generator.cc , .hh

#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
	G4RunManager *runManager = new G4RunManager();
	runManager->SetUserInitialization(new MyDetectorConstruction()); // intialise construction
	runManager->SetUserInitialization(new MyPhysicsList()); //intialise physics list
	runManager->SetUserInitialization(new MyActionInitialization());
	runManager->Initialize();

	// so what we have done now is the following, first we dont want lot of memory to go in graphics, so we have to set the graphics to be in a different thread. So we have to create a UI executive.
	//It will only be created if we have no arguments. If we have arguments then it will not be created. By arguments we mean to say number of command line arguments, one is sim.  
	G4UIExecutive *ui = 0;
	if(argc == 1){
		ui = new G4UIExecutive (argc, argv);
	}
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();

	G4UImanager *UImanager = G4UImanager ::GetUIpointer();
	// below is for the case when we have no arguments. So we will run the macro file.
	if (ui){
	// all the below coomands are for visualisation and now they will go into the vis.macro file.
	// we want it to load the vis.macro file and run it.
	UImanager->ApplyCommand("/control/execute vis.mac");
	// write now problem is that the vis.mac file is not being executed. As it is not the build directory.
	// So to do we will edit our CMakeLists.txt file and modify it so as it detects all macros made by us.

	//UImanager->ApplyCommand("/vis/open OGL");
	//UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
	//UImanager->ApplyCommand("/vis/drawVolume");
	//UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
	//UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
	//UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate"); // it accumulates all the events at last.

	ui->SessionStart();
	}
	// below is for running the macro file.
	else
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	return 0;
}


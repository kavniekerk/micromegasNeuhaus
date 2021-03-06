#include "DetectorConstruction.hpp"
#include "ActionInitialization.hpp"
#include "OutputManager.hpp"
#include "PhysicsList.hpp"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

/**
 * Main method of the particle conversion simulation
 * @param  argc  Number of given command line parameters
 * @param  argv  Values of given command line parameters
 * @return Status code
 */
int main(int argc, char** argv) {
   TString outputfileName;
   TString macroFolder;

   //[[[cog from MMconfig import *; cog.outl("macroFolder = \"{}\";".format(conf['particleconversion']['macro_folder'])) ]]]
   //[[[end]]]

   if (argc == 2) {
      G4cout << "Using command line parameters as out file: ";
      outputfileName = argv[1];
      G4cout << outputfileName << G4endl;
   } else if (argc == 3) {
      G4cout << "Using command line parameters as out file: ";
      outputfileName = argv[1];
      macroFolder = argv[2];
      G4cout << outputfileName << " and also for macro directory " << macroFolder << G4endl;
   } else {
      //[[[cog from MMconfig import *; import os; cog.outl("outputfileName = \"{}\";".format(conf["particleconversion"]["out_filename"])) ]]]
      //[[[end]]]
   }
   //TODO: Implementing return code

   // Choose the Random engine
   G4Random::setTheEngine(new CLHEP::RanecuEngine);

   // Construct the default run manager
   #ifdef G4MULTITHREADED
      G4MTRunManager* runManager = new G4MTRunManager;
   #else
      G4RunManager* runManager = new G4RunManager;
   #endif

   // Set mandatory initialization classes
   runManager->SetUserInitialization(new PhysicsList);
   DetectorConstruction* detectorConstruction = new DetectorConstruction();
   runManager->SetUserInitialization(detectorConstruction);

   // User action initialization
   runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, outputfileName));

   // Initialize visualization
   G4VisManager* visManager = new G4VisExecutive;
   // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
   // G4VisManager* visManager = new G4VisExecutive("Quiet");
   visManager->Initialize();

   // Process macro or start UI session
   /*[[[cog
   from MMconfig import *

   def ui_start():
      cog.outl("G4UIExecutive* ui = new G4UIExecutive(argc, argv);")
      cog.outl("G4UImanager* UImanager = G4UImanager::GetUIpointer();")
      cog.outl('UImanager->ApplyCommand("/control/macroPath " + macroFolder);')
      cog.outl('UImanager->ApplyCommand("/control/execute vis.mac");')
      cog.outl("ui->SessionStart();")
      cog.outl("delete ui;")

   if "macro_path" in conf["particleconversion"]:
      macro_path = conf["particleconversion"]["macro_path"]
      if macro_path != "": # run given macro file
         cog.outl("G4UImanager* UImanager = G4UImanager::GetUIpointer();")
         cog.outl('UImanager->ApplyCommand("/control/macroPath " + macroFolder);')
         cog.outl("UImanager->ApplyCommand(\"/control/execute {}\");".format(macro_path))
      else:
         ui_start()
   else:
      ui_start()
   ]]]*/
   //[[[end]]]

   delete visManager;
   delete runManager;
}

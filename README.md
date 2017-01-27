Basic plan and interface adapter for testing PLEXIL execution semantics.

Plan intended to control/imitate an imaginary "Roomba" vacuum cleaner robot.

buildTest.sh will compile both the Interface adapter ("TestInterface") and the plan ("Test.ple") to the appropriate formats.

runSim.sh will start the plexil viewer with plan loaded. Remove -v option if command line operation of plexilexec is desired instead of plexil viewer GUI. 

Tested with plexil-4.0.1

Interface adapter based off of SampleAdapter under $PLEXIL_HOME/example/sample-app1


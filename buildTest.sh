#!/bin/bash

if [ -z $PLEXIL_HOME ]; then
   echo "ERROR: $PLEXIL_HOME not defined"
fi

gcc -shared -o $PWD/libTestInterface.so -fPIC $PWD/TestInterface.cc -I "$PLEXIL_HOME/include/" -I "$PLEXIL_HOME/src/value/" -I "$PLEXIL_HOME/src/"

cp $PWD/libTestInterface.so $PLEXIL_HOME/lib/libTestInterface.so

plexilc $PWD/Test.ple

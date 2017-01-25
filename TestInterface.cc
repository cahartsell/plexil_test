
/*
 * Basic PLEXIL Interface Adapter
 * Intended for testing detailed semantics of PLEXIL interfacing
 * 
 * Charles Hartsell 1-18-2017
 */

#include "TestInterface.hh"

// PLEXIL API
#include "AdapterConfiguration.hh"
#include "AdapterFactory.hh"
#include "AdapterExecInterface.hh"
#include "Debug.hh"
#include "Expression.hh"
#include "StateCacheEntry.hh"

#include <string>

#define DRIVE_TIME 5 //seconds

using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::string;
using std::vector;
using std::copy;

static TestInterface *Adapter;

static vector<Value> const EmptyArgs;

static State createState (const string& state_name, const vector<Value>& value)
{
  State state(state_name, value.size());
  if (value.size() > 0)
  {
    for(size_t i=0; i<value.size();i++)
    {
      state.setParameter(i, value[i]);
    }
  }
  return state;
}

TestInterface::TestInterface(AdapterExecInterface& execInterface, const pugi::xml_node& configXml)
  : InterfaceAdapter(execInterface, configXml)
{
  debugMsg("TestInterface", " created.");
}

bool TestInterface::initialize()
{
  g_configuration->defaultRegisterAdapter(this);
  Adapter = this;
  Adapter->at_waypoint = true;
  Adapter->current_waypoint = 0;
  Adapter->start_time = std::time(NULL);
  debugMsg("TestInterface", " initialized.");
  return true;
}

bool TestInterface::start()
{
  debugMsg("TestInterface", " started.");
  return true;
}

bool TestInterface::stop()
{
  debugMsg("TestInterface", " stopped.");
  return true;
}

bool TestInterface::reset()
{
  debugMsg("TestInterface", " reset.");
  return true;
}

bool TestInterface::shutdown()
{
  debugMsg("TestInterface", " shutdown.");
  return true;
}

void TestInterface::invokeAbort(Command *cmd)
{
}

void TestInterface::executeCommand(Command *cmd)
{
  const string &name = cmd->getName();
  debugMsg("TestInterface:Command"," executing command: " << name);

  std::vector<Value> args;
  args = cmd->getArgValues();

  // Storage for argument variables
  string s;
  int32_t i;
  double d;
  State st;
  
  if (name == "debugMsg"){
    args[0].getValue(s);
    debugMsg("TestInterface:Command", s);
  }
  else if (name == "driveToNextWaypoint"){
    Adapter->start_time = std::time(NULL);
    at_waypoint = false;
  }
  else if (name =="update"){
    // Driving to waypoint takes fixed amount of time
    if ((std::time(NULL) - Adapter->start_time) > DRIVE_TIME){
      if(at_waypoint == false){
	debugMsg("TestInterface:Command", "Updating at_waypoint");
	at_waypoint = true;
	st = createState("at_waypoint", EmptyArgs);
	Adapter->propagateValueChange (st, vector<Value> (1, at_waypoint));
      }
    }
  }
  else{
    debugMsg("TestInterface:Command", "Unknown command: " << name);
  }

  // PLEXIL Executive does not block waiting for acknowledgement. However, sequential plan steps pend
  m_execInterface.handleCommandAck(cmd, COMMAND_SENT_TO_SYSTEM);
}

void TestInterface::lookupNow(State const &state, StateCacheEntry &entry)
{
  string const &name = state.name();
  debugMsg("TestInterface:Lookup", "Received Lookup request for " << name);
  const vector<Value> &args = state.parameters();
  Value retval;

  if (name == "at_waypoint"){
    retval = at_waypoint;
  }
  else{
    debugMsg("TestInterface:Lookup", "Unknown lookup variable: " << name);
    entry.setUnknown();
    return;
  }

  // Return result to PLEXIL
  entry.update(retval);
}

void TestInterface::subscribe(const State &state)
{
  debugMsg("TestInterface:subscribe", " subscribing to state: " << state.name());
  subscribedStates.insert(state);
}

void TestInterface::unsubscribe(const State &state)
{
  debugMsg("TestInterface:unsubscribe", " unsubscribing from state: " << state.name());
  subscribedStates.erase(state);
}

void TestInterface::propagateValueChange(const State &state, const vector<Value> &value) const
{
  m_execInterface.handleValueChange(state, value.front());
  m_execInterface.notifyOfExternalEvent();
}

// Required function
extern "C" {
  void initTestInterface() {
    REGISTER_ADAPTER(TestInterface, "TestInterface");
  }
}

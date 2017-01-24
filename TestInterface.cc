
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

TestInterface::TestInterface(AdapterExecInterface& execInterface, const pugi::xml_node& configXml)
  : InterfaceAdapter(execInterface, configXml)
{
  debugMsg("TestInterface", " created.");

}

bool TestInterface::initialize()
{
  g_configuration->defaultRegisterAdapter(this);
  Adapter = this;
  Adapter->at_waypoint = false;
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
  debugMsg("TestInterface:Command"," executing command " << name);

  std::vector<Value> args;
  args = cmd->getArgValues();

  // Storage for argument variables
  string s;
  int32_t i;
  double d;
  
  if (name == "debugMsg"){
    args[0].getValue(s);
    debugMsg("TestInterface:Command", s);
  }
  else if (name == "driveToNextWaypoint"){
    Adapter->start_time = std::time(NULL);
    debugMsg("TestInterface:Command", "Driving to waypoint");
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
  const vector<Value> &args = state.parameters();
  Value retval;

  if (name == "at_waypoint"){
    // Very basic driving model. Assumes a constant time to drive from waypoint to waypoint.
    if ((std::time(NULL) - Adapter->start_time) > DRIVE_TIME){
      retval = true;
    }
  }
  else{
    // Should return "Unknown"
    retval = false;
    debugMsg("TestInterface:Command", "Unknown lookup variable: " << name);
  }

  // Return result to PLEXIL
  entry.update(retval);
}

// Necessary boilerplate
extern "C" {
  void initTestInterface() {
    REGISTER_ADAPTER(TestInterface, "TestInterface");
  }
}

/*
class TestInterface : public InterfaceAdapter
{
public:
  TestInterface (AdapterExecInterface&, const pugi::xml_node&);

  virtual bool initialize();
  virtual bool start();
  virtual bool stop();
  virtual bool reset();
  virtual bool shutdown();
  virtual bool invokeAbort(Command *cmd);

  virtual void executeCommand(Command *cmd);
  virtual void lookupNow (State const& state, StateCacheEntry &entry);
  virtual void subscribe(const State& state);
  virtual void unsubscribe(const State& state);
  void propagateValueChange (const State&, const std::vector<Value>&) const;
};
*/

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

#include <iostream>

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
  debugMsg("TestInterface:Command"," executing command.");

  std::vector<Value> args;
  args = cmd->getArgValues();
  debugMsg("TestInterface:Command", args[0]);

  // PLEXIL Executive BLOCKS until the following command acknowledgement is sent
  m_execInterface.handleCommandAck(cmd, COMMAND_SENT_TO_SYSTEM);
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

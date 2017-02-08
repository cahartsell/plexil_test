/*
 * Basic PLEXIL Interface Adapter
 * Intended for testing detailed semantics of PLEXIL interfacing
 * 
 * Charles Hartsell 1-18-2017
 */

#pragma once

#include <iostream>
#include <ctime>
#include <pthread.h>

#include "Command.hh"
#include "InterfaceAdapter.hh"
#include "Value.hh"

using namespace PLEXIL;

class TestInterface : public InterfaceAdapter
{
public:
  TestInterface (AdapterExecInterface&, const pugi::xml_node&);
  // using compiler's default copy constructor, assignment, destructor
  
  virtual bool initialize();
  virtual bool start();
  virtual bool stop();
  virtual bool reset();
  virtual bool shutdown();
  virtual void invokeAbort(Command *cmd);

  virtual void executeCommand(Command *cmd);
  virtual void lookupNow (State const& state, StateCacheEntry &entry);
  virtual void subscribe(const State& state);
  virtual void unsubscribe(const State& state);
  void propagateValueChange (const State&, const std::vector<Value>&) const;

protected:
  int openSocket();
  static void *listen(void *context);

  pthread_t listen_thread;
  struct addrinfo *servinfo;
  bool at_waypoint, tilt_sensor, drive_stopped, bump_sensor, docking_started;
  int current_waypoint, socket_fd;
  Command *dock_cmd;
  std::time_t start_time, dock_start_time;
  std::set<State> subscribedStates;
};

extern "C" {
  void initTestInterface();
}

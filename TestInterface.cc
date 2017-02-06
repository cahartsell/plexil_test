
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
#include <cstdlib>
#include <cmath>

#include <cstring>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "18463"
#define IP_ADDR "127.0.0.1"
#define MAXDATASIZE 100

#define DRIVE_TIME 5 //seconds
#define TILT_SENSOR_PROB 0.005
#define BUMP_SENSOR_PROB 0.01
#define DOCKING_TIME 10 //seconds

using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::string;
using std::vector;
using std::copy;

static TestInterface *Adapter;

static vector<Value> const EmptyArgs;

//////////////////////////////////////////////////////////////
// Helper Functions
//////////////////////////////////////////////////////////////
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

// Networking Helper functions copied from "Beej's Networking Guide"
// get sockaddr, IPv4 or IPv6:
static void* get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static int connect_to_server()
{
  int sockfd;  
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
			 p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: bind");
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }

  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
	    s, sizeof s);
  debugMsg("TestInterface:Socket", "UDP Socket initialized with address: " << s);
  
  freeaddrinfo(servinfo); // all done with this structure
  return sockfd;
}

//////////////////////////////////////////////////////////////
// Class definitions
//////////////////////////////////////////////////////////////
TestInterface::TestInterface(AdapterExecInterface& execInterface, const pugi::xml_node& configXml)
  : InterfaceAdapter(execInterface, configXml)
{
  debugMsg("TestInterface", " created.");
}

bool TestInterface::initialize()
{
  g_configuration->defaultRegisterAdapter(this);
  Adapter = this;
  at_waypoint = true;
  tilt_sensor = false;
  drive_stopped = false;
  bump_sensor = false;
  current_waypoint = 0;
  start_time = std::time(NULL);
  dock_start_time = std::time(NULL);
  docking_started = false;
  std::srand( std::time(NULL) );
  socket_fd = connect_to_server();
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
  close( socket_fd );
  debugMsg("TestInterface", " shutdown.");
  return true;
}

void TestInterface::invokeAbort(Command *cmd)
{
  const string &name = cmd->getName();
  debugMsg("TestInterface:Command", "Aborting command: " << name);
  bool retval;

  if (name == "driveToNextWaypoint"){
    // Imaginary braking function to stop robot in place
    drive_stopped = true;
    retval = true;
  }
  else{
    debugMsg("TestInterface:Command", "Abort failed. Unknown command: " << name);
    retval = false;
  }

  cmd->acknowledgeAbort(retval);
}

void TestInterface::executeCommand(Command *cmd)
{
  const string &name = cmd->getName();
  debugMsg("TestInterface:Command","executing command: " << name);

  std::vector<Value> args;
  args = cmd->getArgValues();

  // Storage for argument variables
  string s;
  int32_t i, rand_mod;
  double d;
  State st;
  
  if (name == "debugMsg"){
    args[0].getValue(s);
    debugMsg("TestInterface:Command", s);
  }
  else if (name == "driveToNextWaypoint"){
    start_time = std::time(NULL);
    at_waypoint = false;
  }
  else if (name =="update"){
    // Driving to waypoint takes fixed amount of time
    if ((std::time(NULL) - start_time) > DRIVE_TIME){
      if(at_waypoint == false && drive_stopped == false){
	debugMsg("TestInterface:Update", "Updating at_waypoint");
	at_waypoint = true;
	current_waypoint++;
	st = createState("at_waypoint", EmptyArgs);
	Adapter->propagateValueChange(st, vector<Value> (1, at_waypoint));
	st = createState("current_waypoint", EmptyArgs);
	Adapter->propagateValueChange(st, vector<Value> (1, current_waypoint));
      }
    }

    // Very basic random probabilty of tilt_sensor or bump_sensor tripping
    rand_mod = (int)(round( 1/TILT_SENSOR_PROB ));
    i = rand() % rand_mod;
    if( i == 0 ){
      tilt_sensor = true;
      st = createState("tilt_sensor", EmptyArgs);
      Adapter->propagateValueChange(st, vector<Value> (1, tilt_sensor));
    }
    rand_mod = (int)(round( 1/BUMP_SENSOR_PROB ));
    i = rand() % rand_mod;
    if( i == 0 ){
      bump_sensor = true;
      st = createState("bump_sensor", EmptyArgs);
      Adapter->propagateValueChange(st, vector<Value> (1, bump_sensor));
    }

    // Driving to dock and docking takes fixed amount of time
    if(docking_started){
      if((std::time(NULL) - dock_start_time) > DOCKING_TIME){
	debugMsg("TestInterface:Update", "Docking complete. Sending command success message.");
	m_execInterface.handleCommandAck(dock_cmd, COMMAND_SUCCESS);
      }
    }
  }
  else if (name == "reverseAndTurn"){
    // Imaginary function to reverse robot away from obstace and turn to new path
    drive_stopped = false;
    tilt_sensor = false;
    bump_sensor = false;
    start_time = std::time(NULL);
    st = createState("tilt_sensor", EmptyArgs);
    Adapter->propagateValueChange(st, vector<Value> (1, tilt_sensor));
  }
  else if (name == "dock"){
    dock_start_time = std::time(NULL);
    docking_started = true;
    dock_cmd = cmd;
  }
  else{
    debugMsg("TestInterface:Command", "Command failed. Unknown command: " << name);
  }


  // PLEXIL pends on command acknowledgement (does not block - concurrent portions can continue)
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
  else if (name == "current_waypoint"){
    retval = current_waypoint;
  }
  else if (name == "tilt_sensor"){
    retval = tilt_sensor;
  }
  else if (name == "bump_sensor"){
    retval = bump_sensor;
  }
  else{
    debugMsg("TestInterface:Lookup", "LookupNow Failed. Unknown lookup variable: " << name);
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

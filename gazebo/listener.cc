/*
 * Copyright (C) 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>
#include <ignition/math/Pose3.hh>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "18463"
#define IP_ADDR "127.0.0.1"
#define BACKLOG 10

// Sensor ID's
#define WALL_ID 0xA0
#define LEFT_ID 0xA1
#define LEFTFRONT_ID 0xA2
#define RIGHT_ID 0xA3
#define RIGHTFRONT_ID 0xA4

// Command ID's
#define STOP_CMD 0xB0
#define FORWARD_CMD 0xB1
#define REVERSE_CMD 0xB2
#define TURN_CMD 0xB3

// Global Socket ID. Bad idea, for testing only.
int client_fd;
struct addrinfo *servinfo;

/////////////////////////////////////////////////////
// Networking helper functions
// Copied from "Beej's Networking Guide"
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int start_server()
{
  // Open Communication Socket and wait for connection
  int sockfd;  // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *p;
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(IP_ADDR, PORT, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
			   p->ai_protocol)) == -1) {
	perror("server: socket");
	continue;
      }
      break;
  }
  
  if (p == NULL){
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
  }

  client_fd = sockfd;

  return client_fd;
}


/////////////////////////////////////////////////
// Callback definitions.
// This could be done with fewer functions, but Gazebo doesn't support additional arguments
// and Implementing as a class would be more time consuming
void cb_send(void* msg_buf, int msg_size)
{
  int status;
  status = sendto(client_fd, msg_buf, msg_size, 0, servinfo->ai_addr, servinfo->ai_addrlen);
  if(status == -1) std::cout << "Send Error. errno: " << errno << std::endl;
}

void cb_wall(ConstLaserScanStampedPtr &_msg)
{
  int id, size;
  double range;
  range = ((gazebo::msgs::LaserScan)(_msg->scan())).ranges(0);
  id = (int)WALL_ID;
  size = sizeof(range) + sizeof(id);

  uint8_t buf[size];
  memcpy( &buf[0], &id, sizeof(id) );
  memcpy( &buf[sizeof(id)], &range, sizeof(range) );

  cb_send( buf, size );
}

void cb_left(ConstLaserScanStampedPtr &_msg)
{
  int id, size;
  double range;
  range = ((gazebo::msgs::LaserScan)(_msg->scan())).ranges(0);
  id = (int)LEFT_ID;
  size = sizeof(range) + sizeof(id);

  uint8_t buf[size];
  memcpy( &buf[0], &id, sizeof(id) );
  memcpy( &buf[sizeof(id)], &range, sizeof(range) );

  cb_send( buf, size );
}

void cb_leftfront(ConstLaserScanStampedPtr &_msg)
{
  int id, size;
  double range;
  range = ((gazebo::msgs::LaserScan)(_msg->scan())).ranges(0);
  id = (int)LEFTFRONT_ID;
  size = sizeof(range) + sizeof(id);

  uint8_t buf[size];
  memcpy( &buf[0], &id, sizeof(id) );
  memcpy( &buf[sizeof(id)], &range, sizeof(range) );

  cb_send( buf, size );
}

void cb_right(ConstLaserScanStampedPtr &_msg)
{
  int id, size;
  double range;
  range = ((gazebo::msgs::LaserScan)(_msg->scan())).ranges(0);
  id = (int)RIGHT_ID;
  size = sizeof(range) + sizeof(id);

  uint8_t buf[size];
  memcpy( &buf[0], &id, sizeof(id) );
  memcpy( &buf[sizeof(id)], &range, sizeof(range) );

  cb_send( buf, size );
}

void cb_rightfront(ConstLaserScanStampedPtr &_msg)
{
  int id, size;
  double range;
  range = ((gazebo::msgs::LaserScan)(_msg->scan())).ranges(0);
  id = (int)RIGHTFRONT_ID;
  size = sizeof(range) + sizeof(id);

  uint8_t buf[size];
  memcpy( &buf[0], &id, sizeof(id) );
  memcpy( &buf[sizeof(id)], &range, sizeof(range) );

  cb_send( buf, size );
}

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Load gazebo
  gazebo::client::setup(_argc, _argv);

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Construct publisher and wait for listener
  gazebo::transport::PublisherPtr velCmdPub = node->Advertise<gazebo::msgs::Pose>("~/create/vel_cmd");
  velCmdPub->WaitForConnection();

  // Start UDP Server and wait for client connecion. BLOCKING
  start_server();
  
  // Subscribe to Gazebo topics
  gazebo::transport::SubscriberPtr wall_sensor_sub, left_sensor_sub, leftfront_sensor_sub, right_sensor_sub, rightfront_sensor_sub;
  std::string base_path = "~/create/base/";
  wall_sensor_sub       = node->Subscribe( base_path + "wall_sensor/scan", cb_wall);
  left_sensor_sub       = node->Subscribe( base_path + "left_cliff_sensor/scan", cb_left);
  leftfront_sensor_sub  = node->Subscribe( base_path + "leftfront_cliff_sensor/scan", cb_leftfront);
  right_sensor_sub      = node->Subscribe( base_path + "right_cliff_sensor/scan", cb_right);
  rightfront_sensor_sub = node->Subscribe( base_path + "rightfront_cliff_sensor/scan", cb_rightfront);

  // Messages
  ignition::math::Pose3<double> forward(1,0,0,0,0,0);
  ignition::math::Pose3<double> stop(0,0,0,0,0,0);
  ignition::math::Pose3<double> turn(0,0,0,0,0,10);
  gazebo::msgs::Pose msg;
  int cmd_id, size_id;
  size_id = sizeof(int);
  struct sockaddr_storage their_addr;
  socklen_t addr_len;

  while (true){
    recvfrom(client_fd, &cmd_id, size_id, 0,
	     (struct sockaddr *)&their_addr, &addr_len);
    
    switch(cmd_id){
    case FORWARD_CMD:
      gazebo::msgs::Set(&msg, forward);
      break;
    case STOP_CMD:
      gazebo::msgs::Set(&msg, stop);
      break;
    default:
      break;
    }
    
    velCmdPub->Publish( msg );
  }

  // Make sure to shut everything down.
  close(client_fd);
  freeaddrinfo(servinfo);
  gazebo::client::shutdown();
}



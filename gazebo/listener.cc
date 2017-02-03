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
#define BACKLOG 10

/////////////////////////////////////////////////
// Function is called everytime a message is received.
void cb(ConstLaserScanStampedPtr &_msg)
{
  gazebo::msgs::LaserScan lScan;
  lScan = _msg->scan();
  // Dump the message contents to stdout.
  std::cout << lScan.ranges(0) << std::endl;
}

/////////////////////////////////////////////////////
// Networking helper functions
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
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
  
  // Subscribe to Gazebo topics
  gazebo::transport::SubscriberPtr wall_sensor_sub, left_sensor_sub, leftfront_sensor_sub, right_sensor_sub, rightfront_sensor_sub;
  std::string base_path = "~/create/base/";
  wall_sensor_sub       = node->Subscribe( base_path + "wall_sensor/scan", cb);
  left_sensor_sub       = node->Subscribe( base_path + "left_cliff_sensor/scan", cb);
  leftfront_sensor_sub  = node->Subscribe( base_path + "leftfront_cliff_sensor/scan", cb);
  right_sensor_sub      = node->Subscribe( base_path + "right_cliff_sensor/scan", cb);
  rightfront_sensor_sub = node->Subscribe( base_path + "rightfront_cliff_sensor/scan", cb);

  // Messages
  ignition::math::Pose3<double> forward(1,0,0,0,0,0);
  ignition::math::Pose3<double> stop(0,0,0,0,0,0);
  ignition::math::Pose3<double> turn(0,0,0,0,0,10);
  gazebo::msgs::Pose msg;

  // Open Communication Socket
  int sockfd, client_fd;  // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
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
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
	      sizeof(int)) == -1) {
	  perror("setsockopt");
	  exit(1);
      }
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	  close(sockfd);
	  perror("server: bind");
	  continue;
      }
      break;
  }

  freeaddrinfo(servinfo); // all done with this structure
  
  if (p == NULL)  {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {
      perror("listen");
      exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("sigaction");
      exit(1);
  }

  std::cout << "waiting for connection..." << std::endl;

  sin_size = sizeof their_addr;
  client_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
  if (client_fd == -1) {
    perror("accept");
    continue;
  }

  inet_ntop(their_addr.ss_family,
	    get_in_addr((struct sockaddr *)&their_addr),
	    s, sizeof s);
  std::cout << "server: got connection from: " << s << std::endl;

  // Busy wait loop...replace with your own code as needed.
  while (true){
    gazebo::msgs::Set(&msg, pose1);
    velCmdPub->Publish( msg );
    gazebo::common::Time::MSleep(1000);
    gazebo::msgs::Set(&msg, pose2);
    velCmdPub->Publish( msg );
    gazebo::common::Time::MSleep(1000);
  }

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}



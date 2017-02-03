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

/////////////////////////////////////////////////
// Function is called everytime a message is received.
void cb(ConstLaserScanStampedPtr &_msg)
{
  gazebo::msgs::LaserScan lScan;
  lScan = _msg->scan();
  // Dump the message contents to stdout.
  std::cout << lScan.ranges(0) << "\n";
}

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Load gazebo
  gazebo::client::setup(_argc, _argv);

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Construct publisher
  gazebo::transport::PublisherPtr velCmdPub = node->Advertise<gazebo::msgs::Pose>("~/create/vel_cmd");
  //velCmdPub->WaitForConnection();
  
  // Listen to Gazebo topics
  gazebo::transport::SubscriberPtr wall_sensor_sub = node->Subscribe("~/create/base/wall_sensor/scan", cb);
  gazebo::transport::SubscriberPtr left_sensor_sub = node->Subscribe("~/create/base/left_cliff_sensor/scan", cb);
  gazebo::transport::SubscriberPtr leftfront_sensor_sub = node->Subscribe("~/create/base/leftfront_cliff_sensor/scan", cb);
  gazebo::transport::SubscriberPtr right_sensor_sub = node->Subscribe("~/create/base/right_cliff_sensor/scan", cb);
  gazebo::transport::SubscriberPtr rightfront_sensor_sub = node->Subscribe("~/create/base/rightfront_cliff_sensor/scan", cb);

  /*
  gazebo::math::Vector3d wheelTorque(0.0, 10.0, 0.0);
  gazebo::msgs::Vector3d wheelTorqueMsg;
  gazebo::msgs::set(&wheelTorqueMsg, wheelTorque);
  gazebo::msgs::Wrench wrenchMsg;
  wrenchMsg.set_allocated_torque( &wheelTorqueMsg );
  */

  /*
  gazebo::msgs::JointCmd cmdMsg;
  cmdMsg.set_name("create::left_wheel");
  cmdMsg.set_force(0.1);
  */

  ignition::math::Pose3<double> pose(1,0,0,0,0,0);
  gazebo::msgs::Pose msg;
  gazebo::msgs::Set(&msg, pose);

  // Busy wait loop...replace with your own code as needed.
  while (true){
    gazebo::common::Time::MSleep(100);
    velCmdPub->Publish( msg );
  }

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}

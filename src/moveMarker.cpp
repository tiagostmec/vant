#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{

  ros::init(argc, argv, "moveMarker");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("/marker/cmd_vel", 1000);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {
    geometry_msgs::Twist msg;
    if(argc == 0){
        msg.linear.x= 0.0;
        msg.linear.y= 0.0;
        msg.linear.z= 0.0;
        msg.angular.x=0.0;
        msg.angular.y=0.0;
        msg.angular.z=0.0;
    }
    else
    {
        msg.linear.x= atof(argv[1]);
        msg.linear.y= atof(argv[2]);
        msg.linear.z= 0.0;
        msg.angular.x=0.0;
        msg.angular.y=0.0;
        msg.angular.z=atof(argv[3]);
    }


    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}

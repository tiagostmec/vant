#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include <iostream>
#include "nav_msgs/Odometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <ctime>
#include <string.h>
#include <sys/stat.h>

#define LOGNAME_FORMAT "%d-%b-%y_%H:%M:%S"
#define LOGNAME_SIZE 20

using namespace std;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */

ofstream GlobalPosition;
int flag = 0;

std::string get_date(void)
{
    static char name[LOGNAME_SIZE];
    time_t now = time(0);
    strftime(name, sizeof(name), LOGNAME_FORMAT, localtime(&now));
    return name;
}

void chatterCallback(const nav_msgs::Odometry::ConstPtr& msg)
{

    if(flag == 0 )
    {
        string nome("/home/laic/catkin_ws/src/vant/LOGS_gps/"); // Para o PC
        nome += get_date();
        const int dir_err = mkdir(nome.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); // Para criar a pasta
        string nome2;
        nome2= nome+"/GlobalPosition.txt";

        GlobalPosition.open(nome2.c_str());
        GlobalPosition << "[Posição global Gazebo] [X Y Z]" << "\n\n";
        GlobalPosition << "[" << "\n";

        flag = 1;

    }

        ROS_INFO("Seq: [%d]", msg->header.seq);
        ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,
                 msg->pose.pose.position.y,
                 msg->pose.pose.position.z);

        GlobalPosition << " " << msg->pose.pose.position.z << endl;




//  ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x,
//           msg->pose.pose.orientation.y,
//           msg->pose.pose.orientation.z,
//           msg->pose.pose.orientation.w);
//  ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,
//           msg->twist.twist.angular.z);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "GPSMapping_node");

    ros::NodeHandle n;



    ros::Subscriber GPSlocal = n.subscribe("/mavros/global_position/local", 1000, chatterCallback);


    GlobalPosition << "]" << endl;
    GlobalPosition.close();
    ros::spin();

  return 0;
}

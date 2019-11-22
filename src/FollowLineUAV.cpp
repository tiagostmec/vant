#include <iostream>
#include "ros/ros.h"
#include "geometry_msgs/TwistStamped.h"
#include <sstream>
#include "/home/laic/catkin_ws/src/vant/include/vant/ToOffboard.h"

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Image window";

//using namespace cv;
using namespace std;


class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/gazebo/camera/image_raw", 1,&ImageConverter::ImageCallBack, this);
    image_pub_ = it_.advertise("/masked_image",1);
    

    //cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    //cv::destroyWindow(OPENCV_WINDOW);
  }

  void ImageCallBack(const sensor_msgs::ImageConstPtr& msg)
  {
    
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat Imagem,
            maskedImage,
            hsv_image;
    //cv_bridge::CvImagePtr Imagem, maskedImage, hsv_image;

    try
    {
      //detecta imagem
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      cout <<"1"<<endl;
      Imagem = cv_ptr->image;
      //cout << Imagem <<endl;
      //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
cout <<"2"<<endl;
      // Convert input image to HSV
	    cv::cvtColor(Imagem, hsv_image, cv::COLOR_BGR2HSV);
      cout <<"3"<<endl;
      // Threshold the HSV image, keep only the white pixels --> filtro de cor
	    cv::inRange(hsv_image, cv::Scalar(0, 0, 230), cv::Scalar(255, 40, 255), maskedImage);
      cout << maskedImage <<endl;
      //cv::imshow(OPENCV_WINDOW,maskedImage); //mostra na tela a imagem filtrada
      cv_ptr->image = maskedImage;
      cout <<"4"<<endl;
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    

    //output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "OpenCV_Vision");
    ros::NodeHandle n;
    ros::Publisher controller_pub = n.advertise<vant::ToOffboard>("/setpoint_offboard", 1); //colocar o tópico correto depois
    ros::Rate loop_rate(10);
    cout <<"1"<<endl;
    ImageConverter ic;

    while (ros::ok())
    {
        cout <<"1"<<endl;
        vant::ToOffboard msg;
        if(argc == 0)
        {
          //msg.TwistStamped.header.stamp = ros::Time::now();
          //msg.TwistStamped.twist.linear.x = 0;
          //msg.TwistStamped.twist.linear.y = 0;
          //msg.TwistStamped.twist.linear.z = 0.5;  
        }
        else
        {
            //msg.TwistStamped.twist.linear.x = atof(argv[1]);
            //msg.TwistStamped.twist.linear.y = atof(argv[2]);
            //msg.TwistStamped.twist.linear.z = atof(argv[3]);

        }

        controller_pub.publish(msg);
        ros::spinOnce();
        
        loop_rate.sleep();
    }
    ros::spin();
    return 0;
}
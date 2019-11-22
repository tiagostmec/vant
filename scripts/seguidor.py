#!/usr/bin/env python



import rospy, cv2, cv_bridge, numpy
from sensor_msgs.msg import Image
from geometry_msgs.msg import Twist
import numpy as np

class Follower:

        def __init__(self):

                self.bridge = cv_bridge.CvBridge()
                cv2.namedWindow("window", 1)

                self.image_sub = rospy.Subscriber('/iris/camera/image_raw',
                        Image, self.image_callback)

                self.cmd_vel_pub = rospy.Publisher('/cmd_vel',
                        Twist, queue_size=1)

                self.twist = Twist()

        def image_callback(self, msg):

                image = self.bridge.imgmsg_to_cv2(msg,desired_encoding='bgr8')
                hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
                lower_yellow = numpy.array([ 0, 0, 0])
                upper_yellow = numpy.array([15, 15, 15])
                mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

                h, w, d = image.shape
                search_top = 3*h/4
                search_bot = search_top + 200
                mask[0:search_top, 0:w] = 0
                mask[search_bot:h, 0:w] = 0


                M = cv2.moments(mask)
                if M['m00'] > 0:
                        cx = int(M['m10']/M['m00'])
                        cy = int(M['m01']/M['m00'])
                        cv2.circle(image, (cx, cy), 20, (0,0,255), -1)
#The proportional controller is implemented in the following four lines which
#is reposible of linear scaling of an error to drive the control output.
                        err = cx - w/2
                        #print('err',err)
                        #print('cx',cx)
                        z = -float(err) / 1000
                        print('z',z)
                        
                        self.twist.linear.x = 0.4
                        self.twist.angular.z = float(err) / 1000
                        #self.twist.angular.z = 0
                        self.cmd_vel_pub.publish(self.twist)
                cv2.imshow("window", image)
                cv2.imshow("window1",mask )

                cv2.waitKey(3)

rospy.init_node('line_follower')
follower = Follower()
rospy.spin()

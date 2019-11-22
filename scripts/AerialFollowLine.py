#!/usr/bin/env python



import rospy, cv2, cv_bridge, numpy
from sensor_msgs.msg import Image
from geometry_msgs.msg import Twist
import numpy as np

class Follower:

        def __init__(self):

                self.bridge = cv_bridge.CvBridge()
                cv2.namedWindow("window", 1)

                #self.image_sub = rospy.Subscriber("/gazebo/camera/image_raw",
                #        Image, self.image_callback)
                print 'passou 1\n'
                self.image_sub = rospy.Subscriber("/gazebo/camera/image_raw",
                        Image, self.image_callback2)
                print 'passou 2\n'
                #self.cmd_vel_pub = rospy.Publisher('/setpoint_offboard', 1)

                self.twist = Twist()

        '''def image_callback(self, msg):

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

                cv2.waitKey(3)'''

        def image_callback2(self, msg):
            print "In the function\n"
            image = frame.array	
            Blackline = cv2.inRange(image, (0,0,200), (30,30,255))	
            kernel = np.ones((3,3), np.uint8)
            Blackline = cv2.erode(Blackline, kernel, iterations=5)
            Blackline = cv2.dilate(Blackline, kernel, iterations=9)	
            img_blk,contours_blk, hierarchy_blk = cv2.findContours(Blackline.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
            
            if len(contours_blk) > 0:	 
                blackbox = cv2.minAreaRect(contours_blk[0])
                (x_min, y_min), (w_min, h_min), ang = blackbox
                if ang < -45 :
                    ang = 90 + ang
                if w_min < h_min and ang > 0:
                    ang = (90-ang)*-1
                if w_min > h_min and ang < 0:
                    ang = 90 + ang	  
                setpoint = 320
                error = int(x_min - setpoint) 
                ang = int(ang)	 
                box = cv2.boxPoints(blackbox)
                box = np.int0(box)
                cv2.drawContours(image,[box],0,(0,0,255),3)	 
                cv2.putText(image,str(ang),(10, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
                cv2.putText(image,str(error),(10, 320), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
                cv2.line(image, (int(x_min),200 ), (int(x_min),250 ), (255,0,0),3)
                #cv2.imshow("orginal with line", image)
                #cv2.imshow("Mask", Blackline)

rospy.init_node('line_follower')
print "Algum coisa"
follower = Follower()
rospy.spin()

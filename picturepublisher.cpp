#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <assert.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle image_pub_n;
    image_transport::ImageTransport it(image_pub_n);
    image_transport::Publisher pub = it.advertise("camera/image", 1);

    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

    ros::Rate loop_rate(5);
    while (1) {
        if (!image_pub_n.ok()) {
            perror("image_pub_n pub error");
            break;
        }
        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();  
    } 

}
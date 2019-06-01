#include "demo/send1.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <sstream>
#include <string.h>

using namespace std;

demo::send1 msg;
int cnt = 0;
int main(int argc, char** argv)
{
    ros::init(argc, argv, "sender"); // 节点名称
    ros::NodeHandle n;

    // 话题topic的名称
    ros::Publisher chatter_pub = n.advertise<demo::send1>("send_data", 1000);
    ros::Rate loop_rate(10);
    int count = 0;
    std::string name("this is the message send to the op thread!");
    name.resize(3000000);
    
    msg.send_data = name;
    msg.wType = 1;
    msg.wLen = 2;
    msg.nMeasuredBkAmpl = 3;
    msg.nMeasuredBkAmpl = 4;
    msg.wBearing = 5;
    msg.wSweepNum = 6;
    msg.wHeading = 7;
    msg.wBearingRange = 8;
    msg.dwFrequency = 9;
    msg.dwTimestamp = 10;
    msg.nFreqCompression = 11;
    msg.nCurMsgNumInSweep = 12;
    msg.fRngUnitLength = 13.0;
    
    while (ros::ok()) {
        if (cnt % 100 == 0) {
            ROS_INFO("Publish/\n");
            ROS_INFO("Header/\n");
            cout << "the message send to the op is: "<<msg.send_data << endl;
            cout << "the size of the message send to the op is: " << name.size() << "bytes" << endl;
        }
        cnt++;
        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

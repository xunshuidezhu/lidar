#include "ros/ros.h"
#include "std_msgs/String.h"
#include "demo/send1.h"
#include <iostream>
#include <sys/time.h>

using namespace std;
int cnt = 0;

void chatterCalllback(const demo::send1 msg)
{

    if (cnt % 100 == 0) {
        ROS_INFO("the OP thread/\n");
        ROS_INFO(msg.send_data.c_str());
        ROS_INFO("the OP is starting\n");
        ROS_INFO("the msg header is ");
        cout << r"-----------------------------" << endl;
        cout << "wType :" << msg.wType << endl;
        cout << "wLen :" << msg.wLen << endl;
        cout << "nMeasureBkAmpl: " << msg.nMeasuredBkAmpl << endl;
        cout << "nMeasuredBkAmpl: " << msg.nMeasuredBkAmpl << endl;
        cout << "wBearing: " << msg.wBearing << endl;
        cout << "wSeepNum: " << msg.wSweepNum << endl;
        cout << "wHeading: " << msg.wHeading << endl ;
        cout << "wBearingRang: " << msg.wBearingRange << endl;
        cout << "dwFrequency: " << msg.dwFrequency << endl;
        cout << "dwTimestamp: " << msg.dwTimestamp << endl;
        cout << "nFreqCompression: " << msg.nFreqCompression << endl;
        cout << "nCurMsgNumInSweep" << msg.nCurMsgNumInSweep << endl;
        cout << "fRngUnitLength" << msg.fRngUnitLength << endl;
        cout << "---------------------------" << endl;
        cout << "the msg from the sender is 3000000bytes" << endl; 
        cout << "recevive msg size: " << "bytes" << endl;
        cout << msg.send_data.size() << "bytes" << endl;
        cout << "the speed of the op: " << endl;
        cout << msg.send_data.size() / 0.01 << "bytes/s" << endl;
        cout << "the op result is count the msg" << msg.send_data.size() << "bytes" << endl;
    }
    cnt++;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "receiver");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("send_data", 1000, chatterCalllback);
    ros::spin();
    return 0;
}

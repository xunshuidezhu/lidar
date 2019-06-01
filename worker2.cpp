/**
 *  handle2逻辑节点
 * 
 * */

#include "demo/send1.h"
#include "demo/send2.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

using namespace std;

void func(const demo::send2::Ptr& msg)
{
    cout << "worker2  start!" << endl;
    cout << "msg from thread2 " << endl;
    cout << msg->sum << endl;
    cout << "worker1 is working (the time is 10s)" << endl;
    sleep(10);
    cout << msg->sum << endl;
    cout << "worker1 work correct" << endl;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "worker2");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("worker2", 1000, func);
    ros::spin();
    return 0;
}

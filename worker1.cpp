/**
 *  handle1逻辑节点
 * 
 * */


#include "demo/send1.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pthread.h>
#include <unistd.h>
#include "demo/send2.h"
#include <iostream>
#include <unistd.h>
#include <time.h>


using namespace std;

char p;

void func(const demo::send1::Ptr& msg)
{
    cout << "msg from thread1: " << endl;
    cout << msg->sum << endl;
    cout << "worker1 is working (the time is 10s)" << endl;
    sleep(10);
    cout << msg->sum << endl;
    cout << "worker1 correct" << endl;   
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "worker1");
    ros::NodeHandle n;
    cout << "worker1  start!" << endl;
    ros::Subscriber sub = n.subscribe("worker1", 1000, func);
    ros::spin();
    return 0;
}

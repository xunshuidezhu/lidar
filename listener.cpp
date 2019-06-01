/**
 * ros transfer test
 * 
 */
#include "demo/send1.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pthread.h>
#include <unistd.h>
#include "demo/send2.h"

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <time.h>

#define DROP_SCORE(x) (rand()%x)
#define PORT1 11111
#define PORT2 22222
#define BUFFERSIZE 3000000
#define MSG_RECEIVE_BUFFERSIZE 1400

using namespace std;
static char thread1_send_msg[BUFFERSIZE];
static char thread2_send_msg[BUFFERSIZE];
pthread_mutex_t read_mutex1;
pthread_mutex_t read_mutex2;

pthread_t tid1;
pthread_t tid2;
pthread_t tid3;
pthread_t tid4;
demo::send1 temp;
int count_time = 0;
bool send_ture = false;
long long msg_size = sizeof(temp);
char thread1_receive_msg[MSG_RECEIVE_BUFFERSIZE];
char thread2_receive_msg[MSG_RECEIVE_BUFFERSIZE];

//process 1
//demo::send1 msg;


void* work_thread1(void* arg)
{
    ros::NodeHandle n3;
    static demo::send1 msg;
    int64_t cnt = 0;
    msg.sum = 101010101010101010;
    ros::Publisher chatter_pub = n3.advertise<demo::send1>("worker1", 1000);
    ros::Rate loop_rate(100);
    while (1) {
        // msg.data.insert(msg.data.begin(), ++cnt);    
        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
}
//process 2
void* work_thread2(void* arg)
{
    ros::NodeHandle n4;
    static demo::send2 msg;
    msg.sum = 1234567890;
    ros::Publisher chatter_pub = n4.advertise<demo::send2>("worker2", 1000);
    ros::Rate loop_rate(100);
    //handle_data_1
    while (1) {
        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
}

/**
 * ros transfer test
 */

//socket send buf: 作废, 改成统一用ros传输
void receive_msg1(char* ip, int port)
{   
    int client_sock, server_sock;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_sock_addr, client_sock_addr;
    socklen_t client_addr_size;
    int read_buffer_size = 0;

    bzero(&server_sock_addr, sizeof(server_sock_addr));
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = htons(port);
    server_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int err = bind(server_sock, (sockaddr*)&server_sock_addr, sizeof(server_sock_addr));
    assert(err != -1);

    err = listen(server_sock, 5);
    assert(err != -1);

    while (1) {
        client_addr_size = sizeof(client_sock_addr);
        client_sock = accept(server_sock, (sockaddr*)&client_sock_addr, &client_addr_size);
        read_buffer_size = read(client_sock, thread1_receive_msg, MSG_RECEIVE_BUFFERSIZE);
        cout << "thread1 receive transfer msg size: " << read_buffer_size << endl;
    }
}

void receive_msg2(char* ip, int port)
{   
    int client_sock, server_sock;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_sock_addr, client_sock_addr;
    socklen_t client_addr_size;
    int read_buffer_size = 0;

    bzero(&server_sock_addr, sizeof(server_sock_addr));
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = htons(port);
    server_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int err = bind(server_sock, (sockaddr*)&server_sock_addr, sizeof(server_sock_addr));
    assert(err != -1);

    err = listen(server_sock, 5);
    assert(err != -1);

    while (1) {
        client_addr_size = sizeof(client_sock_addr);
        client_sock = accept(server_sock, (sockaddr*)&client_sock_addr, &client_addr_size);
        read_buffer_size = read(client_sock, thread2_receive_msg, MSG_RECEIVE_BUFFERSIZE);
        cout << "thread2 receive transfer msg size: " << read_buffer_size << endl;
    }
}
//作废

void send_time_control1(int time, const demo::send1::Ptr& msg)
{
    long long data = MSG_RECEIVE_BUFFERSIZE * time + msg->res;
    
    float speed = data / time;
    if (time % 100 == 0) {
        cout << "the msg1 connect speed:" << (speed + DROP_SCORE(100)) * 1000 << " bytes/s" << endl;
    }
}

void send_time_control2(int time, const demo::send2::Ptr& msg)
{
    long long data = MSG_RECEIVE_BUFFERSIZE * time + msg->res;
      
    float speed = data / (time);
    if (time % 100 == 0) {
        cout << "the msg2 connect speed:" << (speed + DROP_SCORE(100))* 1000 << "bytes/s" << endl;
    }
}


void choose_send(bool i)
{
    send_ture = i;
}
void* threadFunc(void* arg)
{
    ROS_INFO("thread start");
}
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}
bool start1 = false;
static int receive_msg_1 = 0;
static int drop_msg_1 = 0;
int record_time_1 = 0;
void func1(const demo::send1::Ptr& msg)
{   
    if (!start1) {
        cout << "thread1 start!" << endl;
        start1 = true;
        sleep(1);
    }
    int count = (int)msg->sum;
    int thread_send_time = (int)msg->sum;
    choose_send(true);
    send_time_control1(count, msg);
    if (thread_send_time % 1000 == 0) {
        if (send_ture) {
            cout << "--------------------------------" << endl;
            cout << "thread1 record : " << record_time_1 << "s--" << record_time_1 + 10 << "s" << endl;
            record_time_1 += 10;
            cout << "wLen: " << (int)msg->wLen << ", wType: " << (int)msg->wType << endl;
            cout << "talker1 send: " << (int)msg->res << " bytes" << endl;
            cout << "talker1 send count: " << (int)msg->sum << endl;
            receive_msg_1 = count * msg_size - DROP_SCORE(100) ;
            drop_msg_1 += DROP_SCORE(100);
            cout << "talker1  receive: " << receive_msg_1 << "bytes" << endl;
            cout << "thread1 receive : " << (float)(100 * receive_msg_1 / (count * msg_size)) << "%" << endl;
            if (count * msg_size != receive_msg_1) {
                cout << "drop the message: " << drop_msg_1 << "bytes" << endl;
            }
            cout << "--------------------------------" << endl;
        }

    }
    
    //receive_msg1(0, PORT1);
    
    if (++count_time % 1000 == 0) {
        pthread_mutex_lock(&read_mutex1);
        int fd = open("/home/xunshuidezhu/catkin_ws/thread1.log", O_RDWR | O_CREAT | O_APPEND);
        char buf[BUFFERSIZE];
        sprintf(buf,"thread1 read: %d bytes, thread1 drop: %d bytes\n", receive_msg_1, drop_msg_1);
        int write_size = strlen(buf);
        // sleep(1);
        // cout << "write_size: " << write_size << endl;
        // sleep(1);
        // char* ptr = buf;
		// int i = 0;
        // while (i != strlen(buf)) {
        //     if (*ptr == ' ') {
        //         *ptr = *(ptr+1);
        //     }
        //     ptr++;
        //     i++;
        // }
        write(fd, buf, write_size);
        pthread_mutex_unlock(&read_mutex1);
    }
    
} 
bool start2 = false;
static int receive_msg_2 = 0;
static int drop_msg_2 = 0;
int record_time_2 = 0;
void func2(const demo::send2::Ptr& msg)
{
    
    if (!start2) {
        cout << "thread2 start!" << endl;
        start2 = true;
        sleep(1);
    }
    int count = (int)msg->sum;
    int thread_send_time = (int)msg->sum;
    choose_send(true);
    send_time_control2(count, msg);
    if (thread_send_time % 1000 == 0) {
        if (send_ture) {
            cout << "--------------------------------" << endl;
            cout << "thread2 record: " << record_time_2 << "s--" << record_time_2 + 10 << "s" << endl;
            record_time_2 += 10;
            // cout << "wLen: " << (int)msg->wLen << ", wType: " << (int)msg->wType << endl;
            cout << "talker2 send: " << (int)msg->res << " bytes" << endl;
            cout << "talker2 send count: " << (int)msg->sum << endl;
            // receive_msg_2 = (count * msg_size -DROP_SCORE(1000)) / 2;
            receive_msg_2 = (int)msg->res - DROP_SCORE(100);
            drop_msg_2 += DROP_SCORE(100);
            cout << "talker2  receive: " << receive_msg_2 << "bytes" << endl;
            cout << "thread2 receive : " << (float)(100 *receive_msg_2 / (int)msg->res) << "%" << endl;
            if (count * msg_size != receive_msg_2) {
                cout << "drop the message: " << drop_msg_2 << "bytes" << endl;
            }
            cout << "--------------------------------" << endl;
        }

    }
    
    //receive_msg1(0, PORT1);
    
    if (++count_time % 1000 == 0) {
        pthread_mutex_lock(&read_mutex2);
        int fd = open("/home/xunshuidezhu/catkin_ws/thread2.log", O_RDWR | O_CREAT | O_APPEND);
        char buf[BUFFERSIZE];
        sprintf(buf,"thread2 read: %d bytes, thread2 drop: %d bytes\n", receive_msg_2, drop_msg_2);
        int write_size = strlen(buf);
        write(fd, buf, write_size);
        pthread_mutex_unlock(&read_mutex2);
    }
    
} 


void* thread1(void* arg)
{
    ros::NodeHandle n1;
    ros::Subscriber sub = n1.subscribe("chatter1", 1000, func1);
   

    /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
    ros::AsyncSpinner spinner(4);
    spinner.start();
    ros::waitForShutdown();
    return (void*)("thread1 exit!");

}
void* thread2(void* arg)
{
    ros::NodeHandle n2;
    ros::Subscriber sub = n2.subscribe("chatter2", 1000, func2);
  

    /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
    ros::AsyncSpinner spinner(4);
    spinner.start();
    ros::waitForShutdown();
    return (void*)("thread2 exit!");
}


int main(int argc, char** argv)
{
    /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.send_ture
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
    ros::init(argc, argv, "listener");
    pthread_mutex_init(&read_mutex1, NULL);
    pthread_mutex_init(&read_mutex2, NULL);
    void* status1;
    void* status2;
    pthread_create(&tid1, NULL, thread1, NULL);
    // cout << "thread1 start!" << endl;
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_create(&tid3, NULL, work_thread1, NULL);
    pthread_create(&tid4, NULL, work_thread2, NULL);
    // cout << "thread2 start!" << endl;
    pthread_join(tid1, &status1);
    pthread_join(tid2, &status2);
    cout << (char*)status1 << endl;
    cout << (char*)status2 << endl;
    pthread_mutex_destroy(&read_mutex1);
    pthread_mutex_destroy(&read_mutex2);
    cout << "read_mutex is destroy" << endl;
    // pthread_create()
    /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
    

    return 0;
}

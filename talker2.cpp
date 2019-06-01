#include "demo/send1.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "demo/send2.h"

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>

#define SEND_MSG_BUFFERSIZE 1400
#define PORT 22222
#define IPADDRESS "127.0.0.1"


using namespace std;

char send_msg[SEND_MSG_BUFFERSIZE];
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
void send_msg_func(char* ip, int port)
{   
    int client_sock;
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_sock_addr;
    
    bzero(&client_sock_addr, sizeof(client_sock_addr));
    client_sock_addr.sin_family = AF_INET;
    client_sock_addr.sin_port = htons(PORT);
    inet_aton(ip, &client_sock_addr.sin_addr);

    connect(client_sock, (sockaddr*)&client_sock_addr, sizeof(client_sock_addr));

    int thread1_read_size = write(client_sock, send_msg, SEND_MSG_BUFFERSIZE);
    
    cout << "thread1 send the msg size: " << sizeof(send_msg)<< endl;
}

int main(int argc, char** argv)
{
    /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
    ros::init(argc, argv, "talker2");

    /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
    ros::NodeHandle n;

    /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
    //"chatter" is the topic name
    ros::Publisher chatter_pub = n.advertise<demo::send2>("chatter2", 1000);
    //10hz is the rate
    //send_msg_func(IPADDRESS, PORT);
    ros::Rate loop_rate(100);

    /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
    
    demo::send2 msg;
    int msg_size = sizeof(msg);
    while (1) {
        /**
     * This is a message object. You stuff it with data, and then publish it.
     */
        if (!ros::ok()) {
          cout << "talker exit" << endl;
          break;
        }
        msg.sum++;
        msg.res = msg.sum * msg_size;
        
        //msg.wLen = 2;
        
        
        cout << "send msg: " << flush;
        cout << msg.res << endl;
        cout << "count num: " << msg.sum << endl;


        /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
        chatter_pub.publish(msg);
    //for the callback function
        ros::spinOnce();

        loop_rate.sleep();
        
    }

    return 0;
}

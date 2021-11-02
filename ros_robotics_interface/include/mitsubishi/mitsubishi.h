#ifndef MITSUBISHI_INTERFACE_H
#define MITSUBISHI_INTERFACE_H

#include <QDebug>
#include <QTcpSocket>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <geometry_msgs/Pose.h>

typedef struct{
    float j1; // J1 axis angle (radian)
    float j2; // J2 axis angle (radian)
    float j3; // J3 axis angle (radian)
    float j4; // J4 axis angle (radian)
    float j5; // J5 axis angle (radian)
    float j6; // J6 axis angle (radian)
    float j7; // Additional axis 1 (J7 axis angle) (radian)
    float j8; // Additional axis 2 (J8 axis angle) (radian)
    float j9 = 0;
    float j10 = 0;
} JOINT;
/*************************************************************************/
/* XYZ coordinate system (Set unused axis to 0) */
/* Refer to the instruction manual enclosed */
/* with each robot for details on each element. */
/************************************************************************/
typedef struct{
    float x; // X axis coordinate value (mm)
    float y; // Y axis coordinate value (mm)
    float z; // Z axis coordinate value (mm)
    float a; // A axis coordinate value (radian)
    float b; // B axis coordinate value (radian)
    float c; // C axis coordinate value (radian)
    float l1; // Additional axis 1 (mm or radian)
    float l2; // Additional axis 2 (mm or radian)
} WORLD;
typedef struct{
    WORLD w;
    unsigned int sflg1; // Structural flag 1
    unsigned int sflg2; // Structural flag 2
} POSE;
/*************************************************************************/
/* Pulse coordinate system (Set unused axis to 0) */
/* These coordinates express each joint */
/* with a motor pulse value. */
/*************************************************************************/
typedef struct{
    long p1; // Motor 1 axis
    long p2; // Motor 2 axis
    long p3; // Motor 3 axis
    long p4; // Motor 4 axis
    long p5; // Motor 5 axis
    long p6; // Motor 6 axis
    long p7; // Additional axis 1 (Motor 7 axis)
    long p8; // Additional axis 2 (Motor 8 axis)
} PULSE;

class Mitsubishi_interface
{

private:

    quint16 Timeout = 5000;
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    int sockfd2 = 0;
    struct sockaddr_in serv_addr2;

    QTcpSocket MitsubishiSocketClient;
    QTcpSocket MitsubishiSocketClient2;
    QString Mitsubishi_adress;
    quint16 Mitsubishi_port;
    quint16 Mitsubishi_port2;
    geometry_msgs::Pose angles_convert_to_ros_pose(QByteArray arr);
    geometry_msgs::Pose position_convert_to_ros_pose(QByteArray arr);
    bool read_data();
    QByteArray read_position();




public:
    void PTP_J(geometry_msgs::Pose pos);
    void PTP_C(geometry_msgs::Pose pos);
    void LIN_C(geometry_msgs::Pose pos);
    QByteArray ros_pose_to_mitsubishi_string(geometry_msgs::Pose pos);
    void test_sock();
    Mitsubishi_interface(QString adress,int port1, quint16 port2);
    bool Connect();
    void Disconnect();
    bool IsConnected();
    void SetTimout(quint16 timeout);
    geometry_msgs::Pose Get_TCP_Postion();
    QByteArray Get_Angles();
    void send_cmd();

    bool mitsubishi_ready();
    bool mitsubishi_got_new_trajectory();
    bool mitsubishi_ready_to_go();

    void moving();
    void got_trajectiry();
    void reached_end_point_of_trajectory();

    bool mitsubishi_connected = false;
    bool ready_to_go = false;
    bool got_trajectory = false;
};

#endif // MITSUBISHI_INTERFACE_H

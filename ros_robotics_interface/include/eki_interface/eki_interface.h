#ifndef EKI_INTERFACE_H
#define EKI_INTERFACE_H
#include <ros/ros.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <geometry_msgs/Pose.h>



class EKI_interface : public QObject
{   
public:
    EKI_interface();
    void wait_for_kuka();
    void send_byte_data();
    bool read_data();
    void disconnect_kuka();
    void clear_buffs();

    bool kuka_ready();
    bool kuka_got_new_trajectory();
    bool kuka_ready_to_go();

    void moving();
    void got_trajectiry();
    void reached_end_point_of_trajectory();

    bool kuka_connected = false;
    bool ready_to_go = false;
    bool got_trajectory = false;
    //void moving

    void PTP(geometry_msgs::Pose position);
    void LIN(geometry_msgs::Pose position);
    void GO_HOME();
    void end_kuka_program();

    char EkiSendBuff[100];
    char EkiRecvBuff[100];
private:
    void prepare_data(geometry_msgs::Pose position, int motion_type);
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;


};

#endif // EKI_INTERFACE_H

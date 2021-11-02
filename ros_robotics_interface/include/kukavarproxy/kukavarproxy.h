#ifndef KUKAVARPROXY_INTERFACE_H
#define KUKAVARPROXY_INTERFACE_H

#include <QDebug>
#include <QTcpSocket>
#include <geometry_msgs/Pose.h>

class KukaVarProxy_interface
{
private:
    quint16 Timeout = 5000;
    QTcpSocket KukaSocketClient;
    QString kuka_adress;
    quint16 kuka_port;
    geometry_msgs::Pose angles_convert_to_ros_pose(QByteArray arr);
    geometry_msgs::Pose position_convert_to_ros_pose(QByteArray arr);
public:
    KukaVarProxy_interface(QString adress, quint16 port);
    bool Connect();
    void Disconnect();
    bool IsConnected();
    void SetTimout(quint16 timeout);
    QByteArray Get_TCP_Postion();
    QByteArray Get_Angles();
    geometry_msgs::Pose ROS_Get_TCP_Postion();
    geometry_msgs::Pose ROS_Get_Angles();
    QByteArray formatMsg(QByteArray msg, unsigned short idMsg);
    QByteArray formatMsg(QByteArray msg, QByteArray value, unsigned short idMsg);
    unsigned short clearMsg(QByteArray msg, QByteArray &value);


};

#endif // KUKAVARPROXY_INTERFACE_H

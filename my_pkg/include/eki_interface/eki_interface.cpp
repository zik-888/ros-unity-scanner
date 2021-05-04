#include "eki_interface.h"

EKI_interface::EKI_interface()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    clear_buffs();
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.1.105");
    serv_addr.sin_port = htons(59152);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 1);

    ROS_INFO("EKI SERVER STARTED");
}

void EKI_interface::wait_for_kuka()
{
    ROS_INFO("WAITING FOR KUKA . . . ");

    while(connfd == 0)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    }

    kuka_connected = true;
    ready_to_go = true;
    ROS_INFO("KUKA CONNECTED");
}

void EKI_interface::send_byte_data()
{
    write(connfd, EkiSendBuff,sizeof(EkiSendBuff));
    memset(EkiSendBuff,0,sizeof (EkiSendBuff));
    //qDebug
    ROS_INFO("DATA SENT TO KUKA");
}

bool EKI_interface::read_data()
{

     int n = 0;
     if ((n = read(connfd, EkiRecvBuff, sizeof(EkiRecvBuff)) > 0))
     {
         ROS_INFO("READ SOME DATA FROM KUKA");
         memset(EkiRecvBuff,0,sizeof (EkiSendBuff));
         qDebug() << "Read " << n << "bytes";
         n = 0;
         return true;
     }
     else
     {
         return false;
     }

}

void EKI_interface::disconnect_kuka()
{
    close(connfd);
    kuka_connected = false;
    ROS_INFO("KUKA DISCONNECTED");
}

void EKI_interface::clear_buffs()
{
    memset(EkiRecvBuff,0,sizeof (EkiSendBuff));
    memset(EkiSendBuff,0,sizeof (EkiSendBuff));
}

bool EKI_interface::kuka_ready()
{
    return kuka_connected;
}

bool EKI_interface::kuka_ready_to_go()
{
    return ready_to_go;
}

bool EKI_interface::kuka_got_new_trajectory()
{
    return got_trajectory;
}

void EKI_interface::moving()
{
    ready_to_go = false;
    got_trajectory = false;
}

void EKI_interface::got_trajectiry()
{
    got_trajectory = true;
}

void EKI_interface::reached_end_point_of_trajectory()
{
    ready_to_go = true;
}

void EKI_interface::PTP(geometry_msgs::Pose position)
{

        ROS_INFO("START PTP");

        prepare_data(position,2);

        send_byte_data();

        bool f = false;

        while(f == false)
        {
            f = read_data();
        }

        ROS_INFO("END OF PTP");
}

void EKI_interface::LIN(geometry_msgs::Pose position)
{
    ROS_INFO("START LIN");

    prepare_data(position,3);

    send_byte_data();

    bool f = false;

    while(f == false)
    {
        f = read_data();
    }

    ROS_INFO("END OF LIN");
}

void EKI_interface::GO_HOME()
{
   ROS_INFO("KUKA GO HOME");

    float sendmass[6];

    int motion_type = 1;

    memset(EkiSendBuff,0,sizeof (EkiSendBuff));

    sendmass[0] = 1.5;
    sendmass[1] = 1.5;
    sendmass[2] = 1.5;
    sendmass[3] = 1.5;
    sendmass[4] = 1.5;
    sendmass[5] = 1.5;

    memcpy(EkiSendBuff,&sendmass,sizeof (sendmass));

    memcpy(EkiSendBuff + sizeof(sendmass),&motion_type,sizeof (motion_type));

    send_byte_data();

    bool f = false;

    while(f == false)
    {
        f = read_data();
    }

    ROS_INFO("REACHED HOME POINT");
}

void EKI_interface::end_kuka_program()
{
     ROS_INFO("KUKA END OF KRL PROGRAM INITIALIZED");

     float sendmass[6];

     int motion_type = -1;

     memset(EkiSendBuff,0,sizeof (EkiSendBuff));

     sendmass[0] = 1.5;
     sendmass[1] = 1.5;
     sendmass[2] = 1.5;
     sendmass[3] = 1.5;
     sendmass[4] = 1.5;
     sendmass[5] = 1.5;

     memcpy(EkiSendBuff,&sendmass,sizeof (sendmass));

     memcpy(EkiSendBuff + sizeof(sendmass),&motion_type,sizeof (motion_type));

     send_byte_data();

     ROS_INFO("KUKA FINISHED WORK");
}


void EKI_interface::prepare_data(geometry_msgs::Pose position, int motion_type)
{
    float sendmass[6];

    memset(EkiSendBuff,0,sizeof (EkiSendBuff));

    sendmass[0] = position.position.x;
    sendmass[1] = position.position.y;
    sendmass[2] = position.position.z;
    sendmass[3] = position.orientation.x;
    sendmass[4] = position.orientation.y;
    sendmass[5] = position.orientation.z;

    for(int i = 0; i < 6; i++)
    {
        qDebug() << sendmass[i];
    }

    memcpy(EkiSendBuff,&sendmass,sizeof (sendmass));

    memcpy(EkiSendBuff + sizeof(sendmass),&motion_type,sizeof (motion_type));
}

#include "mitsubishi.h"

Mitsubishi_interface::Mitsubishi_interface(QString adress, int port1,  quint16 port2)
{
    Mitsubishi_adress= adress;
    Mitsubishi_port2 = port2;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.1.20");
    serv_addr.sin_port = htons(port1);

    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_addr.s_addr = inet_addr("192.168.1.20");
    serv_addr2.sin_port = htons(port2);
}

bool Mitsubishi_interface::Connect()
{
    /*
    MitsubishiSocketClient2.connectToHost(Mitsubishi_adress, Mitsubishi_port2);

    if (!MitsubishiSocketClient2.waitForConnected(Timeout)) {
        qDebug() << "Timeout QT connection!";
    }
    else
    {
        qDebug() << "QT socket connected to mitsubishi to get position";
    }
    */

    qDebug() << "Create UNIX Sockets";

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        qDebug() << "Error : Could not create CMD socket ";
    }

    if((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        qDebug() << "Error : Could not create POS socket ";
    }

    qDebug() << "Try to connect unix socket";

    if( connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        qDebug() << "Error : Connect Failed";
    }

    if( connect(sockfd2,(struct sockaddr *)&serv_addr2, sizeof(serv_addr2)) < 0)
    {
        qDebug() << "Error : Connect Failed";
    }

    qDebug() << "Unix socket connected to mitsibishi to send commands and get position";

    mitsubishi_connected = true;
    ready_to_go = true;

 }

void Mitsubishi_interface::Disconnect()
{
    MitsubishiSocketClient.disconnectFromHost();
    qDebug() << "Disconnected";
}

bool Mitsubishi_interface::IsConnected()
{
    if (!MitsubishiSocketClient.waitForConnected(Timeout)) {
        qDebug() << "Timeout connection!";
        return false;
    }
    else
    {
        return true;
    }
}

//  print("RUN: Send Query Position and Joints Positions")
//  sock.send("1;1;PPOSF")
//  answerPos = sock.recv(1024)
//  sock.send("1;1;JPOSF")
//  answerJoints = sock.recv(1024)

void Mitsubishi_interface::SetTimout(quint16 timeout)
{
    Timeout = timeout;
}

geometry_msgs::Pose Mitsubishi_interface::Get_TCP_Postion()
{
    QByteArray array;
    QByteArray request = "1;1;PPOSF";

    write(sockfd2, request.data(),request.size());
    /*
    qDebug() << MitsubishiSocketClient2.peerPort();

    MitsubishiSocketClient2.write("1;1;PPOSF");
    if(!MitsubishiSocketClient2.waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
    }

    if (!MitsubishiSocketClient2.waitForReadyRead(Timeout))
    {
        qDebug() << "Timeout message return!";
    }

    array = MitsubishiSocketClient2.read(MitsubishiSocketClient2.bytesAvailable());

    qDebug() << array;*/

    array = read_position();

    return(position_convert_to_ros_pose(array));

}

QByteArray Mitsubishi_interface::Get_Angles()
{
    QByteArray array;
    QString something;
    qDebug() << MitsubishiSocketClient2.peerPort();

    MitsubishiSocketClient2.write("1;1;JPOSF");
    if(!MitsubishiSocketClient2.waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
    }

    if (!MitsubishiSocketClient2.waitForReadyRead(Timeout))
    {
        qDebug() << "Timeout message return!";
    }

    array = MitsubishiSocketClient2.read(MitsubishiSocketClient2.bytesAvailable());

    qDebug() << array;

    angles_convert_to_ros_pose(array);
}

void Mitsubishi_interface::send_cmd()
{
    QByteArray array;
    QString something;
    char buff[32];

    memset(buff,0,sizeof (buff));

    qDebug() << "CMD PORT: " << MitsubishiSocketClient.peerPort();

    MitsubishiSocketClient.write("2");

    if(!MitsubishiSocketClient.waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
    }

    /*if (!MitsubishiSocketClient.waitForReadyRead(Timeout))
    {
        qDebug() << "Timeout message return!";
    }*/

    array = MitsubishiSocketClient.read(MitsubishiSocketClient2.bytesAvailable());

    JOINT testj;

    testj.j1 = 90;
    testj.j2 = 0;
    testj.j3 = 90;
    testj.j4 = 0;
    testj.j5 = 90;
    testj.j6 = 90;
    //testj.j7 = 0;
    //testj.j8 = 0;

    sleep(5);

    memcpy(buff,&testj,sizeof (JOINT));

    qDebug() << sizeof (JOINT);

    MitsubishiSocketClient.write(buff);

    if(!MitsubishiSocketClient.waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
    }


    MitsubishiSocketClient.write(buff);

    if(!MitsubishiSocketClient.waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
    }

    qDebug() << "sent!";

    //  qDebug() << array;

}

bool Mitsubishi_interface::mitsubishi_ready()
{
    return mitsubishi_connected;
}

bool Mitsubishi_interface::mitsubishi_got_new_trajectory()
{
    return got_trajectory;
}

bool Mitsubishi_interface::mitsubishi_ready_to_go()
{
    return ready_to_go;

}

void Mitsubishi_interface::moving()
{
    ready_to_go = false;
    got_trajectory = false;
}

void Mitsubishi_interface::got_trajectiry()
{
    got_trajectory = true;
}

void Mitsubishi_interface::reached_end_point_of_trajectory()
{
    ready_to_go = true;
}


geometry_msgs::Pose Mitsubishi_interface::position_convert_to_ros_pose(QByteArray arr)
{
    geometry_msgs::Pose position_array;

    QString str(arr);
    str.chop(str.size() - str.lastIndexOf("C") - 8);
    qDebug() << str;
    str.remove(";",Qt::CaseInsensitive);
    str.remove("QoK",Qt::CaseInsensitive);

    qDebug() << str;

    position_array.position.x  = str.mid(str.indexOf("X")+1,str.indexOf("Y") - str.indexOf("X")-1).toFloat();
    position_array.position.y  = str.mid(str.indexOf("Y")+1,str.indexOf("Z") - str.indexOf("Y")-1).toFloat();
    position_array.position.z  = str.mid(str.indexOf("Z")+1,str.indexOf("A") - str.indexOf("Z")-1).toFloat();
    position_array.orientation.x = str.mid(str.indexOf("A") + 1, str.indexOf("B") - str.indexOf("A") - 1).toFloat();
    position_array.orientation.y = str.mid(str.indexOf("B") + 1, str.indexOf("C") - str.indexOf("B") - 1).toFloat();
    position_array.orientation.z = str.mid(str.indexOf("C") + 1, 5).toFloat();

    qDebug() << "X" << position_array.position.x;
    qDebug() << "Y" << position_array.position.y;
    qDebug() << "Z" << position_array.position.z;
    qDebug() << "A" << position_array.orientation.x;
    qDebug() << "B" << position_array.orientation.y;
    qDebug() << "C" << position_array.orientation.z;

    return position_array;
}

bool Mitsubishi_interface::read_data()
{
    char buffer[1024];
    memset(buffer,0,sizeof (buffer));
    int n = 0;
    if ((n = read(sockfd, buffer, sizeof(buffer)) > 0))
    {
        qDebug()<<"READ SOME DATA FROM MITSUBISHI";
        memset(buffer,0,sizeof (buffer));
        qDebug() << "Read " << n << "bytes";
        n = 0;
        return true;
    }
    else
    {
        return false;
    }
}

QByteArray Mitsubishi_interface::read_position()
{
    char buffer[1024];
    int n = 0;
    memset(buffer,0,sizeof (buffer));
    //bool f = false;
    qDebug()<<"READ SOME DATA FROM MITSUBISHI";
    while (n <= 0)
    {
        n = read(sockfd2, buffer, sizeof(buffer));
    }
    QByteArray array(buffer);

    return array;
}

void Mitsubishi_interface::PTP_J(geometry_msgs::Pose pos)
{
    char sendc = '2';

    write(sockfd, &sendc,sizeof(sendc));

    QByteArray pos_arr = ros_pose_to_mitsubishi_string(pos);

    write(sockfd, pos_arr.data(),pos_arr.size());


    bool f = false;

    while(f == false)
    {
        f = read_data();
    }

    qDebug() <<"END OF PTP";

}

void Mitsubishi_interface::PTP_C(geometry_msgs::Pose pos)
{
    char sendc = '1';

    write(sockfd, &sendc,sizeof(sendc));

    QByteArray pos_arr = ros_pose_to_mitsubishi_string(pos);

    write(sockfd, pos_arr.data(),pos_arr.size());

    bool f = false;

    while(f == false)
    {
        f = read_data();
    }

    qDebug() <<"END OF PTP";
}

void Mitsubishi_interface::LIN_C(geometry_msgs::Pose pos)
{
    char sendc = '7';

    write(sockfd, &sendc,sizeof(sendc));

    QByteArray pos_arr = ros_pose_to_mitsubishi_string(pos);

    write(sockfd, pos_arr.data(),pos_arr.size());

    bool f = false;

    while(f == false)
    {
        f = read_data();
    }

    qDebug() <<"END OF PTP";
}

QByteArray Mitsubishi_interface::ros_pose_to_mitsubishi_string(geometry_msgs::Pose pos)
{
    QByteArray pos_array;

    QByteArray p1,p2,p3,p4,p5,p6;

    pos_array.append('(').append(p1.setNum(pos.position.x).append(',')).
                          append(p2.setNum(pos.position.y).append(',')).
                          append(p3.setNum(pos.position.z).append(',')).
                          append(p4.setNum(pos.orientation.x).append(',')).
                          append(p5.setNum(pos.orientation.y).append(',')).
                          append(p6.setNum(pos.orientation.z).append(')'));
   return pos_array;
}


void Mitsubishi_interface::test_sock()
{
   /* char buff[sizeof(JOINT)];

    QByteArray ddddata("(90.00,90.00,90.00,90.00,90.00,90.00)");

    char *c = ddddata.data();

    qDebug() << "Size = " << sizeof (JOINT);

    memset(buff,'0',sizeof (buff));

    JOINT testj;

    testj.j1 = 3.14/2;
    testj.j2 = 0;
    testj.j3 = 3.14/2;
    testj.j4 = 0;
    testj.j5 = 3.14/2;
    testj.j6 = 3.14/2;
    testj.j7 = 0;
    testj.j8 = 0;

    sleep(5);

    memcpy(buff,&testj,sizeof (JOINT));
*/
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = inet_addr("192.168.1.20");
   // serv_addr.sin_port = htons(10004);
   // serv_addr.sin_addr.s_addr = inet_addr("192.168.43.86");
   // serv_addr.sin_port = htons(10004);

      serv_addr.sin_addr.s_addr = inet_addr("192.168.1.104");
      serv_addr.sin_port = htons(48569);

      QByteArray ba;
      float ar = 62.2;
      ba.setNum(ar);
    qDebug() << "CREATE Socket";
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        qDebug() << "FUCK";
    }
    qDebug() << "tRY CONNETC";
    if( connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        qDebug() << "FUCK";
    }
    qDebug() << "connected";

    char sendc = '2';

    write(sockfd, &sendc,sizeof(sendc));

    qDebug() << "FUCK YES";

    sleep(5);

    qDebug() << "end sleep";

    char ss[3] = {'2','.','2'};

    write(sockfd, ba.data(),ba.size());

    close(sockfd);
}

geometry_msgs::Pose Mitsubishi_interface::angles_convert_to_ros_pose(QByteArray arr)
{
    geometry_msgs::Pose angles_array;

    QString str(arr);
    str.chop(str.size() - str.lastIndexOf("J6") - 9);
    qDebug() << str;
    str.remove(";",Qt::CaseInsensitive);
    str.remove("QoK",Qt::CaseInsensitive);

    qDebug() << str;
/*
    QString x,y,z,a,b,c;
    x  = str.mid(str.indexOf("J1")+2,str.indexOf("J2") - str.indexOf("J1")-2);
    y  = str.mid(str.indexOf("J2")+2,str.indexOf("J3") - str.indexOf("J2")-2);
    z  = str.mid(str.indexOf("J3")+2,str.indexOf("J4") - str.indexOf("J3")-2);
    a = str.mid(str.indexOf("J4") + 2, str.indexOf("J5") - str.indexOf("J4") - 2);
    b = str.mid(str.indexOf("J5") + 2, str.indexOf("J6") - str.indexOf("J5") - 2);
    c = str.mid(str.indexOf("J6") + 2, 5);

    qDebug() << x;
    qDebug() << y;
    qDebug() << z;
    qDebug() << a;
    qDebug() << b;
    qDebug() << c;
    */

    angles_array.position.x  = str.mid(str.indexOf("J1")+2,str.indexOf("J2") - str.indexOf("J1")-2).toFloat();
    angles_array.position.y  = str.mid(str.indexOf("J2")+2,str.indexOf("J3") - str.indexOf("J2")-2).toFloat();
    angles_array.position.z  = str.mid(str.indexOf("J3")+2,str.indexOf("J4") - str.indexOf("J3")-2).toFloat();
    angles_array.orientation.x = str.mid(str.indexOf("J4") + 2, str.indexOf("J5") - str.indexOf("J4") - 2).toFloat();
    angles_array.orientation.y = str.mid(str.indexOf("J5") + 2, str.indexOf("J6") - str.indexOf("J5") - 2).toFloat();
    angles_array.orientation.z = str.mid(str.indexOf("J6") + 2, 5).toFloat();

    qDebug() << "J1" << angles_array.position.x;
    qDebug() << "J2" << angles_array.position.y;
    qDebug() << "J3" << angles_array.position.z;
    qDebug() << "J4" << angles_array.orientation.x;
    qDebug() << "J5" << angles_array.orientation.y;
    qDebug() << "J6" << angles_array.orientation.z;

    return angles_array;

}

#include "kukavarproxy.h"

KukaVarProxy_interface::KukaVarProxy_interface(QString adress, quint16 port)
{
    kuka_adress = adress;
    kuka_port = port;
}

bool KukaVarProxy_interface::Connect()
{
    KukaSocketClient.connectToHost(kuka_adress, kuka_port);

    if (!KukaSocketClient.waitForConnected(Timeout)) {
        qDebug() << "Timeout connection!";
        return false;
    }
    else
    {
        qDebug() << "Connected to KUKA";
        return true;
    }
 }

void KukaVarProxy_interface::Disconnect()
{
    KukaSocketClient.disconnectFromHost();
    qDebug() << "Disconnected";
}

bool KukaVarProxy_interface::IsConnected()
{
    if (!KukaSocketClient.waitForConnected(Timeout)) {
        qDebug() << "Timeout connection!";
        return false;
    }
    else
    {
        return true;
    }
}

void KukaVarProxy_interface::SetTimout(quint16 timeout)
{
    Timeout = timeout;
}

QByteArray KukaVarProxy_interface::Get_TCP_Postion()
{

    KukaSocketClient.write(formatMsg("$POS_ACT",43981));
    if(!KukaSocketClient.waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
    }

    if (!KukaSocketClient.waitForReadyRead(Timeout))
    {
        qDebug() << "Timeout message return!";
    }
    QByteArray returnMsg = KukaSocketClient.read(KukaSocketClient.bytesAvailable());
    unsigned char ok = returnMsg.right(1).at(0);
    QByteArray value;
    unsigned short idMsg = clearMsg(returnMsg,value);
    //qDebug()<<"Value:"<<value;
    return value;




}

QByteArray KukaVarProxy_interface::Get_Angles()
{
    KukaSocketClient.write(formatMsg("$AXIS_ACT",43981));
    if(!KukaSocketClient.waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
    }

    if (!KukaSocketClient.waitForReadyRead(Timeout)) {
        qDebug() << "Timeout message return!";
    }
    QByteArray returnMsg = KukaSocketClient.read(KukaSocketClient.bytesAvailable());
    unsigned char ok = returnMsg.right(1).at(0);
    QByteArray value;
    unsigned short idMsg = clearMsg(returnMsg,value);
    //qDebug()<<"Value:"<<value;
    return value;
}

geometry_msgs::Pose KukaVarProxy_interface::ROS_Get_TCP_Postion()
{
    return position_convert_to_ros_pose(Get_TCP_Postion());
}

geometry_msgs::Pose KukaVarProxy_interface::ROS_Get_Angles()
{
    return angles_convert_to_ros_pose(Get_Angles());
}

QByteArray KukaVarProxy_interface::formatMsg(QByteArray msg, unsigned short idMsg)
{
    const char READVARIABLE=0;

    QByteArray header, block;
    int blocklen,varNameLen;
    unsigned char hByte, lByte;
    unsigned char hByteMsg,lByteMsg;

    varNameLen=msg.size();
    hByte=(varNameLen & 0xff00) >> 8;
    lByte=(varNameLen & 0x00ff);

    block.append(READVARIABLE).append(hByte).append(lByte).append(msg);
    blocklen=block.size();

    hByte=(blocklen & 0xff00) >> 8;
    lByte=(blocklen & 0x00ff);

    hByteMsg=(idMsg & 0xff00) >> 8;
    lByteMsg=(idMsg & 0x00ff);

    header.append(hByteMsg).append(lByteMsg).append(hByte).append(lByte);
    block.prepend(header);

    return block;
}

QByteArray KukaVarProxy_interface::formatMsg(QByteArray msg, QByteArray value, unsigned short idMsg)
{
    const char WRITEVARIABLE=1;

    QByteArray header, block;
    int blocklen,varNameLen,varValueLen;
    unsigned char hByte, lByte;
    unsigned char hByteMsg,lByteMsg;

    varNameLen=msg.size();
    hByte=(varNameLen & 0xff00) >> 8;
    lByte=(varNameLen & 0x00ff);

    block.append(WRITEVARIABLE).append(hByte).append(lByte).append(msg);

    varValueLen=value.size();
    hByte=(varValueLen & 0xff00) >> 8;
    lByte=(varValueLen & 0x00ff);

    block.append(hByte).append(lByte).append(value);

    blocklen=block.size();

    hByte=(blocklen & 0xff00) >> 8;
    lByte=(blocklen & 0x00ff);

    hByteMsg=(idMsg & 0xff00) >> 8;
    lByteMsg=(idMsg & 0x00ff);

    header.append(hByteMsg).append(lByteMsg).append(hByte).append(lByte);
    block.prepend(header);

    //qDebug()<<"Message send:"<<block.toHex();

    return block;
}

unsigned short KukaVarProxy_interface::clearMsg(QByteArray msg, QByteArray &value)
{
    short lenMsg,func,lenValue;
    unsigned short idReadMsg;

    if(msg.size() > 0){
        idReadMsg=((unsigned char)msg[0])<<8 | ((unsigned char)msg[1]);
        //qDebug() << "Message ID: " << idReadMsg;

        lenMsg=((unsigned char)msg[2])<<8 | ((unsigned char)msg[3]);
        //qDebug() << "Message Length:" << lenMsg;

        func=((int)msg[4]);


        lenValue=((unsigned char)msg[5])<<8 | ((unsigned char)msg[6]);
        //qDebug() << "Value Length:" << lenValue;

        value = msg.mid(7,lenValue);
        return idReadMsg;

    }
    else
    {
        value = QByteArray("");
        return 0;
    }
}



geometry_msgs::Pose KukaVarProxy_interface::angles_convert_to_ros_pose(QByteArray arr)
{
    geometry_msgs::Pose angles_array;

    QString str(arr);
    str.remove("{",Qt::CaseInsensitive);
    str.remove("}",Qt::CaseInsensitive);
    str.remove(",",Qt::CaseInsensitive);
    str.remove("E6AXIS: ",Qt::CaseInsensitive);

    angles_array.position.x = str.mid(str.indexOf("A1") + 2, str.indexOf("A2") - str.indexOf("A1") - 2).toFloat();
    angles_array.position.y = str.mid(str.indexOf("A2") + 2, str.indexOf("A3") - str.indexOf("A2") - 2).toFloat();
    angles_array.position.z = str.mid(str.indexOf("A3") + 2, str.indexOf("A4") - str.indexOf("A3") - 2).toFloat();
    angles_array.orientation.x = str.mid(str.indexOf("A4") + 2, str.indexOf("A5") - str.indexOf("A4") - 2).toFloat();
    angles_array.orientation.y = str.mid(str.indexOf("A5") + 2, str.indexOf("A6") - str.indexOf("A5") - 2).toFloat();
    angles_array.orientation.z = str.mid(str.indexOf("A6") + 2, str.indexOf("E1") - str.indexOf("A6") - 2).toFloat();


    qDebug() << "A1" << angles_array.position.x;
    qDebug() << "A2" << angles_array.position.y;
    qDebug() << "A3" << angles_array.position.z;
    qDebug() << "A4" << angles_array.orientation.x;
    qDebug() << "A5" << angles_array.orientation.y;
    qDebug() << "A6" << angles_array.orientation.z;


    return angles_array;
}

geometry_msgs::Pose KukaVarProxy_interface::position_convert_to_ros_pose(QByteArray arr)
{
    geometry_msgs::Pose position_array;

    QString str(arr);
    str.remove("{",Qt::CaseInsensitive);
    str.remove("}",Qt::CaseInsensitive);
    str.remove(",",Qt::CaseInsensitive);
    str.remove("E6POS: ",Qt::CaseInsensitive);

    position_array.position.x = str.mid(str.indexOf("X") + 2, str.indexOf("Y") - str.indexOf("X") - 2).toFloat();
    position_array.position.y = str.mid(str.indexOf("Y") + 2, str.indexOf("Z") - str.indexOf("Y") - 2).toFloat();
    position_array.position.z = str.mid(str.indexOf("Z") + 2, str.indexOf("A") - str.indexOf("Z") - 2).toFloat();
    position_array.orientation.x = str.mid(str.indexOf("A") + 2, str.indexOf("B") - str.indexOf("A") - 2).toFloat();
    position_array.orientation.y = str.mid(str.indexOf("B") + 2, str.indexOf("C") - str.indexOf("B") - 2).toFloat();
    position_array.orientation.z = str.mid(str.indexOf("C") + 2, str.indexOf("S") - str.indexOf("C") - 2).toFloat();


    qDebug() << "X" << position_array.position.x;
    qDebug() << "Y" << position_array.position.y;
    qDebug() << "Z" << position_array.position.z;
    qDebug() << "A" << position_array.orientation.x;
    qDebug() << "B" << position_array.orientation.y;
    qDebug() << "C" << position_array.orientation.z;


    return position_array;
}

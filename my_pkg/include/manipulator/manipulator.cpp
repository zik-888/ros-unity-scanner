#include "manipulator.h"

geometry_msgs::Pose return_pose(geometry_msgs::Pose pose)
{
    return pose;
}

float ConvertToDegrees(float radian)
{
    return(radian * (180/PI));
}

float ConvertToRadians(float degree)
{
    return(degree * (PI/180));
}

geometry_msgs::Pose set_point(float x, float y, float z, float a, float b, float c)
{
    geometry_msgs::Pose point;
    point.position.x = x;
    point.position.y = y;
    point.position.z = z;
    point.orientation.x = a;
    point.orientation.y = b;
    point.orientation.z = c;
    point.orientation.w = 0;

    return point;
}

void print_pose(geometry_msgs::Pose pose_to_print)
{
    qDebug() << "Position: ";
    qDebug() << "x " << pose_to_print.position.x;
    qDebug() << "y " << pose_to_print.position.y;
    qDebug() << "z " << pose_to_print.position.z;
    qDebug() << "a " << pose_to_print.orientation.x;
    qDebug() << "b " <<pose_to_print.orientation.y;
    qDebug() << "c " <<pose_to_print.orientation.z;

}

/*
Vrep_man::Vrep_man()
{

}

void Vrep_man::man_init(simxInt clientID, manipulator_handles Man_Handles)
{
    ClientID = clientID;
    simxInt res = 0;
    simxInt ret[8] = {0,0,0,0,0,0,0,0};
    for (int i = 0;i < 6;i++)
    {
        ret[i] = simxGetObjectHandle(clientID, Man_Handles.Man_Joint[i],&joint_handles[i],simx_opmode_blocking);
        res = res + ret[i];
    }
    ret[6] = simxGetObjectHandle(clientID,Man_Handles.Man_Target,&target_handle,simx_opmode_blocking);
    ret[7] = simxGetObjectHandle(clientID,Man_Handles.Man_Tip,&tip_handle,simx_opmode_blocking);
    res = res + ret[6] + ret[7];

    sleep(1);

    simxGetStringSignal(clientID,Man_Handles.possig,&signal,&sLength,simx_opmode_buffer);
    sleep(1);
    simxGetStringSignal(clientID,Man_Handles.possig,&signal,&sLength,simx_opmode_streaming);
    simxGetStringSignal(clientID,Man_Handles.possig,&signal,&sLength,simx_opmode_streaming);
    simxGetStringSignal(clientID,Man_Handles.possig,&signal,&sLength,simx_opmode_streaming);
    sleep(1);




    if (res == 0 && simxGetStringSignal(clientID,Man_Handles.possig,&signal,&sLength,simx_opmode_streaming) == simx_return_ok)
    {
        qDebug() << "Got manipulator handles and initialized position signal";
        simxFloat x  = 0;
        simxFloat y  = 0;
        simxFloat z  = 0;
        std::memcpy(&x, signal, sizeof(simxFloat));
        std::memcpy(&y, signal+sizeof (simxFloat), sizeof(simxFloat));
        std::memcpy(&z, signal+2*sizeof (simxFloat), sizeof(simxFloat));
        qDebug() << x;
        qDebug() << y;
        qDebug() << z;

    }
    else
    {
        qDebug() << "Error, can't get manipulator handles or position signal";
            for (int i = 0;i < sizeof (res);i++)
            {
                qDebug() << "Handle " << i << "return code" << ret[i];
            }
    }

    //std::free(signal);

    signame = Man_Handles.signame;

    //ClientID = clientID;
    man_ready = true;

}

geometry_msgs::Pose Vrep_man::get_tip_position()
{
    simxFloat position[3] = {0, 0, 0};
    simxFloat orientation[3] = {0, 0, 0};

    simxGetObjectPosition(ClientID,tip_handle,-1,position,simx_opmode_buffer);
    simxGetObjectOrientation(ClientID,tip_handle,-1,orientation,simx_opmode_buffer);

    geometry_msgs::Pose retpos;

    retpos.position.x = position[0];
    retpos.position.y = position[1];
    retpos.position.z = position[2];
    retpos.orientation.x = orientation[0];
    retpos.orientation.y = orientation[1];
    retpos.orientation.z = orientation[2];

    last_position = retpos;

    return retpos;
}

geometry_msgs::Pose Vrep_man::get_tip_position_from_signal(manipulator_handles Man_Handles)
{

    simxFloat x  = 0;
    simxFloat y  = 0;
    simxFloat z  = 0;
    simxFloat orientation[3] = {0, 0, 0};

    if (simxGetStringSignal(ClientID,Man_Handles.possig,&signal,&sLength,simx_opmode_streaming)==simx_return_ok)
    {
        qDebug() << "Obtained tip position";
        qDebug() << "Size = "<< sLength/sizeof (simxFloat);
        std::memcpy(&x, &signal, sizeof(simxFloat));
        std::memcpy(&y, &signal+sizeof (simxFloat), sizeof(simxFloat));
        std::memcpy(&z, &signal+2*sizeof (simxFloat), sizeof(simxFloat));
        qDebug() << x;
        qDebug() << y;
        qDebug() << z;
    }
    else
    {
       qDebug() << "Can't obtain tip position";
    }

    geometry_msgs::Pose retpos;

    retpos.position.x = x;
    retpos.position.y = y;
    retpos.position.z = z;
    retpos.orientation.x = orientation[0];
    retpos.orientation.y = orientation[1];
    retpos.orientation.z = orientation[2];

    last_position = retpos;

    return retpos;
}

void Vrep_man::set_target_position(geometry_msgs::Pose pose)
{
    simxFloat position[3] = {(simxFloat)pose.position.x, (simxFloat)pose.position.y, (simxFloat)pose.position.z};
    simxFloat orientation[3] = {(simxFloat)pose.orientation.x, (simxFloat)pose.orientation.y, (simxFloat)pose.orientation.z};
    last_target = pose;

    qDebug() << "TARGET SET";

    simxSetObjectPosition(ClientID,target_handle,-1,position,simx_opmode_oneshot_wait);
    simxSetObjectOrientation(ClientID,target_handle,-1,orientation,simx_opmode_oneshot_wait);
}

void Vrep_man::joint_init()
{
    for (int i = 0;i < 6; i++)
    {
        simxGetJointPosition(ClientID,joint_handles[i],&joint_angles[i],simx_opmode_streaming);
        extApi_sleepMs(100);
    }

    simxFloat position[3] = {0, 0, 0};
    simxFloat orientation[3] = {0, 0, 0};

    simxGetObjectPosition(ClientID,tip_handle,-1,position,simx_opmode_streaming);
    simxGetObjectOrientation(ClientID,tip_handle,-1,orientation,simx_opmode_streaming);
}

void Vrep_man::get_joint_angles()
{
    int res[6];
    for (int i = 0;i < 6; i++)
    {
       res[i] = simxGetJointPosition(ClientID,joint_handles[i],&joint_angles[i],simx_opmode_buffer);
       //qDebug() << "RES_ANGLE " << i << " = " << res[i];
    }
}

void Vrep_man::print_angles()
{
    qDebug() << "Robot angles";
    for (int i = 0;i < 6; i++)
    {
        qDebug() << ConvertToDegrees(joint_angles[i]);
    }
}

void Vrep_man::print_position()
{
    qDebug() << "Last_position";
    qDebug() << last_position.position.x;
    qDebug() << last_position.position.y;
    qDebug() << last_position.position.z;
    qDebug() << last_position.orientation.x;
    qDebug() << last_position.orientation.y;
    qDebug() << last_position.orientation.z;
}

bool Vrep_man::target_reached(geometry_msgs::Pose target_pose)
{

    geometry_msgs::Pose current_pose = get_tip_position();


    //qDebug() << "TARGET POSE";
    //print_pose(target_pose);
    //qDebug() << "REAL POSE";
    //print_pose(current_pose);

    simxFloat delta_x = target_pose.position.x - current_pose.position.x;
    simxFloat delta_y = target_pose.position.y - current_pose.position.y;
    simxFloat delta_z = target_pose.position.z - current_pose.position.z;

    simxFloat distance = std::sqrt(delta_x*delta_x + delta_y*delta_y + delta_z*delta_z);

    //qDebug() << "DELTA X " << delta_x;
   // qDebug() << "DELTA Y " << delta_y;
    //qDebug() << "DELTA Z " << delta_z;
    if (distance <= delta)
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool Vrep_man::robot_ready()
{
    return man_ready;
}

bool Vrep_man::robot_ready_to_go()
{
    return ready_to_go;
}

void Vrep_man::PTP(geometry_msgs::Pose position)
{

    //get_tip_position();
    set_target_position(position);
    velocity_calc(position);
    int i = 0;
    qDebug() << "PTP WAS CALLED";
    while (target_reached(position) == false )
    {
        //i++;
        //qDebug() << "Iteration number " << i ;
        //get_joint_angles();
    }

    simxFloat sig  = 0;
    //simxGetFloatSignal(ClientID,signame,&sig,simx_opmode_oneshot_wait);
    //qDebug() << "recvvel" << sig;
    velocity_remove();

}

void Vrep_man::velocity_calc(geometry_msgs::Pose target_position)
{
    geometry_msgs::Pose poss;
    poss = get_tip_position();
    simxFloat distance = sqrt(std::pow((target_position.position.x - poss.position.x),2) + std::pow((target_position.position.y - poss.position.y),2) + std::pow((target_position.position.z - poss.position.z),2));
    //qDebug() << "distance" << distance;
    simxInt res = simxSetFloatSignal(ClientID,signame,velocity/distance,simx_opmode_oneshot_wait);
    //simxInt res = simxSetFloatSignal(clientID,"TestSig",0.3,simx_opmode_oneshot_wait);
    //qDebug() << "calcvel" << velocity/distance;
    //qDebug() << "callres" << res;
}

void Vrep_man::velocity_remove()
{
    simxInt res = simxClearFloatSignal(ClientID,signame,simx_opmode_oneshot_wait);
}

bool Vrep_man::got_trajectiry()
{
    ready_to_go = true;
    return ready_to_go;
}

bool Vrep_man::reached_end_point_of_trajectory()
{
    ready_to_go = false;
    return ready_to_go;
}

void Vrep_man::set_velocity(int value)
{
    if (value <= 100 && value >=0)
    {
        velocity = value*0.01;
        qDebug() << "Current velocity value = " << velocity << "%";
    }
    else
    {
        qDebug() << "Wrong velocity value";
    }
}*/



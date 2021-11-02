#include <iostream>
#include <action_scan.h>

void ScanningAction::transformation(std::vector<double> *point)
{
    Eigen::Matrix4d m;
    Eigen::VectorXd v(4);
    Eigen::VectorXd vf(4);

    m(0,0) = -0.999129;
    m(0,1) = 0.036617;
    m(0,2) = -0.020016;
    m(0,3) = 884.021545;

    m(1,0) = -0.036543;
    m(1,1) = -0.999324;
    m(1,2) = -0.004048;
    m(1,3) = -5.667044;

    m(2,0) = -0.020150;
    m(2,1) = -0.003313;
    m(2,2) = 0.999792;
    m(2,3) = -20.834656;

    m(3,0) = 0.000000;
    m(3,1) = 0.000000;
    m(3,2) = 0.000000;
    m(3,3) = 1.000000;

    v(0) = (*point)[0];
    v(1) = (*point)[1];
    v(2) = (*point)[2];
    v(3) = 1;

    vf = m * v;

    (*point)[0] = vf(0);
    (*point)[1] = vf(1);
    (*point)[2] = vf(2);
}

void ScanningAction::transformation_inv(std::vector<double> *point)
{
    Eigen::Matrix4d m;
    Eigen::VectorXd v(4);
    Eigen::VectorXd vf(4);

    m(0,0) = -0.999129;
    m(0,1) = 0.036617;
    m(0,2) = -0.020016;
    m(0,3) = 884.021545;

    m(1,0) = -0.036543;
    m(1,1) = -0.999324;
    m(1,2) = -0.004048;
    m(1,3) = -5.667044;

    m(2,0) = -0.020150;
    m(2,1) = -0.003313;
    m(2,2) = 0.999792;
    m(2,3) = -20.834656;

    m(3,0) = 0.000000;
    m(3,1) = 0.000000;
    m(3,2) = 0.000000;
    m(3,3) = 1.000000;

    v(0) = (*point)[0];
    v(1) = (*point)[1];
    v(2) = (*point)[2];
    v(3) = 1;

    vf = m.inverse() * v;

    (*point)[0] = vf(0);
    (*point)[1] = vf(1);
    (*point)[2] = vf(2);
}

/*void ScanningAction::executeCB(const ros_scanner_interface::ScanningGoalConstPtr &goal, SPolyModel *scan_model) // struct SPolyModel &scan_model
{
    result_.vertices.clear();
    result_.triangles.clear();

    //  init scanner and connect to scanner
    //
    CScanner scanner;
    CTrajectory trajectory;

    // Initialization scanner;
    if(scanner.init(scanner.status, scanner.api, scanner.apiLib,
                    scanner.system, scanner.sensor, scanner.ipAddress) == true)
    {
        ROS_INFO("INITIALIZATION SUCCESS");
        std::cout << "Status: " << scanner.status << std::endl
                  << "api:    " << scanner.api << std::endl
                  << "system: " << scanner.system << std::endl
                  << "sensor: " << scanner.sensor << std::endl;

        scanner.status = GoSensor_Trigger(scanner.sensor);
    }
    else
    {
        ROS_ERROR("INITIALIZATION FAILED");
    }

    // helper variables
    ros::Rate r(1);
    bool success = true;

    // init services
    ros::ServiceClient client_scan_traj = nh.serviceClient<ros_scanner_interface::TrajectoryService>("HMI_srv");
    ros::ServiceClient client_Scan_Position = nh.serviceClient<ros_scanner_interface::PoseRobotService>("Scan_Pos_srv");

    // publish info to the console for the user
    ROS_INFO("%s: Executing, staring scanning of area (%f, %f), (%f, %f)", action_name_.c_str(), goal->x1, goal->y1, goal->x2, goal->y2);
    ros_scanner_interface::TrajectoryService srv_trajectory;
    srv_trajectory.response.answer = false;

    //rotation
    //float x1_rot = goal->x1*cos(PI) - goal->y1*sin(PI) - 890;
    //float y1_rot = goal->y1*cos(PI) + goal->x1*sin(PI) - 890;

    //float x2_rot = goal->x2*cos(PI) - goal->y2*sin(PI);
    //float y2_rot = goal->y2*cos(PI) + goal->x2*sin(PI);

    float x1_rot = -goal->x1 + 890;
    float y1_rot = -goal->y1;

    float x2_rot = -goal->x2 + 890;
    float y2_rot = -goal->y2;

    std::cout << "FROM HMI:" << std::endl
              << goal->x1 << " ; " << goal->y1 << std::endl
              << goal->x2 << " ; " << goal->y2 << std::endl;
    std::cout << "FROM HMI ROT:" << std::endl
              << x1_rot << " ; " << y1_rot << std::endl
              << x2_rot << " ; " << y2_rot << std::endl;

    srv_trajectory = trajectory.TrajectoryForming(x1_rot, y1_rot, x2_rot, y2_rot);
    std::cout << "Size of srv_trajectory = " << srv_trajectory.request.rpose.size() << std::endl
              << "first point = " << srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-2] << std::endl
              << "last point = " << srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1] << std::endl;

    PositionTIP stopPosition;
    stopPosition.x = srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1].position.x;
    stopPosition.y = srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1].position.y;
    stopPosition.z = srv_trajectory.request.rpose[srv_trajectory.request.rpose.size()-1].position.z;

    feedback_.status = GO_TO_TARGET_POINT;
    as_.publishFeedback(feedback_);
    if(client_scan_traj.call(srv_trajectory))
    {
        ROS_INFO("Success! Trajectory sended.");
        if(srv_trajectory.response.answer == true)
        {
            ROS_WARN("SCANNING");
            *scan_model = scanner.scanning(as_, action_name_, stopPosition, client_Scan_Position, scanner);
            ROS_INFO("Finish action");
        }
        else
        {
            ROS_ERROR("Mitsubish not response");
        }
    }
    else
    {
        ROS_ERROR("Failed send trajectory");
        feedback_.status = ERROR_SENDING_TRAJECTORY;  // Failed trajectory
        as_.publishFeedback(feedback_);

        success = false;
        as_.setAborted(result_, "Failed");
    }
    if(success)
    {
        ROS_WARN("Success. Send Model");
        feedback_.status = SCANNING_SUCCESS;
        as_.publishFeedback(feedback_);

        for(int j = 0; j < (*scan_model).Points.size(); j++)
        {
            result_.ScanningResult_::vertices.push_back({});
            //result_.vertices[j].x = scan_model.Points[j][0];
            //result_.vertices[j].y = scan_model.Points[j][1];
            SPoint mitsubishiPos;
            mitsubishiPos.x = 890 - 6.2;
            mitsubishiPos.y = - 19.4;
            mitsubishiPos.z = - 27.66;

            transformation(&((*scan_model).Points[j]));


            result_.vertices[j].x = (*scan_model).Points[j][0];
            result_.vertices[j].y = (*scan_model).Points[j][1];
            result_.vertices[j].z = (*scan_model).Points[j][2];

            //ROS_INFO("x = %f, y = %f, z = %f", result_.vertices[j].x, result_.vertices[j].y, result_.vertices[j].z);
        }

        for(int k = 0; k < (*scan_model).Faces.size(); k++)
        {
            result_.ScanningResult_::triangles.push_back({});
            result_.triangles[k].vertex_indices[0] = (*scan_model).Faces[k][0];
            result_.triangles[k].vertex_indices[1] = (*scan_model).Faces[k][1];
            result_.triangles[k].vertex_indices[2] = (*scan_model).Faces[k][2];

            //ROS_INFO("triangle [%d; %d; %d]", result_.triangles[k].vertex_indices[0],result_.triangles[k].vertex_indices[1],result_.triangles[k].vertex_indices[2]);
        }

        ROS_INFO("Packing data (Points = %lu, Faces = %lu)", result_.vertices.size(), result_.triangles.size());

        ROS_INFO("%s: Scanning is succeeded", action_name_.c_str());
        // set the action state to succeeded
        as_.setSucceeded(result_);
    }
}; //, ros::NodeHandle &nh)*/

//ScanningAction::~ScanningAction(){};



using System;
using RosSharp.RosBridgeClient.MessageTypes.MyPkg;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Robots.RobotModel;
using RosSharp.RosBridgeClient;
using Sirenix.OdinInspector;
using thelab.mvc;
using UniRx;
using UnityEngine;

public class UnityJointPositionServiceClient : Element<DemonOLPApplication>
{
    private RosConnector _connector;
    private ScanPositionServiceRequest _request = new ScanPositionServiceRequest();

    public RobotModel kinematic;

    public float[] response = new float[6];

    public string serviceName = "Scan_Pos_srv";

    public bool synchronizationData = false;

    private void Start()
    {
        _connector = GetComponent<RosSharp.RosBridgeClient.RosConnector>();

        response.ObserveEveryValueChanged(v => v).Subscribe(v => UpdateJoints()).AddTo(this);
    }

    private void Update()
    {
        if (synchronizationData)
        {
            SendMsg();
        }
                
    }

    [Button]
    public void SendMsg()
    {
        _request = new ScanPositionServiceRequest();

        _connector.RosSocket
            .CallService<ScanPositionServiceRequest, ScanPositionServiceResponse>(serviceName, ServiceHandler, _request);
    }
    
    private void ServiceHandler(ScanPositionServiceResponse message)
    {
        response[0] = (float)message.artur_scan_pose.position.x;
        response[1] = (float)message.artur_scan_pose.position.y;
        response[2] = (float)message.artur_scan_pose.position.z;
        response[3] = (float)message.artur_scan_pose.orientation.x;
        response[4] = (float)message.artur_scan_pose.orientation.y;
        response[5] = (float)message.artur_scan_pose.orientation.z;


        UpdateJoints();

        Debug.Log("IsMessageAccept: " + response);
    }


    [Button]
    public void UpdateJoints()
    {
        kinematic.fk.ActualAngle = response;
    }
        
}

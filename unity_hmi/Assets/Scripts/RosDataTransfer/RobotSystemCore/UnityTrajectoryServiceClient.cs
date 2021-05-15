// using System.Collections;
// using System.Collections.Generic;
// using UnityEngine;
// using RosSharp.RosBridgeClient.MessageTypes.MyPkg;
// using System.Linq;
// using thelab.mvc;
//
// public class UnityTrajectoryServiceClient : Element<DemonOLPApplication>
// {
//     public TrajectoryServiceRequest Request = new TrajectoryServiceRequest();
//
//     public bool response;
//
//     public Vector3[] position;
//     public Quaternion[] rotation;
//
//     //private readonly Quaternion defRotation; //new Quaternion(-154.51f, -84.14f, -63.57f, 0f);
//
//     public string serviceName = "HMI_srv";
//
//     public void SendMsg()
//     {
//         if(app.model.CommandModels.Count != 0)
//         {
//             position = (from c in app.model.CommandModels
//                         from p in c.poses
//                         select p.Position).ToArray();
//
//             // rotation = (from c in app.model.CommandModels
//             //             from p in c.poses
//             //             select new Quaternion(p.Rotation.x,
//             //                                   p.Rotation.y, 
//             //                                   p.Rotation.z, 0)).ToArray();
//         }
//
//         print(position.Count());
//         print(rotation.Count());
//
//         var poses = from p in position
//                     from r in rotation
//                     let rp = RosSharp.TransformExtensions.Unity2Ros(new Vector3(p.x, p.y, p.z))
//                     let rq = /*RosSharp.TransformExtensions.Unity2Ros*/(new Vector3(r.x, r.y, r.z))
//                     select new RosSharp.RosBridgeClient.MessageTypes.Geometry.Pose
//                     {
//                         position = new RosSharp.RosBridgeClient.MessageTypes.Geometry.Point(rp.x, rp.y, rp.z),
//                         orientation = new RosSharp.RosBridgeClient.MessageTypes.Geometry.Quaternion(rq.x, rq.y, rq.z, r.w)
//                     };
//
//         
//         //var poses2 = position.Join(rotation, p => p, r => r, (p, r) =>
//         //                        new RosSharp.RosBridgeClient.MessageTypes.Geometry.Pose 
//         //                        {
//         //                            position = new RosSharp.RosBridgeClient.MessageTypes.Geometry.Point(p.x, p.y, p.z),
//         //                            orientation = new RosSharp.RosBridgeClient.MessageTypes.Geometry.Quaternion(r.x, r.y, r.z, 0)
//         //                        });
//
//
//         Request = new TrajectoryServiceRequest(1, poses.ToArray());
//
//         GetComponent<RosSharp.RosBridgeClient.RosConnector>()
//             .RosSocket.CallService<TrajectoryServiceRequest, TrajectoryServiceResponse>(serviceName, ServiceHandler, Request);
//     }
//
//     //Handler
//     private void ServiceHandler(TrajectoryServiceResponse message)
//     {
//         response = message.answer;
//         Debug.Log("IsMessageAccept: " + message.answer);
//     }
// }
//


using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using RosSharp.RosBridgeClient.MessageTypes.MyPkg;
using System.Linq;
using RosSharp.RosBridgeClient;
using thelab.mvc;
using RQuaternion = RosSharp.RosBridgeClient.MessageTypes.Geometry.Quaternion;
using RPoint = RosSharp.RosBridgeClient.MessageTypes.Geometry.Point;
using RPose = RosSharp.RosBridgeClient.MessageTypes.Geometry.Pose;

/// <summary>
/// Требуется дописать правильные ориентации.
/// </summary>
public class UnityTrajectoryServiceClient : Element<DemonOLPApplication>
{
    public TrajectoryServiceRequest Request = new TrajectoryServiceRequest();

    public bool response;

    public Vector3[] position;
    public Quaternion[] rotation;

    public string serviceName = "HMI_srv";
    
    private RosConnector _rosConnector;
    private readonly Vector3 _defRotation = new Vector3(-119.53f, 0.14f, 1.96f);

    private void Start()
    {
        _rosConnector = GetComponent<RosConnector>();
    }

    public void SendMsg()
    {
        if(app.model.CommandModels.Count != 0)
        {
            position = (from c in app.model.CommandModels
                        from p in c.poses
                        select p.Position).ToArray();
        }

        var poses = from p in position
            let rp = RosSharp.TransformExtensions.Unity2Ros(new Vector3(p.x, p.y, p.z))
            select new RPose
            {
                position = new RPoint(rp.x, rp.y, rp.z),
                orientation = new RQuaternion(_defRotation.x, _defRotation.y, _defRotation.z, 0f)
            };
        
        Request = new TrajectoryServiceRequest(1, poses.ToArray());

        _rosConnector.RosSocket.CallService<TrajectoryServiceRequest, TrajectoryServiceResponse>(serviceName, ServiceHandler, Request);
    }

    //Handler
    private void ServiceHandler(TrajectoryServiceResponse message)
    {
        response = message.answer;
        print("IsMessageAccept: " + message.answer);
    }
}


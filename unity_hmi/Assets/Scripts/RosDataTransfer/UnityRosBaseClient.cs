using System;
using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.Actionlib;
using thelab.mvc;
using UnityEngine;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;
using Sirenix.OdinInspector;

public class UnityRosBaseClient : Element<DemonOLPApplication>
{
    [Serializable]
    protected enum CommunicationPattern{ Topic, Service, Action }

    protected RosConnector RosConnector;
    protected CommunicationPattern TypeCommunication;
    public string nodeName;
    //
    // // Start is called before the first frame update
    // protected void Start()
    // {
    //     RosConnector = GetComponent<RosConnector>();
    // }

}

public class UnityRosActionClient : UnityRosBaseClient
{
    public IActionClient actionClient;
    
    [ShowInInspector]
    public string Status => actionClient == null ? string.Empty : actionClient.Status;
    [ShowInInspector]
    public string Feedback => actionClient == null ? string.Empty :actionClient.Feedback;
    [ShowInInspector]
    public string Result => actionClient == null ? string.Empty :actionClient.Result;

    // protected override void Start()
    // {
    //     
    // }

}


public interface IActionClient
{
    Message Order { set; get; }
    
    string Status { get; }

    string Feedback { get; }

    string Result { get; }
}


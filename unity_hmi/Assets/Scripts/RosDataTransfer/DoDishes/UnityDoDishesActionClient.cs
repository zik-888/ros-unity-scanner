using RosSharp.RosBridgeClient;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(RosConnector))]
public class UnityDoDishesActionClient : MonoBehaviour
{
    private RosConnector rosConnector;
    public DoDishesActionClient doDishesActionClient;

    public string actionName;
    public uint order = 20;
    public string status = "";
    public string feedback = "";
    public string result = "";

    private void Start()
    {
        rosConnector = GetComponent<RosConnector>();
        doDishesActionClient = new DoDishesActionClient(actionName, rosConnector.RosSocket);
        doDishesActionClient.Initialize();
    }

    private void Update()
    {
        status = doDishesActionClient.GetStatusString();
        feedback = doDishesActionClient.GetFeedbackString();
        result = doDishesActionClient.GetResultString();
    }


    public void RegisterGoal()
    {
        doDishesActionClient.order = order;
    }

}
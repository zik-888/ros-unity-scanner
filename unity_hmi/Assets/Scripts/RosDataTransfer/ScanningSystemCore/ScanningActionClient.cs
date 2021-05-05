using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Timers;
using System.Linq;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using RosSharp.RosBridgeClient.MessageTypes.Shape;
using Sirenix.Serialization;
using UniRx;



[Serializable]
public class ScanningActionClient : ActionClient<ScanningAction, ScanningActionGoal, ScanningActionResult,
                                                 ScanningActionFeedback, ScanningGoal, ScanningResult, ScanningFeedback>
{
    public ScanningGoal Order = new ScanningGoal();
    public ReactiveProperty<ScanningResult> reactOrder { set; get; } = new ReactiveProperty<ScanningResult>();
    
    private string status = "";
    private string feedback = "";
    private string result = "";
    
    public string Status
    {
        get => status;
        set => status = value;
    }

    public string Feedback
    {
        get => feedback;
        set => feedback = value;
    }

    public string Result
    {
        get => result;
        set => result = value;
    }

    public ScanningActionClient(string actionName, RosSocket rosSocket)
    {
        this.actionName = actionName;
        this.rosSocket = rosSocket;

        action = new ScanningAction();
        goalStatus = new GoalStatus();
    }

    protected override ScanningActionGoal GetActionGoal()
    {
        action.action_goal.goal = Order;
        return action.action_goal;
    }

    protected override void OnStatusUpdated()
    {
        if (goalStatus != null)
            Status = ((ActionStatus)(goalStatus.status)).ToString();
    }

    protected override void OnFeedbackReceived()
    {
        if (action != null)
            Feedback = string.Join(",", (StatusOfSystem)action.action_feedback.feedback.status);
    }

    protected override void OnResultReceived()
    {
        if (action != null)
            result = $"Result time: {DateTime.Now}, Length: {action.action_result.result.vertices.Length}";

        reactOrder.Value = action.action_result.result;
    }
}

using System;
using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using UniRx;
using UnityEngine;

[Serializable]
public class StatusActionClient : ActionClient<StatusAction, StatusActionGoal, StatusActionResult,
    StatusActionFeedback, StatusGoal, StatusResult, StatusFeedback>
{
    public StatusGoal Order = new StatusGoal();
    public ReactiveProperty<StatusResult> reactOrder { set; get; } = new ReactiveProperty<StatusResult>();
    
    public string Status { get; set; } = "";

    public string Feedback { get; set; } = "";

    public string Result{ get; set; } = "";
    
    public StatusActionClient(string actionName, RosSocket rosSocket)
    {
        this.actionName = actionName;
        this.rosSocket = rosSocket;

        action = new StatusAction();
        goalStatus = new GoalStatus();
    }

    protected override StatusActionGoal GetActionGoal()
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
            Feedback = string.Join(",", action.action_feedback.feedback.status);
    }

    protected override void OnResultReceived()
    {
        if (action != null)
            Result = $"Result time: {DateTime.Now}, Data: {action.action_result.result.status}";

        reactOrder.Value = action.action_result.result;
    }
}

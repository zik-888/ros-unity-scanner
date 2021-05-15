using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Windows.Forms;
using UniRx;
using UnityEngine;
using Message = RosSharp.RosBridgeClient.Message;


public enum ModeOfProcessing
{
    ALIGN,
    DEFORM,
    ALIGN_AND_DEFORM
    // ... e.t.
};

[Serializable]
public class ProcessingActionClient : ActionClient<ProcessingAction, ProcessingActionGoal, ProcessingActionResult,
                                                   ProcessingActionFeedback, ProcessingGoal, ProcessingResult, ProcessingFeedback>, IActionClient
{
    public Message Order { set; get; } = new ProcessingGoal();

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

    public Action<ProcessingResult> ActionResult { set; private get; }

    public ProcessingActionClient(string actionName, RosSocket rosSocket)
    {
        this.actionName = actionName;
        this.rosSocket = rosSocket;

        action = new ProcessingAction();
        action.action_goal = new ProcessingActionGoal();
        goalStatus = new GoalStatus();
    }

    protected override ProcessingActionGoal GetActionGoal()
    {
        action.action_goal.goal = (ProcessingGoal) Order;
        return action.action_goal;
    }

    protected override void OnFeedbackReceived()
    {
        if (action != null)
            Feedback = string.Join(",", (StatusOfSystem)action.action_feedback.feedback.status);
    }

    protected override void OnResultReceived()
    {
        if (action != null)
            Result = $"Result time: {System.DateTime.Now}, " +
                     $"vertices_CAD_Length: {action.action_result.result.vertices_CAD.Length}";

        ActionResult(action.action_result.result);
    }

    protected override void OnStatusUpdated()
    {
        if (goalStatus != null)
            Status = ((ActionStatus)(goalStatus.status)).ToString();
    }
    
}

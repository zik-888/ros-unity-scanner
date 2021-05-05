using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using RosSharp.RosBridgeClient.MessageTypes.RosBookSamples;

public class DoDishesActionClient : ActionClient<DoDishesAction, DoDishesActionGoal, DoDishesActionResult, DoDishesActionFeedback,
                                                                 DoDishesGoal, DoDishesResult, DoDishesFeedback>
{
    public uint order;
    public string status = "";
    public string feedback = "";
    public string result = "";

    public DoDishesActionClient(string actionName, RosSocket rosSocket)
    {
        this.actionName = actionName;
        this.rosSocket = rosSocket;

        action = new DoDishesAction();
        goalStatus = new GoalStatus();
    }

    protected override DoDishesActionGoal GetActionGoal()
    {
        action.action_goal.goal.dishes = order;
        return action.action_goal;
    }

    protected override void OnStatusUpdated()
    {
        // Not implemented for this particular application
    }

    protected override void OnFeedbackReceived()
    {
        // Not implemented for this particular application since get string directly returns stored feedback
    }

    protected override void OnResultReceived()
    {
        // Not implemented for this particular application since get string directly returns stored result
    }

    public string GetStatusString()
    {
        if (goalStatus != null)
        {
            return ((ActionStatus)(goalStatus.status)).ToString();
        }
        return "";
    }

    public string GetFeedbackString()
    {
        if (action != null)
            return string.Join(",", action.action_feedback.feedback.dishes_cleaned);
        return "";
    }

    public string GetResultString()
    {
        if (action != null)
            return string.Join(",", action.action_result.result.total_dishes_cleaned);
        return "";
    }
}

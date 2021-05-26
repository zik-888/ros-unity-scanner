using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.RosBookSamples;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class ActionClientDoDishes : ActionClient<DoDishesAction, DoDishesActionGoal, 
                                              DoDishesActionResult, DoDishesActionFeedback,
                                              DoDishesGoal, DoDishesResult, DoDishesFeedback>
{
    public uint dishesOrder;
    public string status = "";
    public string feedback = "";
    public string result = "";

    public ActionClientDoDishes(string actionName, RosSocket rosSocket)
    {
        this.actionName = actionName;
        this.rosSocket = rosSocket;

        action = new DoDishesAction();
        goalStatus = new GoalStatus();
    }


    protected override DoDishesActionGoal GetActionGoal()
    {
        action.action_goal.goal.dishes = dishesOrder;
        return action.action_goal;
    }

    protected override void OnFeedbackReceived()
    {
        Debug.Log("OnFeedbackReceived");
        Debug.Log($"Feedback: {GetFeedbackString()}");
    }

    protected override void OnResultReceived()
    {
        Debug.Log("OnResultReceived");
        Debug.Log($"Result: {GetResultString()}");


        Debug.Log($"Before CG: { GetStatusString()}");
        CancelGoal();
        Debug.Log($"After CG: { GetStatusString()}");
    }

    protected override void OnStatusUpdated()
    {
        //Debug.Log("OnStatusUpdated");
    }

    public string GetStatusString()
    {
        if(goalStatus != null)
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



    //public string actionName = "do_dishes";

    //protected static RosSocket rosSocket;
    //protected static FibonacciActionClient fibonacciActionClient;

    //// Start is called before the first frame update
    //void Start()
    //{
    //    rosSocket = GetComponent<RosConnector>().RosSocket;
    //    fibonacciActionClient = new FibonacciActionClient(actionName, rosSocket);
    //    //fibonacciActionClient.action.
    //    fibonacciActionClient.Initialize();
    //}

    ////Handler
    //private static void ServiceHandler(AddTwoIntsResponse message)
    //{
    //    Debug.Log("Sum: " + message.sum);
    //}

}

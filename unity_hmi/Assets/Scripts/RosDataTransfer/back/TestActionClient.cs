using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.Actionlib;
using RosSharp.RosBridgeClient.MessageTypes.Actionlib;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestActionClient : MonoBehaviour
{
    public string actionName = "do_dishes";

    protected static RosSocket rosSocket;
    public static ActionClientDoDishes actionClient;


    // Start is called before the first frame update
    void Start()
    {
        rosSocket = GetComponent<RosConnector>().RosSocket;
        actionClient = new ActionClientDoDishes(actionName, rosSocket);
        actionClient.Initialize();

        actionClient.dishesOrder = 10;
        actionClient.SendGoal();

        

        //StartCoroutine(TestCoroutine());
    }

    //private void Update()
    //{
    //    if(actionClient.goalStatus.status != GoalStatus.SUCCEEDED)
    //    {
    //        Debug.Log($"Feedback: {actionClient.GetFeedbackString()}");
    //        Debug.Log($"Status: {actionClient.GetStatusString()}");
    //    }
    //    else
    //    {
            
    //        Debug.Log($"Result: {actionClient.GetResultString()}");
    //    }
        
    //}

    IEnumerator TestCoroutine()
    {
        while (true)
        {
            yield return new WaitForSeconds(1f);
            Debug.Log($"Feedback: {actionClient.GetFeedbackString()}");
            //Debug.Log($"Result: {actionClient.GetResultString()}");
            //Debug.Log($"Status: {actionClient.GetStatusString()}");
        }
    }

    //private void OnDestroy()
    //{
    //    actionClient.CancelGoal();
    //    actionClient.Terminate();
    //}

    private void OnApplicationQuit()
    {
        print(actionClient.GetStatusString());

        if((ActionStatus)actionClient.goalStatus.status == ActionStatus.SUCCEEDED)
        {
            actionClient.Terminate();
        }
        else
        {
            actionClient.CancelGoal();
            actionClient.Terminate();
        }

        

        //(ActionStatus)actionClient.goalStatus.status;
        
    }
}

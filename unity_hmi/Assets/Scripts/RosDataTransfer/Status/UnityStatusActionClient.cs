using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using Sirenix.OdinInspector;
using thelab.mvc;
using UniRx;
using UnityEngine;

public class UnityStatusActionClient : Element<DemonOLPApplication>
{
    private RosConnector _rosConnector;
    private StatusActionClient _statusActionClient;
    public string actionName;
    public NodeType nodeType = NodeType.Kuka;
    public float timeoutSec = 1f;

    private Coroutine timeoutTimer = null;
    
    
    [ShowInInspector] 
    public StatusGoal goal => _statusActionClient == null ? StatusGoal.DefGoal : _statusActionClient.Order;
    [ShowInInspector]
    public string Status => _statusActionClient == null ? string.Empty : _statusActionClient.Status;
    [ShowInInspector]
    public string Feedback => _statusActionClient == null ? string.Empty :_statusActionClient.Feedback;
    [ShowInInspector]
    public string Result => _statusActionClient == null ? string.Empty :_statusActionClient.Result;
    
    // Start is called before the first frame update
    void Start()
    {
        _rosConnector = GetComponent<RosConnector>();
        _statusActionClient = new StatusActionClient(actionName, _rosConnector.RosSocket);
        _statusActionClient.Initialize();

        _statusActionClient.reactOrder
            .ObserveEveryValueChanged(x => x.Value)
            .Subscribe(xs => UpdateModel(xs))
            .AddTo(this);
    }

    [Button]
    public void UpdateModel(StatusResult result)
    {
        if(result == null)
            return;

        if(timeoutTimer != null)
            StopCoroutine(timeoutTimer);
        
        timeoutTimer = StartCoroutine(TimeoutTimer());
    }

    IEnumerator TimeoutTimer()
    {
        app.model.rosNodeStatus[(int)nodeType].Value = true;
        yield return new WaitForSeconds(timeoutSec);
        app.model.rosNodeStatus[(int)nodeType].Value = false;
    }
}

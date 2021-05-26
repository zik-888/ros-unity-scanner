using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnityScanningStatusSubscriber : UnitySubscriber<Status>
{
    public string errorcode;
    public long status;

    protected override void ReceiveMessage(Status message)
    {
        errorcode = message.errorcode;
        status = message.status;
    }


}

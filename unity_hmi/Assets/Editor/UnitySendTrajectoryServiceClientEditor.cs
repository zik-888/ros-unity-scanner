using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(UnityTrajectoryServiceClient))]
public class UnitySendTrajectoryServiceClientEditor : Editor
{
    public override void OnInspectorGUI()
    {
        DrawDefaultInspector();

        if (GUILayout.Button("Send Msg"))
        {
            ((UnityTrajectoryServiceClient)target).SendMsg();
        }
    }
}

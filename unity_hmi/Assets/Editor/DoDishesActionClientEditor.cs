using UnityEditor;
using UnityEngine;

namespace RosSharp.RosBridgeClient.Actionlib
{
    [CustomEditor(typeof(UnityDoDishesActionClient))]
    public class DoDishesActionClientEditor : Editor
    {
        public override void OnInspectorGUI()
        {
            DrawDefaultInspector();

            if (GUILayout.Button("Send Goal"))
            {
                ((UnityDoDishesActionClient)target).RegisterGoal();
                ((UnityDoDishesActionClient)target).doDishesActionClient.SendGoal();
            }

            if (GUILayout.Button("Cancel Goal"))
            {
                ((UnityDoDishesActionClient)target).doDishesActionClient.CancelGoal();
            }
        }
    }
}
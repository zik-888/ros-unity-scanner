using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KinematicChainFinder : MonoBehaviour
{
    public RobotJoint[] joint;
    public RobotTool tool;
    public int axesSize = 500;

    public Matrix4x4[] mtxs;
    
    
    // Start is called before the first frame update
    void Awake()
    {
        joint = GetComponentsInChildren<RobotJoint>();
        tool = GetComponentInChildren<RobotTool>();

        mtxs = new Matrix4x4[joint.Length];

        for (int i = 0; i < joint.Length; i++)
        {
            mtxs[i] = new Matrix4x4();
            
            mtxs[i].SetTRS(joint[i].transform.position, joint[i].transform.localRotation, Vector3.one);
        }
        
        
    }
    
    // Update is called once per frame
    void Update()
    {
        foreach (var mtx in mtxs)
        {
            //DrawAxes(mtx);
        }
    }
    
    private void DrawAxes(Matrix4x4 mtx)
    {
        Debug.DrawRay(mtx.GetColumn(3), ((Vector3)mtx.GetColumn(0)).normalized * axesSize , Color.red);
        Debug.DrawRay(mtx.GetColumn(3), ((Vector3)mtx.GetColumn(1)).normalized * axesSize , Color.green);
        Debug.DrawRay(mtx.GetColumn(3), ((Vector3)mtx.GetColumn(2)).normalized * axesSize , Color.blue);
    }

    
}

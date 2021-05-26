using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using UnityEngine;

public class InverseKinematic : MonoBehaviour
{
    public Transform point;
    public ForwardKinematic fk;

    protected InverseKinematic6DofRobotSolver solver;
    
    // Start is called before the first frame update
    void Start()
    {
        solver = new InverseKinematic6DofRobotSolver(fk);
    }

    // Update is called once per frame
    [Button]
    void Set()
    {
        var angles = solver.SetSolve(new Pose(point.position, point.rotation));
        
        for (int i = 0; angles.Length > i; i++)
        {
            fk.joints[i].Angle = angles[i];
        }
    }
}

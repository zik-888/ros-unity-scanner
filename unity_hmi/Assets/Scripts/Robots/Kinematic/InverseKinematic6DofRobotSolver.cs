using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using thelab.mvc;
using UnityEngine;

public class InverseKinematic6DofRobotSolver
{
    private ForwardKinematic fk;
    private CCSInverseKinematicSolver solver;

    public InverseKinematic6DofRobotSolver(ForwardKinematic fk)
    {
        this.fk = fk;
        solver = new CCSInverseKinematicSolver(fk.joints.Take(5).ToArray());
    }


    [Button]
    public float[] SetSolve(Pose targetPose)
    {
        var targetPos = targetPose.position;
        var targetRot = targetPose.rotation.eulerAngles;
        var prevPos = GetPrevJoint(targetPos, targetRot);
        
        var angles = solver.SetIK(prevPos);
        
        var link4Pose = ForwardKinematics(angles, 3);
        var link5Pose = ForwardKinematics(angles, 4);
        
        #region Debug
        
        Debug.DrawRay(link4Pose.GetColumn(3), link4Pose.GetColumn(0), Color.green, 1000f);
        Debug.DrawRay(link4Pose.GetColumn(3), link4Pose.GetColumn(1), Color.red, 1000f);
        Debug.DrawRay(link4Pose.GetColumn(3), link4Pose.GetColumn(2), Color.blue, 1000f);
        
        Debug.DrawRay(link5Pose.GetColumn(3), link5Pose.GetColumn(0), Color.green, 1000f);
        Debug.DrawRay(link5Pose.GetColumn(3), link5Pose.GetColumn(1), Color.red, 1000f);
        Debug.DrawRay(link5Pose.GetColumn(3), link5Pose.GetColumn(2), Color.blue, 1000f);        
        
        #endregion
        
        angles[4] = Vector3.SignedAngle(targetPos - prevPos, link4Pose.GetColumn(2), -link5Pose.GetColumn(0));
        
        Plane p1 = new Plane(link4Pose.GetColumn(3), prevPos, targetPos);
        Plane p2 = new Plane(link4Pose.GetColumn(3), link5Pose.GetColumn(3), link5Pose.GetColumn(3) + link5Pose.GetColumn(1));
        
        angles[3] = Vector3.SignedAngle(p1.normal, p2.normal, -link4Pose.GetColumn(2));

        return angles;
    }
    
    public Matrix4x4 ForwardKinematics(float[] angles, int num)
    {

        Vector3 prevPoint = fk.joints[0].transform.position;
        Quaternion rotation = Quaternion.identity;
        
        for (int i = 1; i < num + 1; i++) {
            // Rotates around a new axis
            rotation *= Quaternion.AngleAxis(angles[i - 1], fk.joints[i - 1].moveAxis);
            Vector3 nextPoint = prevPoint + rotation * fk.joints[i].positionOffset;

            prevPoint = nextPoint;
        }
        
        Matrix4x4 mtx = new Matrix4x4();
        mtx.SetTRS(prevPoint, rotation, Vector3.one);
        
        return mtx;
    }
    
    protected Vector3 GetPrevJoint(Vector3 targetPos, Vector3 targetRot)
    {
        Matrix4x4 mtx = new Matrix4x4();
        mtx.SetTRS(targetPos, Quaternion.Euler(targetRot), Vector3.one);

        return targetPos - (Vector3)mtx.GetColumn(2) * fk.joints[fk.joints.Length - 1].positionOffset.magnitude;
    }

}

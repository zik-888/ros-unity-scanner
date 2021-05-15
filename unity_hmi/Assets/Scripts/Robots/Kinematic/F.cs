using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.Serialization;

public class F : MonoBehaviour
{
    [SerializeField] private Transform point = default;
    
    [SerializeField] private ForwardKinematic fk;
    [SerializeField] private Vector3 targetPos;
    [SerializeField] private Vector3 targetRot = default;
    
    public RobotJoint[] Joints;
    public float[] Angles;

    [Header("CCS Param")]
    public float SamplingDistance = 5f;
    public float LearningRate = 100f;
    public float DistanceThreshold = 0.01f;


    private void Start()
    {
        //Joints = fk.JointMoves;
        
        float[] angles = new float[Joints.Length];
        
        for (int i = 0; i < Joints.Length; i++)
            angles[i] = Joints[i].Angle; 
        
        Angles = angles;
    }

    // private void Update()
    // {
    //     SetIK();
    // }

    [Button]
    public void SetIK()
    {
        targetPos = point.position;

        InverseKinematics(targetPos, targetRot, ref Angles);
        
        for (int i = 0; Joints.Length > i; i++)
        {
            Joints[i].Angle = Angles[i];
        }

        // Vector3 v4 = ForwardKinematics(Angles, 4);
        //
        // Plane p1 = new Plane(v4, v4 + Vector3.up, targetPos);
        // // Plane p2 = new Plane(v4, targetPos, );
        //
        // fk.joints[4] = Mathf.Atan2(- Mathf.Sin() * Mathf.Sin(), );
    }

    
    public Matrix4x4 ForwardKinematics(float[] angles, int num) {
        
        Vector3 prevPoint = Joints[0].transform.position;
        Quaternion rotation = Quaternion.identity;
        
        for (int i = 1; i < num + 1; i++) {
            // Rotates around a new axis
            rotation *= Quaternion.AngleAxis(angles[i - 1], Joints[i - 1].moveAxis);
            Vector3 nextPoint = prevPoint + rotation * Joints[i].positionOffset;

            prevPoint = nextPoint;
        }
        
        Matrix4x4 mtx = new Matrix4x4();
        mtx.SetTRS(prevPoint, rotation, Vector3.one);
        
        return mtx;
    }
    
    // public Vector3 ForwardKinematics(float[] angles, int num) {
    //     
    //     Vector3 prevPoint = Joints[0].transform.position;
    //     Quaternion rotation = Quaternion.identity;
    //     
    //     for (int i = 1; i < num + 1; i++) {
    //         // Rotates around a new axis
    //         rotation *= Quaternion.AngleAxis(angles[i - 1], Joints[i - 1].moveAxis);
    //         Vector3 nextPoint = prevPoint + rotation * Joints[i].positionOffset;
    //
    //         prevPoint = nextPoint;
    //     }
    //     return prevPoint;
    // }
    //
    public Vector3 ForwardKinematics(float[] angles) {
        
        
        Vector3 prevPoint = Joints[0].transform.position;
        Quaternion rotation = Quaternion.identity;
        
        for (int i = 1; i < Joints.Length; i++) {
            // Rotates around a new axis
            rotation *= Quaternion.AngleAxis(angles[i - 1], Joints[i - 1].moveAxis);
            Vector3 nextPoint = prevPoint + rotation * Joints[i].positionOffset;

            prevPoint = nextPoint;
        }
        return prevPoint;
    }

    public float DistanceFromTarget(Vector3 target, float[] angles) {
        Vector3 point = ForwardKinematics(angles);
        return Vector3.Distance(point, target);
    }

    public float PartialGradient(Vector3 target, float[] angles, int i) {
        // Saves the angle,
        // it will be restored later
        float angle = angles[i];

        // Gradient : [F(x+SamplingDistance) - F(x)] / h
        float f_x = DistanceFromTarget(target, angles);

        angles[i] += SamplingDistance;
        float f_x_plus_d = DistanceFromTarget(target, angles);

        float gradient = (f_x_plus_d - f_x) / SamplingDistance;

        // Restores
        angles[i] = angle;

        return gradient;
    }

    public void InverseKinematics(Vector3 targetPos, Vector3 targetRot, float[] angles) 
    {
        if (DistanceFromTarget(targetPos, angles) < DistanceThreshold)
            return;

        for (int i = angles.Length - 1; i >= 0; i--) {
            // Gradient descent
            // Update : Solution -= LearningRate * Gradient
            float gradient = PartialGradient(targetPos, angles, i);
            angles[i] -= LearningRate * gradient;

            // Early termination
            if (DistanceFromTarget(targetPos, angles) < DistanceThreshold)
                return;
            
            Joints[i].Angle = angles[i];
        }
    }
    
    public void InverseKinematics(Vector3 targetPos, Vector3 targetRot, ref float[] angles) 
    {
        // if (DistanceFromTarget(targetPos, angles) < DistanceThreshold)
        //     return;
        
        var prevPos = GetPrevJoint(targetPos, targetRot);

        int g = 0;

        while (DistanceFromTarget(prevPos, angles) >= DistanceThreshold)
        {
            print(g++);
            
            for (int i = angles.Length - 1; i >= 0; i--) {
                // Gradient descent
                // Update : Solution -= LearningRate * Gradient
                float gradient = PartialGradient(prevPos, angles, i);
                angles[i] -= LearningRate * gradient;

                // Early termination
                if (DistanceFromTarget(prevPos, angles) < DistanceThreshold)
                    break;
            }
            
            if(g > 500)
                break;
        }
        
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
        
        
        
    }
    
    protected Vector3 GetPrevJoint(Vector3 targetPos, Vector3 targetRot)
    {
        Matrix4x4 mtx = new Matrix4x4();
        mtx.SetTRS(targetPos, Quaternion.Euler(targetRot), Vector3.one);

        return targetPos - (Vector3)mtx.GetColumn(2) * fk.joints[fk.joints.Length - 1].positionOffset.magnitude;
    }
}

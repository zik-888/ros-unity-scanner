using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.Serialization;

[Serializable]
public class CCSInverseKinematicSolver
{
    protected RobotJoint[] Joints;

    [Header("CCS Param")]
    public float SamplingDistance = 0.1f;
    public float LearningRate = 100f;
    public float DistanceThreshold = 0.01f;
    public int ItertionCountThreshold = 500;


    public CCSInverseKinematicSolver(RobotJoint[] Joints)
    {
        this.Joints = Joints;
    }
    

    public float[] SetIK(Vector3 targetPos)
    {
        var angles  = new float[Joints.Length];
        
        for (int i = 0; i < Joints.Length; i++)
            angles[i] = Joints[i].Angle; 
        
        InverseKinematics(targetPos, ref angles);
        
        for (int i = 0; Joints.Length > i; i++)
        {
            Joints[i].Angle = angles[i];
        }

        return angles;
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
    
    public void InverseKinematics(Vector3 targetPos, ref float[] angles) 
    {
        for (int g = 0; DistanceFromTarget(targetPos, angles) >= DistanceThreshold && g < ItertionCountThreshold; g++)
        {
            for (int i = angles.Length - 1; i >= 0; i--) {
                // Gradient descent
                // Update : Solution -= LearningRate * Gradient
                float gradient = PartialGradient(targetPos, angles, i);
                angles[i] -= LearningRate * gradient;

                // Early termination
                if (DistanceFromTarget(targetPos, angles) < DistanceThreshold)
                    break;
            }
        }
    }
}

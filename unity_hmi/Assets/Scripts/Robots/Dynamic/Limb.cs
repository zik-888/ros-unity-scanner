using System;
using System.Collections;
using System.Collections.Generic;
using RosSharp;
using UnityEngine;

[Serializable]
public class LimbData
{
    public int id;
    public RobotJoint robotJoint;
    public Matrix3x3 Inertia = new Matrix3x3(new float[][] {new float[]{1e-3f, 0, 0}, new float[]{0, 1e-3f, 0}, new float[] {0, 0, 1e-3f}}); // I
    public float Mass; // m
    public Vector3 CenterOfMass; // c
}

public class Limb : MonoBehaviour
{
    [SerializeField]
    protected Limb prevLimb = null;
    [SerializeField]
    protected Limb nextLimb = null;
    public ForwardKinematic fk;

    public RobotJoint GetJoint => fk.joints[defData.id];
    
    
    // ---- Def

    public LimbData defData = new LimbData();
    
    // --------
    
    // ---- Angular Data

    public Vector3 W = Vector3.zero;
    public Vector3 dW = Vector3.zero;
    
    // -----------------
    
    // ---- Linear Data

    public Vector3 dV = Vector3.zero;
    public Vector3 dVc = Vector3.zero;

    // ----------------
    
    // ---- Forces Data

    public Vector3 F = Vector3.zero;
    public Vector3 N = Vector3.zero;
    
    // ---------------
    
    // ---- Output forces Data

    public Vector3 f = Vector3.zero;
    public Vector3 n = Vector3.zero;
    public float t = 0f;
    
    // -----------------------

    
    public void RootToFinal(float q, float dq, float ddq)
    {
        if (prevLimb != null)
        {
            W = fk.GetRotateMatrix(defData.id, q) * prevLimb.W + dq * fk.GetAxisRotation(defData.id);
            
            dW = fk.GetRotateMatrix(defData.id, q) * prevLimb.dW +
                 Vector3.Cross(fk.GetRotateMatrix(defData.id, q) * prevLimb.W, dq * fk.GetAxisRotation(defData.id)) +
                 ddq * fk.GetAxisRotation(defData.id);


            dV = fk.GetRotateMatrix(defData.id, q) * (Vector3.Cross(prevLimb.dW, fk.GetPositionNextFrame(defData.id)) + 
                                                      Vector3.Cross(prevLimb.W, Vector3.Cross(prevLimb.W, fk.GetPositionNextFrame(defData.id))) + 
                                                      prevLimb.dV);

            dVc = Vector3.Cross(dW, defData.CenterOfMass) +
                  Vector3.Cross(W, Vector3.Cross(W, defData.CenterOfMass)) + 
                  dV;


            F = defData.Mass * dVc;

            N = defData.Inertia * dW + 
                Vector3.Cross(W, defData.Inertia * W);
        }
    }


    public void FinalToRoot(float q, float dq, float ddq)
    {
        if (nextLimb != null)
        {
            f = fk.GetRotateMatrix(defData.id, q).Transpose() * nextLimb.f + F;
            
            n = N + 
                fk.GetRotateMatrix(defData.id, q).Transpose() * nextLimb.n + 
                Vector3.Cross(defData.CenterOfMass, F) + 
                Vector3.Cross(fk.GetPositionNextFrame(defData.id), fk.GetRotateMatrix(defData.id, q).Transpose() * nextLimb.f);
            
            t = Vector3.Dot(n, fk.GetAxisRotation(defData.id));
        }
    }


}

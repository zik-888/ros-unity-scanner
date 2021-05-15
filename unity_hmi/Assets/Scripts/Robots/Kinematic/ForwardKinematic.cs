using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Robots.RobotModel;
using RosSharp;
using RosSharp.RosBridgeClient.MessageTypes.Sensor;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using thelab.mvc;
using UnityEngine;
using M4x4 = System.Numerics.Matrix4x4;

[Serializable]
public class ForwardKinematic
{
    
    public RobotJoint[] joints;

    public float[] ActualAngle
    {
        get
        {
            return joints.Select(j => j.Angle).ToArray();
        }
        set
        {
            for (int i = 0; i < value.Length; i++)
            {
                joints[i].Angle = value[i];
            }
        }
    }


    // private void Update()
    // {
    //     float[] linkLength = new float[] {400, 25, 315, 35, 356, 80};
    //     float size = 0.1f;
    //     
    //     var mtx01 = GetT01(joints[0].Angle, linkLength[0]);
    //     DrawAxes(mtx01, size);
    //     
    //     var mtx12 = mtx01.Multiply(GetT12(joints[1].Angle, linkLength[1]));
    //     DrawAxes(mtx12, size);
    //     
    //     var mtx23 = mtx12.Multiply(GetT23(joints[2].Angle, linkLength[2]));
    //     DrawAxes(mtx23, size);
    //     
    //     var mtx34 = mtx23.Multiply(GetT34(joints[3].Angle, linkLength[3]));
    //     DrawAxes(mtx34, size);
    //     
    //     var mtx45 = mtx34.Multiply(GetT45(joints[4].Angle, linkLength[4]));
    //     DrawAxes(mtx45, size);
    //     
    //     var mtx56 = mtx45.Multiply(GetT56(joints[5].Angle, linkLength[5]));
    //     DrawAxes(mtx56, size);
    // }

    private void DrawAxes(Matrix4x4 mtx, float size)
    {
        Debug.DrawRay(mtx.GetColumn(3), ((Vector3)mtx.GetColumn(0)).normalized * size, Color.red);
        Debug.DrawRay(mtx.GetColumn(3), ((Vector3)mtx.GetColumn(1)).normalized * size, Color.green);
        Debug.DrawRay(mtx.GetColumn(3), ((Vector3)mtx.GetColumn(2)).normalized * size, Color.blue);
    }

    public Func<float, float, Matrix4x4> this[int number]
    {
        get
        {
            switch (number)
            {
                case 0: return GetT01;
                case 1: return GetT12;
                case 2: return GetT23;
                case 3: return GetT34;
                case 4: return GetT45;
                case 5: return GetT56;
            }

            return null;
        }
    }
    
    // -----------------------------------------------------------------------------------------
    public Matrix4x4 GetT01(float q1, float l1) => 
        Matrix4x4.TRS(new Vector3(0, l1, 0), Quaternion.AngleAxis(q1, Vector3.up), Vector3.one);
    
    public Matrix4x4 GetT12(float q2, float a2) =>
        Matrix4x4.TRS(new Vector3(0, 0, a2), Quaternion.LookRotation(Vector3.up, Vector3.right) *
                                              Quaternion.Euler(0, q2, 0), Vector3.one);
    
    public Matrix4x4 GetT23(float q3, float a3) =>
        Matrix4x4.TRS(new Vector3(a3, 0, 0), Quaternion.LookRotation(Vector3.forward, Vector3.up) *
                                             Quaternion.Euler(0, q3, 0), Vector3.one);
    
    public Matrix4x4 GetT34(float q4, float d4) =>
        Matrix4x4.TRS(new Vector3(0, 0, d4), Quaternion.LookRotation(Vector3.up, Vector3.right) *
                                             Quaternion.Euler(0, q4, 0), Vector3.one);
    
    public Matrix4x4 GetT45(float q5, float d5) =>
        Matrix4x4.TRS(new Vector3(0, d5, 0), Quaternion.LookRotation(Vector3.up, Vector3.forward) *
                                             Quaternion.Euler(0, q5, 0), Vector3.one);
    
    public Matrix4x4 GetT56(float q6, float d6) =>
        Matrix4x4.TRS(new Vector3(0, 0, d6), Quaternion.LookRotation(Vector3.up, Vector3.forward) *
                                             Quaternion.Euler(0, q6, 0), Vector3.one);
    
    /// <summary>
    /// Возвращает матрицу поворота из i-ой СК в i+1
    /// </summary>
    /// <param name="i"></param>
    /// <returns></returns>
    public Matrix3x3 GetRotateMatrix(int i, float angle)
    {
        Matrix3x3 mtx = null;
        Matrix4x4 mtxBuf = Matrix4x4.identity;

        switch (i)
        {
            case 1:
                mtxBuf = GetT01(angle, 400);
                break;
            case 2:
                mtxBuf = GetT12(angle, 25);
                break;
            case 3: 
                mtxBuf = GetT23(angle, 315);
                break;
            case 4:
                mtxBuf = GetT34(angle, 35);
                break;
            case 5:
                mtxBuf = GetT45(angle, 356);
                break;
            case 6:
                mtxBuf = GetT56(angle, 80);
                break;
        }
        
        mtx = new Matrix3x3(new Vector3[] { mtxBuf.GetColumn(0), mtxBuf.GetColumn(1), mtxBuf.GetColumn(2) });
        
        return mtx;
    }

    public Vector3 GetAxisRotation(int i)
    {
        return Vector3.one;
    }

    public Vector3 GetPositionNextFrame(int i)
    {
        Vector3 vctr = Vector3.zero;
        
        switch (i)
        {
            case 1:
                vctr = GetT01(0, 400).GetColumn(3);
                break;
            case 2:
                vctr = GetT12(0, 25).GetColumn(3);
                break;
            case 3: 
                vctr = GetT23(0, 315).GetColumn(3);
                break;
            case 4:
                vctr = GetT34(0, 35).GetColumn(3);
                break;
            case 5:
                vctr = GetT45(0, 356).GetColumn(3);
                break;
            case 6:
                vctr = GetT56(0, 80).GetColumn(3);
                break;
        }

        return vctr;
    }
}


public static class MatrixExtension
{

    public static Matrix4x4 Multiply(this Matrix4x4 value1, Matrix4x4 value2)
    {
        Matrix4x4 result;

        // First row
        result.m00 = value1.m00 * value2.m00 + value1.m01 * value2.m10 + value1.m02 * value2.m20 + value1.m03 * value2.m30;
        result.m01 = value1.m00 * value2.m01 + value1.m01 * value2.m11 + value1.m02 * value2.m21 + value1.m03 * value2.m31;
        result.m02 = value1.m00 * value2.m02 + value1.m01 * value2.m12 + value1.m02 * value2.m22 + value1.m03 * value2.m32;
        result.m03 = value1.m00 * value2.m03 + value1.m01 * value2.m13 + value1.m02 * value2.m23 + value1.m03 * value2.m33;

        // Second row
        result.m10 = value1.m10 * value2.m00 + value1.m11 * value2.m10 + value1.m12 * value2.m20 + value1.m13 * value2.m30;
        result.m11 = value1.m10 * value2.m01 + value1.m11 * value2.m11 + value1.m12 * value2.m21 + value1.m13 * value2.m31;
        result.m12 = value1.m10 * value2.m02 + value1.m11 * value2.m12 + value1.m12 * value2.m22 + value1.m13 * value2.m32;
        result.m13 = value1.m10 * value2.m03 + value1.m11 * value2.m13 + value1.m12 * value2.m23 + value1.m13 * value2.m33;

        // Third row
        result.m20 = value1.m20 * value2.m00 + value1.m21 * value2.m10 + value1.m22 * value2.m20 + value1.m23 * value2.m30;
        result.m21 = value1.m20 * value2.m01 + value1.m21 * value2.m11 + value1.m22 * value2.m21 + value1.m23 * value2.m31;
        result.m22 = value1.m20 * value2.m02 + value1.m21 * value2.m12 + value1.m22 * value2.m22 + value1.m23 * value2.m32;
        result.m23 = value1.m20 * value2.m03 + value1.m21 * value2.m13 + value1.m22 * value2.m23 + value1.m23 * value2.m33;

        // Fourth row
        result.m30 = value1.m30 * value2.m00 + value1.m31 * value2.m10 + value1.m32 * value2.m20 + value1.m33 * value2.m30;
        result.m31 = value1.m30 * value2.m01 + value1.m31 * value2.m11 + value1.m32 * value2.m21 + value1.m33 * value2.m31;
        result.m32 = value1.m30 * value2.m02 + value1.m31 * value2.m12 + value1.m32 * value2.m22 + value1.m33 * value2.m32;
        result.m33 = value1.m30 * value2.m03 + value1.m31 * value2.m13 + value1.m32 * value2.m23 + value1.m33 * value2.m33;

        return result;
    }
}
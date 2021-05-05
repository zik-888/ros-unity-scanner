using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Diagnostics;
using MathWorks.MATLAB.NET.Utility;
using MathWorks.MATLAB.NET.Arrays;
using MathWorks.MATLAB.NET.InverseKinematic;
using RosSharp;
using Sirenix.OdinInspector;
using Matlab = MathWorks.MATLAB.NET.InverseKinematic;

public class MatlabIK : MonoBehaviour
{
    public Transform point;
    
    public ForwardKinematic fk;
    
    Matlab.InverseKinematic ik = new Matlab.InverseKinematic();
    
    //MWCharArray urdfPath = new MWCharArray(@"C:\Users\HaoAsakura\YandexDisk\SS_ISH\Demon\matlab_funk\panda.urdf");
    MWNumericArray targetPoint = null;

    [SerializeField]
    public Matrix4x4 mtx;
    
    // Start is called before the first frame update
    void Start()
    {
        TargetPointInit();
    }

    [Button]
    public void IKCalc()
    {
        TargetPointInit(point);
        
        Stopwatch stopWatch = new Stopwatch();
        stopWatch.Start();
        
        var joints = (MWStructArray)ik.ik_calc(targetPoint);
        
        TimeSpan ts = stopWatch.Elapsed;

        // Format and display the TimeSpan value.
        string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
            ts.Hours, ts.Minutes, ts.Seconds,
            ts.Milliseconds / 10);
        
        print(elapsedTime);

        
        
        for (int i = 1; i < 8; i++)
        {
            if (joints != null)
            {
                var angle = ((MWNumericArray)joints["JointPosition", i]).ToScalarDouble() * Mathf.Rad2Deg;
                print(angle);

                try
                {
                    fk.joints[i - 1].Angle = (float)angle;
                }
                catch (Exception e)
                {
                    print($"I: {i}");
                    print(e);
                }

            }
        }

    }

    private void TargetPointInit()
    {
        int matrixOrder = 4;
        
        targetPoint = new MWNumericArray(
            MWArrayComplexity.Real,
            MWNumericType.Double,
            matrixOrder, matrixOrder);
        
        targetPoint[1, 1] = 1;
        targetPoint[2, 1] = 0;
        targetPoint[3, 1] = 0;
        targetPoint[4, 1] = 0;
                
        targetPoint[1, 2] = 0;
        targetPoint[2, 2] = 1;
        targetPoint[3, 2] = 0;
        targetPoint[4, 2] = 0;
                
        targetPoint[1, 3] = 0;
        targetPoint[2, 3] = 0;
        targetPoint[3, 3] = 1;
        targetPoint[4, 3] = 0;
                
        targetPoint[1, 4] = 0.565;
        targetPoint[2, 4] = 0;
        targetPoint[3, 4] = 0.563;
        targetPoint[4, 4] = 1;
    }
    
    private void TargetPointInit(Transform point)
    {
        //var p = point.worldToLocalMatrix;
        
        
        int matrixOrder = 4;
        
        targetPoint = new MWNumericArray(
            MWArrayComplexity.Real,
            MWNumericType.Double,
            matrixOrder, matrixOrder);


        mtx = new Matrix4x4();
        mtx.SetTRS(
            TransformExtensions.Unity2Ros(point.transform.position),
            TransformExtensions.Unity2Ros(point.transform.rotation),
            Vector3.one);
        
        // x
        targetPoint[1, 1] = (double)mtx.m00;
        targetPoint[2, 1] = (double)mtx.m10;
        targetPoint[3, 1] = (double)mtx.m20;
        targetPoint[4, 1] = 0;
                
        // y
        targetPoint[1, 2] = (double)mtx.m01;
        targetPoint[2, 2] = (double)mtx.m11;
        targetPoint[3, 2] = (double)mtx.m21;
        targetPoint[4, 2] = 0;
                
        // z
        targetPoint[1, 3] = (double)mtx.m02;
        targetPoint[2, 3] = (double)mtx.m12;
        targetPoint[3, 3] = (double)mtx.m12;
        targetPoint[4, 3] = 0;
                
        // p
        targetPoint[1, 4] = (double)mtx.m03; // x
        targetPoint[2, 4] = (double)mtx.m13; // y
        targetPoint[3, 4] = (double)mtx.m23; // z
        targetPoint[4, 4] = 1;
    }

}

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Serialization;

[Serializable]
public class RobotJoint : MonoBehaviour
{
    //public bool logs;
    
    [Header("Limits")]
    public Vector3Int moveAxis = Vector3Int.zero;
    public Vector2 angleLimit = Vector2.zero;

    [Space(5f)]
    [Header("Angles")]
    [SerializeField]
    private float _angle = default;
    
    public float Angle
    {
        set
        {
            if (!(_angle < angleLimit.x) || !(_angle > angleLimit.y))
                _angle = value;

            
        }
        get => _angle;
    }
    
    public float dAngle;
    public float ddAngle;
    
    [Space(5f)]
    [Header("Offsets")]
    public float angleOffset;
    public Vector3 positionOffset;

    private float _previewAngle = 0;
    private float _previewDAngle = 0;

    private void Awake()
    {
        //_angle = GetActualAngle();
        
        positionOffset = transform.localPosition;
        
    }


    private void Update()
    {
        dAngle = (_previewAngle - Angle) / Time.fixedTime;
        ddAngle = (_previewDAngle - dAngle) / Time.fixedTime;

        _previewAngle = Angle;
        _previewDAngle = dAngle;
        
        // if(logs)
        // print($"a: {Angle}, dA: {dAngle}, ddA: {ddAngle}");
        
        transform.localRotation = Quaternion.Euler((Vector3) moveAxis * (_angle - angleOffset));

        
    }

    public void DrawAxes(Matrix4x4 mtx)
    {
        Debug.DrawRay(gameObject.transform.position, mtx.GetColumn(0) * 500, Color.red);
        Debug.DrawRay(gameObject.transform.position, mtx.GetColumn(1) * 500, Color.green);
        Debug.DrawRay(gameObject.transform.position, mtx.GetColumn(2) * 500, Color.blue);
    }

    public float GetActualAngle()
    {
        if (moveAxis.x == 1)
        {
            return transform.localRotation.x;
        }
        if (moveAxis.y == 1)
        {
            return transform.localRotation.y;
        }
        if (moveAxis.z == 1)
        {
            return transform.localRotation.z;
        }

        return 0f;
    }
    
}

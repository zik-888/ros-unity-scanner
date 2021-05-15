using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RobotTool : MonoBehaviour
{
    public Matrix4x4 mtx = new Matrix4x4();
    Matrix4x4 mtxP = new Matrix4x4();
    private Matrix4x4 mtx2;

    public Vector3 p5;
    public float T0 = 0;
    public float T1 = 0;
    public float A = 0;
    private Vector3 rot;
    
    
    
    // Start is called before the first frame update
    void Start()
    {
        
        //mtx.rotation = transform.rotation;
        
        mtxP.SetTRS(transform.parent.position, transform.parent.rotation, Vector3.one);
        mtx.SetTRS(transform.position, transform.rotation, Vector3.one);
        
        var plane = new Plane(Vector3.up, transform.parent.position);
        plane.Raycast(new Ray(transform.position, -mtx.GetColumn(2)), out T1);
        T0 = Vector3.Distance(transform.position - (Vector3)mtx.GetColumn(2).normalized * T1, 
            transform.parent.position);
        
        A = Vector3.Angle(mtx.GetColumn(2), mtxP.GetColumn(2));
        
        
        
        Debug.DrawRay(transform.position, Quaternion.AngleAxis(A, mtx.GetColumn(1)) * (Vector3)(mtx.GetColumn(2)) * 500, Color.cyan, 1000f);
        
        float px = transform.position.x;
        float py = transform.position.y;
        float pz = transform.position.z;
        
        float rx = transform.rotation.x;
        float ry = transform.rotation.y;
        float rz = transform.rotation.z;
        
        

        float ax = Mathf.Cos(rz * 3.14f / 180.0f) * Mathf.Cos(ry * 3.14f / 180.0f);
        float ay = Mathf.Sin(rz * 3.14f / 180.0f) * Mathf.Cos(ry * 3.14f / 180.0f);
        float az = -Mathf.Sin(ry * 3.14f / 180.0f);

        rot = new Vector3(ax, ay, az);
        
        // float p5x = px - (L5 + L6) * ax;
        // float p5y = py - (L5 + L6) * ay;
        // float p5z = pz - (L5 + L6) * az;




        mtx2 = new Matrix4x4();

        mtx.SetTRS(transform.localPosition, transform.localRotation, Vector3.one);
        mtx2 = mtx2.inverse.Multiply(mtx);

    }

    // Update is called once per frame
    void Update()
    {


        //Debug.DrawRay(mtx2.MultiplyVector(transform.position), Vector3.up*500, Color.cyan);
        
        //print($"P: {GetComponentInParent<Transform>().position - transform.position}, R: {transform.rotation}");
        
        //print($"P: {transform.localPosition}, R: {transform.rotation}");
    }


    public void GetParentPosition()
    {
    }
}

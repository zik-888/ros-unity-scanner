using System;
using System.Collections;
using System.Collections.Generic;
using RootMotion.FinalIK;
using UnityEngine;
using static UnityEngine.Mathf;

public class InverseKinematicKr6 : MonoBehaviour
{
    public Transform body1;
    public Transform body2;
    public Transform body3;
    
    public Transform point;
    public Transform childPoint;
    public Transform pChildPoint;

    public Vector3 positionFor5Link;
    public Vector3 projectionPosFor5Link;

    public ForwardKinematic fk;


    float[] linkLength = new float[] { 400 / 100, 25 / 100, 315 / 100, 35 / 100, 356 / 100, 80 / 100 };


    public float[] GetIK()
    {
        float[] q = new float[6];

        q[0] = Atan2(positionFor5Link.x, positionFor5Link.z);

        Vector2 PK5 = new Vector2(Sqrt(Pow(positionFor5Link.z, 2) + Pow(positionFor5Link.x, 2)) + linkLength[1], positionFor5Link.y - linkLength[0]);

        float l23 = Sqrt(Pow(linkLength[3], 2) + Pow(linkLength[4], 2));
        float q2_ = Atan2(linkLength[3], linkLength[4]);
        
        
        float q1_acos = Acos((Pow(PK5.x, 2) + Pow(PK5.y, 2)  + Pow(linkLength[2], 2) - Pow(l23, 2)) 
                                   / (2 * Sqrt(Abs(Pow(PK5.x, 2) + Pow(PK5.y, 2))) * linkLength[2]));

        q[1] = -Atan2(PK5.y, PK5.x);

        if (!float.IsNaN(q1_acos))
        {
            q[1] = -Atan2(PK5.y, PK5.x) - q1_acos;
        }
        
        float q2_acos = (-Pow(PK5.x, 2) - Pow(PK5.y, 2) + Pow(linkLength[2], 2) + Pow(l23, 2)) / (2 * l23 * linkLength[2]);
        q[2] = PI - Acos(q2_acos);

        if(q2_acos > 1)
            q[2] = PI;
        
        if (q2_acos < -1 || float.IsNaN(q[2]))
            q[2] = 0;

        q[2] -= q2_;


        for (int i = 0; i < q.Length; i++)
        {
            q[i] *= Rad2Deg;
        }
        
        return q;
    }

    private IKSolve solver;
    // Start is called before the first frame update
    void Start()
    {
        solver = new IKSolve(body1, body2, body3, Vector3.right, 1f);
    }

    // Update is called once per frame
    void Update()
    {
        positionFor5Link = GetPrevJoint(0.08f);
        childPoint.position = positionFor5Link;

        projectionPosFor5Link = new Vector3(0, positionFor5Link.y, Abs(Sqrt(Pow(positionFor5Link.x, 2) + Pow(positionFor5Link.z, 2))));
        pChildPoint.position = projectionPosFor5Link;
        
        var(q1, q2) = solver.Solve(projectionPosFor5Link);

        //print($"q1: {q1}, q2: {q2}");
        //print($"p1: {positionFor5Link}, p2: {projectionPosFor5Link}");
        
        //fk.joints[0].Angle = Atan2(projectionPosFor5Link.x, projectionPosFor5Link.z) * Rad2Deg;
        fk.joints[1].Angle = q1;
        fk.joints[2].Angle = q2;
        
        
    }

    public Vector3 GetPrevJoint(float lenth)
    {
        Matrix4x4 mtx = new Matrix4x4();
        mtx.SetTRS(point.position, point.rotation, Vector3.one);

        return point.position - (Vector3)mtx.GetColumn(2) * lenth;
    }
}


public class IKSolve
{
    private Transform bone1;
    private Transform bone2;
    private Transform bone3;
    private Vector3 bendNormal;
    private float weight;
        
    public IKSolve(Transform bone1, Transform bone2, Transform bone3, 
        Vector3 bendNormal,
        float weight)
    {
        this.bone1 = bone1;
        this.bone2 = bone2;
        this.bone3 = bone3;
        this.weight = weight;
        this.bendNormal = bendNormal;
    }
    
    public (float, float) Solve(Vector3 targetPosition)
    {
        //if (weight <= 0f) return (0.0f, 0.0f);

        Debug.Log($"b1 {bone1.rotation}, b2 {bone2.rotation}");
        
        // Direction of the limb in solver
        targetPosition = Vector3.Lerp(bone3.position, targetPosition, weight);
			
        Vector3 dir = targetPosition - bone1.position;
			
        // Distance between the first and the last node solver positions
        float length = dir.magnitude;
        //if (length == 0f) return (0.0f, 0.0f);
			
        float sqrMag1 = (bone2.position - bone1.position).sqrMagnitude;
        float sqrMag2 = (bone3.position - bone2.position).sqrMagnitude;
			
        // Get the general world space bending direction
        Vector3 bendDir = Vector3.Cross(dir, bendNormal);
			
        // Get the direction to the trigonometrically solved position of the second node
        Vector3 toBendPoint = GetDirectionToBendPoint(dir, length, bendDir, sqrMag1, sqrMag2);
			
        // Position the second node
        Quaternion q1 = Quaternion.FromToRotation(bone2.position - bone1.position, toBendPoint);
        if (weight < 1f) q1 = Quaternion.Lerp(Quaternion.identity, q1, weight);

        bone1.rotation = q1 * bone1.rotation;

        Quaternion q2 = Quaternion.FromToRotation(bone3.position - bone2.position, targetPosition - bone2.position);
        if (weight < 1f) q2 = Quaternion.Lerp(Quaternion.identity, q2, weight);

        bone2.rotation = q2 * bone2.rotation;

        // print($"b1: {bone1.rotation.eulerAngles}, l1:{bone1.localRotation.eulerAngles} \n" +
        //       $"b2: {bone2.rotation.eulerAngles}, l2:{bone2.localRotation.eulerAngles} ");

        float a1;
        Vector3 va1 = Vector3.right;
        bone1.rotation.ToAngleAxis(out a1, out va1);
        
        float a2;
        Vector3 va2 = Vector3.right;
        bone2.rotation.ToAngleAxis(out a2, out va2);
        
        //print($"a1: {a1 * va1.x}, a2:{a2 * va2.x - a1 * va1.x}");

        float firstAngle = a1 * va1.x;
        float secondAngle = a2 * va2.x - firstAngle;
        
        return (float.IsNaN(firstAngle) ?  0f : firstAngle, float.IsNaN(secondAngle) ?  0f : secondAngle);
    }

    //Calculates the bend direction based on the law of cosines. NB! Magnitude of the returned vector does not equal to the length of the first bone!
    private static Vector3 GetDirectionToBendPoint(Vector3 direction, float directionMag, Vector3 bendDirection, float sqrMag1, float sqrMag2) {
        float x = ((directionMag * directionMag) + (sqrMag1 - sqrMag2)) / 2f / directionMag;
        float y = (float)Math.Sqrt(Mathf.Clamp(sqrMag1 - x * x, 0, Mathf.Infinity));
			
        if (direction == Vector3.zero) return Vector3.zero;
        return Quaternion.LookRotation(direction, bendDirection) * new Vector3(0f, y, x);
    }
}

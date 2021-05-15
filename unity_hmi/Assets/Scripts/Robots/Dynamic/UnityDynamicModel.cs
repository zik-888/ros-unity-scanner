using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Dynamic;
using UnityEngine;

public class UnityDynamicModel : MonoBehaviour
{
    [SerializeField] private ForwardKinematic fk;
    [SerializeField] private DynamicModel dm = new DynamicModel();

    public double[] tau;

    private void Start()
    {
        StartCoroutine(TestCoroutine());
    }
        
    IEnumerator TestCoroutine()
    {
        while(true)
        {
            yield return new WaitForSeconds(0.1f);

                
            tau = 
                InverseDynamic.GetTorques(
                    dm, 
                    fk.joints.Select(q => (double)q.Angle).ToArray(), 
                    fk.joints.Select(dq => (double)dq.dAngle).ToArray(), 
                    fk.joints.Select(ddq => (double)ddq.ddAngle).ToArray());

            // print("TAU");
            // foreach (var VARIABLE in tau)
            // {
            //     print(VARIABLE);
            // }
        }
    }

}

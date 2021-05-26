using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommandPointController : MonoBehaviour
{
    public Action<MarchingFormingPoint> SetPose { set; private get; }
    
    private MarchingFormingPoint point = new MarchingFormingPoint(0, Vector3.zero);
    private Color defColor;

    private void OnMouseEnter()
    {
        defColor = GetComponent<MeshRenderer>().material.color;
        GetComponent<MeshRenderer>().material.color = Color.yellow;
    }
    private void OnMouseExit()
    {
        GetComponent<MeshRenderer>().material.color = defColor;
    }

    private void OnMouseDrag()
    {
        if(Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), 
                           out RaycastHit hit, 
                           100f,  
                           1 << 9))
        {
            
            point.Position = hit.point;
            point.Rotation = gameObject.transform.rotation.eulerAngles;
            point.triangleIndex = hit.triangleIndex;

            transform.position = hit.point;
        }

        
    }

    private void OnMouseUpAsButton()
    {
        try
        {
            SetPose(point);
        }
        catch (Exception e)
        {
            Debug.LogError(e.Message);
        }

    }
}

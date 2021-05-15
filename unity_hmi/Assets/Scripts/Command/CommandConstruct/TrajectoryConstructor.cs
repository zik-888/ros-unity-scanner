using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Command.CommandConstruct;
using UnityEngine;

public class TrajectoryConstructor : MonoBehaviour
{
    public TrPoint[] points = new TrPoint[] {};
    public TrPoint[] Points
    {
        set
        {
            points = value;
        }
        
        get => points;
    }

    public LineRenderer render;
    
    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(UpdatePoints());
    }

    private void Update()
    {
        render.positionCount = points.Length;
        render.SetPositions(points.Select(p => p.transform.position).ToArray());
    }

    IEnumerator UpdatePoints()
    {
        while (true)
        {
            yield return new WaitForSeconds(.1f);


            if (Points.Length != transform.childCount)
            {
                Points = transform.GetComponentsInChildren<TrPoint>();
            }
            
        }
        
    }
    
}

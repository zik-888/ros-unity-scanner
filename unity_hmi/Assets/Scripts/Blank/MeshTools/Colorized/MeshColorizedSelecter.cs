using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class MeshColorizedSelecter : MonoBehaviour
{
    public int[] triangles;
    public Vector3[] vert;
    public Color[] color;
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (!Input.GetMouseButtonDown(0)
            || !Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out RaycastHit hit)
            || hit.triangleIndex == -1)
            return;

        var m = hit.collider.GetComponent<MeshFilter>().mesh;

        triangles = m.triangles;
        vert = m.vertices;
        color = new Color[vert.Length];

        var tInd = hit.triangleIndex;

        var fv = m.triangles[3 * tInd];
        var dv = m.triangles[3 * tInd + 1];
        var tv = m.triangles[3 * tInd + 2];

        Debug.DrawLine(m.vertices[fv], m.vertices[dv], Color.yellow, 1000f);
        Debug.DrawLine(m.vertices[dv], m.vertices[tv], Color.yellow, 1000f);
        Debug.DrawLine(m.vertices[tv], m.vertices[fv], Color.yellow, 1000f);
        
        for (int i = 0; i < color.Length; i++)
        {
            color[i] = Color.white;

            if (i == fv || i == dv || i == tv)
                color[i] = Color.blue;
        }
        
        m.SetColors(color);
        
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using MeshSystem.MeshAddition;
using MeshSystem;

public class TestMesh : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        print("start");

        var gMesh = gameObject.GetComponent<MeshFilter>().mesh;

        //CMesh mesh = new CMesh(gMesh.vertices, gMesh.triangles);

        //mesh.InitSurfaces();

        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

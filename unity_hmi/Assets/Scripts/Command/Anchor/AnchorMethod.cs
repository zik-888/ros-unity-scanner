using System.Collections;
using System.Collections.Generic;
using System.Linq;
using MeshSystem;
using MeshSystem.MeshAddition;
using Sirenix.OdinInspector;
using UnityEngine;

public class AnchorMethod : MonoBehaviour
{
    public GameObject anchorPointPrefab;

    [Button]
    public void CreateAnchorPoints()
    {
        var surfaces = CMeshPool.GetMesh(0).Surfaces;
        List<Vector3> vertexes = new List<Vector3>();
            
        foreach (var s in surfaces) // заполняем очередь
        {
            var vertices = s.GetVertices();
                
            foreach (var v in vertices)
            {
                vertexes.Add(CMeshPool.GetMesh(0).Vertices[v]);
            }
        }

        vertexes = vertexes.Distinct(new Vector3Comparer()).ToList();

        foreach (var v in vertexes)
        {
            var a = GameObject.Instantiate(anchorPointPrefab);
            a.transform.position = v;
        }
    }
}

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Blank.MeshTools;

using Parabox.CSG;
using Sirenix.OdinInspector;
using Boolean = Parabox.CSG.Boolean;

public class MeshSplitterBind : MonoBehaviour
{
    public enum SplitMode
    {
        Off,
        ON,
        OnAndSelect
    }
    
    
    [SerializeField]
    private SplitMode splitMode = SplitMode.Off;

    [SerializeField] private MeshSplitter splitter;
    

    public void SetSplitMode(bool mode)
    {
        switch (mode)
        {
            case true:
                splitMode = SplitMode.ON;
                splitter = gameObject.AddComponent<MeshSplitter>();
                break;
            case false:
                splitMode = SplitMode.Off;
                if(splitter != null)
                    Destroy(splitter);
                break;
            
        }
    }
    
    [Button]
    private void SubtractMesh(GameObject cube, GameObject sphere)
    {
        // Initialize two new meshes in the scene
        // GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        // GameObject sphere = GameObject.CreatePrimitive(PrimitiveType.Plane);
        // sphere.transform.localScale = Vector3.one * 1.3f;

        // Perform boolean operation
        CSG_Model result = Boolean.Subtract(cube, sphere);

        // Create a gameObject to render the result
        var composite = new GameObject();
        composite.AddComponent<MeshFilter>().sharedMesh = result.mesh;
        composite.AddComponent<MeshRenderer>().sharedMaterials = result.materials.ToArray();
    }
}

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Blank.MeshTools;

using Sirenix.OdinInspector;

public class MeshSplitterBind : MonoBehaviour
{
    public enum SplitMode
    {
        Off,
        ON,
        OnAndSelect
    }
    
#pragma warning disable 414
    [SerializeField]
    private SplitMode splitMode = default;

    [SerializeField] private MeshSplitter splitter;
    
#pragma warning restore 414
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


        // Create a gameObject to render the result
    }
}

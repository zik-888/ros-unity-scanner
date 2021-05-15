using System;
using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using UnityEngine;


public enum BlankType { Cad, Scan, AlignedCad }

[System.Serializable]
public class BlankModel
{
    public int Id;

    public bool designMode;

    public BlankType type = BlankType.Cad;

    [OdinSerialize]
    private BlankPose blankPose;
    public BlankPose BlankPose
    {
        set => blankPose = new BlankPose(value);

        get => blankPose;
    }

    [OdinSerialize]
    public GameObject BlankObject { set; get; }

    public Mesh Mesh => BlankObject.GetComponent<MeshFilter>().mesh;

    public BlankModel(BlankPose blankPose, GameObject blankObject, BlankType type = BlankType.Cad)
    {
        this.blankPose = null;
        this.BlankObject = blankObject;
        this.type = type;

        BlankPose = blankPose;
        BlankObject.AddComponent<BlankController>();
    }


    public BlankModel(GameObject blankObject, BlankType type = BlankType.Cad)
    {
        this.blankPose = null;
        this.BlankObject = blankObject;
        this.type = type;

        BlankPose = new BlankPose(Vector3.zero, Vector3.zero, 1f);
        BlankObject.AddComponent<BlankController>();
    }

}

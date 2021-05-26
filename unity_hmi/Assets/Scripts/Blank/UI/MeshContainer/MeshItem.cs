using System;
using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UniRx;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using System.Linq;
using RosSharp;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using Pose = RosSharp.RosBridgeClient.MessageTypes.Geometry.Pose;
using Mesh = UnityEngine.Mesh;
using Quaternion = RosSharp.RosBridgeClient.MessageTypes.Geometry.Quaternion;
using Transform = RosSharp.RosBridgeClient.MessageTypes.Geometry.Transform;
using Vector3 = UnityEngine.Vector3;

public class MeshItem : Element<DemonOLPApplication>, ISelectHandler, IDeselectHandler
{
    public MeshProperties properties;

    public Text text;
    public RectTransform alignmentButton;
    private int _id;

    public bool selected = false;

    public BlankPose BlankPose { set; get; }

    private UnityProcessingActionClient _processingClient;

    private void Start()
    {
        app.model.BlankModels[_id]
                 .ObserveEveryValueChanged(x => x.BlankPose)
                 .Subscribe(x => PoseChangeIntoModel(x))
                 .AddTo(this);

        alignmentButton.gameObject.SetActive(app.model.BlankModels[_id].type != BlankType.Scan);
        
        _processingClient = GameObject.Find("RosConnector").GetComponent<UnityProcessingActionClient>();
    }

    public void PoseAlignment()
    {
        // triangles
        var meshTriangleArray = TransferMethods.TraingleConvert(app.model.BlankModels[_id].Mesh.triangles);

        // vertices
        var pointArray = TransferMethods.VertexConvert(app.model.BlankModels[_id].Mesh.vertices);

        // empty trajectory
        var trajectory = new Pose[] { };

        _processingClient.SendGoal
        (
            new ProcessingGoal (meshTriangleArray, pointArray, trajectory, (int)ModeOfProcessing.ALIGN), 
            PoseAlignmentResult
        );
    }

    private void PoseAlignmentResult(ProcessingResult result)
    {
        var mesh = new Mesh
        {
            vertices = TransferMethods.VertexConvert(result.vertices_CAD),
            triangles = TransferMethods.TraingleConvert(result.triangles_CAD)
        };

        app.model.BlankModels[_id].BlankObject.GetComponent<MeshFilter>().mesh = mesh;
    }

    public void CheckDeform()
    {
        
    }

    private void CheckDeformResult()
    {
        
    }

    public void ApplyDeform()
    {
        // triangles
        var meshTriangleArray = TransferMethods.TraingleConvert(app.model.BlankModels[_id].Mesh.triangles);

        // vertices
        var pointArray = TransferMethods.VertexConvert(app.model.BlankModels[_id].Mesh.vertices);

        // trajectory
        var trajectory = app.model.CommandModels
            .Where(c => c.blankNum == _id)
            .SelectMany(c => c.poses)
            .Select(c => GetPose(c.Position, c.Rotation)).ToArray();

        _processingClient.SendGoal
        (
            new ProcessingGoal (meshTriangleArray, pointArray, trajectory, (int)ModeOfProcessing.DEFORM), 
            ApplyDeformResult
        );
    }
    
    public void ApplyDeformResult(ProcessingResult result)
    {
        throw new Exception("ApplyDeformResult");
    }

    public Pose GetPose(Vector3 position, Vector3 rotation)
    {
        var p = TransformExtensions.Unity2Ros(position);
        var r = TransformExtensions.Unity2Ros(rotation);

        return new Pose
        (
            new Point(p.x, p.y, p.z),
            new Quaternion(r.x, r.y, r.z, 0)
        );
    }


    public void PoseChangeIntoModel(BlankPose pose)
    {
        properties.InitProperties(pose);
        //app.model.blankModels[id].BlankObject.GetComponent<BlankController>().PoseChangeIntoModel(pose);
    }


    public void OnChangeBlankPose(BlankPose blankPose) 
        => app.model.BlankModels[_id].BlankPose = blankPose;

    public int Id 
    { 
        set 
        {
            _id = value;
            text.text = $"Blank: {value}";
            name = $"MeshItem, {_id}";

            app.model.BlankModels[_id].BlankObject.GetComponent<BlankController>().Id = _id;
        }
    }

    public void OnSelect(BaseEventData eventData)
    {
        print("OnSelect");

        selected = true;
    }

    public void Remove()
    {
        app.model.BlankModels.RemoveAt(_id);

        for(int i = 0; app.model.CommandModels.Count > i; i++)
        {
            if (app.model.CommandModels[i].type != CommandType.PtP)
            {
                if (app.model.CommandModels[i].blankNum == _id)
                {
                    app.model.CommandModels.Remove(app.model.CommandModels[i]);
                }
            }
        }   
    }

    public void OnDeselect(BaseEventData eventData)
    {
        print("OnDeselect");

        selected = false;
    }
}
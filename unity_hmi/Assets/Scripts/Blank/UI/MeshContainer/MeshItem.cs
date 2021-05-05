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
using Pose = RosSharp.RosBridgeClient.MessageTypes.Geometry.Pose;
using Mesh = UnityEngine.Mesh;

public class MeshItem : Element<DemonOLPApplication>, ISelectHandler, IDeselectHandler
{
    public MeshProperties properties;

    public Text text;
    public RectTransform alignmentButton;
    private int id;

    public bool selected = false;

    public BlankPose BlankPose { set; get; }

    private void Start()
    {
        app.model.BlankModels[id]
                 .ObserveEveryValueChanged(x => x.BlankPose)
                 .Subscribe(x => PoseChangeIntoModel(x))
                 .AddTo(this);

        alignmentButton.gameObject.SetActive(app.model.BlankModels[id].type != BlankType.Scan);
    }

    public void PoseAlignment()
    {
        var client = GameObject.Find("RosConnector").GetComponent<UnityProcessingActionClient>();

        // triangles

        var meshTriangleArray = TransferMethods.TraingleConvert(app.model.BlankModels[id].Mesh.triangles);

        // vertices

        var pointArray = TransferMethods.VertexConvert(app.model.BlankModels[id].Mesh.vertices);

        // trajectory

        var trajectory = new Pose[] { };

        client.SendGoal(new ProcessingGoal(meshTriangleArray, 
                                           pointArray, 
                                           trajectory, 
                                           (int)ModeOfProcessing.ALIGN),
                        PoseAlignmentResult);
    }

    private void PoseAlignmentResult(ProcessingResult result)
    {
        var mesh = new Mesh();

        mesh.vertices = TransferMethods.VertexConvert(result.vertices_CAD);

        mesh.triangles = TransferMethods.TraingleConvert(result.triangles_CAD);

        app.model.BlankModels[id].BlankObject.GetComponent<MeshFilter>().mesh = mesh;
    }


    public void Ros2UnityTransform()
    {
        var vertices = app.model.BlankModels[id].Mesh.vertices;

        var mesh = new Mesh();

        int length = app.model.BlankModels[id].Mesh.triangles.Length;

        var zTriangle = app.model.BlankModels[id].Mesh.triangles;

        //// меняю в массиве 2 и 3 элемент местами
        for (int i = 0; length / 3 > i; i++)
        {
            int temp = zTriangle[i * 3 + 1];
            zTriangle[i * 3 + 1] = zTriangle[i * 3 + 2];
            zTriangle[i * 3 + 2] = temp;
        }

        mesh.vertices = vertices.Select(p => RosSharp.TransformExtensions.Ros2Unity(p)).ToArray();
        mesh.triangles = app.model.BlankModels[id].Mesh.triangles.Concat(zTriangle).ToArray();

        app.model.BlankModels[id].BlankObject.GetComponent<MeshFilter>().mesh = mesh;
    }


    public void PoseChangeIntoModel(BlankPose pose)
    {
        properties.InitProperties(pose);
        //app.model.blankModels[id].BlankObject.GetComponent<BlankController>().PoseChangeIntoModel(pose);
    }


    public void OnChangeBlankPose(BlankPose blankPose) 
        => app.model.BlankModels[id].BlankPose = blankPose;

    public int Id 
    { 
        set 
        {
            id = value;
            text.text = $"Blank: {value}";
            name = $"MeshItem, {id}";

            app.model.BlankModels[id].BlankObject.GetComponent<BlankController>().Id = id;
        }
    }

    public void OnSelect(BaseEventData eventData)
    {
        print("OnSelect");

        selected = true;
    }

    public void Remove()
    {
        app.model.BlankModels.RemoveAt(id);

        for(int i = 0; app.model.CommandModels.Count > i; i++)
        {
            if (app.model.CommandModels[i].type != CommandType.PtP)
            {
                if (app.model.CommandModels[i].blankNum == id)
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
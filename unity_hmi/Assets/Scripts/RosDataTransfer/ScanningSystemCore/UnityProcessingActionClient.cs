using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UniRx;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using System;
using System.IO;
using System.Linq;
using RosSharp;
using Sirenix.OdinInspector;
using UnityEngine.UIElements;
using MeshTriangle = RosSharp.RosBridgeClient.MessageTypes.Shape.MeshTriangle;
using Point = RosSharp.RosBridgeClient.MessageTypes.Geometry.Point;
using RPose = RosSharp.RosBridgeClient.MessageTypes.Geometry.Pose;
using RQuaternion = RosSharp.RosBridgeClient.MessageTypes.Geometry.Quaternion;
using Vector3 = UnityEngine.Vector3;

public class UnityProcessingActionClient : Element<DemonOLPApplication>
{
    public string actionName;

    
    private RosConnector _rosConnector;
    private ProcessingActionClient _processingActionClient;
    private Action<ProcessingResult> _actionResult;

    [ShowInInspector]
    public string Status => _processingActionClient == null ? string.Empty : _processingActionClient.Status;
    [ShowInInspector]
    public string Feedback => _processingActionClient == null ? string.Empty :_processingActionClient.Feedback;
    [ShowInInspector]
    public string Result => _processingActionClient == null ? string.Empty :_processingActionClient.Result;

    private void Start()
    {
        _rosConnector = GetComponent<RosConnector>();
        _processingActionClient = new ProcessingActionClient(actionName, _rosConnector.RosSocket);
        _processingActionClient.Initialize();

        // _processingActionClient.reactOrder.ObserveEveryValueChanged(x => x.Value)
        //                                  .Subscribe(xs => UpdateModel(xs))
        //                                  .AddTo(this);
        
        _processingActionClient.ActionResult = new Action<ProcessingResult>(UpdateModel);
    } 

    [Button]
    public void ParseObj(GameObject alignedModel)
    {
        string text = "";

        Mesh mesh = alignedModel.GetComponent<MeshFilter>().mesh;

        text += "o 2_5406743593731755192 \n";

        var vl = mesh.vertices.Length;

        for (int i = 0; i < vl; i++)
        {
            var bufV = TransformExtensions.Unity2Ros(mesh.vertices[i]);
            text += $"v {bufV.x} {bufV.y} {bufV.z}\n";
        }

        var tl = mesh.triangles.Length;
        
        for (int i = 0; i < tl / 3; i++)
        {
            text += $"f {mesh.triangles[i * 3] + 1} {mesh.triangles[i * 3 + 1] + 1} {mesh.triangles[i * 3 + 2] + 1}\n";
        }
        
        string writePath = @"Assets/Scenes/hta.txt";
        
        using (StreamWriter sw = new StreamWriter(writePath, false, System.Text.Encoding.Default))
        {
            sw.WriteLine(text);
        }

    }
    
    [Button]
    public void TestSendMsg(GameObject alignedModel)
    {
        // this._actionResult = UpdateModel;
        
        
        Mesh mesh = alignedModel.GetComponent<MeshFilter>().mesh;
        

        var triangles = new MeshTriangle[mesh.triangles.Length / 3];

        for (int i = 0; i < triangles.Length; i++)
        {
            triangles[i] = new MeshTriangle(new uint[]
            {
                (uint) mesh.triangles[i * 3],
                (uint) mesh.triangles[i * 3 + 1],
                (uint) mesh.triangles[i * 3 + 2]
            });
        }
        
        var vertices = 
            from v in mesh.vertices
            let o = TransformExtensions.Unity2Ros(v + alignedModel.transform.position)
            select new Point((double)o.x, (double)o.y, (double)o.z);

        
        _processingActionClient.Order = new ProcessingGoal(
            triangles, 
            vertices.ToArray(), 
            new RPose[]{ }, 
            (int)ModeOfProcessing.ALIGN);
        
        _processingActionClient.SendGoal();
    }
    

    public void SendGoal()
    {
        RegisterGoal();
        _processingActionClient.SendGoal();
    }

    public void SendGoal(ProcessingGoal order, Action<ProcessingResult> actionResult)
    {
        this._actionResult = actionResult;
        _processingActionClient.Order = order;
        _processingActionClient.SendGoal();
    }

    private void RegisterGoal()
    {
        _processingActionClient.
        Order = new ProcessingGoal(new MeshTriangle[] { new MeshTriangle(new uint[] { 0, 1, 2 })},
                                   new Point[] { new Point(0, 1, 2) },
                                   new RPose[] { new RPose(new Point(0, 2, 3), new RQuaternion()) },
                                   (int)ModeOfProcessing.ALIGN);
    }

    public void CancelGoal()
    {
        _processingActionClient.CancelGoal();
    }

    public void UpdateModel(ProcessingResult result)
    {
        
        print("Зашёл");
        Debug.Log("UpdateModel");
        Debug.Log(result.error);
        Debug.Log(result.triangles_CAD.Length);
        Debug.Log(result.vertices_CAD.Length);

        if(_actionResult != null)
            _actionResult(result);
        
        if(result != null)
        {
            var mv = ScanningResultToMesh(new ScanningResult(result.triangles_CAD, result.vertices_CAD));

            var go = new GameObject("ScanModel", typeof(MeshFilter), typeof(MeshRenderer));
            
            go.GetComponent<MeshFilter>().mesh = mv;
            
            app.model.BlankModels.Add(new BlankModel(go, BlankType.AlignedCad));
        }
    }

    private Mesh ScanningResultToMesh(ScanningResult result)
    {
        var mesh = new Mesh();
        
        
        var vertex = result.vertices.Select(v => TransformExtensions.Ros2Unity(new Vector3((float)v.x, (float)v.y, (float)v.z))).ToArray();
        
        var triangle = (from t in result.triangles
            from tt in t.vertex_indices
            select (int)tt).ToArray();

        
        vertex = vertex.Select(v => new Vector3(v.x, v.y, v.z)).ToArray();
        
        mesh.vertices = vertex;
        mesh.triangles = triangle;
        mesh.RecalculateNormals();
        return mesh;
    }
    
    
    

}

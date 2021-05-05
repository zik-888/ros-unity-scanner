using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.ScanningSystemCore;
using UnityEngine;
using UniRx;
using thelab.mvc;
using System;
using System.Collections.Generic;
using System.Globalization;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using RosSharp.RosBridgeClient.MessageTypes.Shape;
using System.Linq;
using RosSharp;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using Mesh = UnityEngine.Mesh;
using Vector3 = UnityEngine.Vector3;
using System.IO;
using Assets.Scripts.Command.PerformanceMethod;
using UnityEngine.ProBuilder;
using MIConvexHull;

public class UnityScanningActionClient : Element<DemonOLPApplication>
{
    
    
    
    private RosConnector _rosConnector;
    
    private ScanningActionClient _scanningActionClient;
    
    public string actionName;

    [ShowInInspector] 
    public ScanningGoal goal => _scanningActionClient == null ? ScanningGoal.DefGoal: _scanningActionClient.Order;
    [ShowInInspector]
    public string Status => _scanningActionClient == null ? string.Empty : _scanningActionClient.Status;
    [ShowInInspector]
    public string Feedback => _scanningActionClient == null ? string.Empty :_scanningActionClient.Feedback;
    [ShowInInspector]
    public string Result => _scanningActionClient == null ? string.Empty :_scanningActionClient.Result;

    private void Start()
    {
        _rosConnector = GetComponent<RosConnector>();
        _scanningActionClient = new ScanningActionClient(actionName, _rosConnector.RosSocket);
        _scanningActionClient.Initialize();

        _scanningActionClient.reactOrder.ObserveEveryValueChanged(x => x.Value)
                                       .Subscribe(xs => UpdateModel(xs))
                                       .AddTo(this);
    }

    [Button]
    public void TestSend(string h = @"C:\Users\HaoAsakura\Downloads\Telegram Desktop\real_scan_1.obj")
    {
        
       
        print(h);
        
        List<Point> vertex= new List<Point>();
        List<MeshTriangle> triangle = new List<MeshTriangle>();
        
        using (FileStream fstream = File.OpenRead(h))
        {
            // преобразуем строку в байты
            byte[] array = new byte[fstream.Length];
            // считываем данные
            fstream.Read(array, 0, array.Length);
            // декодируем байты в строку
            string textFromFile = System.Text.Encoding.Default.GetString(array);

            var splitedString = textFromFile.Split('\n').Select(s => s.Split(new char[] {' '}, 2)).ToArray();


            foreach (var a in splitedString)
            {
                switch (a[0])
                {
                    case "v":
                        vertex.Add(StringToPoint(a[1]));
                        break;
                    case "f":
                        triangle.Add(StringToMeshTriangle(a[1]));
                        
                        
                        break;
                }
            }

            //
            // var convexHull = vertex.Select(v => new double[] {v.x, v.y, v.z}).ToArray();
            //
            // //convexHull.Result.Faces.Select(f => f.Vertices.Select(v => v.))
            //
            // var T = MIConvexHull.Triangulation.CreateDelaunay(convexHull);
            //
            // var vertex2 =  T.Cells.Select(c => c.Vertices.Select(v => new Point(v.Position[0], v.Position[1], v.Position[2])).Join());
            //     
            
            
            UpdateModel(new ScanningResult(triangle.ToArray(), vertex.ToArray()));

        }
    }

    // ReSharper disable Unity.PerformanceAnalysis
    [Button]
    public void SendGoal()
    {
        RegisterGoal();
        _scanningActionClient.SendGoal();
    }


    private void RegisterGoal()
    {
        DataLog.Log($"{DateTime.Now.TimeOfDay}: {app.model.ScanArea.Value.ToScanningGoal()}");
        _scanningActionClient.Order = app.model.ScanArea.Value.ToScanningGoal();
        
    }

    [Button]
    public void CancelGoal()
    {
        _scanningActionClient.CancelGoal();
    }

    public void UpdateModel(ScanningResult result)
    {
        if(result != null)
        {
            var mv = ScanningResultToMesh(result);

            var go = new GameObject("ScanModel", typeof(MeshFilter), typeof(MeshRenderer));
            Destroy(go);

            go.transform.position = mv.vector;
            
            go.GetComponent<MeshFilter>().mesh = mv.mesh;
            
            app.model.BlankModels.Add(new BlankModel(new BlankPose(mv.vector, Vector3.zero, 1f), go, BlankType.Scan));
        }
    }

    private MeshVector ScanningResultToMesh(ScanningResult result)
    {
        var mesh = new Mesh();
        
        
        var vertex = result.vertices.Select(v => TransformExtensions.Ros2Unity(new Vector3((float)v.x, (float)v.y, (float)v.z))).ToArray();

        // var triangle = (from t in result.triangles
        //     //let n = new int[3] { (int)t.vertex_indices[0], (int)t.vertex_indices[2], (int)t.vertex_indices[1] }
        //     from tt in t.vertex_indices
        //     select (int)tt).ToArray();
        
        var triangle = (from t in result.triangles
            from tt in t.vertex_indices
            select (int)tt).ToArray();

        #region find center of model

        Vector3 delta = Vector3.zero;
        
        delta.x = vertex.Average(p => p.x);
        delta.y = vertex.Average(p => p.y);
        delta.z = vertex.Average(p => p.z);
        
        #endregion
        
        vertex = vertex.Select(v => new Vector3(v.x, v.y, v.z) - delta).ToArray();
        
        mesh.vertices = vertex;
        mesh.triangles = triangle;
        //mesh.normals = CustomMesh.GetNormals(vertex, triangle);
        mesh.RecalculateBounds();
        mesh.RecalculateNormals();
        mesh.RecalculateTangents();
        mesh.Optimize();
        return new MeshVector()
        {
            mesh = mesh,
            vector = delta
        };
    }

    private static Vector3 StringToVector3(string sVector)
    {
        var s = sVector.Split(' ');

        
        
        return new Vector3(float.Parse(s[0], CultureInfo.InvariantCulture), 
                           float.Parse(s[1], CultureInfo.InvariantCulture), 
                           float.Parse(s[2], CultureInfo.InvariantCulture));
    }
    
    private static Point StringToPoint(string sVector)
    {
        var s = sVector.Split(' ');

        
        
        return new Point(double.Parse(s[0], CultureInfo.InvariantCulture), 
            double.Parse(s[1], CultureInfo.InvariantCulture), 
            double.Parse(s[2], CultureInfo.InvariantCulture));
    }

    private static int[] StringToIntArray(string sVector)
    {
        var s = sVector.Split(' ');

        return new int[]
        {
            int.Parse(s[0]),
            int.Parse(s[1]),
            int.Parse(s[2])
        };
    }
    
    private static MeshTriangle StringToMeshTriangle(string sVector)
    {
        var s = sVector.Split(' ');

        return new MeshTriangle(new uint[]{ uint.Parse(s[0]), uint.Parse(s[1]), uint.Parse(s[2])});
    }
    

    private struct MeshVector
    {
        public Mesh mesh;
        public Vector3 vector;
    }


}

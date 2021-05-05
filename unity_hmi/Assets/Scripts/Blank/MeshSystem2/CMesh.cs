using MeshSystem.MeshAddition;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;
using MeshSystem;
using MeshSystem.Surfaces;

namespace MeshSystem
{
    public struct CMesh
    {
        public int id;

        //public Vector3 Position;
        //public Vector3 Rotation;
        //public float Scale;

        public Triangle[] Triangles { private set; get; }
        public Vector3[] Vertices { private set; get; }
        public List<Surface> Surfaces { private set; get; }


        public CMesh(Vector3[] vertices, int[] triangles, Vector3 position, Vector3 rotation, float scale)
        {
            var bufId = CMeshPool.GetAvailableIndex();

            UnityEngine.Debug.Log($"position: {position}, v1: {vertices[0]}");

            Vertices = vertices.Select(v => v * scale + position).ToArray();

            UnityEngine.Debug.Log($"v1: {vertices[0]}");

            Triangles = new Triangle[triangles.Length / 3];

            Triangles = Triangles
                    .AsParallel()
                    .Select((t, i) => new Triangle(bufId, i, triangles[i * 3],
                                                             triangles[i * 3 + 1],
                                                             triangles[i * 3 + 2]))
                    .ToArray();

            id = bufId;

            Surfaces = new List<Surface>();

            UnityEngine.Debug.Log($"InitMesh: {id}");

            //Scale = scale;
            //Position = position;
            //Rotation = rotation;

            CMeshPool.Push(this);

            DebugVerticesLog();
        }

        public CMesh(Vector3[] vertices, int[] triangles, Transform transform)
        {
            Stopwatch stopWatch = new Stopwatch();
            stopWatch.Start();

            var bufId = CMeshPool.GetAvailableIndex();

            //Vertices = vertices.Select(v => transform.rotation * (v * transform.localScale.x + transform.position)).ToArray();

            Vertices = vertices.Select(v => transform.rotation * v * transform.localScale.x + transform.position).ToArray();

            Triangles = new Triangle[triangles.Length / 3];

            Triangles = Triangles
                    .AsParallel()
                    .Select((t, i) => new Triangle(bufId, i, triangles[i * 3],
                                                             triangles[i * 3 + 1],
                                                             triangles[i * 3 + 2]))
                    .ToArray();

            id = bufId;

            Surfaces = new List<Surface>();

            UnityEngine.Debug.Log($"InitMesh: {id}");


            CMeshPool.Push(this);

            //DebugVerticesLog();

            stopWatch.Stop();

            TimeSpan ts = stopWatch.Elapsed;

            // Format and display the TimeSpan value.
            string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                ts.Hours, ts.Minutes, ts.Seconds,
                ts.Milliseconds / 10);
            UnityEngine.Debug.Log(elapsedTime.ToString());
        }

        public void InitSurfaces()
        {
            Stopwatch stopWatch = new Stopwatch();
            stopWatch.Start();

            // устанавливаем прилегающие треугольники
            for (int i = 0; Triangles.Length > i; i++)
            {
                Triangles[i].SetLinkedTriangle();
            }

            // заполняем поверхности треугольниками
            Surfaces.Add(new Surface(id, 0, 0));

            do
            {
                List<int> join = new List<int>();

                foreach (var a in Surfaces)
                {
                    join.AddRange(a.triangles);
                }

                var queue = Triangles.Select(t => t.Index).Except(join);

                if (queue.Count() == 0)
                    break;

                // узнаём оставшиеся трианглы

                Surfaces.Add(new Surface(id, Surfaces.Count(), queue.First()));

            } while (true);

            foreach (var a in Surfaces)
            {
                a.SetColorDebuging();
            }

            stopWatch.Stop();

            TimeSpan ts = stopWatch.Elapsed;

            // Format and display the TimeSpan value.
            string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                ts.Hours, ts.Minutes, ts.Seconds,
                ts.Milliseconds / 10);
            //print(elapsedTime.ToString());
        }

        private void DebugVerticesLog()
        {
            foreach (var a in Triangles)
            {
                foreach(var b in a.Edges)
                {
                    UnityEngine.Debug.Log("V");
                    UnityEngine.Debug.DrawLine(b.v0.Position, b.v1.Position, Color.red, 1000f);
                }
            }

            
        }
    }
}



using System;
using System.Linq;
using MeshSystem;
using thelab.mvc;
using UnityEngine;
using Sirenix.OdinInspector;


namespace Blank.MeshTools
{
    public class MeshSplitter : Element<DemonOLPApplication>
    {
        private Camera _camera;
        [SerializeField]
        private (Vector3, Vector3) _cutPlanePoints;

        private GameObject go;
        
        
        private void Start()
        {
            _camera = Camera.main;
        }

        private void Update()
        {
            if (Input.GetMouseButtonDown(0))
            {
                if (Physics.Raycast(_camera.ScreenPointToRay(Input.mousePosition), 
                    out RaycastHit hit, 
                    100000f,
                    LayerMask.GetMask("Blank")))
                {
                    _cutPlanePoints.Item1 = hit.point;
                }
            }

            if (Input.GetMouseButtonUp(0))
            {
                if (Physics.Raycast(_camera.ScreenPointToRay(Input.mousePosition), 
                    out RaycastHit hit, 
                    100000f,
                    LayerMask.GetMask("Blank")))
                {
                    _cutPlanePoints.Item2 = hit.point;
                    go = hit.collider.gameObject;
                    SplitMesh();
                }
            }
        }

        private void SplitMesh()
        {
            Plane plane = new Plane(
                _cutPlanePoints.Item1 - go.transform.position, 
                _cutPlanePoints.Item2 - go.transform.position, 
                _cutPlanePoints.Item2 + Vector3.up - go.transform.position);

            Debug.DrawRay((_cutPlanePoints.Item1 + _cutPlanePoints.Item2) / 2, plane.normal * 100000f, Color.yellow, 1000f);
            DrawPlane((_cutPlanePoints.Item1 + _cutPlanePoints.Item2) / 2, plane.normal * 100f);
    
            

            // var mesh = go.GetComponent<MeshFilter>().mesh;
            //
            // var newVerteces = mesh.vertices.Where(v => plane.GetSide(v)).Select((v, i) => (v, i));
            //
            // var newTriangles = mesh.triangles.Where(t => newVerteces.Select(v => v.i).Contains(t));
            //
            // var cmesh = new CMesh(mesh.vertices, mesh.triangles, go.transform);
            //
            // cmesh.Triangles.Where(t => t.GetVertexIndeces.Contains(newVerteces.Select(v => v.i).Any()));
            //
            // var tr =
            //     from t in cmesh.Triangles
            //     from v in newVerteces
            //     where t.GetVertexIndeces.Contains(v.i)
            //     select t;
            //
            //
            // var Triangles = 
            //     mesh.triangles
            //         .Where((t, i) => )
            //         .Select((t, i) => new Triangle(bufId, i, triangles[i * 3],
            //         triangles[i * 3 + 1],
            //         triangles[i * 3 + 2]))
            //     .ToArray();
            //
            //
            // print($"oldSize: {mesh.vertices.Length}, {mesh.triangles.Length} \n" +
            //       $"newSize: {newVerteces.Count()}, {newTriangles.Count()}");
            //
            // mesh.vertices = newVerteces.Select(v => v.v).ToArray();
            // mesh.triangles = newTriangles.ToArray();
            //
            // go.GetComponent<MeshFilter>().mesh = mesh;
            //
            // mesh.vertices = newVerteces.ToArray();

        }
        
        private void DrawPlane(Vector3 position, Vector3 normal) 
        {
            Vector3 v3 = new Vector3();
 
            if (normal.normalized != Vector3.forward)
                v3 = Vector3.Cross(normal, Vector3.forward).normalized * normal.magnitude;
            else
                v3 = Vector3.Cross(normal, Vector3.up).normalized * normal.magnitude;;
     
            var corner0 = position + v3;
            var corner2 = position - v3;
            var q = Quaternion.AngleAxis(90.0f, normal);
            v3 = q * v3;
            var corner1 = position + v3;
            var corner3 = position - v3;
 
            Debug.DrawLine(corner0, corner2, Color.green);
            Debug.DrawLine(corner1, corner3, Color.green);
            Debug.DrawLine(corner0, corner1, Color.green);
            Debug.DrawLine(corner1, corner2, Color.green);
            Debug.DrawLine(corner2, corner3, Color.green);
            Debug.DrawLine(corner3, corner0, Color.green);
            Debug.DrawRay(position, normal, Color.red);
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
}
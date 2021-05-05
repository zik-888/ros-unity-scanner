using MeshSystem;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using thelab.mvc;
using UnityEngine;

namespace Assets.Scripts.Command.PutPointOnSurface
{
    public class PutThreePointOnSurface : Element<DemonOLPApplication>
    {
        public Action<MarchingFormingPoint[]> finishMethod;
        public Action<MarchingFormingPoint> putPoint;

        protected int idFormPoint = 0;

        MarchingFormingPoint[] points = new MarchingFormingPoint[3];


        // Update is called once per frame
        void Update()
        {
            if (!Input.GetMouseButtonDown(0)
                || !Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out RaycastHit hit)
                || hit.triangleIndex == -1)
                return;

            print($"triangleIndex: {hit.triangleIndex}");

            var angles = CMeshPool.GetMesh(0).Triangles[hit.triangleIndex].GetEulerAngle();

            points[idFormPoint] = new MarchingFormingPoint(hit.triangleIndex, hit.point, angles);

            putPoint(new MarchingFormingPoint(hit.triangleIndex, hit.point, angles));

            idFormPoint++;

            if (idFormPoint == points.Length)
            {
                finishMethod(points);
                Destroy(this);
            }
        }
    }
}

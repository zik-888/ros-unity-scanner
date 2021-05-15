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
    public class PutManyPointOnSurface : Element<DemonOLPApplication>
    {
        public Action<FormingPoint> putPoint;

        protected int idFormPoint = 0;


        // Update is called once per frame
        void Update()
        {
            if (!Input.GetMouseButtonDown(0)
                || !Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out RaycastHit hit)
                || hit.triangleIndex == -1)
                return;
            

            var angles = CMeshPool.GetMesh(0).Triangles[hit.triangleIndex].GetEulerAngle();

            putPoint(new CommandPose(hit.point, angles));

            idFormPoint++;
        }
    }
}

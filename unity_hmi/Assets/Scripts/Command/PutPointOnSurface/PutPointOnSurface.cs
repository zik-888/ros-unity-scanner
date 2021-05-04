using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using thelab.mvc;
using UnityEngine;

namespace Assets.Scripts.Command.PutPointOnSurface
{
    public class PutPointOnSurface : Element<DemonOLPApplication>
    {
        public Action<FormingPoint> getPose;

        // Update is called once per frame
        void Update()
        {
            if (!Input.GetMouseButtonDown(0)
                || !Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out RaycastHit hit))
                return;

            //var angles = CustomMeshPool.GetMesh(0).Triangles[hit.triangleIndex].GetEulerAngle;

            getPose(new CommandPose(hit.point, Vector3.zero));

            Destroy(this);
        }
    }
}

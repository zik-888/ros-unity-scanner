using System;
using thelab.mvc;
using UnityEngine.Serialization;

namespace Robots.RobotModel
{
    public class RobotModel : Element<DemonOLPApplication>
    {
        public RobotType type;
        public ForwardKinematic fk = new ForwardKinematic();


        private void Start()
        {
            app.model.RobotModels.Add(this);
            
        }
        
        

    }
}
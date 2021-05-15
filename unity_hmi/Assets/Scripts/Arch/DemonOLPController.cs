using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Robots.RobotModel;
using thelab.mvc;
using UnityEngine;

public class DemonOLPController : Controller<DemonOLPApplication>
{
    private void Update()
    {
        //print($"F: {app.model.ScanArea.Value.firstPoint}, D: {app.model.ScanArea.Value.secondPoint}");
    }

    private void Awake()
    {
        //app.model.commandModels.Add(new LineCommandModel(0));
        //app.model.commandModels.Add(new LineCommandModel(0));

        //var trjBfrCorr = new CommandPose[]
        //{
        //    new CommandPose(new Vector3(24.0729f, -321.951f, 224.89f), Vector3.zero),
        //    new CommandPose(new Vector3(51.071f, -422.082f, 218.181f), Vector3.zero),
        //    new CommandPose(new Vector3(36.867f, -370.201f, 191.598f), Vector3.zero),
        //    new CommandPose(new Vector3(36.014f, -367.037f, 191.901f), Vector3.zero),
        //    new CommandPose(new Vector3(34.332f, -360.78f, 193.042f), Vector3.zero),
        //    new CommandPose(new Vector3(32.698f, -354.679f, 194.886f), Vector3.zero),
        //    new CommandPose(new Vector3(31.130f, -348.81f, 197.41f), Vector3.zero),
        //    new CommandPose(new Vector3(29.649f, -343.24f, 200.583f), Vector3.zero),
        //    new CommandPose(new Vector3( 28.27f, -338.039f, 204.369f), Vector3.zero),
        //    new CommandPose(new Vector3(27.013f, -333.267f, 208.721f), Vector3.zero),
        //    new CommandPose(new Vector3(25.891f, -328.985f, 213.588f), Vector3.zero),
        //    new CommandPose(new Vector3(24.918f, -325.24f, 218.91f), Vector3.zero),
        //    new CommandPose(new Vector3( 24.10f, -322.081f, 224.622f), Vector3.zero),
        //    new CommandPose(new Vector3(42.809f, -392.121f, 194.507f), Vector3.zero),
        //    new CommandPose(new Vector3(41.142f, -385.993f, 192.787f), Vector3.zero),
        //    new CommandPose(new Vector3(44.422f, -398.029f, 196.911f), Vector3.zero),
        //    new CommandPose(new Vector3(45.961f, -403.645f, 199.972f), Vector3.zero),
        //    new CommandPose(new Vector3(47.408f, -408.904f, 203.652f), Vector3.zero),
        //    new CommandPose(new Vector3(48.745f, -413.741f, 207.908f), Vector3.zero),
        //    new CommandPose(new Vector3( 49.95f, -418.098f, 212.687f), Vector3.zero),
        //    new CommandPose(new Vector3(51.026f, -421.922f, 217.931f), Vector3.zero),
        //    new CommandPose(new Vector3(39.440f, -379.721f, 191.773f), Vector3.zero),
        //    new CommandPose(new Vector3(37.724f, -373.376f, 191.476f), Vector3.zero)
        //};


        //var trjAfrCorr = new CommandPose[]
        //{
        //    new CommandPose(new Vector3(70.8794f, -321.872f, 216.756f), Vector3.zero),
        //    new CommandPose(new Vector3(90.2843f, -424.203f, 214.948f), Vector3.zero),
        //    new CommandPose(new Vector3(80.7800f, -372.399f, 186.030f), Vector3.zero),
        //    new CommandPose(new Vector3(80.3902f, -369.173f, 186.147f), Vector3.zero),
        //    new CommandPose(new Vector3(79.1307f, -362.903f, 186.945f), Vector3.zero),
        //    new CommandPose(new Vector3(77.7815f, -356.438f, 188.577f), Vector3.zero),
        //    new CommandPose(new Vector3(76.6146f, -350.478f, 190.758f), Vector3.zero),
        //    new CommandPose(new Vector3(75.5270f, -344.644f, 193.685f), Vector3.zero),
        //    new CommandPose(new Vector3(74.3746f, -339.198f, 197.183f), Vector3.zero),
        //    new CommandPose(new Vector3(73.5170f, -334.300f, 201.282f), Vector3.zero),
        //    new CommandPose(new Vector3(72.6110f, -329.567f, 205.992f), Vector3.zero),
        //    new CommandPose(new Vector3(71.6858f, -325.485f, 211.136f), Vector3.zero),
        //    new CommandPose(new Vector3(70.8552f, -322.182f, 216.647f), Vector3.zero),
        //    new CommandPose(new Vector3(85.1116f, -394.661f, 189.959f), Vector3.zero),
        //    new CommandPose(new Vector3(83.9525f, -388.442f, 187.935f), Vector3.zero),
        //    new CommandPose(new Vector3(86.0373f, -400.600f, 192.679f), Vector3.zero),
        //    new CommandPose(new Vector3(87.1405f, -406.130f, 195.969f), Vector3.zero),
        //    new CommandPose(new Vector3(88.0143f, -411.314f, 199.860f), Vector3.zero),
        //    new CommandPose(new Vector3(88.8922f, -416.052f, 204.403f), Vector3.zero),
        //    new CommandPose(new Vector3(89.6999f, -420.363f, 209.365f), Vector3.zero),
        //    new CommandPose(new Vector3(90.1755f, -424.064f, 214.787f), Vector3.zero),
        //    new CommandPose(new Vector3(82.6342f, -382.295f, 186.604f), Vector3.zero),
        //    new CommandPose(new Vector3(81.3233f, -375.773f, 185.999f), Vector3.zero)
        //};

        //for (int i = 0; trjBfrCorr.Length > i; i++)
        //{
        //    //var buf = trjBfrCorr[i].position;
        //    //trjBfrCorr[i].position.y = buf.z;
        //    //trjBfrCorr[i].position.z = buf.y;

        //    //trjBfrCorr[i].position = trjBfrCorr[i].position * 0.01f;

        //    //trjBfrCorr[i].position = RosSharp.TransformExtensions.Ros2Unity(trjBfrCorr[i].position);
        //}


        //app.model.commandModels[0].poses = trjBfrCorr;
        //app.model.commandModels[1].poses = trjAfrCorr;
    }

    
}

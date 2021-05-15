using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UniRx;
using System.Linq;
using System;
using System.Text;
using System.IO;
using Robots.RobotModel;

public enum PathFormMethod { PointToPoint, PickMarching, DrawProjection };


[Serializable]
public class DemonOLPModel : Model<DemonOLPApplication>
{
    public Material[] materials;

    
    
    public ReactiveProperty<SceneState> SceneState = new ReactiveProperty<SceneState>(new SceneState());

    public ReactiveCollection<RobotModel> RobotModels = new ReactiveCollection<RobotModel>();
    public ReactiveCollection<BlankModel> BlankModels = new ReactiveCollection<BlankModel>();
    public ReactiveCollection<CommandModel> CommandModels = new ReactiveCollection<CommandModel>();

    public ReactiveProperty<bool> IsSelectScanArea = new ReactiveProperty<bool>(false);
    public ReactiveProperty<ScanArea> ScanArea = new ReactiveProperty<ScanArea>(new ScanArea());

    public ReactiveProperty<bool>[] rosNodeStatus = new ReactiveProperty<bool>[]
    {
        new ReactiveProperty<bool>(false), 
        new ReactiveProperty<bool>(false), 
        new ReactiveProperty<bool>(false)
    };
    

    //public CommandModel[] commandModelsList;

    //public bool A = true;
    //public int B = 0;
    private void Update()
    {

        try
        {
            //sceneState.Value = new SceneState(A, 0);
            //commandModelsList = commandModels.ToArray();
            //blankModels[0].BlankPose = new BlankPose(Vector3.one * (Mathf.Sin(Time.time) - 0.5f), Vector3.zero, 1);
            //blankModels[0].BlankPose.position = Vector3.one * (Mathf.Sin(Time.time) - 0.5f);
            //print(sceneState.Value.isMainActive);
        }
        catch { }
    }


}

public class SceneState
{
    public bool isMainActive = true;
    public int loadableBlank = -1;

    public SceneState(bool isMainActive = true, int loadableBlank = -1)
    {
        this.isMainActive = isMainActive;
        this.loadableBlank = loadableBlank;
    }

}



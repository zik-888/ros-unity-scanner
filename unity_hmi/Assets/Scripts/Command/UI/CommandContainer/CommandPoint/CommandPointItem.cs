using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UnityEngine.UI;
using UniRx;
using System.Linq;
using Assets.Scripts.Command.PutPointOnSurface;

public class CommandPointItem : Element<DemonOLPApplication>
{
    protected Transform Container3DTransform;
    [SerializeField]
    protected Text headerText;
    [SerializeField]
    protected int commandId;
    [SerializeField]
    protected Toggle putButton = default;
    [SerializeField]
    protected CommandPointProperties properties = default;
    [SerializeField]
    protected GameObject pointPrefab = default;
    protected GameObject PointObject = null;

    protected PutPointOnSurface PUTPoint;

    public int CommandId
    {
        set 
        { 
            commandId = value;
            headerText.text = $"C: {commandId}, P: {id}";
            name = $"C: {commandId}, P: {id}";
        }
    }

    [SerializeField]
    protected int id;

    public int Id
    {
        set 
        { 
            id = value;
            headerText.text = $"C: {commandId}, P: {id}";
            name = $"C: {commandId}, P: {id}";
        }
    }

    private void Start()
    {
        if (app.model.CommandModels[commandId].GetType().BaseType == typeof(SpecialCommandModel))
        {
            var model = (SpecialCommandModel)app.model.CommandModels[commandId];
            
            model.ObserveEveryValueChanged(x => x.FormingPoints[id])
                 .Subscribe(x => PoseChangeIntoModel(x))
                 .AddTo(this);
            
            putButton.gameObject.SetActive(false);
        }
        else
        {
            app.model.CommandModels[commandId]
                .ObserveEveryValueChanged(x => x.poses[id])
                .Subscribe(x => PoseChangeIntoModel(x))
                .AddTo(this);
        }
        
        
    }

    protected void PoseChangeIntoModel(FormingPoint pose)
    {

        if(pose != null)
        {
            properties.SetProperties(pose);

            if (PointObject == null)
            {
                PointObject = Instantiate(pointPrefab, Container3DTransform);
                PointObject.GetComponent<CommandPointController>().SetPose = P;
            }

            PointObject.transform.position = pose.Position;
            PointObject.transform.rotation = Quaternion.Euler(pose.Rotation);
        }
        
    }

    public void P(MarchingFormingPoint point)
    {
        app.model.CommandModels[commandId].poses[id] = new CommandPose(point.Position, point.Rotation);
    }

    public void InitItem(int commandId, int pointId, Transform container3DTransform)
    {
        this.commandId = commandId;
        id = pointId;

        headerText.text = $"C: {commandId}, P: {pointId}";
        name = $"C: {commandId}, P: {pointId}";

        this.Container3DTransform = container3DTransform;
    }

    public void PutPoint(bool value)
    {
        switch (value)
        {
            case true:

                PUTPoint = gameObject.AddComponent<PutPointOnSurface>();
                PUTPoint.getPose = GetPose;

                break;
            case false:
                Destroy(PUTPoint);
                break;
        }   
    }

    public void GetPose(FormingPoint pose)
    {
        print(pose.Position.ToString());
        app.model.CommandModels[commandId].poses[id] = (CommandPose)pose;
        putButton.isOn = false;

        if(PointObject != null)
        {
            PointObject.transform.position = pose.Position;
            PointObject.transform.rotation = Quaternion.Euler(pose.Rotation);
        }
    }

    private void OnDestroy()
    {
        Destroy(PointObject);
        Destroy(gameObject);
    }
}

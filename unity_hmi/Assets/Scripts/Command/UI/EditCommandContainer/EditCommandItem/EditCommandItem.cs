using System;
using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using System.Linq;
using Assets.Scripts.Command.PerformanceMethod;
using Command.UI;
using UnityEngine.Serialization;

public class EditCommandItem : RenderForCommandItem
{
    // components
    [SerializeField]
    protected Toggle putButton;
    // prefabs
    [SerializeField]
    private GameObject formingPointPrefab = default;
    // defaultFields
    protected readonly List<GameObject> FormingPointList = new List<GameObject>();
    protected ICommandPerformance PerformanceMethod;


    // UnityMethods
    private void Start()
    {
        UpdateType();
        UpdatePointItemList();
    }
    
    // OtherMethods
    private void UpdateType()
    {
        var type = app.model.CommandModels[id].type;
        
        switch (type) 
        {
            case CommandType.Marching:
                PerformanceMethod = gameObject.AddComponent<MarchingMethodPerformance>();
                break;
            case CommandType.CircleInterp:
                PerformanceMethod = gameObject.AddComponent<CircleMethodPerformance>();
                break;
            case CommandType.LineInterp:
                PerformanceMethod = gameObject.AddComponent<LineMethodPerformance>();
                break;
            case CommandType.SplineInterp:
                PerformanceMethod = gameObject.AddComponent<SplineMethodPerformance>();
                break;
        }
    }

    public void StartMethod(bool value)
    {
        ClearFormPoints();
        switch (value)
        {
            case true:
                PerformanceMethod.StartMethod(AddFormingPoint, FinishMethod);
                break;
            case false:
                PerformanceMethod.CancelMethod();
                break;
        }
    }

    protected void FinishMethod(FormingPoint[] points)
    {
        
        ClearFormPoints();
            
        putButton.isOn = false;
        print("FinishMethod");

        var c = from t in points
                select new CommandPose(t.Position, t.Rotation);

        app.model.CommandModels[id].poses = c.ToArray();

        // Место для отображения

        UpdatePointItemList();
    }

    public void AddFormingPoint(FormingPoint point)
    {
        var a = Instantiate(formingPointPrefab, Container3DTransform);
        a.transform.position = point.Position;
        FormingPointList.Add(a);

        if (app.model.CommandModels[id].GetType().BaseType == typeof(SpecialCommandModel))
        {
            var model = (SpecialCommandModel) app.model.CommandModels[id];
            model.FormingPoints[FormingPointList.Count - 1] = point;
            print($"points: {point}");
        }
    }

    public void ClearFormPoints()
    {
        foreach (var a in FormingPointList)
        {
            Destroy(a.gameObject);
        }

        FormingPointList.Clear();
    }
}

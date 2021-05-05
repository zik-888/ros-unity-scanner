using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;
using UniRx;

public class BindScanArea : Element<DemonOLPApplication>
{
    public InputField x1;
    public InputField y1;
    public InputField x2;
    public InputField y2;

    //public UnityEngine.UI.Toggle isSelectArea;

    private void Start()
    {
        app.model.ScanArea
                 .ObserveEveryValueChanged(x => x.Value)
                 .Subscribe(xs => SetScanningAreaView(xs))
                 .AddTo(this);
    }

    public void BindIsSelected(bool value)
    {
        DataLog.Log($"Plz select a scan area");
        app.model.IsSelectScanArea.Value = value;
    }

    public void SetScanningAreaView(ScanArea value)
    {
        if (value != null)
        {
            DataLog.Log($"ScanArea selected: {value}");


            x1.text = Round(value.firstPoint.x, 100).ToString();
            y1.text = Round(value.firstPoint.y, 100).ToString();
            x2.text = Round(value.secondPoint.x, 100).ToString();
            y2.text = Round(value.secondPoint.y, 100).ToString();
        }
    }

    public void ReloadScannAreaModel()
    {

    }

    private float Round(float value, float order)
    {
        return ((int)(value * order)) / order;
    }
}

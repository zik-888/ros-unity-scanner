using System;
using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using thelab.mvc;
using UniRx;
using UnityEngine;
using UnityEngine.UI;

public class ColorControll : Element<DemonOLPApplication>
{
    public Image image;
    
    public Color falseColor = Color.red;
    public Color trueColor = Color.green;

    public NodeType nodeType;

    private void Start()
    {
        app.model.rosNodeStatus
            .ObserveEveryValueChanged(x => x.Value)
            .Subscribe(xs => ActiveConnect(xs[(int)nodeType]))
            .AddTo(this);
    }

    [Button]
    public void ActiveConnect(bool value)
    {
        switch (value)
        {
            case true:
                image.color = trueColor;
                break;
            case false:
                image.color = falseColor;
                break;
        }
    }
    
    

}

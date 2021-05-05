using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;

public class PointToPointView : Element<DemonOLPApplication>
{
    public void AddCommand()
    {
        app.model.CommandModels.Add(new PointToPointCommandModel());
    }
}

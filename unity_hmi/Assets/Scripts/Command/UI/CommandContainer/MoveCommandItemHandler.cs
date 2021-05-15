using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using static UnityEngine.UI.Extensions.ReorderableList;

public class MoveCommandItemHandler : Element<DemonOLPApplication>
{

    public void Droped(ReorderableListEventStruct value)
    {

        app.model.CommandModels.Move(value.FromIndex, value.ToIndex);
    }

}

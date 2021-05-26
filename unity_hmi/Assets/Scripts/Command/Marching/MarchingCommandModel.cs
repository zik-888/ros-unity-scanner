using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MarchingCommandModel : SpecialCommandModel
{
    public MarchingCommandModel(int blankNum)
    {
        this.blankNum = blankNum;
        type = CommandType.Marching;
        FormingPoints = new MarchingFormingPoint[3];
    }
}

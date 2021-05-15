using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;

public enum CommandType { PtP, Marching, OnAnchorPoints, LineInterp, CircleInterp, SplineInterp }

[System.Serializable]
public class CommandModel
{
    public CommandType type;
    public CommandPose[] poses;
    public int blankNum = -1;

}

[System.Serializable]
public class PointToPointCommandModel : CommandModel
{
    public PointToPointCommandModel()
    {
        type = CommandType.PtP;
        poses = new CommandPose[1];
    }
}

public class SpecialCommandModel : CommandModel
{
    
    public FormingPoint[] FormingPoints;
}


public class OnAnchorPointsCommandModel : SpecialCommandModel
{
    public OnAnchorPointsCommandModel(int blankNum)
    {
        this.blankNum = blankNum;
        type = CommandType.OnAnchorPoints;
        FormingPoints = new CommandPose[2];
    }
}


public class CircleCommandModel : SpecialCommandModel
{

    public CircleCommandModel(int blankNum)
    {
        this.blankNum = blankNum;
        type = CommandType.CircleInterp;
        FormingPoints = new CommandPose[3];
    } 
}

public class SplineCommandModel : SpecialCommandModel
{

    public SplineCommandModel(int blankNum)
    {
        this.blankNum = blankNum;
        type = CommandType.SplineInterp;
    }
}

public class LineCommandModel : CommandModel
{

    public LineCommandModel(int blankNum)
    {
        this.blankNum = blankNum;
        type = CommandType.LineInterp;
    }
}



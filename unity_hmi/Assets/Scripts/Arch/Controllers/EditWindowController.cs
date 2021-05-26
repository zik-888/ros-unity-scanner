using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using MeshSystem;
using Sirenix.Serialization;

public class EditWindowController : Element<DemonOLPApplication>
{
    [SerializeField]
    protected Transform TDContainer;

    protected GameObject blank;
    
    [OdinSerialize]
    protected CMesh mesh;

    [SerializeField]
    protected new maxCamera camera;

    private void OnEnable()
    {

        int a = app.model.SceneState.Value.loadableBlank;

        try
        {
            blank = Instantiate(app.model.BlankModels[a].BlankObject, TDContainer);
            Destroy(blank.GetComponent<BlankController>());
            blank.AddComponent<BlankControllerInEditorMod>();

            camera.target = blank.transform;

            print(blank.transform);

            //mesh = new CMesh(app.model.blankModels[a].Mesh.vertices, 
            //                 app.model.blankModels[a].Mesh.triangles,
            //                 app.model.blankModels[a].BlankPose.position,
            //                 app.model.blankModels[a].BlankPose.rotation,
            //                 app.model.blankModels[a].BlankPose.scale);

            mesh = new CMesh(app.model.BlankModels[a].Mesh.vertices,
                app.model.BlankModels[a].Mesh.triangles,
                blank.transform);

            mesh.InitSurfaces();



        }
        catch
        {
            print("EditWindowControllerBug");
        }
    }

    private void OnDisable()
    {
        mesh.Dispose();
        Destroy(blank);
    }

    public void ToMainScene()
    {
        app.model.SceneState.Value = new SceneState();
    }

    public void AddMarchingCommand()
    {
        app.model.CommandModels.Add((CommandModel)new MarchingCommandModel(app.model.SceneState.Value.loadableBlank));
    }

    public void AddOnAnchorPointsCommand()
    {
        app.model.CommandModels.Add(new OnAnchorPointsCommandModel(app.model.SceneState.Value.loadableBlank));
    }

    public void AddCircleCommand()
    {
        app.model.CommandModels.Add(new CircleCommandModel(app.model.SceneState.Value.loadableBlank));
    }

    public void AddSplineCommand()
    {
        app.model.CommandModels.Add(new SplineCommandModel(app.model.SceneState.Value.loadableBlank));
    }
    public void AddLineCommand()
    {
        app.model.CommandModels.Add(new LineCommandModel(app.model.SceneState.Value.loadableBlank));
    }
}

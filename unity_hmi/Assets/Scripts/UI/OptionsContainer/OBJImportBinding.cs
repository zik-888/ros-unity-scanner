using Dummiesman;
using System.IO;
using UnityEngine;
using UnityEngine.UI;
using SFB;
using System;
using thelab.mvc;

public class OBJImportBinding : Element<DemonOLPApplication>
{
    public InputField inputField;
    public string objPath = @"Assets/Model DataBase/Test.obj";
    //protected string objPath = @"Assets/Model DataBase/cube/untitled.obj";

    public void ChangePath(string path)
    {
        objPath = @path;
    }

    public void Open()
    {
        string[] path = StandaloneFileBrowser.OpenFilePanel("Overwrite with obj", @"Assets\Model DataBase", "obj", false);

        //print(path[0]);

        if (path.Length != 0)
        {
            DataLog.Log(path[0]);
            print(path[0]);
            objPath = @path[0];
            inputField.text = objPath;
        }
    }

    public void Load()
    {
        if (!File.Exists(objPath))
        {
            Debug.LogError("Please set FilePath in ObjFromFile.cs to a valid path.");

            GameLog.Log("Please set FilePath in ObjFromFile.cs to a valid path.");
        }
        else
        {
            try
            {
                var loadedOBJJ = new OBJLoader().Load(objPath);
                var loadedOBJ = loadedOBJJ.transform.GetChild(0).gameObject;
                Destroy(loadedOBJJ);

                app.model.BlankModels.Add(new BlankModel(new BlankPose(Vector3.forward * 440f, 
                                                                       Vector3.zero, 
                                                                       0.3f), loadedOBJ));
            }
            catch (Exception e)
            {
                GameLog.Log(e.Message);
            }
        }
    }
}


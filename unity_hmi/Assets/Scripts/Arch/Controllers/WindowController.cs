using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UnityEngine.SceneManagement;
using UniRx;

public class WindowController : Element<DemonOLPApplication>
{
    public Transform mainWindow;
    public Transform editWindow;

    private void Start()
    {
        app.model.SceneState
                 .ObserveEveryValueChanged(x => x.Value)
                 .Subscribe(x => LoadMainScene(x))
                 .AddTo(this);

    }

    public void LoadMainScene(SceneState sceneState)
    {
        if (sceneState != null) 
        {


            switch (sceneState.isMainActive)
            {
                case true:
                    SetMain();
                    break;
                case false:
                    SetBlank();
                    break;
            }
        }

        

    }

    protected void SetMain()
    {
        mainWindow.gameObject.SetActive(true);
        editWindow.gameObject.SetActive(false);


    }

    protected void SetBlank()
    {
        mainWindow.gameObject.SetActive(false);
        editWindow.gameObject.SetActive(true);


    }

    

}

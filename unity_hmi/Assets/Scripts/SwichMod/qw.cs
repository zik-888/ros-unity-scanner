using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UnityEngine.SceneManagement;

public class qw : Element<DemonOLPApplication>
{
    public bool Load2Scene = false;
    private bool waitAction = false;

    private void Update()
    {
        if (waitAction != Load2Scene)
        {

            switch (Load2Scene)
            {
                case true:
                    waitAction = true;
                    Load();
                    break;
                case false:
                    waitAction = false;
                    Unload();
                    break;
            }

        }
    }

    private void Start()
    {
        Object.DontDestroyOnLoad(app.model);
    }

    private void Load()
    {
        
        SceneManager.LoadScene(1);
    }

    private void Unload()
    {
        SceneManager.UnloadSceneAsync(1, UnloadSceneOptions.None);
    }


}

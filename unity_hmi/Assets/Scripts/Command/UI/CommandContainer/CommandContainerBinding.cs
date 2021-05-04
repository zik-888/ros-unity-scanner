using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UniRx;
using UnityEditorInternal;
using UnityEngine.UI.Extensions;

public class CommandContainerBinding : Element<DemonOLPApplication>
{
    public UnityEngine.UI.Extensions.ReorderableList list;

    [SerializeField]
    protected GameObject commandItem;

    [SerializeField]
    protected Transform container3DTransform;



    // Start is called before the first frame update
    void Start()
    {

        app.model.CommandModels
                 .ObserveAdd()
                 .Subscribe(x => AddComand(x))
                 .AddTo(this);

        app.model.CommandModels
                 .ObserveRemove()
                 .Subscribe(x => RemoveComand(x))
                 .AddTo(this);

        app.model.CommandModels
                 .ObserveMove()
                 .Subscribe(x => MoveComand(x))
                 .AddTo(this);

        //UpdateList();

    }

    protected void AddComand(CollectionAddEvent<CommandModel> collectionAddEvent)
    {
        if (app.model.SceneState.Value.isMainActive == true)
        {
            var loadBlank = Instantiate(commandItem, list.Content.transform);
            loadBlank.GetComponent<CommandItem>().InitItem(collectionAddEvent.Index, container3DTransform);
        }
        
    }

    protected void RemoveComand(CollectionRemoveEvent<CommandModel> collectionRemoveEvent)
    {
        if (app.model.SceneState.Value.isMainActive == true)
        {
            Destroy(list.Content.GetChild(collectionRemoveEvent.Index).gameObject);

            if (gameObject.activeSelf == true)
                StartCoroutine(RemoveCommandInNextFrame());
        }
    }

    IEnumerator RemoveCommandInNextFrame()
    {
        yield return new WaitForFixedUpdate();

        for (int i = 0; list.Content.childCount > i; i++)
        {
            list.Content.GetChild(i).GetComponent<CommandItem>().Id = i;
            //app.model.blankModels[i].blankController.Id = i;
        }
    }

    protected void MoveComand(CollectionMoveEvent<CommandModel> collectionMoveEvent)
    {
        StartCoroutine(MoveCommandInNextFrame());
    }

    IEnumerator MoveCommandInNextFrame()
    {
        yield return new WaitForFixedUpdate();

        for (int i = 0; list.Content.childCount > i; i++)
        {
            list.Content.GetChild(i).GetComponent<CommandItem>().Id = i;
        }

    }

    public void UpdateList()
    {
        for (int i = 0; list.Content.childCount > i; i++)
        {
            Destroy(list.Content.GetChild(i).gameObject);
        }


        for (int i = 0; app.model.CommandModels.Count > i; i++)
        {
            var a = app.model.CommandModels[i];

            var loadBlank = Instantiate(commandItem, list.Content.transform);
            loadBlank.GetComponent<CommandItem>().InitItem(i, container3DTransform);
        }
    }

    private void OnEnable()
    {
        UpdateList();
    }
}

using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UniRx;
using UnityEditorInternal;
using UnityEngine.UI.Extensions;

public class EditCommandContainerBinding : Element<DemonOLPApplication>
{
    public UnityEngine.UI.Extensions.ReorderableList list;
    
    [SerializeField]
    protected GameObject editCommandItem;

    [SerializeField]
    protected Transform container3DTransform;

    // Start is called before the first frame update
    void Start()
    {
        app.model.CommandModels
                 .ObserveAdd()
                 .Subscribe(x => AddCommand(x))
                 .AddTo(this);

        app.model.CommandModels
                 .ObserveRemove()
                 .Subscribe(x => RemoveCommand(x))
                 .AddTo(this);

    }


    protected void AddCommand(CollectionAddEvent<CommandModel> collectionAddEvent)
    {
        if (collectionAddEvent.Value.type != CommandType.PtP)
        {
            var a = collectionAddEvent.Value;

            if (app.model.SceneState.Value.loadableBlank == a.blankNum)
            {


                var loadBlank = Instantiate(editCommandItem, list.Content.transform);
                loadBlank.GetComponent<EditCommandItem>().InitItem(collectionAddEvent.Index, container3DTransform);
            }
        }
        
    }

    protected void RemoveCommand(CollectionRemoveEvent<CommandModel> collectionRemoveEvent)
    {
        UpdateList();
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

            if (a.type != CommandType.PtP)
            {

                if (app.model.SceneState.Value.loadableBlank == a.blankNum)
                {
                    var loadBlank = Instantiate(editCommandItem, list.Content.transform);
                    loadBlank.GetComponent<EditCommandItem>().InitItem(i, container3DTransform);
                }
            }
        }
    }

    private void OnEnable()
    {
        UpdateList();
    }
}

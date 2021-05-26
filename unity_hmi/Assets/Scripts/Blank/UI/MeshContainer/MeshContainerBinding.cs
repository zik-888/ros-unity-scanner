using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UniRx;
using System.Linq;
using UnityEditorInternal;
using UnityEngine.UI.Extensions;

public class MeshContainerBinding : Element<DemonOLPApplication>
{
    public UnityEngine.UI.Extensions.ReorderableList list;
    public GameObject meshItem;

    protected Transform container3DTransform;


    // Start is called before the first frame update
    void Start()
    {
        app.model.BlankModels
                 .ObserveAdd()
                 .Subscribe(x => AddBlank(x))
                 .AddTo(this);

        app.model.BlankModels
                 .ObserveRemove()
                 .Subscribe(x => RemoveBlank(x))
                 .AddTo(this);

        container3DTransform = GameObject.Find("3D MainContainer").transform;
    }

    protected void AddBlank(CollectionAddEvent<BlankModel> collectionAddEvent)
    {

        var loadBlank = Instantiate(meshItem, list.Content.transform);
        loadBlank.GetComponent<MeshItem>().Id = collectionAddEvent.Index;

        app.model.BlankModels[collectionAddEvent.Index].BlankObject = Instantiate(collectionAddEvent.Value.BlankObject, 
                                                                                  container3DTransform);
        app.model.BlankModels[collectionAddEvent.Index].BlankObject.GetComponent<BlankController>().Id = collectionAddEvent.Index;
    }

    protected void RemoveBlank(CollectionRemoveEvent<BlankModel> collectionRemoveEvent)
    {
        Destroy(list.Content.GetChild(collectionRemoveEvent.Index).gameObject);
        Destroy(collectionRemoveEvent.Value.BlankObject);

        StartCoroutine(RemoveBlankInNextFrame());
    }

    IEnumerator RemoveBlankInNextFrame()
    {
        yield return new WaitForFixedUpdate();

        for (int i = 0; list.Content.childCount > i; i++)
        {
            list.Content.GetChild(i).GetComponent<MeshItem>().Id = i;
            //app.model.blankModels[i].blankController.Id = i;
        }
    }
}

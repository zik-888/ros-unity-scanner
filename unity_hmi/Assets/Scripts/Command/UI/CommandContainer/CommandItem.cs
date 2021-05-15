using System;
using System.Collections;
using System.Collections.Generic;
using Command.UI;
using thelab.mvc;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class CommandItem : RenderForCommandItem
{
    private void Start()
    {
        UpdatePointItemList();
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlaneRender : MonoBehaviour
{
    public RectTransform rectTransform;
    public Rect rect;
    
    
    // Start is called before the first frame update
    void Start()
    {
        rectTransform = gameObject.AddComponent<RectTransform>();
    }

    // Update is called once per frame
    void Update()
    {
            //rect.Set(rect.x, rect.y, rect.width, rect.height);

    }
}

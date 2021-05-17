using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public class CursorAreaSelectedView : Element<DemonOLPApplication>
{
    public Text text;

    public RectTransform canvasTransform;

    public Vector2 vector2;

    public float delta = 50f;
    public Vector2 deltaVector = Vector2.one;

    protected RectTransform selfTransform;

    // Start is called before the first frame update
    void Start()
    {
        selfTransform = GetComponent<RectTransform>();
    }

    // Update is called once per frame
    void Update()
    {
        selfTransform.anchoredPosition3D = Input.mousePosition + new Vector3(deltaVector.x, deltaVector.y) / 2 * delta;

        text.text = $"x: {System.Math.Round(vector2.x, 3)}, y: {System.Math.Round(vector2.y, 3)}";

    }

    private void OnDisable()
    {
        text.enabled = false;
    }

    private void OnEnable()
    {
        text.enabled = true;
    }
}

using System.Collections;
using System.Collections.Generic;
using thelab.mvc;
using UnityEngine;
using UnityEngine.EventSystems;
using UniRx;
using System.Runtime.Serialization;

public class ScanAreaSelect : Element<DemonOLPApplication>
{
    public CursorAreaSelectedView cursor;

    private Camera _camera;
    private bool isActive = false;

    private LineRenderer line;

    public ScanArea areaSelected = new ScanArea();

    public float lineHight = 5f;

    [SerializeField]
    private Transform spaceAreaView = default;
    [SerializeField] protected float scale = 1f / 1000f;
    

    // Start is called before the first frame update
    private void Start()
    {
        _camera = Camera.main;

        line = gameObject.AddComponent<LineRenderer>();
        line.positionCount = 4;
        line.loop = true;
        line.startWidth = 0.01f;
        line.endWidth = 0.01f;
        line.numCornerVertices = 50;
        line.alignment = LineAlignment.View;
        line.enabled = false;

        // app.model.IsSelectScanArea
        //          .ObserveEveryValueChanged(x => x.Value)
        //          .Subscribe(xs => SetActive(xs))
        //          .AddTo(this);
        
        app.model.ScannerModel.DisplayScanArea
            .ObserveEveryValueChanged(x => x.Value)
            .Subscribe(xs => SetActive(xs))
            .AddTo(this);


        cursor.gameObject.SetActive(true);

        cursor.enabled = false;

    }

    public void SetActive(bool value)
    {
        isActive = value;
        line.enabled = value;
        spaceAreaView.gameObject.SetActive(value);

    }

    private void OnMouseDown()
    {
        
        //gameObject.AddComponent<CursorAreaSelectedView>();

        if (isActive && !EventSystem.current.IsPointerOverGameObject())
        {
            line.enabled = true;

            Physics.Raycast(_camera.ScreenPointToRay(Input.mousePosition), out RaycastHit hit, 100000f);
            
            
            if (hit.collider.tag == "SelectedArea")
            {
                cursor.enabled = true;

                areaSelected.firstPoint = ScanArea.Vector3to2(hit.point);
            }
        }
    }

    private void OnMouseDrag()
    {
        if (isActive && !EventSystem.current.IsPointerOverGameObject())
        {
            if (Physics.Raycast(_camera.ScreenPointToRay(Input.mousePosition), out RaycastHit hit, 100000f, LayerMask.GetMask("ScanArea")))
            {
                areaSelected.secondPoint = ScanArea.Vector3to2(hit.point);
                cursor.vector2 = areaSelected.secondPoint;

                line.SetPosition(0, areaSelected.FirstGroup + Vector3.up * lineHight);
                line.SetPosition(1, new Vector3(areaSelected.secondPoint.x, 0, areaSelected.firstPoint.y) + Vector3.up * lineHight);
                line.SetPosition(2, areaSelected.SecondGroup + Vector3.up * lineHight);
                line.SetPosition(3, new Vector3(areaSelected.firstPoint.x, 0, areaSelected.secondPoint.y) + Vector3.up * lineHight);

                SetSpace(areaSelected);
            }
        }
    }

    private void OnMouseUp()
    {

        cursor.enabled = false;
        //Destroy(GetComponent<CursorAreaSelectedView>());

        if (isActive && !EventSystem.current.IsPointerOverGameObject())
        {
            app.model.ScannerModel.ScanArea.Value = areaSelected;
            SetSpace(app.model.ScannerModel.ScanArea.Value);
        }
    }

    private void SetSpace(Vector4 value)
    {
        float x = value.x - value.z;
        float z = value.y - value.w;
        float y = 0.01f;

        spaceAreaView.localScale = new Vector3(x, y, z) * scale;

        x = (value.x + value.z) / 2;
        z = (value.y + value.w) / 2;

        spaceAreaView.position = new Vector3(x, 0f, z);
    }

    private void SetSpace(ScanArea value)
    {
        float x = value.firstPoint.x - value.secondPoint.x;
        float z = value.firstPoint.y - value.secondPoint.y;
        float y = 0.01f;

        spaceAreaView.localScale = new Vector3(x, y, z) * scale;

        x = (value.firstPoint.x + value.secondPoint.x) / 2;
        z = (value.firstPoint.y + value.secondPoint.y) / 2;

        spaceAreaView.position = new Vector3(x, 0f, z);
    }
}

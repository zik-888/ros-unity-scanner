using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using thelab.mvc;
using UnityEngine.EventSystems;
using UnityEngine.Serialization;
using UnityEngine.UI;

namespace Command.UI
{
    public class RenderForCommandItem : Element<DemonOLPApplication>, ISelectHandler, IDeselectHandler
    {
        // components
        [SerializeField]
        protected Text headerText;
        [SerializeField]
        protected LineRenderer line = default;
        // containers
        protected Transform Container3DTransform;
        [FormerlySerializedAs("PointItemContent")] [SerializeField]
        protected Transform pointItemContent = default;
        // prefabs
        [SerializeField]
        protected GameObject pointItemPrefab = default;
        // defaultFields
        protected readonly List<CommandPointItem> PointItemList = new List<CommandPointItem>();
        [SerializeField] protected int id;
        
        
        // Properties
        protected CommandType Type => app.model.CommandModels[id].type;
        
        public int Id
        {
            set
            {
                id = value;

                headerText.text = $"{Type}: {id}";
                name = $"EditCommandItem, {id}";
                Container3DTransform.name = $"Command3DContainer, {id}";
            }

            get => id;
        }
        
        
        // UnityMethods
        private void Start()
        {
            UpdatePointItemList();
        }
        
        public void OnDestroy()
        {
            if(Container3DTransform != null)
                Destroy(Container3DTransform.gameObject);
        }
        
        
        // RenderMethods
        public void LineRender(FormingPoint[] formingPoints)
        {
            switch (Type)
            {
                case CommandType.LineInterp:
                    print("LineRenderer");

                    line.positionCount = formingPoints.Length;
                    line.SetPositions(formingPoints.Select(p => p.Position).ToArray());
                    break;

                case CommandType.CircleInterp:
                    print("Circle");

                    line.positionCount = formingPoints.Length;
                    line.SetPositions(formingPoints.Select(p => p.Position).ToArray());
                    break;
                case CommandType.Marching:
                    print("MarchingRenderer");

                    line.positionCount = formingPoints.Length;
                    line.SetPositions(formingPoints.Select(p => p.Position).ToArray());

                    break;
            }
        }
        
        
        // OtherMethods
        public void InitItem(int id, Transform main3DTransform)
        {
            Container3DTransform = new GameObject().transform;
            Container3DTransform.parent = main3DTransform;
            Id = id;
        }
        
        protected void UpdatePointItemList()
        {
            ClearItemPoints();
            
            CommandPose[] points = app.model.CommandModels[id].poses;
            
            if(points == null)
                return;
            
            SpecialCommandModel fPoints;
            
            switch (Type)
            {
                case CommandType.LineInterp:
                    LineRender(points);
                    PointsRender(points);
                    break;
                case CommandType.PtP:
                    PointsRender(points);
                    break;
                case CommandType.Marching:
                    LineRender(points);
                    fPoints = (SpecialCommandModel) app.model.CommandModels[id];

                    PointsRender(fPoints.FormingPoints);
                    break;
                case CommandType.CircleInterp:
                    LineRender(points);
                    fPoints = (SpecialCommandModel) app.model.CommandModels[id];
                    PointsRender(fPoints.FormingPoints);
                    break;
            }
        }

        protected void PointsRender(FormingPoint[] points)
        {
            if(points != null)
                foreach (var a in points)
                {
                    var b = Instantiate(pointItemPrefab, pointItemContent).GetComponent<CommandPointItem>();
                    PointItemList.Add(b);
                    b.InitItem(Id, PointItemList.Count - 1, Container3DTransform);
                }
        }
        
        public void ClearItemPoints()
        {
            foreach(var a in PointItemList)
            {
                Destroy(a);
            }

            PointItemList.Clear();
        }
        
        public void Remove()
        {
            app.model.CommandModels.RemoveAt(id);
        }
        
        
        // UnityEvent
        public void OnSelect(BaseEventData eventData)
        {
            print("OnSelect");
        }
        
        public void OnDeselect(BaseEventData eventData)
        {
            print("OnDeselect");
        }
    }
}
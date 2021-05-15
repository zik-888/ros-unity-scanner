using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using thelab.mvc;
using UnityEngine;
using UnityEngine.EventSystems;
using UniRx;
using UnityEngine.SceneManagement;

public class BlankController : Element<DemonOLPApplication>, IPointerDownHandler, ISelectHandler
{
    [SerializeField]
    protected int id;

    public int Id
    {
        set
        {
            id = value;
            name = $"Blank, {id}";
        }
        get => id;
    }


    private float _pastClickTime = 0;

    private void Start()
    {

        if (app.model.BlankModels[Id].type == BlankType.AlignedCad)
        {
            GetComponent<MeshRenderer>().material = app.model.materials[3];
        }
        else
        {
            GetComponent<MeshRenderer>().material = app.model.materials[0];
        }
        
        
        
        gameObject.AddComponent<MeshCollider>();
        gameObject.GetComponent<MeshRenderer>().shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
        gameObject.GetComponent<MeshRenderer>().receiveShadows = false;

        app.model.BlankModels[id]
                 .ObserveEveryValueChanged(x => x.BlankPose)
                 .Subscribe(x => PoseChangeIntoModel(x))
                 .AddTo(this);

        gameObject.layer = 9;


        //gameObject.AddComponent(typeof(ParticleSystem));
    }
    
    [Button]
    public void ApplyToParticleSystem()
    {
        
        Color color = Color.blue;
        float particleSize = 5;

        var positions = GetComponent<MeshFilter>().mesh.vertices;
        
        var ps = GetComponent<ParticleSystem>();
        if (ps == null)
            return;
 
        var particles = new ParticleSystem.Particle[positions.Length];
 
        for (int i = 0; i < particles.Length; ++i)
        {
            particles[i].position = positions[i];
            particles[i].startSize = particleSize;
            particles[i].startColor = color;
        }
        ps.SetParticles(particles);
        ps.Pause();
    }


    public void PoseChangeIntoModel(BlankPose pose)
    {
        transform.position = pose.UnityPosition;
        transform.rotation = Quaternion.Euler(pose.UnityRotation);
        transform.localScale = Vector3.one * pose.UnityScale;
    }

    private void OnMouseDown()
    {
        DoubleClick();
    }



    public void OnPointerDown(PointerEventData eventData)
    {
        //DoubleClick(); 
    }

    private void DoubleClick()
    {
        if (Time.time - _pastClickTime < 0.25f)
        {
            print("DoubleClick");

            app.model.SceneState.Value = new SceneState(false, id);
        }

        _pastClickTime = Time.time;
    }

    public void OnSelect(BaseEventData eventData)
    {
        print(eventData.selectedObject.name);
    }

}

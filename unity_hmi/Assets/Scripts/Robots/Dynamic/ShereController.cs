using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShereController : MonoBehaviour
{
    public float radius = 0.5f;
    public float period = 0.5f;
    public Vector3 pos = new Vector3();
    // Start is called before the first frame update
    void Start()
    {
        pos = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = new Vector3(
            radius * Mathf.Cos(Time.time * period) + pos.x,
            radius * Mathf.Sin(Time.time * period) + pos.y,
            pos.z
        );
    }
}

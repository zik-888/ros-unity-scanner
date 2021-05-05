using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.Serialization;
using UnityEngine.UI;

public class MeshProperties : MonoBehaviour
{
    [SerializeField]
    private InputField[] posIF = default;

    [SerializeField]
    private InputField[] rotIF = default;

    [SerializeField]
    private InputField scaleIF = default;

    [System.Serializable]
    /// <summary>
    /// The callback sent anytime the Inputfield is updated.
    /// </summary>
    public class OnChangeEvent : UnityEvent<BlankPose> { }

    [FormerlySerializedAs("m_OnValueChange")]
    [SerializeField]
    private OnChangeEvent m_OnValueChanged = new OnChangeEvent();

    public BlankPose pose;


    public void InitProperties(BlankPose pose)
    {
        this.pose = pose;
        posIF[0].text = pose.RosPosition.x.ToString();
        posIF[1].text = pose.RosPosition.y.ToString();
        posIF[2].text = pose.RosPosition.z.ToString();

        rotIF[0].text = pose.RosRotation.x.ToString();
        rotIF[1].text = pose.RosRotation.y.ToString();
        rotIF[2].text = pose.RosRotation.z.ToString();

        scaleIF.text = pose.RosScale.ToString();
    }


    public void EditXPosition(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            pose.RosPosition = new Vector3(float.Parse(value), pose.RosPosition.y, pose.RosPosition.z);
            m_OnValueChanged.Invoke(pose);
        }
        catch { }
    }

    public void EditYPosition(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            pose.RosPosition = new Vector3(pose.RosPosition.x, float.Parse(value), pose.RosPosition.z);
            m_OnValueChanged.Invoke(pose);
        }
        catch { }
    }

    public void EditZPosition(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            pose.RosPosition = new Vector3(pose.RosPosition.x, pose.RosPosition.y, float.Parse(value));
            m_OnValueChanged.Invoke(pose);
        }
        catch { }
    }

    public void EditXRotation(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            pose.RosRotation = new Vector3(float.Parse(value), pose.RosRotation.y, pose.RosRotation.z);
            m_OnValueChanged.Invoke(pose);
        }
        catch { }
    }

    public void EditYRotation(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            pose.RosRotation = new Vector3(pose.RosRotation.x, float.Parse(value), pose.RosRotation.z);
            m_OnValueChanged.Invoke(pose);
        }
        catch { }
    }

    public void EditZRotation(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            pose.RosRotation = new Vector3(pose.RosRotation.x, pose.RosRotation.y, float.Parse(value));
            m_OnValueChanged.Invoke(pose);
        }
        catch { }
    }

    public void EditScale(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            pose.RosScale = float.Parse(value);
            m_OnValueChanged.Invoke(pose);
        }
        catch { }
    }
}

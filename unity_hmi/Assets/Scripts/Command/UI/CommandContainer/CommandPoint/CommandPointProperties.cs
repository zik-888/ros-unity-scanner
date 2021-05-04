using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.Serialization;
using UnityEngine.UI;

public class CommandPointProperties : MonoBehaviour
{
    [SerializeField]
    private InputField[] posIF = new InputField[3];

    [SerializeField]
    private InputField[] rotIF = new InputField[3];

    [System.Serializable]
    /// <summary>
    /// The callback sent anytime the Inputfield is updated.
    /// </summary>
    public class OnChangeEvent : UnityEvent<FormingPoint> { }

    [FormerlySerializedAs("m_OnValueChange")]
    [SerializeField]
    private OnChangeEvent m_OnValueChanged = new OnChangeEvent();

    public FormingPoint Pose;

    public void SetProperties(FormingPoint pose)
    {
        if (pose != null)
        {

            this.Pose = pose;
            posIF[0].text = Round(pose.RosPosition.x, 100f).ToString();
            posIF[1].text = Round(pose.RosPosition.y, 100f).ToString();
            posIF[2].text = Round(pose.RosPosition.z, 100f).ToString();

            rotIF[0].text = Round(pose.RosRotation.x, 100f).ToString();
            rotIF[1].text = Round(pose.RosRotation.y, 100f).ToString();
            rotIF[2].text = Round(pose.RosRotation.z, 100f).ToString();
        }

    }


    public void EditXPosition(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            Pose.RosPosition = new Vector3(float.Parse(value), Pose.RosPosition.y, Pose.RosPosition.z);
            m_OnValueChanged.Invoke(Pose);
        }
        catch { }
    }

    public void EditYPosition(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            Pose.RosPosition =  new Vector3(Pose.RosPosition.x, float.Parse(value), Pose.RosPosition.z);
            m_OnValueChanged.Invoke(Pose);
        }
        catch { }
    }

    public void EditZPosition(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            Pose.RosPosition = new Vector3(Pose.RosPosition.x, Pose.RosPosition.y, float.Parse(value));
            m_OnValueChanged.Invoke(Pose);
        }
        catch { }
    }

    public void EditXRotation(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            Pose.RosRotation = new Vector3(float.Parse(value), Pose.RosRotation.y, Pose.RosRotation.z);
            m_OnValueChanged.Invoke(Pose);
        }
        catch { }
    }

    public void EditYRotation(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            Pose.RosRotation = new Vector3(Pose.RosRotation.x, float.Parse(value), Pose.RosRotation.z);
            m_OnValueChanged.Invoke(Pose);
        }
        catch { }
    }

    public void EditZRotation(string value)
    {
        value = value.Replace('.', ',');

        try
        {
            Pose.RosRotation = new Vector3(Pose.RosRotation.x, Pose.RosRotation.y, float.Parse(value));
            m_OnValueChanged.Invoke(Pose);
        }
        catch { }
    }

    private float Round(float value, float order)
    {
        return ((int)(value * order)) / order;
    }
}

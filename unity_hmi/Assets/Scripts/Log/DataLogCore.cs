using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DataLogCore : MonoBehaviour
{
    [SerializeField]
    private InputField inputField = default;

    string buf;

    private void FixedUpdate()
    {
        if(DataLog.GetData() != buf)
        {
            buf = DataLog.GetData();
            print(buf);
        }
        
        inputField.text = DataLog.GetData();
    }
}


public static class DataLog
{
    private static string data;

    public static void Log(string value)
    {
        data = value;
    }

    public static string GetData()
    {
        return data;
    }
}
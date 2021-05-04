using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class TestImport : MonoBehaviour
{
    // The imported function
    [DllImport("ConsoleApplication2.dll", EntryPoint = "InitUrdfModel")]
    public static extern bool InitUrdfModel([MarshalAs(UnmanagedType.LPStr)] string ansiStr);
    
    [DllImport("ConsoleApplication2.dll", EntryPoint = "TestSort")]
    public static extern void TestSort(int[] a, int length);
    
    [DllImport("ConsoleApplication2.dll", EntryPoint = "AcceptStr")]
    public static extern bool AcceptStr([MarshalAs(UnmanagedType.LPStr)] string ansiStr);
    //
    // [DllImport("ptest")]
    // private static extern void AcceptArray1([In, Out] byte[] arr, int length);
    
    //string path = "C:\\Users\\HaoAsakura\\source\\repos\\ConsoleApplication2\\ConsoleApplication2\\nao.urdf";
    // public bool f = true;
    //
    // public int[] a = new int[] {1, 5, 3};
    //
    // public int s = 0;
    //
}

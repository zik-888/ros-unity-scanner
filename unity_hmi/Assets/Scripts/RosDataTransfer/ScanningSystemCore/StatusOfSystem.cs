using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum StatusOfSystem
{
    INIT_SYSTEM,
    READY_TO_SCAN,
    GO_TO_TARGET_POINT,
    SCANNING,
    SCANNING_SUCCESS,
    ERROR_SENDING_TRAJECTORY,
    ERROR_GET_ROBOT_POSITION,
    ERROR_CONNECTION_TO_SCANNER,
    ALIGN_MODE,
    DEFROM_MODE,
    ALIGN_AND_DEFORM_MODE,
    PROCESSING_SUCCESS
    // ... e.t.
};

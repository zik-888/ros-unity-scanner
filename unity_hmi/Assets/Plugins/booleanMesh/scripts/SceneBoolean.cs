using UnityEngine;
using System.Collections;
using Sirenix.OdinInspector;

public class SceneBoolean : MonoBehaviour {

	public MeshCollider meshColliderA;
	public MeshCollider meshColliderB;

	[Button]
	// Use this for initialization
	void Init() {

		// Create new GameObject
		GameObject newObject = new GameObject();
		MeshFilter meshFilter = newObject.AddComponent<MeshFilter>();
		MeshRenderer meshRenderer = newObject.AddComponent<MeshRenderer>();
		
		meshRenderer.materials = new Material[2]
		{
			meshColliderA.GetComponent<Renderer>().materials[0],
			meshColliderB.GetComponent<Renderer>().materials[0]
		};
	
		// Assign booleanMesh
		BooleanMesh booleanMesh = new BooleanMesh(meshColliderA,meshColliderB);
		//meshFilter.mesh = booleanMesh.Difference();
		//meshFilter.mesh = booleanMesh.Union();
		meshFilter.mesh = booleanMesh.Difference();
	
	}	

}

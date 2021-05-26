using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MeshSystem
{
    public static class CMeshPool
    {
        private static List<CMesh> Pool;

        public static CMesh GetMesh(int id)
        {   
            return Pool[id];
        }

        public static int Push(CMesh customMesh)
        {
            if (Pool == null)
                Pool = new List<CMesh>();

            int pointer = Pool.Count;
            Pool.Add(customMesh);

            return pointer;
        }

        public static int GetAvailableIndex()
        {
            if (Pool == null)
                return 0;

            return Pool.Count;
        }

        public static bool Remove(int index)
        {
            if (Pool == null)
                return false;

            if (Pool.Count > index)
            {
                Pool.RemoveAt(index);
                return true;
            }
            else { return false; }
        }

        public static void Clear()
        {
            if (Pool != null)
                Pool.Clear();
        }
    }
}

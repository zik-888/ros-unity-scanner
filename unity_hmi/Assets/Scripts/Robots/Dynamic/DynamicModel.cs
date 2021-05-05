using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using RosSharp;
using UnityEngine;
using MathNet.Numerics;
using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.LinearAlgebra.Double;

namespace Dynamic
{
    public enum JType
    {
        Rx,
        Ry,
        Rz
    }

    [Serializable]
    public class DynamicModel
    {
        [SerializeField]
        private int nb;
        [SerializeField]
        private JType[] typesJoint;
        [SerializeField]
        private Matrix<double>[] I;
        [SerializeField]
        private Matrix<double>[] XTree;


        public DynamicModel()
        {
            nb = 6;

            typesJoint = new JType[]
            {
                JType.Rz,
                JType.Ry,
                JType.Ry,
                JType.Rx,
                JType.Ry,
                JType.Rx,
                JType.Rz
            };


            I = new Matrix<double>[7];

            I[0] = DenseMatrix.OfArray(new double[,]
            {
                {0.0334, 0, 0, 0, 0.1852, -0.0890},
                {0, 0.0284, -0.0013, -0.1852, 0, 0},
                {0, -0.0013, 0.0091, 0.0890, 0, 0},
                {0, -0.1852, 0.0890, 2.7400, 0, 0},
                {0.1852, 0, 0, 0, 2.7400, 0},
                {-0.0890, 0, 0, 0, 0, 2.7400}
            });

            I[1] = DenseMatrix.OfArray(new double[,]
            {
                {0.034, 0, 0, 0, -0.0883, -0.1880},
                {0, 0.0091, 0.0013, 0.0883, 0, 0},
                {0, 0.0013, 0.0290, 0.1880, 0, 0},
                {0, 0.0883, 0.1880, 2.7400, 0, 0},
                {-0.0883, 0, 0, 0, 2.7400, 0},
                {-0.1880, 0, 0, 0, 0, 2.7400}
            });

            I[2] = DenseMatrix.OfArray(new double[,]
            {
                {0.0125, -0.0060, 2.2413e-04, 0, 0.0755, 0.0753},
                {-0.0060, 0.0175, 2.0925e-04, -0.0755, 0, -0.1118},
                {2.2413e-04, 2.0925e-04, 0.0158, -0.0753, 0.1118, 0},
                {0, -0.0755, -0.0753, 2.3800, 0, 0},
                {0.0755, 0, 0.1118, 0, 2.3800, 0},
                {0.0753, -0.1118, 0, 0, 0, 2.3800}
            });

            I[3] = DenseMatrix.OfArray(new double[,]
            {
                {0.0131, 0.0064, 3.2280e-04, 0, -0.0759, 0.0802},
                {0.0064, 0.0138, -1.9827e-04, 0.0759, 0, 0.0858},
                {3.2280e-04, -1.9827e-04, 0.0160, -0.0802, -0.0858, 0},
                {0, 0.0759, -0.0802, 2.3800, 0, 0},
                {-0.0759, 0, -0.0858, 0, 2.3800, 0},
                {0.0802, 0.0858, 0, 0, 0, 2.3800}
            });

            I[4] = DenseMatrix.OfArray(new double[,]
            {
                {0.0703, 6.5028e-07, -1.0513e-05, 0, 0.2854, 0.1673},
                {6.5028e-07, 0.0586, 0.0097, -0.2854, 0, 0},
                {-1.0513e-05, 0.0097, 0.0147, -0.1673, 0, 0},
                {0, -0.2854, -0.1673, 2.7400, 0, 0},
                {0.2854, 0, 0, 0, 2.7400, 0},
                {0.1673, 0, 0, 0, 0, 2.7400}
            });

            I[5] = DenseMatrix.OfArray(new double[,]
            {
                {0.0033, -0.0012, -2.1222e-04, 0, -0.0165, 0.0141},
                {-0.0012, 0.0083, -1.9657e-05, 0.0165, 0, -0.0791},
                {-2.1222e-04, -1.9657e-05, 0.0098, -0.0141, 0.0791, 0},
                {0, 0.0165, -0.0141, 1.5500, 0, 0},
                {-0.0165, 0, 0.0791, 0, 1.5500, 0},
                {0.0141, -0.0791, 0, 0, 0, 1.5500}
            });

            I[6] = DenseMatrix.OfArray(new double[,]
            {
                {0.0032, -1.8634e-04, -3.4540e-04, 0, -0.0351, 0.0058},
                {-1.8634e-04, 0.0032, -4.7258e-04, 0.0351, 0, -0.0059},
                {-3.4540e-04, -4.7258e-04, 8.4561e-04, -0.0058, 0.0059, 0},
                {0, 0.0351, -0.0058, 0.5400, 0, 0},
                {-0.0351, 0, 0.0059, 0, 0.5400, 0},
                {0.0058, -0.0059, 0, 0, 0, 0.5400}
            });

            XTree = new Matrix<double>[7];

            XTree[0] = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0.4, 0, 1, 0, 0},
                {-0.4, 0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 1}
            });

            XTree[1] = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0.025, 0, 1, 0},
                {0, -0.025, 0, 0, 0, 1}
            });

            XTree[2] = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0.315, 0, 1, 0},
                {0, -0.315, 0, 0, 0, 1}
            });

            XTree[3] = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0.035, 0, 1, 0, 0},
                {-0.035, 0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 1}
            });

            XTree[4] = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0.365, 0, 1, 0},
                {0, -0.365, 0, 0, 0, 1}
            });

            XTree[5] = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0.08, 0, 1, 0},
                {0, -0.08, 0, 0, 0, 1}
            });

            XTree[6] = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 1}
            });
        }


        public int NB => nb;

        public Matrix<double> GetI(int i)
        {
            return I[i];
        }

        public JType GetJType(int i)
        {
            return typesJoint[i];
        }

        public Matrix<double> GetXTree(int i)
        {
            return XTree[i];
        }

    }
    
}
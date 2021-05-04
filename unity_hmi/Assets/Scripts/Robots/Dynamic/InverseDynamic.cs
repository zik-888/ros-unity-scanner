using System;
using System.Linq;
using MathNet.Numerics;
using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.LinearAlgebra.Double;

namespace Dynamic
{
    
    public class InverseDynamic
    {
        public static double[] GetTorques(DynamicModel dynamicModel, double[] q, double[] dq, double[] ddq)
        {
            var aGrav = GetGravity();

            var s = new Vector<double>[dynamicModel.NB];
            var xup = new Matrix<double>[dynamicModel.NB];

            var v = new Vector<double>[dynamicModel.NB];
            var a = new Vector<double>[dynamicModel.NB];
            var f = new Vector<double>[dynamicModel.NB];

            Vector<double> tau = new DenseVector(dynamicModel.NB);

            for (var i = 0; i < dynamicModel.NB; i++)
            {
                var (XJ, S) = JCalc(dynamicModel.GetJType(i), q[i]);

                s[i] = S;

                var vJ = s[i] * dq[i];

                xup[i] = XJ * dynamicModel.GetXTree(i);


                if (i == 0)
                {
                    v[i] = vJ;
                    a[i] = xup[i] * -aGrav + s[i] * ddq[i];
                }
                else
                {
                    v[i] = xup[i] * v[i - 1] + vJ;
                    a[i] = xup[i] * a[i - 1] + s[i] * ddq[i] + Crm(v[i]) * vJ;
                }

                f[i] = dynamicModel.GetI(i) * a[i] + Crf(v[i]) * dynamicModel.GetI(i) * v[i];
            }

            for (var i = dynamicModel.NB - 1; i >= 0; i--)
            {
                tau[i] = s[i] * f[i];

                if (i != 0) 
                    f[i - 1] = f[i - 1] + xup[i].Transpose() * f[i];
            }

            foreach (var VARIABLE in f)
            {
                Console.WriteLine(VARIABLE);
            }

            return tau.ToArray();
        }

        private static (Matrix<double>, Vector<double>) JCalc(JType type, double q)
        {
            var s = new DenseVector(6);

            var xj = RotX(q);

            switch (type)
            {
                case JType.Rx:
                    xj = RotX(q);
                    s[0] = 1f;
                    break;
                case JType.Ry:
                    xj = RotY(q);
                    s[1] = 1f;
                    break;
                case JType.Rz:
                    xj = RotZ(q);
                    s[2] = 1f;
                    break;
            }

            return (xj, s);
        }

        private static Matrix<double> RotX(double theta)
        {
            var c = (double) Math.Cos(theta);
            var s = (double) Math.Sin(theta);

            var m = DenseMatrix.OfArray(new double[,]
            {
                {1, 0, 0, 0, 0, 0},
                {0, c, s, 0, 0, 0},
                {0, -s, c, 0, 0, 0},
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0, 0, c, s},
                {0, 0, 0, 0, -s, c}
            });

            return m;
        }

        private static Matrix<double> RotY(double theta)
        {
            var c = (double) Math.Cos(theta);
            var s = (double) Math.Sin(theta);

            var m = DenseMatrix.OfArray(new double[,]
            {
                {c, 0, -s, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {s, 0, c, 0, 0, 0},
                {0, 0, 0, c, 0, -s},
                {0, 0, 0, 0, 1, 0},
                {0, 0, 0, s, 0, c}
            });

            return m;
        }

        private static Matrix<double> RotZ(double theta)
        {
            var c = (double) Math.Cos(theta);
            var s = (double) Math.Sin(theta);

            var m = DenseMatrix.OfArray(new double[,]
            {
                {c, s, 0, 0, 0, 0},
                {-s, c, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0, 0, c, s, 0},
                {0, 0, 0, -s, c, 0},
                {0, 0, 0, 0, 0, 1}
            });

            return m;
        }

        private static Vector<double> GetGravity()
        {
            return DenseVector.OfArray(new double[] {0, 0, 0, 0, 0, -9.81});
        }

        private static Matrix<double> Crm(Vector<double> v)
        {
            if (v.Count == 6)
            {
                var m = DenseMatrix.OfArray(new double[,]
                {
                    {0, -v[2], v[1], 0, 0, 0},
                    {v[2], 0, -v[0], 0, 0, 0},
                    {-v[1], v[0], 0, 0, 0, 0},
                    {0, -v[5], v[4], 0, -v[2], v[1]},
                    {v[5], 0, -v[3], v[2], 0, -v[0]},
                    {-v[4], v[3], 0, -v[1], v[0], 0}
                });

                return m;
            }
            else
            {
                var m = DenseMatrix.OfArray(new double[,]
                {
                    {0, 0, 0},
                    {v[2], 0, -v[0]},
                    {-v[1], v[0], 0}
                });

                return m;
            }
        }

        private static Matrix<double> Crf(Vector<double> v)
        {
            return -Crm(v).Transpose();
        }
    }
}
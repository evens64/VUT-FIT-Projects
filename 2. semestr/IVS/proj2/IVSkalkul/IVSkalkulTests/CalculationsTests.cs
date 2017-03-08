using Microsoft.VisualStudio.TestTools.UnitTesting;
using IVSkalkul;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IVSkalkul_lib;

namespace IVSkalkul.Tests
{
    [TestClass()]
    public class CalculationsTests
    {
        private const double EPS = 0.001;
        private const int ITER = 100;

        [TestMethod()]
        public void PlusTest()
        {
            math_pack cs = new math_pack();
            Assert.AreEqual(1, cs.fce_plus(0, 1), 0, "0 + 1 != 1");
            Assert.AreEqual(42, cs.fce_plus(39, 3), 0, "39 + 3 != 42");
            Assert.AreEqual(9, cs.fce_plus(10, -1), 0, "10 + (-1) != 9");
            Assert.AreEqual(9, cs.fce_plus(-1, 10), 0, "-1 + 10 != 9");
            Assert.AreEqual(-10, cs.fce_plus(5, -15), 0, "5 + (-15) != -10");
            Assert.AreEqual(120569, cs.fce_plus(-50, 120619), 0, "-50 + 120619 != 120569");
            Assert.AreEqual(5.2, cs.fce_plus(1.4, 3.8), EPS, "1.4 + 3.8 != 5.2");
            Assert.AreEqual(-5.2, cs.fce_plus(-1.4, -3.8), EPS, "-1.4 + (-3.8) != -5.2");
            Assert.IsTrue(Double.IsInfinity(cs.fce_plus(Double.MaxValue, Double.MaxValue)));
            Assert.IsTrue(Double.IsInfinity(cs.fce_plus(Double.MinValue, -Double.MaxValue)));
        }

        [TestMethod()]
        public void MinusTest()
        {
            math_pack cs = new math_pack();
            Assert.AreEqual(1, cs.fce_minus(2, 1), 0, "2 - 1 != 1");
            Assert.AreEqual(42, cs.fce_minus(60, 18), 0, "60 - 18 != 42");
            Assert.AreEqual(42, cs.fce_minus(20, -22), 0, "20 - (-22) != 42");
            Assert.AreEqual(10, cs.fce_minus(-20, -30), 0, "-20 - (-30) != 10");
            Assert.AreEqual(-5, cs.fce_minus(10, 15), 0, "10 - 15 != -5");
            Assert.AreEqual(1337420, cs.fce_minus(1537420, 200000), 0, "1 537 420 - 200 000 != 1 337 420");
            Assert.AreEqual(5.2, cs.fce_minus(8.4, 3.2), EPS, "8.4 - 3.2 != 5.2");
            Assert.AreEqual(-5.2, cs.fce_minus(-8.4, -3.2), EPS, "-8.4 - (-3.2) != -5.2");
            Assert.IsTrue(Double.IsInfinity(cs.fce_minus(Double.MinValue, Double.MaxValue)));
            Assert.IsTrue(Double.IsInfinity(cs.fce_minus(Double.MaxValue, -Double.MaxValue)));
        }

        [TestMethod()]
        public void MultiplyTest()
        {
            math_pack cs = new math_pack();
            Assert.AreEqual(0, cs.fce_krat(0, 1), 0, "0 * 1 != 0");
            Assert.AreEqual(10, cs.fce_krat(2, 5), 0, "2 * 5 != 10");
            Assert.AreEqual(10, cs.fce_krat(1, 10), 0, "1 * 10 != 10");
            Assert.AreEqual(-15, cs.fce_krat(-3, 5), 0, "-3 * 5 != -15");
            Assert.AreEqual(-15, cs.fce_krat(3, -5), 0, "3 * (-5) != -15");
            Assert.AreEqual(42, cs.fce_krat(-2, -21), 0, "-2 * (-21) != 42");
            Assert.AreEqual(1200000, cs.fce_krat(6, 200000), 0, "6 * 200 000 != 1 200 000");
            Assert.AreEqual(12.6, cs.fce_krat(3, 4.2), EPS, "3 * 4.2 != 12.6");
            Assert.AreEqual(-12.6, cs.fce_krat(-3, 4.2), EPS, "-3 * 4.2 != -12.6");
            Assert.IsTrue(Double.IsInfinity(cs.fce_krat(Double.MaxValue, 2)));
        }

        [TestMethod()]
        public void DivideTest()
        {
            math_pack cs = new math_pack();
            try
            {
                cs.fce_deleno(5, 0);
                Assert.Fail("No exception thrown when dividing by 0");
            }
            catch (DivideByZeroException ex)
            {
                Assert.AreEqual("Cannot divide by zero", ex.Message);
            }
            Assert.AreEqual(1, cs.fce_deleno(6, 6), 0, "6 / 6 != 1");
            Assert.AreEqual(5, cs.fce_deleno(15, 3), 0, "15 / 3 != 5");
            Assert.AreEqual(-5, cs.fce_deleno(-15, 3), 0, "-15 / 3 != -5");
            Assert.AreEqual(-5, cs.fce_deleno(15, -3), 0, "15 / (-3) != -5");
            Assert.AreEqual(5, cs.fce_deleno(-15, -3), 0, "-15 / (-3) != 5");
            Assert.AreEqual(1.2, cs.fce_deleno(6, 5), EPS, "6 / 5 != 1.2");
            Assert.AreEqual(200000, cs.fce_deleno(1000000, 5), 0, "1,000,000 / 5 != 200,000");
        }

        [TestMethod()]
        public void PowTest()
        {
            //implement both Power and Root of a number in this method because of the possibility to express one as the other
            //which means the user can call Power, but depending on the arguments, the method should calculate Root and vice versa
            math_pack cs = new math_pack();
            //the error range shouldn't be required for most of these, but because this is a complicated method, I'm being nice :)
            Assert.AreEqual(16, cs.fce_mocnina(2, 4), EPS, "2^4 != 16");
            Assert.AreEqual(1, cs.fce_mocnina(2, 0), EPS, "2^0 != 1");
            Assert.AreEqual(2, cs.fce_mocnina(2, 1), EPS, "2^1 != 2");
            Assert.AreEqual(16, cs.fce_mocnina(-2, 4), EPS, "(-2)^4 != 16");
            Assert.AreEqual(-8, cs.fce_mocnina(-2, 3), EPS, "(-2)^3 != -8");
            Assert.AreEqual(0.25, cs.fce_mocnina(2, -2), EPS, "2^(-2) != 1/4 (0.25)");
            Assert.AreEqual(1000000, cs.fce_mocnina(10, 6), EPS, "10^6 != 1,000,000");
            Assert.IsTrue(Double.IsInfinity(cs.fce_mocnina(Double.MaxValue, 2)));
            Assert.IsTrue(Double.IsInfinity(cs.fce_mocnina(Double.MinValue, 2)));
        }

        [TestMethod()]
        public void SqrtTest()
        {
            math_pack cs = new math_pack();
            try
            {
                cs.fce_odmocnina(-1, 2, ITER);
                Assert.Fail("Tried to calculate a square root from a negative number");
            }
            catch(Exception ex)
            {
                Assert.AreEqual("Cannot calculate a square root from a negative number", ex.Message);
            }
            try
            {
                cs.fce_odmocnina(4, 1, ITER);
                Assert.Fail("Tried to calculate a 1st root");
            }
            catch (Exception ex)
            {
                Assert.AreEqual("Cannot calculate lower than square root", ex.Message);
            }
            Assert.AreEqual(2, cs.fce_odmocnina(4, 2, ITER), EPS, "4^(1/2) != 2");
            Assert.AreEqual(10, cs.fce_odmocnina(100, 2, ITER), EPS, "100^(1/2) != 10");
            Assert.AreEqual(2, cs.fce_odmocnina(8, 3, ITER), EPS, "8^(1/3) != 2");
            Assert.AreEqual(0, cs.fce_odmocnina(0, 2, ITER), EPS, "0^(1/2) != 0");
        }

        [TestMethod()]
        public void FactorialTest()
        {
            math_pack cs = new math_pack();
            try
            {
                cs.fce_fakt(-4);
                Assert.Fail("No exception thrown for invalid argument (tried to calculate (-4)!)");
            }
            catch (ArgumentException ex)
            {
                Assert.AreEqual("Cannot calculate the factorial of a negative number", ex.Message);
            }
            Assert.AreEqual(1, cs.fce_fakt(0), 0, "0! != 1");
            Assert.AreEqual(1, cs.fce_fakt(1), 0, "1! != 1");
            Assert.AreEqual(2, cs.fce_fakt(2), 0, "2! != 2");
            Assert.AreEqual(120, cs.fce_fakt(5), 0, "5! != 120");
            Assert.AreEqual(3628800, cs.fce_fakt(10), 0, "10! != 3,628,800");
            Assert.IsTrue(Double.IsInfinity(cs.fce_fakt(200)));
        }
    }
}
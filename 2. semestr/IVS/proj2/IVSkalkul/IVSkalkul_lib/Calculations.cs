/**
 * \File:        Calculations.cs
 * \Date:        24.04.2015
 * \Author:      Jan Novak, xnovak1s@fit.vutbr.cz
 * \Project:     IVS:Projekt 2
 * \Description: Knihovna implementujici zakladni matematicke operace.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace IVSkalkul_lib
{
    public class math_pack
    {
        private const double EPS = 0.001;

        #region základní funkce

        private double abs(double a)
        {
            return a < 0 ? -a : a;
        }

        /**
         * \brief Funkce provadejici scitani.
         * \param a Prvni operand
         * \param b Druhy operand
         * \return Vysledek typu double. Infinity pokud nastalo preteceni.
         */

        public double fce_plus(double a, double b)
        {
            return a + b;
        }

        /**
         * \brief Funkce provadejici odcitani a - b.
         * \param a Prvni operand
         * \param b Druhy operand
         * \return Vysledek typu double. Infinity pokud nastalo preteceni.
         */

        public double fce_minus(double a, double b)
        {
            return a - b;
        }

        /**
         * \brief Funkce provadejici nasobeni.
         * \param a Prvni operand
         * \param b Druhy operand
         * \return Vysledek typu double. Infinity pokud nastalo preteceni.
         */

        public double fce_krat(double a, double b)
        {
            return a * b;
        }

        /**
         * \brief Funkce provadejici deleni.
         * \param a Delenec
         * \param b Delitel
         * \throw DivideByZeroException kdyz b = 0
         * \return Vysledek typu double.
         */

        public double fce_deleno(double a, double b)
        {
            if (abs(b - 0) < EPS)
                throw new DivideByZeroException("Cannot divide by zero");
            return a / b;
        }

        #endregion základní funkce

        #region pokročilé funkce

        /**
         * \brief Funkce provadejici mocneni.
         * \param x Zaklad
         * \param y Mocnina
         * \return Vysledek typu double. Infinity pokud nastalo preteceni.
         */

        public double fce_mocnina(double x, int y)
        {
            if (y == 0) return 1;
            if (y < 0)
            {
                y = -y;
                x = 1.0 / x;
            }
            double sum = x;
            for (int i = 1; i < y; i++)
                sum *= x;
            return sum;
        }

        /**
         * \brief Funkce provadejici odmocninu.
         * \param x Zaklad
         * \param y Stupen odmocniny (2 a vyse)
         * \param i Pocet iteraci
         * \throw ArgumentException kdyz x je mensi nez 0 nebo kdyz y je mensi nebo rovno 1
         * \return Vysledek typu double.
         */

        public double fce_odmocnina(double x, int y, int i)
        {
            if (x < 0) throw new ArgumentException("Cannot calculate a square root from a negative number");
            if (y <= 1) throw new ArgumentException("Cannot calculate lower than square root");
            double res = 1;
            for (int j = 0; j < i; j++)
            {
                res = ((y - 1) * res + x / fce_mocnina(res, y - 1)) / y;
            }
            return res;
        }

        /**
         * \brief Funkce provadejici faktorial. Vstup i vystup by melo byt cele cislo, ale lze pracovat i s desetinnymi.
         * \param x Vstupni cislo
         * \throw ArgumentException kdyz x je zaporne cislo
         * \return Vysledek typu double.
         */

        public double fce_fakt(double x)
        {
            if (x < 0) throw new ArgumentException("Cannot calculate the factorial of a negative number");
            if (x <= 1) return 1;
            return x * fce_fakt(x - 1);
        }

        #endregion pokročilé funkce

        #region další funkce

        /**
         * \brief Největší společný dělitel
         * \param a Prvni operand
         * \param b Druhy operand
         * \return Vysledek typu double
         */

        public double fce_NSD(double a, double b)
        {
            return b == 0 ? a : fce_NSD(b, a % b);
        }

        #endregion další funkce
    }
}
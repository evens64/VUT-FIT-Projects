/**
 * \File:        MainWindow.xaml.cs
 * \Date:        24.04.2015
 * \Author:      Filip Široký & Jakub Frýz
 * \Project:     IVS:Projekt 2
 * \Description: Všechny funkce potřebné pro fungování hlavního okna
 */

using IVSkalkul_lib;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace IVSkalkul
{
    public partial class MainWindow : Window
    {
        #region DEBUG konzole

#if DEBUG

        [DllImport("kernel32.dll")]
        public static extern bool AllocConsole();

        [DllImport("kernel32.dll")]
        public static extern bool FreeConsole();

        private bool _shown = false;

        protected override void OnContentRendered(EventArgs e)
        {
            base.OnContentRendered(e);

            if (_shown) return;
            _shown = true;

            AllocConsole();
            Console.ForegroundColor = ConsoleColor.Magenta;
            Console.WriteLine("DEBUG [double(min): {0}, double(max): {1}]", double.MinValue, double.MaxValue);
            Console.ResetColor();

            this.Activate();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            FreeConsole();
        }

#endif

        #endregion DEBUG konzole

        #region globální proměnné

        private int iter = 2000;    /// počet iterací pro fce_odmocnina()
        private int textInput_length = 42;  /// max počet znaků v textInput
        private int textOutput_length = 20; /// max počet znaků v textOutput

        private bool NumberMode = true; /// pomocná proměnná (true = zadávání čísel, false = zadávání značek)
        private bool PointMode = false; /// pomocná proměnná (true = čárka v čísle je, false = není)
        private bool GotResult = false; /// pomocná proměnná (true = dostal jsem výsledek, false = nedostal)
        private bool Switched = false;  /// pomocná proměnná (true = zobrazen zlomek, false = nezobrazen)
        private bool MadeFakt = false;  /// pomocná proměnná (true = byl proveden faktoriál, false = nebyl)
        private bool GotError = false;  /// pomocná proměnná (true = nastala chyba, false = nenastala)

        private string ANSWER = "0";   /// poslední výsledek
        private List<string> ExampleList = new List<string> { };    /// výsledek po kouscích
        private string Example = "";    /// celý výsledek
        private math_pack M = new math_pack();  /// použítí fce z knihovny

        #endregion globální proměnné

        /**
         * \brief Inicializace okna
         */

        public MainWindow()
        {
            InitializeComponent();
        }

        /**
         * \brief Přesouvání okna
         * \param sender Odesílatel funkce
         * \param e Argumenty eventu
         */

        private void Drag(object sender, MouseButtonEventArgs e)
        {
            while (Mouse.LeftButton == MouseButtonState.Pressed)
            {
#if DEBUG
                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.WriteLine("DRAG:\t\t\tPřesun okna");
                Console.ResetColor();
#endif
                this.DragMove();
            }
        }

        /**
         * \brief Minimalizace okna
         * \param sender Odesílatel funkce
         * \param e Argumenty eventu
         */

        private void Minimize(object sender, RoutedEventArgs e)
        {
#if DEBUG
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("MINIMIZE:\t\tMinimalizace okna");
            Console.ResetColor();
#endif
            this.WindowState = WindowState.Minimized;
        }

        /**
         * \brief Uzavření okna
         * \param sender Odesílatel funkce
         * \param e Argumenty eventu
         */

        private void CloseWIN(object sender, RoutedEventArgs e)
        {
#if DEBUG
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("CLOSE:\t\t\tUzavření okna");
            Console.ResetColor();
#endif
            this.Close();
        }

        /**
         * \brief Zobrazení argumentu v textInput.Text
         * \param arg String, který chceme zobrazit
         */

        private void InputShow(string arg)
        {
            if (string.IsNullOrEmpty(arg))    //v případě, že nic nebylo zadáno
            {
                textInput.Text = "0";
            }
            else   //jinak
            {
                //textInput.Text = arg;
                textInput.Text = arg.Length > textInput_length ? "..." + arg.Substring(arg.Length - textInput_length) : arg;
            }
#if DEBUG
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("INPUT:\t\t\t\"{0}\"", textInput.Text);
            Console.ResetColor();
#endif
        }

        /**
         * \brief Zobrazení čísel v textOutput.Text
         * \param arg String, který chceme zobrazit
         */

        private void NumberOutput(string arg)
        {
#if DEBUG
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("NumberOutput(in):\t\"PointMode = {0}, NumberMode = {1}, GotResult = {2}, GotError = {3}\"", PointMode, NumberMode, GotResult, GotError);
            Console.ResetColor();
#endif
            GotError = false;
            if (textOutput.Text.Length + 1 > textOutput_length) return; //kontrola délky
            if (NumberMode == false || GotResult == true || MadeFakt == true)
            {
                NumberMode = true;
                GotResult = false;
                Switched = false;
                MadeFakt = false;
                textOutput.Text = arg;
            }
            else if (string.IsNullOrEmpty(textOutput.Text) || textOutput.Text != "0")
            {
                NumberMode = true;
                textOutput.Text += arg;
            }
            else
            {
                textOutput.Text = arg;
            }

#if DEBUG
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("NumberOutput(in):\t\"PointMode = {0}, NumberMode = {1}, GotResult = {2}, GotError = {3}\"", PointMode, NumberMode, GotResult, GotError);
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("OUTPUT:\t\t\t\"{0}\"", textOutput.Text);
            Console.ResetColor();
#endif
        }

        /**
         * \brief Negace čísla v textOutput.Text
         */

        private void NumberNeg()
        {
            if (NumberMode == true && textOutput.Text != "0")
            {
                if (textOutput.Text[0] != '-')
                {
                    textOutput.Text = "-" + textOutput.Text;
                }
                else
                {
                    textOutput.Text = textOutput.Text.Substring(1);
                }
            }
        }

        /**
         * \brief Vložení známenka
         * \param arg Znaménko k vložení
         */

        private void SignInput(string arg)
        {
#if DEBUG
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("SignInput(in):\t\t\"PointMode = {0}, NumberMode = {1}, GotResult = {2}, GotError = {3}\"", PointMode, NumberMode, GotResult, GotError);
            Console.ResetColor();
#endif

            string NumOut = Switched ? ANSWER : textOutput.Text;
            //if (Switched) return;
            if (GotError == true)
            {
                Example = "0" + arg;
                ExampleList.Add("0");
                ExampleList.Add(arg);
            }
            else if (NumberMode == true || (NumberMode == true && arg == "!") || GotResult == true)
            {
                NumberMode = false;
                //Example += textOutput.Text + arg;
                Example += NumOut + arg;

                //ExampleList.Add(textOutput.Text);
                ExampleList.Add(NumOut);
                ExampleList.Add(arg);
            }
            //else if (Example[Example.Length - 1] == '!')
            //{
            //Example += arg;
            //ExampleList.Add(arg);
            //}
            else
            {
                Example = Example.Remove(Example.Length - 1);
                Example += arg;

                ExampleList[ExampleList.Count - 1] = arg;
            }
            textOutput.Text = "0";
            PointMode = false;
            GotResult = false;
            Switched = false;

#if DEBUG
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("SignInput(out):\t\t\"PointMode = {0}, NumberMode = {1}, GotResult = {2}, GotError = {3}\"", PointMode, NumberMode, GotResult, GotError);
            Console.ResetColor();
#endif
        }

        /**
         * \brief Přepis čísla na zlomek
         */

        private void ResultSwitch()
        {
            if (GotResult)
            {
                Switched = !(Switched);
                if (!(Switched))
                {
                    textOutput.Text = ANSWER;
                }
                else
                {
                    double x = double.Parse(ANSWER);
                    double y = 1;
                    while ((x % 1) != 0)
                    {
                        x *= 10;
                        y *= 10;
                    }
                    double nsd = M.fce_NSD(x, y);
                    x /= nsd;
                    y /= nsd;
                    if (textOutput_length > (x + "|" + y).Length)
                    {
                        textOutput.Text = x + "|" + y;
                    }
                    else
                    {
                        Switched = false;
                    }
                }
            }
        }

        /**
         * \brief Reakce na klávesy klávesnice
         * \param sender Odesílatel funkce
         * \param e Argumenty eventu
         */

        private void PressKey(object sender, KeyEventArgs e)
        {
#if DEBUG
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("KEY:\t\t\tZmáčknuta klávesa \"{0}\"", e.Key);
            Console.ResetColor();
#endif
            //if ((Keyboard.Modifiers == ModifierKeys.Control) && (e.Key == Key.Subtract))
            //{
            //NumberNeg();
            //}
            //else
            //{
            switch (e.Key)
            {
                #region čísla

                case Key.D1:
                case Key.NumPad1:
                    jedna.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D2:
                case Key.NumPad2:
                    dva.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D3:
                case Key.NumPad3:
                    tri.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D4:
                case Key.NumPad4:
                    ctr.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D5:
                case Key.NumPad5:
                    pet.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D6:
                case Key.NumPad6:
                    sest.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D7:
                case Key.NumPad7:
                    sedm.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D8:
                case Key.NumPad8:
                    osm.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D9:
                case Key.NumPad9:
                    devet.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.D0:
                case Key.NumPad0:
                    nula.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.Decimal:   //,
                    tecka.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                #endregion čísla

                #region základní znaménka

                case Key.Add:       //+
                    plus.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.Subtract:  //-
                    minus.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.Multiply:  //×
                    krat.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.Divide:    //÷
                    deleno.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                #endregion základní znaménka

                case Key.Back:
                    zpet.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.Enter:
                    ROVNASE.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;

                case Key.Escape:
                    buttonClose.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                    break;
            }
            //}
        }

        /**
         * \brief Reakce na kliknutí na tlačítko
         * \param sender Odesílatel funkce
         * \param e Argumenty eventu
         */

        private void ButtonClick(object sender, RoutedEventArgs e)
        {
            ROVNASE.Focus();       //aby na tlačítko 'ENTER' reagovalo '='

            Button clickedButton = sender as Button;    //které tlačítko vyvolalo proceduru

#if DEBUG
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("BUTTON:\t\t\tZmáčknuta tlačítko \"{0}\"", clickedButton.Name);
            Console.ResetColor();
#endif

            if (clickedButton == null) return;  //kontrola "kdyby náhodou"

            if ((Keyboard.Modifiers == ModifierKeys.Control) && (clickedButton.Name == "minus"))
            {
                NumberNeg();
            }
            else
            {
                switch (clickedButton.Name)
                {
                    #region čísla

                    case "jedna":   //1
                        NumberOutput("1");
                        break;

                    case "dva":     //2
                        NumberOutput("2");
                        break;

                    case "tri":     //3
                        NumberOutput("3");
                        break;

                    case "ctr":     //4
                        NumberOutput("4");
                        break;

                    case "pet":     //5
                        NumberOutput("5");
                        break;

                    case "sest":    //6
                        NumberOutput("6");
                        break;

                    case "sedm":    //7
                        NumberOutput("7");
                        break;

                    case "osm":     //8
                        NumberOutput("8");
                        break;

                    case "devet":   //9
                        NumberOutput("9");
                        break;

                    case "nula":    //0
                        NumberOutput("0");
                        break;

                    case "tecka":   //.
                        if (!(string.IsNullOrEmpty(textOutput.Text)) && PointMode != true)
                        {
                            PointMode = true;
                            if (NumberMode == false)
                            {
                                textOutput.Text = "0,";
                                NumberMode = true;
                            }
                            else
                            {
                                textOutput.Text += ",";
                            }
                        }
                        break;

                    #endregion čísla

                    #region základní znamínka

                    case "plus":
                        SignInput("+");
                        InputShow(Example);
                        break;

                    case "minus":
                        SignInput("-");
                        InputShow(Example);
                        break;

                    case "krat":
                        SignInput("×");
                        InputShow(Example);
                        break;

                    case "deleno":
                        SignInput("÷");
                        InputShow(Example);
                        break;

                    #endregion základní znamínka

                    #region pokročilá znamínka

                    case "mocnina":
                        SignInput("^");
                        InputShow(Example);
                        break;

                    case "odmocnina":
                        SignInput("√");
                        InputShow(Example);
                        break;

                    case "faktorial":
                        //SignInput("!");
                        //InputShow(Example);
                        try
                        {
                            if (double.Parse(textOutput.Text) > 170) throw new ArgumentOutOfRangeException();
                            textOutput.Text = M.fce_fakt(double.Parse(textOutput.Text)).ToString();
                            MadeFakt = true;
                        }
                        catch (Exception ex)
                        {
#if DEBUG
                            Console.ForegroundColor = ConsoleColor.Red;
                            Console.WriteLine("Exception: {0}", ex.TargetSite.Name);
                            Console.ResetColor();
#endif
                            switch (ex.TargetSite.Name)
                            {
                                case "fce_fakt":
                                    textOutput.Text = "NO NEG FAKT";
                                    break;

                                default:
                                    textOutput.Text = "FAKT ERROR";
                                    break;
                            }
                            GotError = true;
                            PointMode = false;
                            ANSWER = "0";
                            NumberMode = false;
                            Example = string.Empty;
                            ExampleList.Clear();
                            textInput.Text = "0";
                        }

                        break;

                    #endregion pokročilá znamínka

                    #region pomocná tlačítka

                    case "zpet":
                        if (!(string.IsNullOrEmpty(textOutput.Text)) && textOutput.Text.Length > 1)
                        {
                            if (textOutput.Text[textOutput.Text.Length - 1] == '.')
                                PointMode = false;
                            textOutput.Text = textOutput.Text.Remove(textOutput.Text.Length - 1);
                        }
                        else
                        {
                            textOutput.Text = "0";
                        }
                        break;

                    case "clean":
                        textOutput.Text = "0";
                        PointMode = false;
                        break;

                    case "cleanall":
                        Example = string.Empty;
                        ExampleList.Clear();
                        textOutput.Text = "0";
                        textInput.Text = "0";
                        PointMode = false;
                        break;

                    case "switch":
                        ResultSwitch();
                        break;

                    #endregion pomocná tlačítka

                    #region konečná tlačítka

                    case "ANS":
                        textOutput.Text = ANSWER;
                        break;

                    case "ROVNASE":
                        Example += textOutput.Text;
                        ExampleList.Add(textOutput.Text);
                        InputShow(Example);
                        textOutput.Text = "0";
                        InputProcess(Example);
                        break;

                        #endregion konečná tlačítka
                }
            }
        }

        /**
         * \brief Výpočet výsledku
         * \param input Kontrolní string, kde je celý příklad
         */

        private void InputProcess(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                textInput.Text = "0";
            }
            else
            {
                int index = 0;

                try
                {
                    #region výpočet

                    while (ExampleList.Count > 1)   //výpočet celého příkladu
                    {
#if DEBUG
                        Console.WriteLine("//-------------------------");
                        Console.ForegroundColor = ConsoleColor.Blue;
                        Console.WriteLine("ExampleList.Count = {0}", ExampleList.Count);
                        Console.ForegroundColor = ConsoleColor.Cyan;
                        for (int i = 0; i < ExampleList.Count; i++) Console.WriteLine("\tExampleList[{0}]\t=\t{1}", i, ExampleList[i]);
                        Console.ResetColor();
                        Console.WriteLine("//-------------------------");
#endif

                        //prioritník
                        if (ExampleList.Contains("!"))  //pokud stále existuje
                        {
                            index = ExampleList.IndexOf("!");   //nalezení indexu

                            ExampleList[index - 1] = M.fce_fakt(double.Parse(ExampleList[index - 1])).ToString();   //výpočet

                            ExampleList.RemoveAt(index);    //smazaní nadbytečného
                        }
                        else if (ExampleList.Contains("√")) //jinak pokud stále  existuje
                        {
                            index = ExampleList.IndexOf("√");   //nalezení indexu
                            //if (int.Parse(ExampleList[index - 1]) > 15) throw new ArgumentOutOfRangeException();
                            ExampleList[index] = M.fce_odmocnina(double.Parse(ExampleList[index + 1]), int.Parse(ExampleList[index - 1]), iter).ToString();    //výpočet

                            ExampleList.RemoveAt(index + 1);    //smazání nadbytečného
                            ExampleList.RemoveAt(index - 1);    //smazaní nadbytečného
                        }
                        else if (ExampleList.Contains("^"))  //pokud stále existuje
                        {
                            index = ExampleList.IndexOf("^");   //nalezení indexu

                            ExampleList[index] = M.fce_mocnina(double.Parse(ExampleList[index - 1]), int.Parse(ExampleList[index + 1])).ToString();    //výpočet

                            ExampleList.RemoveAt(index + 1);    //smazání nadbytečného
                            ExampleList.RemoveAt(index - 1);    //smazaní nadbytečného
                        }
                        else if (ExampleList.Contains("÷"))  //pokud stále existuje
                        {
                            index = ExampleList.IndexOf("÷");   //nalezení indexu

                            ExampleList[index] = M.fce_deleno(double.Parse(ExampleList[index - 1]), double.Parse(ExampleList[index + 1])).ToString();    //výpočet

                            ExampleList.RemoveAt(index + 1);    //smazání nadbytečného
                            ExampleList.RemoveAt(index - 1);    //smazaní nadbytečného
                        }
                        else if (ExampleList.Contains("×"))  //pokud stále existuje
                        {
                            index = ExampleList.IndexOf("×");   //nalezení indexu

                            ExampleList[index] = M.fce_krat(double.Parse(ExampleList[index - 1]), double.Parse(ExampleList[index + 1])).ToString();    //výpočet

                            ExampleList.RemoveAt(index + 1);    //smazaní nadbytečného
                            ExampleList.RemoveAt(index - 1);    //smazání nadbytečného
                        }
                        else if (ExampleList.Contains("-"))  //pokud stále existuje
                        {
                            index = ExampleList.IndexOf("-");   //nalezení indexu

                            ExampleList[index] = M.fce_minus(double.Parse(ExampleList[index - 1]), double.Parse(ExampleList[index + 1])).ToString();    //výpočet

                            ExampleList.RemoveAt(index + 1);    //smazaní nadbytečného
                            ExampleList.RemoveAt(index - 1);    //smazání nadbytečného
                        }
                        else if (ExampleList.Contains("+"))  //pokud stále existuje
                        {
                            index = ExampleList.IndexOf("+");   //nalezení indexu

                            ExampleList[index] = M.fce_plus(double.Parse(ExampleList[index - 1]), double.Parse(ExampleList[index + 1])).ToString();    //výpočet

                            ExampleList.RemoveAt(index + 1);    //smazaní nadbytečného
                            ExampleList.RemoveAt(index - 1);    //smazání nadbytečného
                        }
                        else
                        {
                            textOutput.Text = "CALC ERROR"; //CHYBA
                            ExampleList.Clear();    //vyčištění
                            Example = string.Empty; //vyčištění
                            InputShow(Example); //vyčištění
                            return;
                        }
                    }

                    #endregion výpočet

                    textOutput.Text = ExampleList[0];   //výpis výsledku
                    ANSWER = ExampleList[0];    //uložení výsledku
                    GotResult = true;   //dostal jsem výsledek
                    MadeFakt = false;
                }
                catch (Exception ex)
                {
#if DEBUG
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("Exception: {0}", ex.TargetSite.Name);
                    Console.ResetColor();
#endif

                    switch (ex.TargetSite.Name)
                    {
                        case "fce_deleno":
                            textOutput.Text = "DIV BY ZERO";
                            break;

                        case "fce_odmocnina":
                            textOutput.Text = "BAD ROOT NUMBER";
                            break;

                        case "StringToNumber":
                            textOutput.Text = "NO DOUBLE EXP";
                            break;

                        default:
                            textOutput.Text = "CALC ERROR";
                            break;
                    }
                    NumberMode = false;
                    GotError = true;
                    ANSWER = "0";
                    textInput.Text = "0";
                }
                Example = string.Empty; //vyčištění
                ExampleList.Clear();    //vyčištění
            }
        }
    }
}
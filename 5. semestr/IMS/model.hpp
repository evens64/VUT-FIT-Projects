// Soubor:         model.hpp
// Kódování:       UTF-8
// Projekt:        IMS projekt
// Autor:          Jakub Frýz          (xfryzj01)
//                 Filip Dostálík      (xdosta46)

/**
 *
 * Výroba brožurovaných knih na lince Trend-Binder
 * firmy CPI Moravia Books s.r.o., Pohořelice
 *
 * Postup výroby:
 * 1.   Snášečka sesnáší archy do jednoho bloku
 *      (Podle zakázky jsou z jednoho bloku 1-2 knihy)
 * 2.   Lepička tento blok slepí dohromady
 * 3.   Frontero ořízne nejdelší hranu (naproti hřbetu)
 *      (podle zakázky, jinak nejede)
 * 4.   Pila rozřízne blok na půl
 *      (podle zakázky, jinak nejede)
 * 5.   Trojřez ořízne všechny 3 hrany
 *      (nebo jenom 2 krajní, pokud jelo frontero)
 * 6.   Linka končí na paleťáku
 *
 */

#ifndef MODEL_H
#define MODEL_H

// ------------------------------------------------------------------------------------------------
// Hlavičky
// ------------------------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "simlib.h"

// ------------------------------------------------------------------------------------------------
// Konstatní proměnné (NASTAVENÍ ZAKÁZKY)
// ------------------------------------------------------------------------------------------------

// doba konání testování (počet hodin * 3600)
const unsigned int HowLong = 1 * 3600;
// VÝKON: knih za hodinu (0.95 * VÝKON)
const double BOOKSpH = 0.95 * 2500;
// VÝKON: sekund za knihu (NEMĚNIT)
const double SECpBOOK = 3600 / BOOKSpH;
// obsazení lepičky (27 ramen, 3 vždy prázdná) (NEMĚNIT)
const int LepickaCAP = (27 - 3);
// Frontero nejede u každé zásilky (true/false)
const bool FronteroON = true;
// Pila nejede u každé zásilky (true/false)
const bool PilaON = true;
// kolik knížek trojřez seřízne najednou (x > 0)
const unsigned int TrojrezCAP = 3;
// kolik knížek bude ve sloupci z paleťáku (x > 0)
const unsigned int PaletakCAP = 4;
// výchylka pro normální rozdělení (NEMĚNIT)
const double vychylka = 0.1;
// počet spuštěných mašin (NEMĚNIT)
const unsigned int machineCount = (FronteroON) ? (PilaON) ? 6 : 5 : (PilaON) ? 5 : 4;

// ------------------------------------------------------------------------------------------------
// Inline fce
// ------------------------------------------------------------------------------------------------
inline double scale(int x) {
  return (((2000 * x) / BOOKSpH) >= 0.5) ? ((2000 * x) / BOOKSpH) : 0.5;
}

// ------------------------------------------------------------------------------------------------
// Generátor bloků (snášečka)
// ------------------------------------------------------------------------------------------------
class Generator : public Event {
  int active = 0;
  void Behavior();
};

// ------------------------------------------------------------------------------------------------
// Deklarace tříd a metod
// ------------------------------------------------------------------------------------------------
class BlokSL : public Process {   // CESTA: Snášečka -> Lepička
  double pocatek;
  void Behavior();
};

class BlokLF : public Process {   // CESTA: Lepička -> Frontero
  double pocatek;
  void Behavior();
};

class BlokLP : public Process {   // CESTA: Lepička -> Pila
  double pocatek;
  void Behavior();
};

class BlokLT : public Process {   // CESTA: Lepička -> Trojřez
  double pocatek;
  void Behavior();
};

class BlokFP : public Process {   // CESTA: Frontero -> Pila
  double pocatek;
  void Behavior();
};

class BlokFT : public Process {   // CESTA: Frontero -> Trojřez
  double pocatek;
  void Behavior();
};

class BlokPT : public Process {   // CESTA: Pila -> Trojřez
  double pocatek;
  void Behavior();
};

class BlokTP : public Process {   // CESTA: Trojřez -> Paleťák
  double pocatek;
  void Behavior();
};

class Kniha : public Process {   // Knihy na paleťe
  double pocatek;
  void Behavior();
};

class Chyba : public Process {
  void Behavior();
};

class ErrorGen : public Event {
  int IMS_ID;
  double IMS_damage_time;
  double IMS_repair_time;
  ErrorGen(int id, double damage, double repair) : Event(), IMS_ID(id), IMS_damage_time(damage), IMS_repair_time(repair) { }
  void Behavior();
};


#endif

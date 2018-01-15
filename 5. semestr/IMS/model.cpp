// Soubor:         model.cpp
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

#include "model.hpp"


// ------------------------------------------------------------------------------------------------
// Třída na uložení stavu mašin
// ------------------------------------------------------------------------------------------------
class machine_queue {
  private:
    int count;          // počet mašin
    bool running[10];   // jede mašina?
    bool damaged[10];   // je mašina poškozena?

  public:
    // konstruktor
    machine_queue(int count) {
      this->count = count;
      for (int i = 0; i < count; i++) {
        running[i] = true;
        damaged[i] = false;
      }
    } /* konstruktor */

    // výpis stavu linky
    void print() {
      for (int i = 0; i < this->count; i++) { std::cout << "[" << i << "] "; }
      std::cout << std::endl;

      for (int i = 0; i < this->count; i++) { std::cout << "[" << ((running[i] == true) ? "t" : "f") << "] "; }
      std::cout << std::endl;

      for (int i = 0; i < this->count; i++) { std::cout << "[" << ((damaged[i] == true) ? "t" : "f") << "] "; }
      std::cout << std::endl << std::endl;
    } /* print */

    // poškodí mašinu 'machine'
    void damage(int machine) {
      if (machine >= count) return;
      if (this->damaged[machine] == true) return;
      this->damaged[machine] = true;

      // jelikož je mašina poškozena, ta a předchozí mašiny se musí vypnout
      for (int i = machine; i >= 0; i--) {
        this->running[i] = false;
      }
    } /* damage */

    // opraví mašinu 'machine'
    void repair(int machine) {
      if (machine >= count) return;
      if (this->damaged[machine] == false) return;
      this->damaged[machine] = false;

      // pokud další mašiny jedou (pokud ne, některá je poškozena)
      if (this->running[machine+1] == true) {
        // projdi předchozí mašiny
        for (int i = machine; i >= 0; i--) {
          // pokud není poškozena, spusť ji (pokud je poškozena, nespouštěj ji a ani jiné před ní)
          if (this->damaged[i] == false) {
            this->running[i] = true;
          } else break;
        }
      }
    } /* repair */

    // true - pokud je mašina 'machine' v provozu
    bool is_running(int machine) {
      if (machine >= count) return false;
      return this->running[machine];
    } /* is_running */

    // true - pokud je mašina 'machine' poškozena
    bool is_damaged(int machine) {
      if (machine >= count) return false;
      return this->damaged[machine];
    } /* is_damaged */
};

// ------------------------------------------------------------------------------------------------
// Globální proměnné
// ------------------------------------------------------------------------------------------------
machine_queue Q(machineCount);

// jednotlivé stroje linky
//Facility    Snasecka  ("Snasecka");
Store       Lepicka   ("Lepicka", LepickaCAP);
Facility    Frontero  ("Frontero");
Facility    Pila      ("Pila");
Store       Trojrez   ("Trojrez", TrojrezCAP);
Store       Paletak   ("Paletak", PaletakCAP);

// statistiky, histogramy
Stat        LepickaS  ("Doba obsluhy (Lepicka)");
Stat        FronteroS ("Doba Obsluhy (Frontero)");
Stat        PilaS     ("Doba Obsluhy (Pila)");
Stat        TrojrezS  ("Doba Obsluhy (Trojrez)");
Stat        PaletakS  ("Doba Obsluhy (Paletak)");

// OdHodnoty    = 0
// Krok         = 15 * 60
// Pocet Trid   = 30
//Histogram   Table("Histogram", 0, 15*60, 30);

int book_count_in = 0;
int book_count_out = 0;

// ------------------------------------------------------------------------------------------------
// Generátor bloků (snášečka)
// ------------------------------------------------------------------------------------------------
void Generator::Behavior() {
  (new BlokSL)->Activate();
  book_count_in++;
  Activate(Time + Normal(SECpBOOK, vychylka));
}

// ------------------------------------------------------------------------------------------------
// Cesta knihy
// ------------------------------------------------------------------------------------------------
void BlokSL::Behavior() {
  Wait(Normal(scale(15), vychylka));

  pocatek = Time;

  Enter(Lepicka, 1);
  Wait(Normal(scale(20), vychylka));
  Leave(Lepicka, 1);

  if (FronteroON) {
    (new BlokLF)->Activate();
  } else if (PilaON) {
    (new BlokLP)->Activate();
  } else {
    (new BlokLT)->Activate();
  }

  LepickaS(Time - pocatek);
}

void BlokLF::Behavior() {
  Wait(Normal(scale(120), vychylka));

  pocatek = Time;

  Seize(Frontero);
  Wait(Normal(scale(6), vychylka));
  Release(Frontero);

  if (PilaON) {
    (new BlokFP)->Activate();
  } else {
    (new BlokFT)->Activate();
  }

  FronteroS(Time - pocatek);
}

void BlokLP::Behavior() {
  Wait(Normal(scale(136), vychylka));

  pocatek = Time;

  Seize(Pila);
  Wait(Normal(scale(5), vychylka));
  Release(Pila);

  (new BlokPT)->Activate();
  (new BlokPT)->Activate();

  PilaS(Time - pocatek);
}

void BlokLT::Behavior() {
  Wait(Normal(scale(150), vychylka));

  pocatek = Time;

  Enter(Trojrez, 1);
  Wait(Normal(scale(3), vychylka));
  if (Trojrez.Full()) {
    Leave(Trojrez, TrojrezCAP);
    for (int i = 0; i < TrojrezCAP; i++) (new BlokTP)->Activate();
  }

  TrojrezS(Time - pocatek);
}

void BlokFP::Behavior() {
  Wait(Normal(scale(24), vychylka));

  pocatek = Time;

  Seize(Pila);
  Wait(Normal(scale(5), vychylka));
  Release(Pila);

  (new BlokPT)->Activate();
  (new BlokPT)->Activate();

  PilaS(Time - pocatek);
}

void BlokFT::Behavior() {
  Wait(Normal(scale(30), vychylka));

  pocatek = Time;

  Enter(Trojrez, 1);
  Wait(Normal(scale(3), vychylka));
  if (Trojrez.Full()) {
    Leave(Trojrez, TrojrezCAP);
    for (int i = 0; i < TrojrezCAP; i++) (new BlokTP)->Activate();
  }

  TrojrezS(Time - pocatek);
}

void BlokPT::Behavior() {
  Wait(Normal(scale(12), vychylka));

  pocatek = Time;

  Enter(Trojrez, 1);
  Wait(Normal(scale(3), vychylka));
  if (Trojrez.Full()) {
    Leave(Trojrez, TrojrezCAP);
    for (int i = 0; i < TrojrezCAP; i++) (new BlokTP)->Activate();
  }

  TrojrezS(Time - pocatek);
}


void BlokTP::Behavior() {
  Wait(Normal(scale(7), vychylka));

  pocatek = Time;

  Enter(Paletak, 1);
  Wait(Normal(scale(2), vychylka));
  if (Paletak.Full()) {
    Leave(Paletak, PaletakCAP);
    for (int i = 0; i < PaletakCAP; i++) (new Kniha)->Activate();
  }

  PaletakS(Time - pocatek);
}

void Kniha::Behavior() {
  book_count_out++;
}

// ------------------------------------------------------------------------------------------------
// Generovaná chyba
// ------------------------------------------------------------------------------------------------
void Chyba::Behavior() { }

// ------------------------------------------------------------------------------------------------
// Generátory poruch
// ------------------------------------------------------------------------------------------------
void ErrorGen::Behavior() {
  if (Q.is_running(this->IMS_ID)) {
    // poškoď mašinu
    (new Chyba)->Activate();
    Activate(IMS_damage_time);
    Q.damage(IMS_ID);
  } else if (Q.is_damaged(this->IMS_ID)) {
    (new Chyba)->Activate();
    Activate(IMS_repair_time);
    Q.repair(IMS_ID);
  }
}

// ------------------------------------------------------------------------------------------------
// main()
// ------------------------------------------------------------------------------------------------
int main(int argc, char **argv) {
  std::string out(std::string(argv[0]) + ".out");
  Print("IMS SIMLIB project\n");
  Print("production line of books\n");
  SetOutput(out.c_str());

  // doba jedné směny
  Init(0, HowLong);

  // aktivace generátoru
  (new Generator)->Activate();

  // spuštění
  Run();

  // výpis proměnných
Print("+----------------------------------------------------------+ \n");
Print("| CUSTOM Moje promenne                                     | \n");
Print("+----------------------------------------------------------+ \n");
Print("|  Počet knih (in):    %d                                |\n", book_count_in);
Print("|  Počet knih (out):   %d                                |\n", book_count_out);
Print("+----------------------------------------------------------+ \n");

  // výpis z mašin
  //Snasecka.Output();
  Lepicka.Output();
  if (FronteroON) {
    Frontero.Output();
  }
  if (PilaON) {
    Pila.Output();
  }
  Trojrez.Output();
  Paletak.Output();

  //výpis STAT
  LepickaS.Output();
  if (FronteroON) {
    FronteroS.Output();
  }
  if (PilaON) {
    PilaS.Output();
  }
  TrojrezS.Output();
  PaletakS.Output();

  // ostatní výpis
  //Table.Output();


  return 0;
}

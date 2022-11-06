#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>

using namespace std;

// STALE

const int MAX_M = 30;
const int MAX_N = 50;


// STRUKTURY

struct plansza {
    char t[MAX_M][MAX_N]; // tablica
    int w;   // wiersze
    int k;   // kolumny
};

struct wymiar {
    int min;   // najmniejsza dopuszczalna wartosc
    int max;   // najwieksza dopuszczalna wartosc
    int wart;  // aktualna wartosc
};

// FUNKCJE

void czytaj_wymiary(plansza& tab, wymiar m, wymiar n, wymiar& lit);
void losuj_wymiary(plansza& tab, wymiar m, wymiar n, wymiar& lit);

void losuj_plansze(plansza& tab, wymiar lit);  //liczba wierszy, liczba kolumn, ile liter
void wypisz_plansze(plansza tab);         //liczba wierszy, liczba kolumn
void linia_gora(int x);
void linia_dol(int x);

void edytuj_plansze(plansza& tab, string znaki, short& p);
void zamiana_duzych(plansza& tab, char a);       //zmienia duze litery z planszy na podany znak
void male_na_duze(plansza& tab, short& p);              //zmienia male litery z planszy na duze

string to_upper(string znaki);                       //zamienia wszystkie litery napisu na duze
bool czy_wlasc_litera(string znaki, int lit);       // sprawdza, czy zawiera tylko duze litery z danego zakresu
bool czy_w_zakresie(wymiar x);       // czy wartosc miesci sie w danym zakresie
bool czy_liczba(string x);    // czy wprowadzone znaki sa liczba
int sti(string n);            // zamienia ciag znakow na liczbe (string to int)

bool wygrana(plansza tab);    // sprawdza, czy gra jest skonczona (nie zostaly zadne male litery)
void zakoncz(bool& gra);

void pomoc(plansza tab, bool& gra, bool pl);
char czyt_akcje();
void akcja(char x, plansza tab, bool& gra, bool pl);
void instrukcja(bool gra);

void naglowek() {
    system("cls");
    cout << "       WITAJ W MOJEJ GRZE!" << endl << endl;
}


//=================================================================================================================

int main()
{
    srand(time(NULL));

    // deklaracje stalych:

    const int minM = 5;
    const int minN = 5;
    const int minLit = 3;
    const int maxLit = 26;

    // deklaracje zmiennych:

    plansza pl;
    string znaki;

    wymiar m;
    wymiar n;
    wymiar lit;

    m.min = minM;
    m.max = MAX_M;

    n.min = minN;
    n.max = MAX_N;

    lit.min = minLit;
    lit.max = maxLit;

    short int przegrana = 0;
    char d;
    bool gra = 1;
    bool ist_pl = 0;  // czy istnieje plansza



 //**********************************************  PROGRAM GLOWNY **************************************************

    while (gra) {

        naglowek();
        pomoc(pl, gra, ist_pl);
        if (!gra) break;

        // zczytywanie danych:

        while (true) {
            cout << "'0'- losuj wymiary planszy, '1'- wpisz wymiary planszy: ";
            cin >> d;
            cout << endl;
            if (d == '0' || d == '1') {
                cout << endl;
                break;
            }
            else cout << "Bledne dane!!" << endl;
        }


        if (d != '0') czytaj_wymiary(pl, m, n, lit);
        else losuj_wymiary(pl, m, n, lit);

        ist_pl = 1;

        losuj_plansze(pl, lit);
        gra = 1;

        // wypisywanie planszy:

        while (gra) {

            wypisz_plansze(pl);


            if (wygrana(pl)) {
                cout << endl << "Koniec gry:\nWYGRANA!! :D" << endl << endl;
                gra = 0;
                break;
            }
            else if (przegrana >= 3) {
                cout << endl << "Koniec gry:\nPRZEGRANA :(" << endl << endl;
                gra = 0;
                break;
            }
            cout << endl;


            while (true) {
                cout << "Podaj ciag liter: ";
                cin >> znaki;
                if (znaki[0] == '?') {
                    pomoc(pl, gra, ist_pl);
                    if (!gra) break;
                }
                else if (czy_wlasc_litera(znaki, lit.wart)) {
                    edytuj_plansze(pl, znaki, przegrana);
                    break;
                }
                else cout << "BLAD: Nieprawidlowe znaki." << endl;

            }
        }
    }

    return 0;
}

//=================================================================================================================

// DEFINICJE FUNKCJI


// zczytywanie wymiarow
void czytaj_wymiary(plansza& tab, wymiar m, wymiar n, wymiar& lit) {

    while (true) {
        string str_m;
        cout << "Podaj liczbe wierszy <" << m.min << ";" << m.max << ">: ";
        cin >> str_m;

        if (czy_liczba(str_m)) m.wart = sti(str_m);
        else m.wart = 0;

        tab.w = m.wart;

        if (czy_w_zakresie(m)) {
            cout << endl;
            break;
        }
        else cout << "Bledne dane!!" << endl;
    }

    while (true) {
        string str_n;
        cout << "Podaj liczbe kolumn <" << n.min << ";" << n.max << ">: ";
        cin >> str_n;

        if (czy_liczba(str_n)) n.wart = sti(str_n);
        else n.wart = 0;

        tab.k = n.wart;

        if (czy_w_zakresie(n)) {
            cout << endl;
            break;
        }
        else cout << "Bledne dane!!" << endl;
    }

    while (true) {
        string str_lit;
        cout << "Podaj zakres liter <" << lit.min << ";" << lit.max << ">" << endl
            << "(np. dla a-e wcisnij 5, a-z- 26):  ";
        cin >> str_lit;

        if (czy_liczba(str_lit)) lit.wart = sti(str_lit);
        else lit.wart = 0;

        if (czy_w_zakresie(lit)) {
            cout << endl;
            break;
        }
        else cout << "Bledne dane!!" << endl;
    }

}

void losuj_wymiary(plansza& tab, wymiar m, wymiar n, wymiar& lit) {
    tab.w = (rand() % (m.max - m.min)) + m.min + 1;
    tab.k = (rand() % (n.max - n.min)) + n.min + 1;
    lit.wart = (rand() % (lit.max - lit.min)) + lit.min + 1;
}



// losowanie, wypisywanie, edytowanie planszy
void losuj_plansze(plansza& tab, wymiar lit) {
    int zn;
    for (int i = 0; i < tab.w; i++) {
        for (int j = 0; j < tab.k; j++) {
            zn = (rand() % lit.wart) + 97;
            tab.t[i][j] = (char)zn;     //losuje znaki od a do np. d (dla lit=4)
        }
    }
    tab.t[0][0] = 'A';
}


void wypisz_plansze(plansza tab) {
    system("cls");
    naglowek();
    cout << endl << "  AKTUALNA PLANSZA" << endl;
    linia_gora(tab.k);

    for (int i = 0; i < tab.w; i++) {
        cout << "  " << (char)186 << " ";
        for (int j = 0; j < tab.k; j++) cout << tab.t[i][j];
        cout << " " << (char)186 << endl;
    }

    linia_dol(tab.k);
    cout << endl;
}

void linia_gora(int x) {
    cout << "  " << (char)201;
    for (int i = 0; i < x + 2; i++) cout << (char)205;
    cout << (char)187 << endl;
}

void linia_dol(int x) {
    cout << "  " << (char)200;
    for (int i = 0; i < x + 2; i++) cout << (char)205;
    cout << (char)188 << endl;
    cout << "  '?'- pomoc" << endl;
}


void edytuj_plansze(plansza& tab, string znaki, short& p) {
    znaki = to_upper(znaki);
    for (int k = 0; k < znaki.length(); k++) {
        zamiana_duzych(tab, znaki[k]);
        male_na_duze(tab, p);
    }
}

void zamiana_duzych(plansza& tab, char a) {
    for (int i = 0; i < tab.w; i++) {
        for (int j = 0; j < tab.k; j++) {
            if (tab.t[i][j] >= 'A' && tab.t[i][j] <= 'Z') {
                tab.t[i][j] = a;
            }
        }
    }
}

void male_na_duze(plansza& tab, short& p) {
    int licz = 0;
    for (int i = 0; i < tab.w; i++) {
        for (int j = 0; j < tab.k; j++) {
            if ((tab.t[i][j] - 32 == tab.t[i - 1][j] && i > 0) ||          // jezeli wartosc komórki -32 (male litery zamienione na duze
                (tab.t[i][j] - 32 == tab.t[i + 1][j] && i < tab.w - 1) ||      // jest rowna wartosci komorki sasiedniej i komórka sasiednia
                (tab.t[i][j] - 32 == tab.t[i][j - 1] && j > 0) ||          // nie znajduje sie poza tablica
                (tab.t[i][j] - 32 == tab.t[i][j + 1] && j < tab.k - 1)
                ) {
                tab.t[i][j] = tab.t[0][0];                             // t[0][0] zawsze jest duza
                licz++;
            }
        }
    }
    if (licz == 0) p++;
}


string to_upper(string znaki) {
    int z;
    for (int i = 0; i < znaki.length(); i++) {
        z = (int)znaki[i];
        z = toupper(z);
        znaki[i] = (char)z;
    }

    return znaki;
}



// sprawdzanie warunkow
bool czy_wlasc_litera(string znaki, int lit) {
    znaki = to_upper(znaki);
    bool s = true;
    for (int i = 0; i < znaki.length(); i++) {
        if (znaki[i] < 'A' || znaki[i] > ('A' + lit - 1)) {       //czy napis zawiera tylko znaki od A do np. D (dla lit=4)
            s = false;
            break;
        }
    }

    return s;
}

bool czy_w_zakresie(wymiar x) {
    bool s = false;
    if (x.wart >= x.min && x.wart <= x.max) {
        s = true;
    }

    return s;
}

bool czy_liczba(string x) {
    bool s = true;
    for (int i = 0; i < x.length(); i++) {
        if (x[i] < '0' || x[i] > '9') {
            s = false;
            break;
        }
    }

    return s;
}

int sti(string n) {
    int mnoz = 1;
    int x = 0;

    for (int i = 0; i < n.length() - 1; i++) {
        mnoz *= 10;
    }
    for (int i = 0; i < n.length(); i++) {
        x += ((int)n[i] - (int)'0') * mnoz;
        mnoz /= 10;
    }

    return x;
}



// wyœwietlanie pomocy i wykonwanie operacji
void pomoc(plansza tab, bool& gra, bool pl) {
    cout << endl
        << "   'i'- instrukcja" << endl
        << "   'z'- zakoncz gre" << endl
        << "   inny klawisz- kontynuuj" << endl;

    char a = czyt_akcje();
    akcja(a, tab, gra, pl);
}

char czyt_akcje() {
    cout << endl << "akcja: ";
    char a;
    cin >> a;
    cout << endl;

    return a;
}

void akcja(char x, plansza tab, bool& gra, bool pl) {
    switch (x) {
    case 'i': instrukcja(gra);
        pomoc(tab, gra, pl);
        break;
    case 'z': zakoncz(gra);
        if (!gra) break;
    default: system("cls");
        naglowek();
        if (pl) wypisz_plansze(tab);
        break;
    }
}

void zakoncz(bool& gra) {
    char z;
    cout << endl << "Czy na pewno chesz zakonczyc gre?"
        << endl << "0- TAK, inny klawisz- NIE: ";
    cin >> z;

    if (z == '0') gra = 0;
    else gra = 1;

    cout << endl;
}

void instrukcja(bool gra) {
    naglowek();
    cout << endl
        << "INSTRUKCJA:" << endl
        << endl
        << "Gra odbywa sie na planszy wypelnionej malymi literami. Poczatkowo jedynie " << endl
        << "litera w lewym gornym rogu jest wielka. Gracz ma mozliwosc zmiany wielkich" << endl
        << "liter na dowolne inne (wielkie) litery. Mala litera zmienia sie na wielka," << endl
        << "gdy przy niej jest taka sama litera rozniaca sie wielkoscia." << endl
        << "Gracz konczy gre, jesli zamieni wsztystkie litery na wielkie." << endl
        << "Gracz przegrywa, jesli wprowadzi 3 razy znak, ktory nie zamieni zadnej" << endl
        << "litery z malej na wielka."
        << endl << endl;
}



// sprawdzanie, czy wygrano
bool wygrana(plansza tab) {
    bool s = true;
    for (int i = 0; i < tab.w; i++) {
        for (int j = 0; j < tab.k; j++) {
            if (tab.t[i][j] > 'Z') {
                s = false;
                break;
            }
        }
    }

    return s;
}

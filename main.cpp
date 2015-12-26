﻿#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>

using namespace std;

const int MAX_N = 600;

const int ROZMIAR_SITA = 1000000; // 10^6
const int PROBY_RM = 50; // proby w teste Rabina-Millera

// mapa dzielnikow pierwszych z krotnosciami
map<long long int, int> lista_dzielnikow;
map<long long int, int> lista_pomocnicza;

int losuj(int m)
{
    return (int)((double)m * (rand() / (RAND_MAX + 1.0)));
}

/* Sito Eratostenesa w wersji logarytmicznej. */
vector<int> sito()
{
    int* p = new int[ROZMIAR_SITA + 1];
    for (int i = 2; i <= ROZMIAR_SITA; i++) {
        p[i] = 1;
    }
    for (int i = 2; i <= ROZMIAR_SITA; i++) {
        if (p[i])
        {
            int j = 2 * i;  /* tu mozna by wpisac i*i, ale uwaga na rozmiar typu! */
            while (j <= ROZMIAR_SITA)
            {
                p[j] = 0;
                j += i;
            }
        }
    }

    vector<int> pierwsze;
    for (int i = 2; i <= ROZMIAR_SITA; i++) {
        if (p[i]) {
            pierwsze.push_back(i);
        }
    }
    delete[] p;
    return pierwsze;
}

/******* Test Millera-Rabina *******/
// return (a*b)%n
unsigned long long int mul(unsigned long long int a, unsigned long long int b, unsigned long long int n){
#define BASE (1ull<<30)
    unsigned long long int A[2] = { a%BASE, a / BASE };
    unsigned long long int B[2] = { b%BASE, b / BASE };
    unsigned long long int ret = (A[0] * B[0]) % n;
    unsigned long long int tmp;
    tmp = (A[1] * B[1]) % n;
    for (int i = 0; i<10; i++)
        tmp = (tmp << 3) % n;
    tmp = (tmp + A[0] * B[1] + A[1] * B[0]) % n;
    for (int i = 0; i<10; i++)
        tmp = (tmp << 3) % n;
    ret = (ret + tmp) % n;
    return ret;
}

//return (a^b)%n
long long int pow(long long int a, long long int b, long long int n){
    long long int ret = 1;
    while (b){
        if (b % 2) ret = mul(ret, a, n);
        b /= 2;
        a = mul(a, a, n);
    }
    return ret;
}

//test millera-rabina, jedna iteracja
bool witness(long long int a, long long int n){
    int t = 0;
    long long int x, n1 = n - 1;
    while (n1 % 2 == 0){
        t++; n1 /= 2;
    }
    x = pow(a, n1, n);
    for (int i = 0; i<t; i++){
        long long int nx = mul(x, x, n);
        if (nx == 1 && x != 1 && x != n - 1)
            return true;
        x = nx;
    }
    return x != 1;
}

//test millera-rabina, ca³y
bool pierwsza(long long int n){
    for (int i = 0; i<20; i++)
        if (witness(1ll + ((long long int)rand() % (n - 1)), n))
            return false;
    return true;
}

long long int pierwiastek (long long int n) {
   return (long long int)(sqrt(n) + 0.5);
}

// sprawdza, czy liczba jest kwadratem liczby całkowitej
bool czy_kwadrat(long long int n)
{
    long long int prw = pierwiastek(n);
    return prw*prw == n;
}

long long int nwd(long long int a, long long int b){
    if (a == 0) {
        return b;
    }
    return nwd(b%a, a);
}

void lacz_mapy(){
    for(auto l : lista_pomocnicza){
	long long int dzielnik = l.first;
        int krotnosc = l.second;
        if(lista_dzielnikow.find(dzielnik) != lista_dzielnikow.end()){
            lista_dzielnikow[dzielnik] += krotnosc;
        }else{
            lista_dzielnikow[dzielnik] = krotnosc;
        }
    }
}

void dodaj_do_listy(long long int n) {
    if (lista_dzielnikow.find(n) != lista_dzielnikow.end()) {
        lista_dzielnikow[n] = lista_dzielnikow[n] + 1;
    }
    else {
        lista_dzielnikow[n] = 1;
    }
}

void dodaj_do_listy2(long long int n) {
    if (lista_pomocnicza.find(n) != lista_pomocnicza.end()) {
        lista_pomocnicza[n] = lista_pomocnicza[n] + 1;
    }
    else {
        lista_pomocnicza[n] = 1;
    }
}

#define bool int

bool czy_wieksze(vector<bool> pierwsza, vector<bool> druga) {
    int wieksza_dlugosc = max(pierwsza.size(), druga.size());
    int przesuniecie_pierwszej = wieksza_dlugosc - pierwsza.size();
    int przesuniecie_drugiej = wieksza_dlugosc - druga.size();
    for(int i=0; i < wieksza_dlugosc; ++i) {
	bool pierwsza_cyfra, druga_cyfra;
	if(i < przesuniecie_pierwszej) {
	    pierwsza_cyfra = 0;
	} else {
	    pierwsza_cyfra = pierwsza[i - przesuniecie_pierwszej];
	}
	if(i < przesuniecie_drugiej) {
	    druga_cyfra = 0;
	} else {
	    druga_cyfra = druga[i - przesuniecie_drugiej];
	}
	if(pierwsza_cyfra == druga_cyfra) {
	}
	else if(pierwsza_cyfra > druga_cyfra) {
	    return true;
	}
	else if(pierwsza_cyfra < druga_cyfra) {
	    return false;
	}
    }
}

vector<bool> odejmij(vector<bool> pierwsza, vector<bool> druga) {
    // UWAGA odejmowanie bez pozyczek
    int wieksza_dlugosc = max(pierwsza.size(), druga.size());
    int przesuniecie_pierwszej = wieksza_dlugosc - pierwsza.size();
    int przesuniecie_drugiej = wieksza_dlugosc - druga.size();
    vector <bool> wynik;
    for(int i=0; i < wieksza_dlugosc; ++i) {
	bool pierwsza_cyfra, druga_cyfra;
	if(i < przesuniecie_pierwszej) {
	    pierwsza_cyfra = 0;
	} else {
	    pierwsza_cyfra = pierwsza[i - przesuniecie_pierwszej];
	}
	if(i < przesuniecie_drugiej) {
	    druga_cyfra = 0;
	} else {
	    druga_cyfra = druga[i - przesuniecie_drugiej];
	}
	if(pierwsza_cyfra == 1 && druga_cyfra == 0) {
	    wynik.push_back(1);
	} else if(pierwsza_cyfra == druga_cyfra) {
	    wynik.push_back(0);
	} else if(pierwsza_cyfra == 0 && druga_cyfra == 1) {
	    cout << "UWAGA POZYCZKA PRZY ODEJMOWNAIU" << endl;
	    exit(1);
	}
	
    }
    return wynik;
    
}

void wypisz_liczbe(vector<bool> n);

vector<bool> generuj_dzielnik(int ile_zer) {
    cout << "generuj_dzielnik("<<ile_zer<<") " << endl;
  
    vector<bool> dzielnik;
    dzielnik.push_back(1);
    dzielnik.push_back(0);
    dzielnik.push_back(1);
    dzielnik.push_back(0);
    
    for(int i = 0; i< ile_zer; ++i ){ 
	dzielnik.push_back(0);
    }
    
    return dzielnik;
}



vector<bool> podziel_przez_10(vector<bool> dzielna) {
  
	    cout << "podziel_przez_10, " << endl << "dzielna: ";
	    wypisz_liczbe(dzielna);
  
    int dlugosc = dzielna.size();
  
    vector<bool> wynik;
    
    for(int i = dlugosc; i>=0; --i) {
    cout << "jeszcze  zyje, i = " << i << ", dlugosc = " << dlugosc << endl;
	vector<bool> dzielnik = generuj_dzielnik(i);
	if(czy_wieksze(dzielna, dzielnik)) {
	    wynik.push_back(1);
	    dzielna = odejmij(dzielna, dzielnik);
	    cout << "Odejmuje liczbe:";
	    wypisz_liczbe(dzielnik);
	    cout << "Od:";
	    wypisz_liczbe(dzielna);
	} else {
	    wynik.push_back(0);
	    cout << "Pomijam odejmowanie:";
	    wypisz_liczbe(dzielnik);
	    wypisz_liczbe(dzielna);
	    
	}
    }
    return wynik;
}

void wypisz_dwa_do_n_minus_jeden(int n) {
    vector<bool> liczba;
    for(int i=0;i<n;++i) {
	liczba.push_back(1);
    }
    
    while(1) {
      cout << "AAAA" << endl;
	wypisz_liczbe(liczba);
	//vector<bool> modulo = modulo_10(liczba)
	vector<bool> liczba = podziel_przez_10(liczba);
	if(liczba.size() == 0 ){
	    break;
	}
    }
}

void wypisz_liczbe(vector<bool> n) {
    for(auto i: n) {
	cout << i << " ";
    }
    cout << "(dlugosc: "<< n.size() << ")"<< endl;
}

int main()
{
    wypisz_dwa_do_n_minus_jeden(8);
  
    return 0;
  
  
  
    ios_base::sync_with_stdio(0);
    srand((unsigned int)time(0));
    //////////////////////////////
    // WEJSCIE                  //
    //////////////////////////////
    int n;
    cin >> n;

    /// opis liczby N w postaci listy czynników
    long long int a[MAX_N + 10];
    for (int i = 0; i<n; i++){
        cin >> a[i];
    }

    /////////////////////////////////
    // ETAP 1: faktoryzacja        //
    /////////////////////////////////

    /// tablica zawieraj¹ca kolejne liczby pierwsze do miliona
    vector<int> liczby_pierwsze = sito();

    for (int i = 0; i < n; ++i) {
        for (vector<int>::iterator iter = liczby_pierwsze.begin(); iter != liczby_pierwsze.end(); ++iter){
            int liczba_pierwsza = *iter;
            while (a[i] % liczba_pierwsza == 0){
                dodaj_do_listy(liczba_pierwsza);
                a[i] = a[i] / liczba_pierwsza;
            }
            // for(auto liczba_pierwsza : liczby_pierwsze) {
            //		sprawdz, czy liczba_pierwsza dzieli a[i]
            //		jesli tak, to dodaj_do_listy(lista, a[i])
            //				   oraz a[i] = a[i] / liczba_pierwsza
        }
    }


    /////////////////////////////////
    // ETAP 2: Rabin-Miller        //
    /////////////////////////////////

    for (int i = 0; i < n; ++i) {
        if (a[i] != 1){
            if (pierwsza(a[i])==1){
                dodaj_do_listy(a[i]);
                a[i]=1;
            }

        }
    }


    /////////////////////////////////
    // ETAP 3: kwadraty pierwszych //
    /////////////////////////////////


    for (int i = 0; i < n; ++i) {
        if (a[i] != 1){
            if (czy_kwadrat(a[i])){
	        long long int p = pierwiastek(a[i]);
                dodaj_do_listy(p);
                dodaj_do_listy(p);
                a[i]=1;
            }

            // sprawdz, czy a[i] jest kwadratem liczby pierwszej
            // jesli tak, to


        }
    }

    /////////////////////////////////
    // ETAP 4: NWD parami          //
    /////////////////////////////////

    for (int i = 0; i < n; ++i) {
        if (a[i] != 1){
            // sprawdz, czy nwd tych dwoch liczb jest wieksze od 1
            // jesli tak, to dodaj to nwd do listy dwukrotnie
            // dodaj tez a[i]/nwd oraz a[j]/nwd
            for (int j = i+1; j < n; ++j) {
                if(a[j]!=1){
		      long long int nwd_ = nwd(a[i], a[j]);
		      if (nwd_>1){
			  dodaj_do_listy(nwd_);
			  dodaj_do_listy(nwd_);
			  dodaj_do_listy(a[i] / nwd_);
			  dodaj_do_listy(a[j] / nwd_);
			  a[i]=1;
			  a[j]=1;
		      }
                }
            }
        }
    }
    
    
    /////////////////////////////////
    // ETAP 4,5: sprawdzenie znanych dzielnikow //
    /////////////////////////////////
    
    for (int i = 0; i < n; ++i) {
	for(auto g : lista_dzielnikow){
	    if(a[i]%g.first==0){
		dodaj_do_listy(g.first);
		dodaj_do_listy((a[i]/g.first));
		a[i] = 1;
	    }
	}
    }

   // lacz_mapy();

    /////////////////////////////////
    // ETAP 5: liczby półpierwsze  //
    /////////////////////////////////

    for (int i = 0; i < n; ++i) {
        if (a[i] != 1){
            dodaj_do_listy(a[i]);
            dodaj_do_listy(a[i] + 1);
        }
        // jezeli po tych wszystkich etapach liczba a[i] wciaz nie jest rowna 1, to
        // znaczy ze ma dwa nie-wiadomo-jakie dzielniki (ale na pewno te dzielniki
        // nie wystepuja nigdzie indziej -> maja krotnosc 1)
    }

    //////////////////////////////
    // WYJSCIE                  //
    //////////////////////////////

    for (map<long long int, int>::iterator iter = lista_dzielnikow.begin(); iter != lista_dzielnikow.end(); ++iter){
      cout << "debug: " << (*iter ).first << ", krotnosc: "<< (*iter ).second << endl;
    }
    
    
    // przejdz po elementach mapy, znajdz dzielniki z maksymalna krotnoscia, policz je
    int NajKrotnosc = 0;
    for (map<long long int, int>::iterator iter = lista_dzielnikow.begin(); iter != lista_dzielnikow.end(); ++iter){
        pair<long long int, int> dzielnik = *iter;
        if (dzielnik.second>NajKrotnosc){
            NajKrotnosc = dzielnik.second;
        }
    }
    int Liczba_NajDziel = 0;
    for (map<long long int, int>::iterator iter = lista_dzielnikow.begin(); iter != lista_dzielnikow.end(); ++iter){
        pair<long long int, int> dzielnik = *iter;
        if (dzielnik.second == NajKrotnosc){
            Liczba_NajDziel++;
        }
    }
    cout << NajKrotnosc << endl;
    wypisz_dwa_do_n_minus_jeden(Liczba_NajDziel);

    // wypisz wynik
    // cout << "...";

    //system("PAUSE");
}

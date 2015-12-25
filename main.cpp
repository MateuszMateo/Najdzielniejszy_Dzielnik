#include <iostream>
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

// sprawdza, czy liczba jest kwadratem liczby całkowitej
bool czy_kwadrat(long long int n)
{
    long long int tst = (long long int)(sqrt(n) + 0.5);
    return tst*tst == n;
}

long long int nwd(long long int a, long long int b){
    if (a == 0) {
        return b;
    }
    return nwd(b%a, a);
}

void lacz_mapy(){
    for(auto l : lista_pomocnicza){
        int krotnosc=lista_pomocnicza[l];
        if(lista_pomocnicza.find(l) != lista_pomocnicza.end){
            lista_pomocnicza[l] += krotnosc;
        }else{
            lista_pomocnicza[l] = krotnosc;
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

int main()
{
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
                dodaj_do_listy(sqrt(a[i]));
                dodaj_do_listy(sqrt(a[i]));
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
            for (int j = 0; j < n; ++j) {
                if(a[j]!=1){
                    if(i>j){
                        if (nwd(a[i], a[j])>1){
                            for(auto g : lista_pomocnicza){
                                if(a[i]%g==0){
                                    dodaj_do_listy2(g);
                                    dodaj_do_listy2((a[i]/g));
                                }
                            }
                            dodaj_do_listy2(nwd(a[i], a[j]));
                            dodaj_do_listy2(a[i] / nwd(a[i], a[j]));
                            dodaj_do_listy2(a[j] / nwd(a[i], a[j]));
                            a[i]=1;
                            a[j]=j;
                        }
                    }
                }
            }
        }
    }

    lacz_mapy();

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
    int ABC=(int)pow((double)2,Liczba_NajDziel)-1;
    cout << ABC << endl;

    // wypisz wynik
    // cout << "...";

    //system("PAUSE");
}

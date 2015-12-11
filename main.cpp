#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>

using namespace std;

int main()
{
    int n;
    long long N=1;
    cin>>n;
    int tablica [n+1];
    for(int i=0;i<n;i++){
        cin>>tablica[i];
    }
    for(int i=0;i<n;i++){
        N=N*tablica[i];
    }

    int P=sqrt(N);
    int Wielokrotnosci[P+1];
    int Dzielniki[P+1];
    int DzielnikiPF[P+1];
    int Wielokrotnosci2[P+1];

    for(int i=0;i<=sqrt(N);i++){
        Wielokrotnosci[i]=0;
        Wielokrotnosci2[i]=0;
    }

    long long o=N;

    for(int j=2,m=1;j*j<=N;j++,m++){
        if(o%j==0){
            DzielnikiPF[m]=1;
            Dzielniki[m]=j;
        }
        while(o%j==0){
            o=o/j;
            Wielokrotnosci[m]++;
        }
    }
    for(int i=0;i<P;i++){
        Wielokrotnosci2[i]=Wielokrotnosci[1];
    }
    sort (Wielokrotnosci2,Wielokrotnosci2+(P+1));
    cout<<Wielokrotnosci2[P]<<endl;
    for(int i=0;i<P;i++){
        if(Wielokrotnosci[i]==Wielokrotnosci2[P]){
            cout<<Dzielniki[i]<<endl;
        }
    }
    //Sprawdzenie czy liczba i jej dzielniki zostały poprawnie odczytane
    for(int i=0;i<5;i++){
        cout<<endl;
    }
    cout<<"liczba to: "<<N<<endl;
    for(int i=0;i*i<=N;i++){
            if(DzielnikiPF[i]==1){
                cout<<"wielokrotnosci "<<Wielokrotnosci[i]<<" -dzielnik:"<<Dzielniki[i]<<endl;
            }
        }

    //od tąd to szkic innego rozwiązania, ponieważ to wyżej nie działa
    int D=0;
    for(int i=0;i*i<=N;i++){
        if(DzielnikiPF[i]==1){
            D++;
        }
    }
    int TablicaW[D+1];
    int TablicaD[D+1];
    int B=0;
    for(int i=0;i*i<=N;i++){
        if(DzielnikiPF[i]==1){
            TablicaW[B]=Wielokrotnosci[i];
            TablicaD[B]=Dzielniki[i];
            B++;
        }
    }

}


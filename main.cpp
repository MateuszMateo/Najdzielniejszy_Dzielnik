#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
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
    cout<<N<<endl;

    int P=sqrt(N);

    int Wielokrotnosci[P+1];
    int Dzielniki[P+1];
    bool DzielnikiPF[P+1];

    for(int i=0;i<=sqrt(N);i++){
        Wielokrotnosci[i]=0;
        DzielnikiPF[i]=0;
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
    for(int i=0;i*i<=N;i++){
        if(DzielnikiPF[i]==1){
            cout<<"wielokrotnosci "<<Wielokrotnosci[i]<<" -dzielnik:"<<Dzielniki[i]<<endl;
        }
    }
}


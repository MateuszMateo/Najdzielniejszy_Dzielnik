#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>

using namespace std;

int main()
{
    const int RS = 100000;
    int l_pierwsze[RS+1];
    for(int i=0;i<=RS;i++){
        l_pierwsze[i] = i;
    }
    for(int j=2;j*j<RS;j++){
        int i = 2;
        if(l_pierwsze[j]>0){
            l_pierwsze[j]=1;
        }
        while(j*i<=RS){
            l_pierwsze[j*i]=0;
            i++;
        }
    }
    for(int i=sqrt(RS);i<RS;i++){
        if(l_pierwsze[i]>1){
            l_pierwsze[i]=1;
        }
    }
    //tablica samych liczb pierwszych//
    int P=1;
    for(int i=0;i<RS;i++){
        if(l_pierwsze[i]){
            P++;
        }
    }
    int s_pierwsze[P+1];
    int l=0;
    for(int i=0;i<RS;i++){
        if(l_pierwsze[i]){
            s_pierwsze[l]=i;
            l++;
        }
    }
    cout<<P<<endl;
    for(int i=0;i<P;i++){
        cout<<s_pierwsze[i]<<", ";
    }
}

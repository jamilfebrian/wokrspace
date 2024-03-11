#include <iostream>
#include <math.h>

char panjang[7][4] = {"km","hm","dam","m","dm","cm","mm"};
double nilai;
using namespace std;

int main(){
    cout<<"Konversi satuan Panjang\n"<<endl;
    cout<<"masukkan nilai panjang (m) : ";
    cin>>nilai;

for(int i=-3; i<=3; i++){
    cout<<nilai*pow(10,i)<<" \t";
    for(int j=4; j<=6; j++){
        cout<<panjang[i+2][j];
} cout<<"\n";
}
} 
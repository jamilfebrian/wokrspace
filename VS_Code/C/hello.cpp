#include <iostream>
#include <string>

using namespace std;

class Mahasiswa{
    public:
        string NAMA;
        string NIM;
        string PRODY;


        Mahasiswa(string inputNama, string inputNIM, string inputPrody){
            Mahasiswa::NAMA = inputNama;
            Mahasiswa::NIM = inputNIM;
            Mahasiswa::PRODY = inputPrody;


        cout << Mahasiswa::NAMA << endl;
        cout << Mahasiswa::NIM << endl;
        cout << Mahasiswa::PRODY << endl;


        }
};

int main(int argc, char const *argv[])
{
    
    return 0;
}


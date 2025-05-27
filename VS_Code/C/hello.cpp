#include <iostream>
using namespace std;

int bil1, bil2;

int main()
{
    cout << "Masukkan Bilangan Pertama : ";
    cin >> bil1;
    cout << "Masukkan Bilangan Kedua : ";
    cin >> bil2;

    if (bil1 < bil2)
    {
        cout << "Angka " << bil1 << " lebih kecil dari Angka " << bil2 << endl;
    }
    else if (bil1 > bil2)
    {
        cout << "Angka " << bil1 << " lebih besar dari Angka " << bil2 << endl;
    }
    else if (bil1 == bil2)
    {
        cout << "Angka " << bil1 << " sama dengan Angka " << bil2 << endl;
    }

    cout << "Akhir dari Program";

    return 0;
}

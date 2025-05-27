#define pin 2

const int maxSize = 10; // Ukuran maksimum array
String myArray[maxSize] = {"15"};  // Array untuk menyimpan nilai
int currentSize = 1;   // Ukuran saat ini dari array

int x = 12;

bool isValueExists(String value) {
    for (int i = 0; i < currentSize; i++) {
        if (myArray[i] == value) {
            return true; // Nilai sudah ada
        }
    }
    return false; // Nilai belum ada
}

// Fungsi push untuk menambahkan elemen ke array
void push(String value) {
  if (isValueExists(value)) {
        Serial.print("Nilai ");
        Serial.print(value);
        Serial.println(" sudah ada di array. Tidak ditambahkan.");
        return;
    }
    
    if (currentSize < maxSize) {
        myArray[currentSize] = value; // Tambahkan di akhir array
        currentSize++; // Tambah ukuran array
        if(currentSize >= maxSize){
          currentSize = maxSize;
        }
    } else {
        Serial.println("Array penuh! Tidak bisa menambahkan nilai.");
    }
}

void removeValue(String value) {
    int index = -1;

    // Cari indeks elemen yang ingin dihapus
    for (int i = 0; i < currentSize; i++) {
        if (myArray[i] == value) {
            index = i;
            break;
        }
    }

    // Jika nilai ditemukan, geser elemen ke kiri
    if (index != -1) {
        for (int i = index; i < currentSize - 1; i++) {
            myArray[i] = myArray[i + 1];
        }
        currentSize--; // Kurangi ukuran array
        Serial.print("Nilai ");
        Serial.print(value);
        Serial.println(" berhasil dihapus.");
    } else {
        Serial.print("Nilai ");
        Serial.print(value);
        Serial.println(" tidak ditemukan.");
    }
}

// Fungsi untuk mencetak isi array
void printArray() {
    Serial.print("Array: ");
    for (int i = 0; i < currentSize; i++) {
        Serial.print(myArray[i]);
        Serial.print(" ");
    } Serial.println();
}

void setup() {
  Serial.begin(9600);
  pinMode(pin, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  int pinValue = digitalRead(pin);
  int remove_value = digitalRead(3);
  int add_value = digitalRead(4);
  String z = String(x);

  if(!pinValue){
    Serial.print("Menambahkan nilai: ");
    Serial.println(x);

    push(z);  // Memanggil fungsi push()
    printArray();    // Menampilkan array
    x++;

    delay(1000);
  }

  if(!remove_value){
    removeValue("13");
  }
  
  if(!add_value){
    push("13");
  }
}

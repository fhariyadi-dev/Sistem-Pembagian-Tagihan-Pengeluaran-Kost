#include <iostream>
#include <string>

using namespace std;

void tampilkanMenu() {
    cout << "\n==============================================" << endl;
    cout << "   SISTEM PATUNGAN & PENGELUARAN ANAK KOS     " << endl;
    cout << "==============================================" << endl;
    cout << "1. Tambah Data Pengeluaran Kos" << endl;
    cout << "2. Lihat Daftar & Total Pengeluaran" << endl;
    cout << "3. Cek Tagihan & Status Bayar Penghuni" << endl;
    cout << "4. Update Status Pembayaran Penghuni" << endl;
    cout << "5. Keluar Program" << endl;
    cout << "==============================================" << endl;
    cout << "Pilih menu (1-5): ";
}

int main() {
    int stateMenu;
    tampilkanMenu();
    cin >> stateMenu;

    if(stateMenu == 1){
        cout << "Item yang terpilih adalah `Tambah Data Pengeluaran Kos`";
    }
    else if(stateMenu == 2){
        cout << "Item yang terpilih adalah `Lihat Daftar & Total Pengeluaran`";
    }
    else if(stateMenu == 3){
        cout << "Item yang terpilih adalah `Cek Tagihan & Status Bayar Penghuni`";
    }
    else if(stateMenu == 4){
        cout << "Item yang terpilih adalah `Update Status Pembayaran Penghuni`";
    }
    else if(stateMenu == 5){
        cout << "Keluar Program...`";
    }
    else {cout << "Pilih dengan mengetik nomor yang tertera";}
    return 0;
}

// lagi proses code nya sabar yawww....
//pengen jadi progammer handal namun enggan ngodingg -imphen
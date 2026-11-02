/* ==============================================================================
   PROGRAM : SISTEM PEMBAGIAN TAGIHAN & PENGELUARAN KOS (PROGRES 1)
   ============================================================================== */

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



const int MAX_PENGHUNI  = 100;
const int MAX_TRANSAKSI = 500;

string namaPenghuni[MAX_PENGHUNI];
int    sudahDibayar[MAX_PENGHUNI];
int    jumlahPenghuni = 0;

string namaBarang[MAX_TRANSAKSI];
int    mingguKe[MAX_TRANSAKSI];
int    kategori[MAX_TRANSAKSI];
int    idxPenalang[MAX_TRANSAKSI];
int    nominal[MAX_TRANSAKSI];
int    jumlahTransaksi = 0;

void cetakGaris(int panjang, char simbol) {
    for (int i = 0; i < panjang; i++) {
        cout << simbol;
    }
    cout << "\n";
}

void cetakJudul(string judul) {
    cout << "\n";
    cetakGaris(65, '=');
    cout << "  " << judul << "\n";
    cetakGaris(65, '=');
}


string namaKategori(int kode) {
    if (kode == 1) {
        return "Kebutuhan Wajib";
    } else if (kode == 2) {
        return "Konsumsi Bersama";
    } else if (kode == 3) {
        return "Darurat / Lainnya";
    }
    return "Tidak Diketahui";
}

int main() {
    cetakJudul("SISTEM PEMBAGIAN TAGIHAN & PENGELUARAN ANAK KOS (C++)");
    cout << "  [Progres 1] Kerangka dasar array dan tampilan siap.\n";


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
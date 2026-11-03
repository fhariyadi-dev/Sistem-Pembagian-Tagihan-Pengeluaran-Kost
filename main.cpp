/* ==============================================================================
   PROGRAM : SISTEM PEMBAGIAN TAGIHAN & PENGELUARAN KOS
   ==============================================================================
*/
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Const disini semua
const int MAX_PENGHUNI  = 100;
const int MAX_TRANSAKSI = 500;
const string NAMA_FILE  = "laporan_akhir_kos.txt";

// Variabel global
string namaPenghuni[MAX_PENGHUNI];
int    sudahDibayar[MAX_PENGHUNI];
int    jumlahPenghuni = 0;
string namaBarang[MAX_TRANSAKSI];
int    mingguKe[MAX_TRANSAKSI];
int    kategori[MAX_TRANSAKSI];
int    idxPenalang[MAX_TRANSAKSI];
int    nominal[MAX_TRANSAKSI];
int    jumlahTransaksi = 0;

// Utilitas tampilan
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

bool muatDataDariTxt() {
    ifstream fileMasuk(NAMA_FILE.c_str());
    if (!fileMasuk.is_open()) {
        return false;
    }

    string baris;
    bool ditemukanData = false;

    while (getline(fileMasuk, baris)) {
        if (baris.find("===DATA_SISTEM===") != string::npos) {
            ditemukanData = true;
            break;
        }
    }

    if (!ditemukanData) {
        fileMasuk.close();
        return false;
    }

    fileMasuk >> jumlahPenghuni;
    if (jumlahPenghuni <= 0 || jumlahPenghuni > MAX_PENGHUNI) {
        jumlahPenghuni = 0;
        fileMasuk.close();
        return false;
    }

    for (int i = 0; i < jumlahPenghuni; i++) {
        getline(fileMasuk >> ws, namaPenghuni[i]);
        fileMasuk >> sudahDibayar[i];
    }

    fileMasuk >> jumlahTransaksi;
    if (jumlahTransaksi < 0 || jumlahTransaksi > MAX_TRANSAKSI) {
        jumlahTransaksi = 0;
    }

    for (int i = 0; i < jumlahTransaksi; i++) {
        getline(fileMasuk >> ws, namaBarang[i]);
        fileMasuk >> mingguKe[i] >> kategori[i] >> idxPenalang[i] >> nominal[i];
    }

    fileMasuk.close();
    return true;
}



void tampilkanMenu() {
    cout << "\n==============================================\n";
    cout << "   SISTEM PATUNGAN & PENGELUARAN ANAK KOS     \n";
    cout << "==============================================\n";
    cout << "  1. Tambah Pengeluaran Baru\n";
    cout << "  2. Lihat Semua Pengeluaran\n";
    cout << "  3. Edit Data Pengeluaran\n";
    cout << "  4. Hapus Data Pengeluaran\n";
    cout << "  5. Cari Pengeluaran (Berdasarkan Nama Barang)\n";
    cout << "  6. Filter Pengeluaran (Berdasarkan Kategori)\n";
    cout << "  7. Filter Pengeluaran (Berdasarkan Penalang Dana)\n";
    cout << "  8. Lihat Laporan Split Bill & Status Tagihan\n";
    cout << "  9. Bayar / Cicil Tagihan Penghuni\n";
    cout << " 10. Reset Data / Input Ulang Penghuni Baru\n";
    cout << "  0. Keluar Program\n";
}

// Input
int bacaPilihan(string pesan, int batasBawah, int batasAtas) {
    int angka;
    cout << pesan;
    cin >> angka;

    while (angka < batasBawah || angka > batasAtas) {
        cout << "  [!] Input harus antara " << batasBawah
             << " sampai " << batasAtas << ". Ulangi: ";
        cin >> angka;
    }
    return angka;
}

int bacaNominal(string pesan, int minimal) {
    int nilai;
    cout << pesan;
    cin >> nilai;

    while (nilai < minimal) {
        cout << "  [!] Nilai minimal adalah " << minimal << ". Ulangi: ";
        cin >> nilai;
    }
    return nilai;
}

// Kalkulasi
int hitungTalanganPenghuni(int idx) {
    int totalTalangan = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (idxPenalang[i] == idx) {
            totalTalangan = totalTalangan + nominal[i];
        }
    }
    return totalTalangan;
}

// Tampilan data penghuni
void tampilkanDaftarPenghuni() {
    cetakJudul("DAFTAR PENGHUNI KOS");
    for (int i = 0; i < jumlahPenghuni; i++) {
        cout << "  " << (i + 1) << ". " << namaPenghuni[i]
             << " (Total Talangan: Rp " << hitungTalanganPenghuni(i) << ")\n";
    }
    cetakGaris(65, '-');
}

void inisialisasiPenghuni() {
    cetakJudul("INISIALISASI DATA PENGHUNI KOS BARU");
    jumlahPenghuni = bacaPilihan("Masukkan jumlah penghuni kos (1-100): ", 1, MAX_PENGHUNI);
    jumlahTransaksi = 0;

    cout << "\nMasukkan nama masing-masing penghuni:\n";
    for (int i = 0; i < jumlahPenghuni; i++) {
        cout << "  Penghuni ke-" << (i + 1) << " : ";
        // FIX BUG TAHAP 2: Menambahkan cin >> ws agar input penghuni ke-1 tidak ter-skip
        getline(cin >> ws, namaPenghuni[i]);
        sudahDibayar[i] = 0;
    }
    cout << "\n[v] Data " << jumlahPenghuni << " penghuni berhasil disimpan.\n";
}

// Utilitas kategori
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

void tambahPengeluaran() {
    cetakJudul("TAMBAH DATA PENGELUARAN KOS");

    if (jumlahTransaksi >= MAX_TRANSAKSI) {
        cout << "  [!] Kapasitas data transaksi sudah penuh!\n";
        return;
    }

    cout << "  Nama Barang / Kebutuhan : ";
    getline(cin >> ws, namaBarang[jumlahTransaksi]);

    mingguKe[jumlahTransaksi] = bacaPilihan("  Minggu ke- (1-52)       : ", 1, 52);

    cout << "\n  Pilihan Kategori:\n";
    cout << "    1. Kebutuhan Wajib\n";
    cout << "    2. Konsumsi Bersama\n";
    cout << "    3. Darurat / Lainnya\n";
    kategori[jumlahTransaksi] = bacaPilihan("  Pilih Kategori (1-3)    : ", 1, 3);

    cout << "\n  Daftar Penghuni (Penalang Dana):\n";
    for (int i = 0; i < jumlahPenghuni; i++) {
        cout << "    " << (i + 1) << ". " << namaPenghuni[i] << "\n";
    }
    int pilihOrang = bacaPilihan("  Nomor Penalang Dana     : ", 1, jumlahPenghuni);

    // BUG TAHAP 3: Lupa mengurangi 1 (pilihOrang - 1) dan lupa menambah jumlahTransaksi++
    idxPenalang[jumlahTransaksi] = pilihOrang;
    nominal[jumlahTransaksi] = bacaNominal("  Nominal Biaya (Rp)      : Rp ", 1);

    cout << "\n[v] Transaksi pengeluaran berhasil ditambahkan!\n";
}

int main() {

    cetakJudul("SISTEM PEMBAGIAN TAGIHAN & PENGELUARAN ANAK KOS (C++)\n (Fitur Auto-Save & Auto-Load File .TXT)");
    inisialisasiPenghuni();

    int stateMenu;
    do {
        tampilkanMenu();
        cin >> stateMenu;
        stateMenu = bacaPilihan("Pilih menu (1-5): ", 1, 5);
        
        if (stateMenu == 1) {
            cout << "Item yang terpilih adalah `Tambah Data Pengeluaran Kos`\n";
        }
        else if (stateMenu == 2) {
            cout << "Item yang terpilih adalah `Lihat Daftar & Total Pengeluaran`\n";
        }
        else if (stateMenu == 3) {
            cout << "Item yang terpilih adalah `Cek Tagihan & Status Bayar Penghuni`\n";
        }
        else if (stateMenu == 4) {
            cout << "Item yang terpilih adalah `Update Status Pembayaran Penghuni`\n";
        }
        else if(stateMenu == 5) {
            cout << "Keluar Program...\n";
        }
        else {
            cout << "Pilih dengan mengetik nomor yang tertera\n";
        }
    } while (stateMenu != 5);

    return 0;
}

// lagi proses code nya sabar yawww....
//pengen jadi progammer handal namun enggan ngodingg -imphen
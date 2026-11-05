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

void editPengeluaran() {
    lihatSemuaPengeluaran();
    if (jumlahTransaksi == 0) return;

    int pilih = bacaPilihan("\nMasukkan nomor transaksi yang akan diedit (0 = Batal): ", 0, jumlahTransaksi);
    if (pilih == 0) return;

    int idx = pilih - 1;
    cout << "\n--- Masukkan Data Baru untuk Transaksi #" << pilih << " ---\n";
    cout << "  Nama Barang Baru (" << namaBarang[idx] << ") : ";
    getline(cin >> ws, namaBarang[idx]);

    mingguKe[idx] = bacaPilihan("  Minggu Baru (1-52)       : ", 1, 52);
    kategori[idx] = bacaPilihan("  Kategori Baru (1-3)      : ", 1, 3);

    for (int i = 0; i < jumlahPenghuni; i++) {
        cout << "    " << (i + 1) << ". " << namaPenghuni[i] << "\n";
    }
    int pilihOrang = bacaPilihan("  Nomor Penalang Baru      : ", 1, jumlahPenghuni);
    idxPenalang[idx] = pilihOrang - 1;

    nominal[idx] = bacaNominal("  Nominal Biaya Baru (Rp)  : Rp ", 1);
    cout << "\n[v] Data transaksi berhasil diperbarui!\n";
}

void hapusPengeluaran() {
    lihatSemuaPengeluaran();
    if (jumlahTransaksi == 0) return;

    int pilih = bacaPilihan("\nMasukkan nomor transaksi yang akan dihapus (0 = Batal): ", 0, jumlahTransaksi);
    if (pilih == 0) return;

    int idx = pilih - 1;
    for (int i = idx; i < jumlahTransaksi; i++) {
        namaBarang[i]  = namaBarang[i + 1];
        mingguKe[i]    = mingguKe[i + 1];
        kategori[i]    = kategori[i + 1];
        idxPenalang[i] = idxPenalang[i + 1];
        nominal[i]     = nominal[i + 1];
    }
    jumlahTransaksi--;
    cout << "\n[v] Transaksi berhasil dihapus!\n";
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
int hitungTotalPengeluaran() {
    int total = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        total = total + nominal[i];
    }
    return total;
}

int hitungTagihanRataRata() {
    if (jumlahPenghuni == 0) {
        return 0;
    }
    return hitungTotalPengeluaran() / jumlahPenghuni;
}

int hitungTagihanBersih(int idx) {
    return hitungTagihanRataRata() - hitungTalanganPenghuni(idx);
}

int hitungSisaTagihan(int idx) {
    int tagihanBersih = hitungTagihanBersih(idx);
    if (tagihanBersih <= 0) {
        return 0;
    }
    int sisa = tagihanBersih - sudahDibayar[idx];
    if (sisa < 0) {
        return 0;
    }
    return sisa;
}

string statusKelunasan(int idx) {
    int tagihanBersih = hitungTagihanBersih(idx);
    if (tagihanBersih < 0) {
        return "LUNAS (BERHAK REFUND)";
    } else if (tagihanBersih == 0 || hitungSisaTagihan(idx) == 0) {
        return "LUNAS";
    }
    return "BELUM LUNAS";
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
        getline(cin >> ws, namaPenghuni[i]);
        sudahDibayar[i] = 0;
    }
    cout << "\n[v] Data " << jumlahPenghuni << " penghuni berhasil disimpan.\n";
}

void cariNamaBarang() {
    cetakJudul("CARI PENGELUARAN BERDASARKAN NAMA BARANG");
    if (jumlahTransaksi == 0) return;

    string kataKunci;
    cout << "  Masukkan kata kunci nama barang: ";
    cin >> kataKunci; 

    int ditemukan = 0;
    int subtotal  = 0;
    cout << "\n--- Hasil Pencarian \"" << kataKunci << "\" ---\n";

    for (int i = 0; i < jumlahTransaksi; i++) {
        if (namaBarang[i].find(kataKunci) != string::npos) {
            cout << "  - [Minggu " << mingguKe[i] << "] " << namaBarang[i]
                 << " | Rp " << nominal[i] << "\n";
            ditemukan++;
            subtotal += nominal[i];
        }
    }
    if (ditemukan == 0) cout << "  [i] Tidak ditemukan.\n";
    else cout << "  Ditemukan " << ditemukan << " transaksi | Subtotal: Rp " << subtotal << "\n";
}

void filterBerdasarkanKategori() {
    cetakJudul("FILTER PENGELUARAN BERDASARKAN KATEGORI");
    if (jumlahTransaksi == 0) return;

    cout << "  1. Kebutuhan Wajib\n  2. Konsumsi Bersama\n  3. Darurat / Lainnya\n";
    int kodeKat = bacaPilihan("  Pilih Kategori (1-3): ", 1, 3);

    cout << "\n--- Daftar Kategori: " << namaKategori(kodeKat) << " ---\n";
    int ditemukan = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (kategori[i] == kodeKat) {
            cout << "  - " << namaBarang[i] << " | Rp " << nominal[i] << "\n";
            ditemukan++;
        }
    }
    if (ditemukan == 0) cout << "  [i] Tidak ada transaksi.\n";
}

void filterBerdasarkanPenalang() {
    cetakJudul("FILTER PENGELUARAN BERDASARKAN PENALANG DANA");
    if (jumlahTransaksi == 0) return;

    for (int i = 0; i < jumlahPenghuni; i++) cout << "  " << (i + 1) << ". " << namaPenghuni[i] << "\n";
    int pilihOrang = bacaPilihan("  Pilih Nomor Penghuni: ", 1, jumlahPenghuni);
    int idxOrang   = pilihOrang - 1;

    cout << "\n--- Riwayat Talangan: " << namaPenghuni[idxOrang] << " ---\n";
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (idxPenalang[i] == idxOrang) {
            cout << "  - " << namaBarang[i] << " | Rp " << nominal[i] << "\n";
        }
    }
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
    idxPenalang[jumlahTransaksi] = pilihOrang - 1;

    nominal[jumlahTransaksi] = bacaNominal("  Nominal Biaya (Rp)      : Rp ", 1);

    jumlahTransaksi++;
    cout << "\n[v] Transaksi pengeluaran berhasil ditambahkan!\n";
}

void lihatSemuaPengeluaran() {
    cetakJudul("DAFTAR SELURUH PENGELUARAN KOS");

    if (jumlahTransaksi == 0) {
        cout << "  [i] Belum ada data transaksi pengeluaran.\n";
        return;
    }

    for (int i = 0; i < jumlahTransaksi; i++) {
        cout << "  " << (i + 1) << ". " << namaBarang[i]
             << " (Minggu " << mingguKe[i] << ")\n";
        cout << "     Kategori : " << namaKategori(kategori[i])
             << " | Penalang : " << namaPenghuni[idxPenalang[i]]
             << " | Nominal : Rp " << nominal[i] << "\n";
        cetakGaris(65, '-');
    }
    cout << "  TOTAL KESELURUHAN (" << jumlahTransaksi << " Transaksi) : Rp "
         << hitungTotalPengeluaran() << "\n";
    cetakGaris(65, '=');
}

void tampilkanLaporanSplitBill() {
    cetakJudul("CEK TAGIHAN & STATUS BAYAR PENGHUNI");

    int total    = hitungTotalPengeluaran();
    int rataRata = hitungTagihanRataRata();

    cout << "  Jumlah Penghuni Kos      : " << jumlahPenghuni << " orang\n";
    cout << "  Total Pengeluaran Kos    : Rp " << total << "\n";
    cout << "  Beban Rata-Rata / Orang  : Rp " << rataRata << "\n";
    cetakGaris(65, '-');

    for (int i = 0; i < jumlahPenghuni; i++) {
        int talangan      = hitungTalanganPenghuni(i);
        int tagihanBersih = hitungTagihanBersih(i);
        int sisa          = hitungSisaTagihan(i);

        cout << "  " << (i + 1) << ". " << namaPenghuni[i] << "\n";
        cout << "     - Beban Rata-Rata : Rp " << rataRata << "\n";
        cout << "     - Uang Talangan   : Rp " << talangan << "\n";

        if (tagihanBersih < 0) {
            cout << "     - Hak Refund      : Rp " << (-tagihanBersih) << "\n";
        } else {
            cout << "     - Tagihan Bersih  : Rp " << tagihanBersih << "\n";
            cout << "     - Sudah Dibayar   : Rp " << sudahDibayar[i] << "\n";
            cout << "     - Sisa Tagihan    : Rp " << sisa << "\n";
        }
        cout << "     - Status Akhir    : " << statusKelunasan(i) << "\n";
        cetakGaris(65, '-');
    }
}

void prosesPembayaranTagihan() {
    tampilkanLaporanSplitBill();
    if (jumlahTransaksi == 0) {
        return;
    }

    int pilih = bacaPilihan("\nPilih nomor penghuni yang ingin membayar (0 = Batal): ", 0, jumlahPenghuni);
    if (pilih == 0) {
        return;
    }

    int idx  = pilih - 1;
    int sisa = hitungSisaTagihan(idx);

    if (sisa <= 0) {
        cout << "  [i] " << namaPenghuni[idx] << " sudah LUNAS.\n";
        return;
    }

    cout << "  Sisa tagihan " << namaPenghuni[idx] << " saat ini: Rp " << sisa << "\n";
    int bayar = bacaPilihan("  Masukkan nominal pembayaran (Rp): ", 1, sisa);

    sudahDibayar[idx] = sudahDibayar[idx] + bayar; // Pakai += biar akumulatif
    cout << "\n[v] Pembayaran Rp " << bayar << " berhasil dicatat!\n";
}

void simpanLaporanKeTxt() {
    ofstream fileKeluar(NAMA_FILE.c_str());
    if (!fileKeluar.is_open()) {
        cout << "  [!] Gagal menyimpan otomatis ke file " << NAMA_FILE << "!\n";
        return;
    }

    int total    = hitungTotalPengeluaran();
    int rataRata = hitungTagihanRataRata();

    fileKeluar << "===DATA_SISTEM===\n";
    fileKeluar << jumlahPenghuni << "\n";
    for (int i = 0; i < jumlahPenghuni; i++) {
        fileKeluar << namaPenghuni[i] << "\n";
        fileKeluar << sudahDibayar[i] << "\n";
    }
    fileKeluar << jumlahTransaksi << "\n";
    for (int i = 0; i < jumlahTransaksi; i++) {
        fileKeluar << namaBarang[i] << "\n";
        fileKeluar << mingguKe[i] << " " << kategori[i] << " " << idxPenalang[i] << " " << nominal[i] << "\n";
    }
}


int main() {

    cetakJudul("SISTEM PEMBAGIAN TAGIHAN & PENGELUARAN ANAK KOS (C++)\n (Fitur Auto-Save & Auto-Load File .TXT)");
    inisialisasiPenghuni();

    int stateMenu;
    do {
        tampilkanMenu();
        cin >> stateMenu;
        stateMenu = bacaPilihan("Pilih menu (1-10): ", 1, 10);
        
        if (stateMenu == 1) {
            cout << "Item yang terpilih adalah `Tambah Data Pengeluaran Kos`\n";
        }
        else if (stateMenu == 2) {
            lihatSemuaPengeluaran();
        }
        else if (stateMenu == 3) {
            tampilkanLaporanSplitBill();
        }
        else if (stateMenu == 4) {
            prosesPembayaranTagihan();
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
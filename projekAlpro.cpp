#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

struct Penumpang {
    string kodBooking;
    string nama;
    string nik;
    string asal;
    string tujuan;
    string tanggal;
    string kelas;
    int nomorKursi;

    Penumpang* next;
    Penumpang* prev;
};

Penumpang* head = nullptr;
Penumpang* tail = nullptr;
int jumlahPenumpang = 0;
const int KAPASITAS_PESAWAT = 50;


void bersihkanLayar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void cetakGaris() {
    cout << "================================================" << endl;
}

// logo pesawat ASCII - dibuat manual pakai karakter biasa
void tampilkanLogo() {
    cout << "          ___                        " << endl;
    cout << "         /   \\                       " << endl;
    cout << "        / SKY \\                      " << endl;
    cout << "   ____/  PASS \\_________            " << endl;
    cout << "  /      _______         \\           " << endl;
    cout << " (=======|  |  |==========)-->>      " << endl;
    cout << "  \\______| _|_ |_________/           " << endl;
    cout << "          / | \\                      " << endl;
    cout << "         /  |  \\                     " << endl;
}

void tampilkanHeader() {
    cetakGaris();
    tampilkanLogo();
    cetakGaris();
    cout << "       SKYPASS TERMINAL SYSTEM        " << endl;
    cout << "    Sistem Pemesanan Tiket Pesawat    " << endl;
    cetakGaris();
}

string buatKodeBooking() {
    return "SKY" + to_string(1000 + jumlahPenumpang + 1);
}

void tungguEnter() {
    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}


void simpanKeFile() {
    FILE* file = fopen("data_tiket.txt", "w");

    if (file == NULL) {
        cout << "[ERROR] Gagal menyimpan data ke file!" << endl;
        return;
    }

    Penumpang* sekarang = head;

    while (sekarang != nullptr) {
        // tulis setiap field satu per baris
        fprintf(file, "%s\n", sekarang->kodBooking.c_str());
        fprintf(file, "%s\n", sekarang->nama.c_str());
        fprintf(file, "%s\n", sekarang->nik.c_str());
        fprintf(file, "%s\n", sekarang->asal.c_str());
        fprintf(file, "%s\n", sekarang->tujuan.c_str());
        fprintf(file, "%s\n", sekarang->tanggal.c_str());
        fprintf(file, "%s\n", sekarang->kelas.c_str());
        fprintf(file, "%d\n", sekarang->nomorKursi);

        sekarang = sekarang->next;
    }

    fclose(file);
}

void muatDariFile() {
    FILE* file = fopen("data_tiket.txt", "r");

    // kalau file belum ada, langsung keluar (program baru pertama kali dibuka)
    if (file == NULL) return;

    char buf[256];        // tempat tampung sementara tiap baris
    string fields[8];     // 8 field per penumpang

    while (true) {
        bool selesai = false;

        // baca 8 baris = 1 data penumpang
        for (int i = 0; i < 8; i++) {
            if (fgets(buf, sizeof(buf), file) == NULL) {
                selesai = true;
                break;
            }

            fields[i] = buf;

            // hapus karakter newline di akhir baris
            if (!fields[i].empty() && fields[i].back() == '\n')
                fields[i].pop_back();
        }

        if (selesai) break;

        Penumpang* baru = new Penumpang();
        baru->kodBooking = fields[0];
        baru->nama       = fields[1];
        baru->nik        = fields[2];
        baru->asal       = fields[3];
        baru->tujuan     = fields[4];
        baru->tanggal    = fields[5];
        baru->kelas      = fields[6];
        baru->nomorKursi = stoi(fields[7]);

        baru->next = nullptr;
        baru->prev = nullptr;

        if (head == nullptr) {
            head = baru;
            tail = baru;
        } else {
            tail->next = baru;
            baru->prev = tail;
            tail = baru;
        }

        jumlahPenumpang++;
    }

    fclose(file);
}


void tambahData() {
    bersihkanLayar();
    tampilkanHeader();
    cout << "[TAMBAH DATA PENUMPANG BARU]" << endl;
    cetakGaris();

    if (jumlahPenumpang >= KAPASITAS_PESAWAT) {
        cout << "Pesawat sudah penuh! Kapasitas: " << KAPASITAS_PESAWAT << " kursi." << endl;
        tungguEnter();
        return;
    }

    Penumpang* baru = new Penumpang();

    baru->kodBooking = buatKodeBooking();
    cout << "Kode Booking otomatis: " << baru->kodBooking << endl;
    cetakGaris();

    cout << "Nama Penumpang      : "; getline(cin, baru->nama);
    cout << "NIK                 : "; getline(cin, baru->nik);
    cout << "Kota Asal           : "; getline(cin, baru->asal);
    cout << "Kota Tujuan         : "; getline(cin, baru->tujuan);
    cout << "Tanggal (YYYY-MM-DD): "; getline(cin, baru->tanggal);
    cout << "Kelas (Ekonomi/Bisnis): "; getline(cin, baru->kelas);
    cout << "Nomor Kursi         : "; cin >> baru->nomorKursi;
    cin.ignore();

    baru->next = nullptr;
    baru->prev = nullptr;

    if (head == nullptr) {
        head = baru;
        tail = baru;
    } else {
        tail->next = baru;
        baru->prev = tail;
        tail = baru;
    }

    jumlahPenumpang++;
    simpanKeFile();

    cout << "\nData penumpang berhasil ditambahkan!" << endl;
    tungguEnter();
}

void tampilkanSemua() {
    bersihkanLayar();
    tampilkanHeader();
    cout << "         [ DAFTAR SEMUA TIKET PESAWAT ]        " << endl;
    cetakGaris();

    if (head == nullptr) {
        cout << "\n  (Belum ada data penumpang)\n" << endl;
        tungguEnter();
        return;
    }

    cout << left;
    cout << "No | Kode Bkg | Nama             | NIK              | Asal       | Tujuan     | Tanggal    | Kelas   | Kursi" << endl;
    cetakGaris();

    Penumpang* sekarang = head;
    int nomor = 1;

    while (sekarang != nullptr) {
        cout << nomor       << "  | "
             << sekarang->kodBooking << " | "
             << sekarang->nama       << " | "
             << sekarang->nik        << " | "
             << sekarang->asal       << " | "
             << sekarang->tujuan     << " | "
             << sekarang->tanggal    << " | "
             << sekarang->kelas      << " | "
             << sekarang->nomorKursi << endl;

        sekarang = sekarang->next;
        nomor++;
    }

    cetakGaris();
    cout << "Total penumpang: " << jumlahPenumpang << " / " << KAPASITAS_PESAWAT << endl;
    tungguEnter();
}

void cariPenumpang() {
    bersihkanLayar();
    tampilkanHeader();
    cout << "         [ CARI DATA PENUMPANG ]               " << endl;
    cetakGaris();

    cout << "Masukkan NIK yang dicari: ";
    string nikCari;
    getline(cin, nikCari);

    Penumpang* sekarang = head;
    bool ketemu = false;

    while (sekarang != nullptr) {
        if (sekarang->nik == nikCari) {
            ketemu = true;
            cout << "\nData ditemukan!" << endl;
            cetakGaris();
            cout << "Kode Booking : " << sekarang->kodBooking << endl;
            cout << "Nama         : " << sekarang->nama       << endl;
            cout << "NIK          : " << sekarang->nik        << endl;
            cout << "Rute         : " << sekarang->asal << " -> " << sekarang->tujuan << endl;
            cout << "Tanggal      : " << sekarang->tanggal    << endl;
            cout << "Kelas        : " << sekarang->kelas      << endl;
            cout << "Nomor Kursi  : " << sekarang->nomorKursi << endl;
            cetakGaris();
            break;
        }
        sekarang = sekarang->next;
    }
    if (!ketemu) {
        cout << "\nData dengan NIK \"" << nikCari << "\" tidak ditemukan." << endl;
    }
    tungguEnter();
}

void editData() {
    bersihkanLayar();
    tampilkanHeader();
    cout << "[EDIT DATA TIKET]" << endl;
    cetakGaris();

    cout << "Masukkan Kode Booking yang ingin diedit: ";
    string kodeCari;
    getline(cin, kodeCari);

    Penumpang* sekarang = head;
    bool ketemu = false;

    while (sekarang != nullptr) {
        if (sekarang->kodBooking == kodeCari) {
            ketemu = true;
            cout << "\nData ditemukan! Masukkan data baru (Enter = tidak diubah):" << endl;
            cetakGaris();

            string input;

            cout << "Nama baru [" << sekarang->nama << "]: ";
            getline(cin, input);
            if (input != "") sekarang->nama = input;

            cout << "Kota Asal baru [" << sekarang->asal << "]: ";
            getline(cin, input);
            if (input != "") sekarang->asal = input;

            cout << "Kota Tujuan baru [" << sekarang->tujuan << "]: ";
            getline(cin, input);
            if (input != "") sekarang->tujuan = input;

            cout << "Tanggal baru [" << sekarang->tanggal << "]: ";
            getline(cin, input);
            if (input != "") sekarang->tanggal = input;

            cout << "Kelas baru [" << sekarang->kelas << "]: ";
            getline(cin, input);
            if (input != "") sekarang->kelas = input;

            simpanKeFile();

            cout << "\nData berhasil diperbarui!" << endl;
            break;
        }
        sekarang = sekarang->next;
    }
    if (!ketemu) {
        cout << "\nKode Booking tidak ditemukan." << endl;
    }
    tungguEnter();
}

void hapusData() {
    bersihkanLayar();
    tampilkanHeader();
    cout << "         [ BATALKAN TIKET ]                    " << endl;
    cetakGaris();

    cout << "Masukkan Kode Booking yang ingin dibatalkan: ";
    string kodeCari;
    getline(cin, kodeCari);

    Penumpang* sekarang = head;
    bool ketemu = false;

    while (sekarang != nullptr) {
        if (sekarang->kodBooking == kodeCari) {
            ketemu = true;

            cout << "\nMembatalkan tiket atas nama: " << sekarang->nama << endl;
            cout << "Yakin? (y/n): ";
            char konfirmasi;
            cin >> konfirmasi;
            cin.ignore();

            if (konfirmasi != 'y' && konfirmasi != 'Y') {
                cout << "Pembatalan dibatalkan." << endl;
                tungguEnter();
                return;
            }

            if (sekarang->prev != nullptr) {
                sekarang->prev->next = sekarang->next;
            } else {
                head = sekarang->next;
            }

            if (sekarang->next != nullptr) {
                sekarang->next->prev = sekarang->prev;
            } else {
                tail = sekarang->prev;
            }

            delete sekarang;
            jumlahPenumpang--;

            simpanKeFile();

            cout << "\nTiket berhasil dibatalkan." << endl;
            break;
        }
        sekarang = sekarang->next;
    }

    if (!ketemu) {
        cout << "\nKode Booking tidak ditemukan." << endl;
    }

    tungguEnter();
}


void urutkanData() {
    bersihkanLayar();
    tampilkanHeader();
    cout << "[URUTKAN DATA]" << endl;
    cetakGaris();
    cout << "Urutkan berdasarkan:" << endl;
    cout << "1. Tanggal Keberangkatan" << endl;
    cout << "2. Nama Penumpang (A-Z)" << endl;
    cout << "Pilihan: ";

    int pilihan;
    cin >> pilihan;
    cin.ignore();

    if (head == nullptr || head->next == nullptr) {
        cout << "\nData kurang dari 2, tidak perlu diurutkan." << endl;
        tungguEnter();
        return;
    }

    bool adaPerubahan;
    do {
        adaPerubahan = false;
        Penumpang* sekarang = head;

        while (sekarang->next != nullptr) {
            bool perluTukar = false;

            if (pilihan == 1 && sekarang->tanggal > sekarang->next->tanggal)
                perluTukar = true;
            if (pilihan == 2 && sekarang->nama > sekarang->next->nama)
                perluTukar = true;

            if (perluTukar) {
                swap(sekarang->kodBooking, sekarang->next->kodBooking);
                swap(sekarang->nama,       sekarang->next->nama);
                swap(sekarang->nik,        sekarang->next->nik);
                swap(sekarang->asal,       sekarang->next->asal);
                swap(sekarang->tujuan,     sekarang->next->tujuan);
                swap(sekarang->tanggal,    sekarang->next->tanggal);
                swap(sekarang->kelas,      sekarang->next->kelas);
                swap(sekarang->nomorKursi, sekarang->next->nomorKursi);
                adaPerubahan = true;
            }

            sekarang = sekarang->next;
        }
    } while (adaPerubahan);

    simpanKeFile();

    cout << "\nData berhasil diurutkan! Menampilkan hasil..." << endl;
    tungguEnter();

    tampilkanSemua();
}

bool loginAdmin() {
    const string USERNAME = "admin";
    const string PASSWORD = "skypass123";
    int percobaan = 0;

    while (percobaan < 3) {
        bersihkanLayar();
        tampilkanHeader();
        cout << "[LOGIN ADMIN]" << endl;
        cetakGaris();

        string user, pass;
        cout << "Username : "; getline(cin, user);
        cout << "Password : "; getline(cin, pass);

        if (user == USERNAME && pass == PASSWORD) {
            cout << "\nLogin berhasil! Selamat datang, Admin." << endl;
            for (int i = 0; i < 100000000; i++);
            return true;
        } else {
            percobaan++;
            cout << "\nUsername atau password salah!" << endl;
            cout << "Sisa percobaan: " << (3 - percobaan) << endl;
            tungguEnter();
        }
    }

    cout << "\nAkun dikunci! Terlalu banyak percobaan gagal." << endl;
    return false;
}

int main() {

    muatDariFile();

    if (!loginAdmin()) {
        return 0;
    }

    int pilihan;

    do {
        bersihkanLayar();
        tampilkanHeader();
        cout << "[MENU UTAMA]" << endl;
        cetakGaris();
        cout << "  1. Tambah Data Penumpang" << endl;
        cout << "  2. Tampilkan Semua Tiket" << endl;
        cout << "  3. Cari Penumpang (by NIK)" << endl;
        cout << "  4. Edit Data Tiket" << endl;
        cout << "  5. Batalkan Tiket" << endl;
        cout << "  6. Urutkan Data" << endl;
        cout << "  7. Keluar" << endl;
        cetakGaris();
        cout << "  Pilihan kamu: ";
        cin >> pilihan;
        cin.ignore();

        if      (pilihan == 1) tambahData();
        else if (pilihan == 2) tampilkanSemua();
        else if (pilihan == 3) cariPenumpang();
        else if (pilihan == 4) editData();
        else if (pilihan == 5) hapusData();
        else if (pilihan == 6) urutkanData();
        else if (pilihan == 7) {
            bersihkanLayar();
            tampilkanHeader();
            cout << "\nTerima kasih telah menggunakan SkyPass Terminal System!" << endl;
            cout << "Sampai jumpa!\n" << endl;
        } else {
            cout << "\nPilihan tidak valid!" << endl;
            tungguEnter();
        }

    } while (pilihan != 7);

    return 0;
}

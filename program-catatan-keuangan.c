#include <stdio.h>
#include <string.h>

#define MAX 100

// Struktur data transaksi
typedef struct {
    char tanggal[11];     // DD/MM/YYYY
    char jenis;           // 'M' = pemasukan, 'K' = pengeluaran
    char kategori[50];
    int jumlah;
} Transaksi;

// Deklarasi fungsi
void tambahTransaksi(Transaksi transaksi[], int *jumlahTransaksi);
int hitungSaldo(Transaksi transaksi[], int jumlahTransaksi);
void lihatLaporan(Transaksi transaksi[], int jumlahTransaksi);
void lihatSaldo(Transaksi transaksi[], int jumlahTransaksi);
void simpanFile(Transaksi transaksi[], int jumlahTransaksi);
void bersihkanBuffer();
void muatFile(Transaksi transaksi[], int *jumlahTransaksi);


// Fungsi muat data dari file saat program dimulai
void muatFile(Transaksi transaksi[], int *jumlahTransaksi) {
    FILE *file = fopen("data_transaksi.txt", "r");
    char line[256];
    int count = 0;

    if (file == NULL) {
        printf("File 'data_transaksi.txt' tidak ditemukan. Mulai dengan data kosong.\n");
        *jumlahTransaksi = 0;
        return;
    }

    // Membaca setiap baris dalam file
    while (fgets(line, sizeof(line), file) != NULL && count < MAX) {
        // Menggunakan strtok untuk memecah string berdasarkan koma (,)
        char *token;

        // Tanggal
        token = strtok(line, ",");
        if (token) strcpy(transaksi[count].tanggal, token);

        // Jenis
        token = strtok(NULL, ",");
        if (token) transaksi[count].jenis = token[0];

        // Kategori
        token = strtok(NULL, ",");
        if (token) strcpy(transaksi[count].kategori, token);

        // Jumlah
        token = strtok(NULL, ",");
        if (token) transaksi[count].jumlah = atoi(token);

        count++;
    }

    fclose(file);
    *jumlahTransaksi = count;
    printf("Data berhasil dimuat dari 'data_transaksi.txt'. Jumlah transaksi: %d\n", *jumlahTransaksi);
}

// Fungsi pembersih buffer input
void bersihkanBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi hitung saldo (saat validasi pengeluaran)
int hitungSaldo(Transaksi transaksi[], int jumlahTransaksi) {
    int saldo = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (transaksi[i].jenis == 'M' || transaksi[i].jenis == 'm') {
            saldo += transaksi[i].jumlah;
        } else if (transaksi[i].jenis == 'K' || transaksi[i].jenis == 'k') {
            saldo -= transaksi[i].jumlah;
        }
    }
    return saldo;
}

// Fungsi Utama Menu 1: tambah transaksi
void tambahTransaksi(Transaksi transaksi[], int *jumlahTransaksi) {
    if (*jumlahTransaksi >= MAX) {
        printf("Kapasitas transaksi penuh!\n");
        return;
    }

    char tanggal[11];
    char jenis;
    char kategori[50];
    int jumlah;

    printf("Masukkan tanggal transaksi (DD/MM/YYYY): ");
    scanf("%10s", tanggal);

    printf("Jenis transaksi (M = Pemasukan, K = Pengeluaran): ");
    scanf(" %c", &jenis);

    printf("Masukkan kategori: ");
    bersihkanBuffer(); // kosongkan buffer sebelum baca string
    fgets(kategori, sizeof(kategori), stdin);
    kategori[strcspn(kategori, "\n")] = '\0'; // hapus newline

    printf("Masukkan jumlah uang: ");
    scanf("%d", &jumlah);

    // Cek jenis transaksi
    if (jenis == 'M' || jenis == 'm') {
        // Pemasukan
        strcpy(transaksi[*jumlahTransaksi].tanggal, tanggal);
        transaksi[*jumlahTransaksi].jenis = jenis;
        strcpy(transaksi[*jumlahTransaksi].kategori, kategori);
        transaksi[*jumlahTransaksi].jumlah = jumlah;

        (*jumlahTransaksi)++;
        printf("Transaksi pemasukan berhasil ditambahkan.\n");
    }
    else if (jenis == 'K' || jenis == 'k'){
        // Pengeluaran
        int saldo = hitungSaldo(transaksi, *jumlahTransaksi);

        if (saldo >= jumlah){
            strcpy(transaksi[*jumlahTransaksi].tanggal, tanggal);
            transaksi[*jumlahTransaksi].jenis = jenis;
            strcpy(transaksi[*jumlahTransaksi].kategori, kategori);
            transaksi[*jumlahTransaksi].jumlah = jumlah;

            (*jumlahTransaksi)++;
            printf("Transaksi pengeluaran berhasil ditambahkan.\n");
        } else {
            printf("Transaksi ditolak. Saldo Anda tidak mencukupi. \n");
        }
    }
    else {
        printf("Jenis transaksi tidak valid! \n");
    }
}

// Fungsi Utama Menu 2: lihat laporan transaksi
void lihatLaporan(Transaksi transaksi[], int jumlahTransaksi) {
    if (jumlahTransaksi == 0) {
        printf("\n Belum ada transaksi.\n");
        return;
    }
    // Header tabel — pakai border dan pemisah |
    printf("\n");
    printf("+------------+--------+-----------------+------------+\n");
    printf("| %-10s | %-6s | %-15s | %-10s |\n", "Tanggal", "Jenis", "Kategori", "Jumlah");
    printf("+------------+--------+-----------------+------------+\n");

    // Isi tabel
    for (int i = 0; i < jumlahTransaksi; i++) {
        char jenisStr[10];
        if (transaksi[i].jenis == 'M' || transaksi[i].jenis == 'm') {
            strcpy(jenisStr, "Masuk");
        } else {
            strcpy(jenisStr, "Keluar");
        }

        // Format baris tabel: 
        printf("| %-10s | %-6s | %-15s | Rp %-7d |\n",
               transaksi[i].tanggal,
               jenisStr,
               transaksi[i].kategori,
               transaksi[i].jumlah);
    }
    // Footer tabel
    printf("+------------+--------+-----------------+------------+\n");
    printf("| %-42s %7d |\n", "Total transaksi:", jumlahTransaksi);
    printf("+------------------------------------------+---------+\n");
}
// Fungsi Utama Menu 3: lihat saldo
void lihatSaldo(Transaksi transaksi[], int jumlahTransaksi) {
    printf("\n======== LIHAT SALDO ========\n");

    if (jumlahTransaksi == 0) {
        printf("Belum ada transaksi.\n");
        printf("Saldo saat ini: 0\n");
        printf("==============================\n");
        return;
    }

    int saldo = hitungSaldo(transaksi, jumlahTransaksi);

    printf("Total pemasukan  : ");
    int pemasukan = 0;
    int pengeluaran = 0;

    // Hitung pemasukan & pengeluaran secara terpisah
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (transaksi[i].jenis == 'M' || transaksi[i].jenis == 'm') {
            pemasukan += transaksi[i].jumlah;
        } else {
            pengeluaran += transaksi[i].jumlah;
        }
    }

    printf("Rp %d\n", pemasukan);
    printf("Total pengeluaran: Rp %d\n", pengeluaran);

    printf("--------------------------------\n");
    printf("Saldo saat ini    : Rp %d\n", saldo);
    printf("================================\n");
}


void simpanFile(Transaksi transaksi[], int jumlahTransaksi) {
    FILE *file = fopen("data_transaksi.txt", "w"); 

    if (file == NULL) {
        printf("Error: Gagal membuka file untuk penyimpanan!\n");
        return;
    }

    // Tulis semua transaksi dari array ke file dengan format CSV (koma terpisah)
    for (int i = 0; i < jumlahTransaksi; i++) {
        fprintf(file, "%s,%c,%s,%d\n", 
                transaksi[i].tanggal, 
                transaksi[i].jenis, 
                transaksi[i].kategori, 
                transaksi[i].jumlah);
    }

    fclose(file);
}

void muatFile(Transaksi transaksi[], int *jumlahTransaksi);    

// Fungsi Main:
int main() {
    Transaksi transaksi[MAX];
    int jumlahTransaksi = 0;
    int pilihan;
    int running = 1;
    // --- PANGGIL MUAT FILE DI SINI ---
    muatFile(transaksi, &jumlahTransaksi);

    while (running) {
        printf("\n===== CATATAN KEUANGAN HARIAN =====\n");
        printf("1. Tambah Transaksi\n");
        printf("2. Lihat Laporan Transaksi\n");
        printf("3. Lihat Saldo\n");
        printf("4. Simpan & Keluar\n");
        printf("===================================\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tambahTransaksi(transaksi, &jumlahTransaksi);
                break;
            case 2:
                lihatLaporan(transaksi, jumlahTransaksi);
                break;
            case 3:
                lihatSaldo(transaksi, jumlahTransaksi);
                break;
            case 4:
                simpanFile(transaksi, jumlahTransaksi);
                printf("\nData berhasil disimpan.\n");
                printf("Terima kasih.\n");
                running = 0;
                break;
            default:
                printf("Pilihan tidak valid, silakan pilih menu yang tersedia.\n");
        }

        // Kembali ke menu (jika belum keluar)
        if (running) {
            printf("\nTekan ENTER untuk kembali ke menu...");
            bersihkanBuffer();
            getchar();
        }
    }
    return 0;
}

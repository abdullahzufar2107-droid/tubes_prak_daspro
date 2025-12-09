#include <stdio.h>
#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

#define MAX 100

// Struktur data transaksi
typedef struct {
    char tanggal[11];     
    char jenis;               
    char keterangan[50];
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

// Fungsi pembersih buffer input
void bersihkanBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi hitung saldo 
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
        printf("%sKapasitas transaksi penuh!%s\n", RED, RESET);
        return;
    }

    char tanggal[11];
    char jenis;
    char keterangan[50];
    int jumlah;

    printf("\n");
    printf("Masukkan tanggal transaksi (DD/MM/YYYY) %s--------------------------->%s ", YELLOW, RESET);
    scanf("%10s", tanggal);

    printf("Masukkan Jenis transaksi (M = Pemasukan, K = Pengeluaran) %s--------->%s ", YELLOW, RESET);
    scanf(" %c", &jenis);

    printf("Masukkan keterangan %s----------------------------------------------->%s ", YELLOW, RESET);
    bersihkanBuffer(); 
    fgets(keterangan, sizeof(keterangan), stdin);
    keterangan[strcspn(keterangan, "\n")] = '\0'; 

    printf("Masukkan jumlah uang %s---------------------------------------------->%s ", YELLOW, RESET);
    scanf("%d", &jumlah);

    // Cek jenis transaksi
    if (jenis == 'M' || jenis == 'm') {
        // Pemasukan
        strcpy(transaksi[*jumlahTransaksi].tanggal, tanggal);
        transaksi[*jumlahTransaksi].jenis = jenis;
        strcpy(transaksi[*jumlahTransaksi].keterangan, keterangan);
        transaksi[*jumlahTransaksi].jumlah = jumlah;

        (*jumlahTransaksi)++;
        printf("%sTransaksi pemasukan berhasil ditambahkan.%s\n", GREEN, RESET);
    }
    else if (jenis == 'K' || jenis == 'k'){
        // Pengeluaran
        int saldo = hitungSaldo(transaksi, *jumlahTransaksi);

        if (saldo >= jumlah){
            strcpy(transaksi[*jumlahTransaksi].tanggal, tanggal);
            transaksi[*jumlahTransaksi].jenis = jenis;
            strcpy(transaksi[*jumlahTransaksi].keterangan, keterangan);
            transaksi[*jumlahTransaksi].jumlah = jumlah;

            (*jumlahTransaksi)++;
            printf("%sTransaksi pengeluaran berhasil ditambahkan.%s\n", GREEN, RESET);
        } else {
            printf("%sTransaksi ditolak. Saldo Anda tidak mencukupi.%s\n", RED, RESET);
        }
    }
    else {
        printf("%sJenis transaksi tidak valid!%s\n", RED, RESET);
    }
}

// Fungsi Utama Menu 2: lihat laporan transaksi
void lihatLaporan(Transaksi transaksi[], int jumlahTransaksi) {
    if (jumlahTransaksi == 0) {
        printf("\n%sBelum ada transaksi.%s\n", RED, RESET);
        printf("%sData Kosong.%s\n", RED, RESET);
        return;
    }

    // Header tabel - pakai border dan pemisah |
    printf("\nLAPORAN TRANSAKSI ANDA\n");
    printf("+---------------------+------------+----------------------+-------------------+\n");
    printf("| %-19s | %-10s | %-20s | %-17s |\n","Tanggal","Jenis","Keterangan","Jumlah");
    printf("|---------------------+------------+----------------------+-------------------|\n");

    // Isi tabel
    for (int i = 0; i < jumlahTransaksi; i++) {
        char jenisStr[10];
        if (transaksi[i].jenis == 'M' || transaksi[i].jenis == 'm')
            strcpy(jenisStr, "Masuk");
        else
            strcpy(jenisStr, "Keluar");
        // format baris tabel:
        printf("| %-19s | %-10s | %-20s | Rp %-14d |\n",
               transaksi[i].tanggal,
               jenisStr,
               transaksi[i].keterangan,
               transaksi[i].jumlah);
    }
    //footer tabel
    printf("|---------------------+------------+----------------------+-------------------|\n");
    printf("| %-67s %7d |\n", "Total transaksi:", jumlahTransaksi);
    printf("+---------------------------------------------------------+-------------------+\n");
}


// Fungsi Utama Menu 3: lihat saldo
void lihatSaldo(Transaksi transaksi[], int jumlahTransaksi) {
    printf("\n================================== SALDO ANDA ==================================\n");

    if (jumlahTransaksi == 0) {
        printf("| %sBelum ada transaksi.%s                                                         |\n", RED, RESET);
        printf("| %sSaldo saat ini: 0%s                                                            |\n", RED, RESET);
        printf("================================================================================\n");
        return;
}

    int saldo = hitungSaldo(transaksi, jumlahTransaksi);

    printf("| Total pemasukan                                        : ");
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

    printf("Rp %-16d |\n", pemasukan);
    printf("| Total pengeluaran                                      : Rp %-16d |\n", pengeluaran);
    printf("|------------------------------------------------------------------------------|\n");
    if (saldo > 0)
        printf("| Saldo saat ini                                         : %sRp %-16d%s |\n", GREEN, saldo, RESET);
    else if (saldo == 0)
        printf("| Saldo saat ini                                         : %sRp %-16d%s |\n", YELLOW, saldo, RESET);
    else
        printf("| Saldo saat ini                                         : %sRp %-16d%s |\n", RED, saldo, RESET);
    printf("================================================================================\n");
}

// Fungsi untuk menyimpan file
void simpanFile(Transaksi transaksi[], int jumlahTransaksi) {
    FILE *file = fopen("data_transaksi.txt", "w"); 

    if (file == NULL) {
        printf("%sError: Gagal membuka file untuk penyimpanan!%s\n", RED, RESET);
        return;
    }

    // Tulis semua transaksi dari array ke file dengan format CSV (koma terpisah)
    for (int i = 0; i < jumlahTransaksi; i++) {
        fprintf(file, "%s,%c,%s,%d\n", 
                transaksi[i].tanggal, 
                transaksi[i].jenis, 
                transaksi[i].keterangan, 
                transaksi[i].jumlah);
    }

    fclose(file);
}

// Fungsi muat data dari file saat program dimulai
void muatFile(Transaksi transaksi[], int *jumlahTransaksi) {
    FILE *file = fopen("data_transaksi.txt", "r");
    char line[100];
    int count = 0;

    if (file == NULL) {
        printf("%sFile 'data_transaksi.txt' tidak ditemukan. Mulai dengan data kosong.%s\n", YELLOW, RESET);
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

        // Keterangan
        token = strtok(NULL, ",");
        if (token) strcpy(transaksi[count].keterangan, token);

        // Jumlah
        token = strtok(NULL, ",");
        if (token) transaksi[count].jumlah = atoi(token);

        count++;
    }

    fclose(file);
    *jumlahTransaksi = count;
    printf("%sData berhasil dimuat dari 'data_transaksi.txt'. Jumlah transaksi: %d%s\n", GREEN, *jumlahTransaksi, RESET);
}

// Fungsi Main:
int main() {
    Transaksi transaksi[MAX];
    int jumlahTransaksi = 0;
    int pilihan;
    int running = 1;
    // --- PANGGIL MUAT FILE DI SINI ---
    muatFile(transaksi, &jumlahTransaksi);

    while (running) {
        printf("\n=========================== CATATAN KEUANGAN HARIAN ===========================\n");
        printf("|                                                                             |\n");
        printf("|  MENU UTAMA:                                                                |\n");
        printf("|                                                                             |\n");
        printf("|     1. Tambah Laporan Transaksi                                             |\n");
        printf("|                                                                             |\n");
        printf("|     2. Lihat Laporan Transaksi                                              |\n");
        printf("|                                                                             |\n");
        printf("|     3. Lihat Saldo                                                          |\n");
        printf("|                                                                             |\n");
        printf("|     4. Simpan & Keluar                                                      |\n");
        printf("|                                                                             |\n");
        printf("===============================================================================\n");
        printf("\n");
        printf("Pilih Menu: ");
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
                printf("\n%sData berhasil disimpan.%s\n", GREEN, RESET);
                printf("%sTerima kasih.%s\n", GREEN, RESET);
                printf("\n");
                running = 0;
                break;
            default:
                printf("%sPilihan tidak valid, silakan pilih menu yang tersedia.%s\n", YELLOW, RESET);
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

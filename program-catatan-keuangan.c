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

// Fungsi utama: tambah transaksi
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
    

// Fungsi utama
int main() {
    Transaksi transaksi[MAX];
    int jumlahTransaksi = 0;
    int pilihan;
    int running = 1;

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
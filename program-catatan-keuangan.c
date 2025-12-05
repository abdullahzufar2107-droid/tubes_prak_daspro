#include <stdio.h>

int pilihan;
int running = 1;   // agar looping

while (running) {

    // ===== BENTUK MENU =====
    printf("\n===== CATATAN KEUANGAN HARIAN =====\n");
    printf("1. Tambah Transaksi\n");
    printf("2. Lihat Laporan Transaksi\n");
    printf("3. Lihat Saldo\n");
    printf("4. Simpan & Keluar\n");
    printf("===================================\n");
    printf("Pilih menu: ");
    scanf("%d", &pilihan);

    // ===== KEPUTUSAN PROGRAM MENU KALIAN =====
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
            running = 0;  // KELUAR DARI LOOPING
            break;

        default:
            printf("Pilihan tidak valid, silakan pilih menu yang tersedia.\n");
    }

    // ===== BALEK KE PILIHAN MENU =====
    if (running) {
        printf("\nTekan ENTER untuk kembali ke menu...");
        getchar(); getchar();   // untuk jeda
    }
}

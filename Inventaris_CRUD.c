#include <stdio.h>
#include <string.h>
#include <math.h> 

#define MAX_BARANG 50

typedef struct {
    char id [10];
    char nama[50];
    int jumlah;
} Barang;   

// Variabel Global
Barang inventaris[MAX_BARANG];
int jumlah_barang = 0;

void tampilkanMenu() {
    printf("\n=== Menu Inventaris Barang ===\n");
    printf("1. Tambah Barang\n");
    printf("2. Tampilkan Semua Barang\n");
    printf("3. Update Stok Barang\n");
    printf("4. Hapus Barang\n");
    printf("5. Cari Barang (ID/Stok)\n");
    printf("6. Keluar\n");
    printf("7. Urutkan Barang Berdasarkan Stok\n");
}

void tambahBarang() {
    if (jumlah_barang >= MAX_BARANG) {
        printf("Inventaris penuh. Tidak bisa menambah barang baru.\n");
        return;
    }
    Barang baru;
    printf("Masukkan ID Barang: ");
    scanf("%s", baru.id);
    printf("Masukkan Nama Barang: ");
    scanf("%s", baru.nama);
    printf("Masukkan Jumlah Barang: ");
    scanf("%d", &baru.jumlah);
    
    inventaris[jumlah_barang++] = baru;
    printf("Barang berhasil ditambahkan.\n");
}

void tampilkanSemuaBarang() {
    if (jumlah_barang == 0) {
        printf("Tidak ada barang dalam inventaris.\n");
        return;
    }
    printf("\n=== Daftar Barang ===\n");
    for (int i = 0; i < jumlah_barang; i++) {
        printf("ID: %s, Nama: %s, Jumlah: %d\n", inventaris[i].id, inventaris[i].nama, inventaris[i].jumlah);
    }
}

void updateStok() {
    char id[10];
    int jumlah_baru;
    printf("Masukkan ID Barang yang akan diupdate: ");
    scanf("%s", id);
    for (int i = 0; i < jumlah_barang; i++) {
        if (strcmp(inventaris[i].id, id) == 0) {
            printf("Masukkan Jumlah Baru untuk %s: ", inventaris[i].nama);
            scanf("%d", &jumlah_baru);
            inventaris[i].jumlah = jumlah_baru;
            printf("Stok barang berhasil diupdate.\n");
            return;
        }
    }
    printf("Barang dengan ID %s tidak ditemukan.\n", id);
}

void hapusBarang() {
    char id[10];
    printf("Masukkan ID Barang yang akan dihapus: ");
    scanf("%s", id);
    for (int i = 0; i < jumlah_barang; i++) {
        if (strcmp(inventaris[i].id, id) == 0) {
            for (int j = i; j < jumlah_barang - 1; j++) {
                inventaris[j] = inventaris[j + 1];
            }
            jumlah_barang--;
            printf("Barang dengan ID %s berhasil dihapus.\n", id);
            return;
        }
    }
    printf("Barang dengan ID %s tidak ditemukan.\n", id);
}

// --- FUNGSI SORTING (QUICK SORT) ---

void swapBarang(Barang* a, Barang* b){
    Barang t = *a;
    *a = *b;
    *b = t;
}

int partitionBarang(Barang arr[], int low, int high) {
    int pivot = arr[high].jumlah;
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].jumlah > pivot) {
            i++;
            swapBarang(&arr[i], &arr[j]);
        }
    }
    swapBarang(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSortBarang(Barang arr[], int low, int high) {
    if (low < high) {
        int pi = partitionBarang(arr, low, high);
        quickSortBarang(arr, low, pi - 1);
        quickSortBarang(arr, pi + 1, high);
    }
}

void urutkanBarang() {
    if (jumlah_barang == 0) {
        printf("Tidak ada barang untuk diurutkan.\n");
        return;
    }
    quickSortBarang(inventaris, 0, jumlah_barang - 1);
    printf("Inventaris berhasil diurutkan berdasarkan Jumlah Stok.\n");
    tampilkanSemuaBarang();
}

// --- FUNGSI SEARCHING (JUMP SEARCH) ---

int min(int a, int b){
    return (a < b) ? a : b;
}

void cariStok(int stok) {
    if (jumlah_barang == 0) {
        printf("Inventaris kosong.\n");
        return;
    }

    int n = jumlah_barang;
    int step = sqrt(n);
    int prev = 0;

    // Tahap 1: Mencari blok
    while (prev < n && inventaris[min(step, n) - 1].jumlah < stok) {
        prev = step;
        step += sqrt(n);
        if (prev >= n) {
            printf("Barang dengan jumlah stok %d tidak ditemukan.\n", stok);
            return;
        }
    }

    // Tahap 2: Linear search pada blok
    int current_index = prev;
    while (current_index < n && inventaris[current_index].jumlah < stok) {
        current_index++;
        if (current_index == min(step, n)) {
            printf("Barang dengan jumlah stok %d tidak ditemukan.\n", stok);
            return;
        }
    }

    // Tahap 3: Verifikasi hasil
    if (current_index < n && inventaris[current_index].jumlah == stok) {
        printf("\n=== Barang Ditemukan (Stok %d) ===\n", stok);
        // Tampilkan semua barang dengan stok yang sama
        for(int i = current_index; i < n && inventaris[i].jumlah == stok; i++){
            printf("ID: %s, Nama: %s, Jumlah: %d\n", inventaris[i].id, inventaris[i].nama, inventaris[i].jumlah);
        }
        return;
    }

    printf("Barang dengan jumlah stok %d tidak ditemukan.\n", stok);
}


void caribarang() {
    int pilihan_cari;
    printf("\nCari berdasarkan:\n");
    printf("1. ID Barang \n");
    printf("2. Jumlah Stok (Jump Search - **Perlu Diurutkan Dulu**)\n");
    printf("Pilih (1/2): ");
    if (scanf("%d", &pilihan_cari) != 1) {
        while (getchar() != '\n');
        printf("Input tidak valid.\n");
        return;
    }

    if (pilihan_cari == 1) {
        char id[10];
        printf("Masukkan ID Barang yang akan dicari: ");
        scanf("%s", id);

        for (int i = 0; i < jumlah_barang; i++) {
            if (strcmp(inventaris[i].id, id) == 0) {
                printf("\n=== Barang Ditemukan ===\n");
                printf("ID: %s, Nama: %s, Jumlah: %d\n", inventaris[i].id, inventaris[i].nama, inventaris[i].jumlah);
                return;
            }
        }
        printf("Barang dengan ID %s tidak ditemukan.\n", id);

    } else if (pilihan_cari == 2) {
        int stok_cari;
        printf("Masukkan Jumlah Stok yang akan dicari: ");
        scanf("%d", &stok_cari);
        cariStok(stok_cari);
    } else {
        printf("Pilihan cari tidak valid.\n");
    }
}

// Fungsi Utama
int main(){
    int pilihan;
    do {
        tampilkanMenu();
        printf("Pilih menu (1-7): ");
        if (scanf("%d", &pilihan) != 1) { 
            while (getchar() != '\n');
            pilihan = 0; 
            continue;
        }
        switch (pilihan) {
            case 1:
                tambahBarang();
                break;
            case 2:
                tampilkanSemuaBarang();
                break;
            case 3:
                updateStok();
                break;
            case 4:
                hapusBarang();
                break;
            case 5:
                caribarang(); 
                break;
            case 6:
                printf("Keluar dari program.\n");
                break;
            case 7:
                urutkanBarang();
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (pilihan != 6);
    return 0;
}


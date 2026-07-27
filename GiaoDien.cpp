/*
    File: GiaoDien.cpp
    Mô tả: Cài đặt các hàm giao diện.
*/

#include "GiaoDien.h"

// ============================================================================
// VẼ KHUNG
// ============================================================================

void VeKhung(int x1, int y1, int x2, int y2) {
    // Góc
    gotoxy(x1, y1); printf("+");
    gotoxy(x2, y1); printf("+");
    gotoxy(x1, y2); printf("+");
    gotoxy(x2, y2); printf("+");
    // Cạnh ngang
    for (int i = x1 + 1; i < x2; i++) {
        gotoxy(i, y1); printf("-");
        gotoxy(i, y2); printf("-");
    }
    // Cạnh dọc
    for (int j = y1 + 1; j < y2; j++) {
        gotoxy(x1, j); printf("|");
        gotoxy(x2, j); printf("|");
    }
}

void VeKhungCoTieuDe(int x1, int y1, int x2, int y2, const char *tieuDe) {
    VeKhung(x1, y1, x2, y2);
    int len = strlen(tieuDe);
    int mid = (x1 + x2) / 2 - len / 2;
    gotoxy(mid - 1, y1);
    SetColor(MAU_VANG_SANG);
    printf(" %s ", tieuDe);
    SetColor(MAU_TRANG);
}

void VeDuongNgang(int x1, int x2, int y) {
    for (int i = x1; i <= x2; i++) {
        gotoxy(i, y);
        printf("-");
    }
}

// ============================================================================
// MENU THANH SÁNG
// ============================================================================

void VeMenu(int x, int y, const char *items[], int soMuc, int viTriChon) {
    for (int i = 0; i < soMuc; i++) {
        gotoxy(x, y + i);
        if (i == viTriChon) {
            SetColor(MAU_TRANG_SANG);
            SetBGColor(MAU_XANH_DUONG);
            printf(" > %s", items[i]);
            int len = strlen(items[i]);
            for (int k = len; k < 40; k++) printf(" ");
            SetBGColor(MAU_DEN);
        } else {
            SetColor(MAU_TRANG);
            printf("   %s", items[i]);
        }
    }
    SetColor(MAU_TRANG);
}

int XuLyMenu(int x, int y, const char *items[], int soMuc) {
    int chon = 0;
    VeMenu(x, y, items, soMuc, chon);
    while (true) {
        int phim = DocPhim();
        if (phim == PHIM_ENTER) return chon;
        if (phim == PHIM_ESC) return -1;
        if (phim == PHIM_LEN) {
            chon = (chon - 1 + soMuc) % soMuc;
            VeMenu(x, y, items, soMuc, chon);
        }
        if (phim == PHIM_XUONG) {
            chon = (chon + 1) % soMuc;
            VeMenu(x, y, items, soMuc, chon);
        }
    }
}

// ============================================================================
// HÀM PHÍM
// ============================================================================

int DocPhim() {
    int c = getch();
    if (c == 0 || c == 224) {
        // Phím mở rộng
        c = getch();
        return c;
    }
    return c;
}

void ChoPhimBatKy() {
    gotoxy(2, 27);
    SetColor(MAU_XANH_LA_SANG);
    printf(" Nhan phim bat ky de tiep tuc...");
    SetColor(MAU_TRANG);
    getch();
}

int HienMenuCon(const char *tieuDe, const char *lc1, const char *lc2) {
    int x = 30, y = 10;
    VeKhungCoTieuDe(x, y, x + 50, y + 6, tieuDe);
    const char *items[2] = { lc1, lc2 };
    int chon = XuLyMenu(x + 4, y + 2, items, 2);
    return chon;
}

// ============================================================================
// THÔNG BÁO
// ============================================================================

void XoaDong(int y) {
    gotoxy(0, y);
    for (int i = 0; i < 120; i++) printf(" ");
    gotoxy(0, y);
}

void HienThongBaoLoi(const char *tb) {
    int x = wherex(), y = wherey();
    gotoxy(2, 27);
    SetColor(MAU_DO_SANG);
    printf(" [LOI] %s", tb);
    SetColor(MAU_TRANG);
    Sleep(2500);
    XoaDong(27);
    gotoxy(x, y);
}

void HienThongBaoThanhCong(const char *tb) {
    int x = wherex(), y = wherey();
    gotoxy(2, 27);
    SetColor(MAU_XANH_LA_SANG);
    printf(" [OK] %s", tb);
    SetColor(MAU_TRANG);
    Sleep(1500);
    XoaDong(27);
    gotoxy(x, y);
}

void HienThongBao(int x, int y, const char *tb, int mau) {
    gotoxy(x, y);
    SetColor(mau);
    printf("%s", tb);
    SetColor(MAU_TRANG);
}

// ============================================================================
// THIẾT LẬP CONSOLE
// ============================================================================

void ThietLapConsole() {
    SetColor(MAU_TRANG);
    SetBGColor(MAU_DEN);
    system("mode con: cols=120 lines=30");
    AnConTro();
}

void HienConTro() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 25;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(h, &info);
}

void AnConTro() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 25;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(h, &info);
}

void InCanGiua(const char *s, int doRong) {
    int len = strlen(s);
    int pad = (doRong - len) / 2;
    for (int i = 0; i < pad; i++) printf(" ");
    printf("%s", s);
    for (int i = 0; i < doRong - pad - len; i++) printf(" ");
}

// ============================================================================
// MÀN HÌNH CHÀO
// ============================================================================

void HienManHinhChao() {
    clrscr();
    SetColor(MAU_XANH_NHAT_SANG);
    VeKhungCoTieuDe(20, 5, 100, 22, "CHUONG TRINH QUAN LY DIEM SINH VIEN HE TIN CHI");
    SetColor(MAU_VANG_SANG);
    gotoxy(35, 9); printf("Do an mon: Cau truc Du lieu & Giai thuat");
    gotoxy(38, 11); printf("Khoa 2023+2024 - DHCQ CLC");
    gotoxy(35, 12); printf("Hoc ky 3 nam hoc 2025-2026");
    SetColor(MAU_TRANG);
    gotoxy(30, 16); printf("Sinh vien thuc hien:");
    SetColor(MAU_XANH_LA_SANG);
    gotoxy(32, 17); printf("Nguyen Viet Anh");
    SetColor(MAU_TRANG);
    gotoxy(30, 20); printf("Nhan phim bat ky de bat dau...");
    getch();
}

// ============================================================================
// MENU CHÍNH
// ============================================================================

int HienMenuChinh() {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "QUAN LY DIEM SINH VIEN HE TIN CHI - MENU CHINH");

    const char *items[8] = {
        "1. Quan ly Mon hoc                                    ",
        "2. Quan ly Lop                                        ",
        "3. Nhap sinh vien vao Lop                             ",
        "4. Quan ly Lop tin chi + Dang ky                      ",
        "5. Nhap diem / In bang diem                           ",
        "6. Luu du lieu vao file                               ",
        "7. Doc du lieu tu file                                ",
        "8. Thoat chuong trinh                                 "
    };

    gotoxy(30, 5);
    SetColor(MAU_VANG_SANG);
    printf("Su dung phim mui ten LEN/XUONG va ENTER de chon");
    SetColor(MAU_TRANG);

    int chon = XuLyMenu(30, 8, items, 8);
    return chon + 1;   // trả về 1..8
}

// ============================================================================
// PHÂN TRANG
// ============================================================================

int TinhTongTrang(int tongSoDong, int soDongMotTrang) {
    if (tongSoDong <= 0) return 1;
    return (tongSoDong + soDongMotTrang - 1) / soDongMotTrang;
}

void KhoiTaoPhanTrang(PhanTrang &pt, int tongSoDong, int soDongMotTrang) {
    pt.tongSoDong = tongSoDong;
    pt.soDongMotTrang = soDongMotTrang;
    pt.tongTrang = TinhTongTrang(tongSoDong, soDongMotTrang);
    pt.trangHienTai = 1;
}

bool TrangTruoc(PhanTrang &pt) {
    if (pt.trangHienTai > 1) { pt.trangHienTai--; return true; }
    return false;
}

bool TrangSau(PhanTrang &pt) {
    if (pt.trangHienTai < pt.tongTrang) { pt.trangHienTai++; return true; }
    return false;
}

int LayViTriBatDau(PhanTrang &pt) {
    return (pt.trangHienTai - 1) * pt.soDongMotTrang;
}

int LayViTriKetThuc(PhanTrang &pt) {
    int cuoi = pt.trangHienTai * pt.soDongMotTrang - 1;
    if (cuoi >= pt.tongSoDong) cuoi = pt.tongSoDong - 1;
    return cuoi;
}

void HienThiThongTinTrang(PhanTrang &pt, int x, int y) {
    gotoxy(x, y);
    SetColor(MAU_VANG_SANG);
    printf(" Trang %d/%d  (Tong %d ban ghi)  [F5]=Truoc  [F6]=Sau  [ESC]=Thoat ",
           pt.trangHienTai, pt.tongTrang, pt.tongSoDong);
    SetColor(MAU_TRANG);
}

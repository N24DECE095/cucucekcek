/*
    File: main.cpp
    Tác giả: TV1 - Nguyễn Việt Anh
    Mô tả: Chương trình chính Task 1 — chỉ 4 chức năng của TV1:
        (e) Nhập môn học
        (f) In DS môn học tăng dần theo tên
        (a) Mở lớp tín chỉ (thêm/xoá/hiệu chỉnh)
        (h) Hủy LTC tự động khi SV < min

    Biên dịch (Windows + MinGW):
        g++ -std=c++11 -o Task1.exe main.cpp Monhoc.cpp Loptinchi.cpp GiaoDien.cpp XuLyFile.cpp
    Hoặc:
        g++ -std=c++11 -o Task1.exe *.cpp
    Chạy:
        .\Task1.exe   (trong CMD thật, KHÔNG chạy trong VS Code Integrated Terminal)
*/

#include "mylib.h"
#include "CauTruc.h"
#include "GiaoDien.h"
#include "Monhoc.h"
#include "Loptinchi.h"
#include "XuLyFile.h"
#include "QuanLyDiem.h"

// ===================== BIẾN TOÀN CỤC =====================
treeMH cayMH = NULL;
DS_LTC dsLtc;
DS_LOP dsLop;

// ===================== KHỞI TẠO / DỌN DẸP =====================
void KhoiTao() {
    cayMH = NULL;
    KhoiTaoDSLTC(dsLtc);
    KhoiTaoDSLop(dsLop);
}

void DonDep() {
    HuyDSLTC(dsLtc);
    GiaiPhongDSLop(dsLop);
    GiaiPhongCayMH(cayMH);
}

// ===================== MÀN HÌNH CHÀO ĐƠN GIẢN =====================
static void ManHinhChao() {
    clrscr();
    SetColor(MAU_XANH_NHAT_SANG);
    VeKhungCoTieuDe(20, 5, 100, 22,
        "QUAN LY DIEM SV HE TIN CHI - TASK 1: MON HOC + LOP TIN CHI");
    SetColor(MAU_VANG_SANG);
    gotoxy(30, 8);  printf("Do an mon Cau truc Du lieu & Giai thuat");
    gotoxy(33, 10); printf("Khoa 2023+2024 DHCQ CLC HK3 2025-2026");
    SetColor(MAU_TRANG);
    gotoxy(30, 13); printf("Cac chuc nang cua TV1 (Task 1):");
    SetColor(MAU_XANH_LA_SANG);
    gotoxy(32, 14); printf("(e) Nhap mon hoc");
    gotoxy(32, 15); printf("(f) In DS mon hoc tang dan theo ten");
    gotoxy(32, 16); printf("(a) Mo/Sua/Xoa lop tin chi");
    gotoxy(32, 17); printf("(h) Huy LTC tu dong khi SV < min");
    SetColor(MAU_TRANG);
    gotoxy(30, 20); printf("Nguoi thuc hien: Nguyen Viet Anh");
    gotoxy(30, 21); printf("Nhan phim bat ky de bat dau...");
    getch();
}

// ===================== MENU CHÍNH — 6 MỤC =====================
static int HienMenuChinhTask1() {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "TASK 1: MON HOC + LOP TIN CHI - MENU CHINH");
    const char *items[7] = {
        "1. Quan ly Mon hoc            (chuc nang e, f)   ",
        "2. Quan ly Lop tin chi        (chuc nang a)      ",
        "3. Huy LTC tu dong khi SV<min (chuc nang h)      ",
        "4. Nhap diem / Hieu chinh diem                   ",
        "5. Luu du lieu vao file                          ",
        "6. Doc du lieu tu file                           ",
        "7. Thoat chuong trinh                            "
    };
    gotoxy(30, 5);
    SetColor(MAU_VANG_SANG);
    printf("Dung phim mui ten LEN/XUONG va ENTER de chon");
    SetColor(MAU_TRANG);
    int chon = XuLyMenu(30, 8, items, 7);
    return chon + 1;    // trả về 1..7
}

// ===================== MAIN =====================
int main() {
    // Thiết lập console UTF-8 + kích thước
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    ThietLapConsole();

    KhoiTao();
    ManHinhChao();

    // Tự động đọc file (nếu có)
    clrscr();
    gotoxy(10, 12);
    printf(" Dang tai du lieu tu file...");
    DocTatCa(cayMH, dsLtc);
    Sleep(800);

    int chon;
    do {
        chon = HienMenuChinhTask1();
        switch (chon) {
            case 1: MenuQuanLyMonhoc(cayMH, dsLtc); break;
            case 2: MenuMoLopTinChi(dsLtc, cayMH); break;
            case 3: HuyLopTinChiTuDong(dsLtc); break;
            case 4: NhapDiem(dsLtc, dsLop, cayMH); break;
            case 5: {
                clrscr();
                gotoxy(30, 12);
                SetColor(MAU_XANH_LA_SANG);
                printf("Dang luu du lieu...");
                SetColor(MAU_TRANG);
                LuuTatCa(cayMH, dsLtc);
                Sleep(500);
                HienThongBaoThanhCong("Da luu xong.");
                break;
            }
            case 6: {
                clrscr();
                gotoxy(30, 12);
                SetColor(MAU_XANH_LA_SANG);
                printf("Dang doc du lieu tu file...");
                SetColor(MAU_TRANG);
                DonDep();
                KhoiTao();
                DocTatCa(cayMH, dsLtc);
                Sleep(500);
                HienThongBaoThanhCong("Da doc xong.");
                break;
            }
            case 7:
            case 0:
            case -1:
                break;   // thoát
        }
    } while (chon != 7 && chon != -1);

    // Tự động lưu trước khi thoát
    clrscr();
    gotoxy(30, 12);
    SetColor(MAU_XANH_LA_SANG);
    printf("Dang luu du lieu truoc khi thoat...");
    SetColor(MAU_TRANG);
    LuuTatCa(cayMH, dsLtc);
    Sleep(800);

    DonDep();

    clrscr();
    gotoxy(35, 14);
    SetColor(MAU_VANG_SANG);
    printf("Cam on ban da su dung Task 1 - QLDSV_HTC!");
    SetColor(MAU_TRANG);
    Sleep(1500);
    return 0;
}

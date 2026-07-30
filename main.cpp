

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

// ===================== MAIN =====================
int main() {
    // Thiết lập console UTF-8 + kích thước
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    ThietLapConsole();

    KhoiTao();

    // Tự động đọc file (nếu có)
    clrscr();
    gotoxy(10, 12);
    printf(" Dang tai du lieu tu file...");
    DocTatCa(cayMH, dsLtc, dsLop);
    Sleep(800);

    // Hiển thị giao diện Giảng viên
    HienMenuGiangVien(dsLtc, dsLop, cayMH);

    // Tự động lưu trước khi thoát
    clrscr();
    gotoxy(30, 12);
    SetColor(MAU_XANH_LA_SANG);
    printf("Dang luu du lieu truoc khi thoat...");
    SetColor(MAU_TRANG);
    LuuTatCa(cayMH, dsLtc, dsLop);
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

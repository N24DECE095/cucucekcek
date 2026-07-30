/*
    File: GiaoDien.h
    Mô tả: Thư viện giao diện Console — vẽ khung, menu thanh sáng, thông báo, phân trang.
    Chỉnh từ GiaoDien.h của QLVT_Final, convert sang std::string.
*/

#ifndef GIAODIEN_H
#define GIAODIEN_H

#include "mylib.h"
#include "CauTruc.h"

// ===================== HẰNG SỐ MÀU =====================
#define MAU_DEN 0
#define MAU_XANH_DUONG 1
#define MAU_XANH_LA 2
#define MAU_XANH_NHAT 3
#define MAU_DO 4
#define MAU_TIM 5
#define MAU_VANG 6
#define MAU_TRANG 7
#define MAU_XAM 8
#define MAU_XANH_DUONG_SANG 9
#define MAU_XANH_LA_SANG 10
#define MAU_XANH_NHAT_SANG 11
#define MAU_DO_SANG 12
#define MAU_TIM_SANG 13
#define MAU_VANG_SANG 14
#define MAU_TRANG_SANG 15

// ===================== HẰNG SỐ PHÍM =====================
#define PHIM_LEN 72
#define PHIM_XUONG 80
#define PHIM_TRAI 75
#define PHIM_PHAI 77
#define PHIM_ENTER 13
#define PHIM_ESC 27
#define PHIM_SPECIAL 224
#define SO_DONG_MOT_TRANG 15

// ===================== PHÂN TRANG =====================
struct PhanTrang {
    int trangHienTai;      // Trang đang hiển thị (bắt đầu từ 1)
    int tongTrang;
    int tongSoDong;
    int soDongMotTrang;
};

// ===================== HÀM VẼ KHUNG =====================
void VeKhung(int x1, int y1, int x2, int y2);
void VeKhungCoTieuDe(int x1, int y1, int x2, int y2, const char *tieuDe);
void VeDuongNgang(int x1, int x2, int y);

// ===================== HÀM MENU THANH SÁNG =====================
// Trả về index của mục được chọn (0-indexed), -1 nếu nhấn ESC
int XuLyMenu(int x, int y, const char *items[], int soMuc);
void VeMenu(int x, int y, const char *items[], int soMuc, int viTriChon);

// ===================== HÀM PHÍM =====================
int  DocPhim();               // Đọc 1 phím, xử lý cả phím mũi tên (2 byte)
void ChoPhimBatKy();          // Chờ nhấn phím bất kỳ
int  HienMenuCon(const char *tieuDe, const char *lc1, const char *lc2);   // menu 2 lựa chọn (Y/N)

// ===================== THÔNG BÁO =====================
void XoaDong(int y);
void HienThongBaoLoi(const char *tb);
void HienThongBaoThanhCong(const char *tb);
void HienThongBao(int x, int y, const char *tb, int mau = MAU_TRANG);

// ===================== THIẾT LẬP CONSOLE =====================
void ThietLapConsole();       // Ẩn con trỏ, set kích thước
void HienConTro();
void AnConTro();
void InCanGiua(const char *s, int doRong);

// ===================== MÀN HÌNH CHÀO + MENU CHÍNH =====================
void HienManHinhChao();
int  HienMenuChinh();         // Trả về lựa chọn 1..7

// ===================== PHÂN TRANG =====================
void KhoiTaoPhanTrang(PhanTrang &pt, int tongSoDong, int soDongMotTrang = SO_DONG_MOT_TRANG);
int  TinhTongTrang(int tongSoDong, int soDongMotTrang);
bool TrangTruoc(PhanTrang &pt);
bool TrangSau(PhanTrang &pt);
int  LayViTriBatDau(PhanTrang &pt);
int  LayViTriKetThuc(PhanTrang &pt);
void HienThiThongTinTrang(PhanTrang &pt, int x, int y);

void VeMenuMonHoc(int x, int y, int viTriChon);
int  XuLyMenuMonHoc();
void MenuMonHoc(treeMH &root, DS_LTC &ds_ltc);

void VeMenuLopTinChi(int x, int y, int viTriChon);
int  XuLyMenuLopTinChi();
void MenuLopTinChi(DS_LTC &ds_ltc, treeMH root);

void VeMenuSinhVien(int x, int y, int viTriChon);
int  XuLyMenuSinhVien();
void MenuSinhVien(DS_LOP &ds_lop);

void VeMenuLop(int x, int y, int viTriChon);
int  XuLyMenuLop();
void MenuLop(DS_LOP &ds_lop);

// ===================== GIAO DIỆN GIẢNG VIÊN =====================
void VeNutGiangVien(int x, int y, int width, int height, const char *label, bool isHover);
void VeAsciiArtGiangVien(int x, int y);
void VeFooterGiangVien(int y = 27);
void VeMenuGiangVien(int viTriChon);
int  XuLyPhimGiangVien(int &viTriChon);
int  XuLyMenuGiangVien();
void HienMenuGiangVien(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH &root);

#endif // GIAODIEN_H

/*
    File: Loptinchi.h
    Tác giả: TV1 - Nguyễn Việt Anh
    Mô tả: Module Lớp tín chỉ — Mảng con trỏ tối đa MAX_LTC.
    Chức năng đề bài:
        (a) Mở lớp tín chỉ: Thêm / Xoá / Hiệu chỉnh
        (h) Hủy lớp tín chỉ tự động khi số SV đăng ký < SOSVMIN

    Kỹ thuật thầy dạy được dùng:
        - Danh sách tuyến tính bằng mảng con trỏ (Chương 3)
        - DSLK đơn Dangky bên trong LTC (Chương 4) — style thầy
        - Chèn có thứ tự Insert_Order 2 con trỏ t, s (theo DSLK_SV_Undo.cpp)
        - XacNhan Y/N + BaoLoi (style thầy)
*/

#ifndef LOPTINCHI_H
#define LOPTINCHI_H

#include "CauTruc.h"
#include "GiaoDien.h"
#include "Monhoc.h"

// ===================== AUTO-INCREMENT MALOPTC =====================
int   SinhMalopTCTiepTheo(DS_LTC &ds);

// ===================== TÌM KIẾM =====================
int         TimLTCTheoMa(DS_LTC &ds, int maloptc);   // trả -1 nếu không có
Loptinchi*  LayLTC(DS_LTC &ds, int maloptc);

// ===================== CRUD (chức năng a) =====================
bool  ThemLopTinChi(DS_LTC &ds, const Loptinchi &ltc);
bool  XoaLopTinChi(DS_LTC &ds, int maloptc);          // chỉ được xoá khi chưa có SV đăng ký
bool  HieuChinhLTC(DS_LTC &ds, int maloptc, const Loptinchi &moi);
void  HuyDSLTC(DS_LTC &ds);                           // giải phóng toàn bộ

// ===================== DSLK ĐĂNG KÝ (helpers cho chức năng h) =====================
// Chèn có thứ tự tăng dần theo MASV — style Insert_Order thầy (2 con trỏ t, s)
void  Insert_Order_DK(PTRDK &First, Dangky dk);

// Tìm 1 node theo MASV
PTRDK TimDKTheoMASV(PTRDK First, const string &masv);

// Đếm SV đã đăng ký nhưng chưa huỷ đăng ký (dùng cho chức năng h)
int   DemSVDangKy(PTRDK First);

// Đếm tổng đăng ký (cả huỷ) — dùng khi lưu file
int   DemSVDangKy_KyCaHuy(PTRDK First);

// Giải phóng DSLK đăng ký
void  GiaiPhongDSDK(PTRDK &First);

// ===================== IN =====================
void  InDSLopTinChi(DS_LTC &ds, treeMH root);

// ===================== MENU CHÍNH — Chức năng (a) =====================
void  MenuMoLopTinChi(DS_LTC &dsltc, treeMH root);

// ===================== CHỨC NĂNG (h) =====================
void  HuyLopTinChiTuDong(DS_LTC &dsltc);

#endif // LOPTINCHI_H

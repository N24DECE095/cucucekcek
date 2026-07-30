#ifndef QUANLYDIEM_H
#define QUANLYDIEM_H

#include "CauTruc.h"
#include "GiaoDien.h"
#include "Monhoc.h"
#include "Loptinchi.h"
#include "SinhVien.h"

// ===================== QUẢN LÝ ĐIỂM =====================
void NhapDiemLTC(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);
void InBangDiemLTC(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);
void InBangDiemTrungBinhTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root); // Chức năng k
void InBangDiemTongKetTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);   // Chức năng l

// Menu con Quản Lý Điểm
void VeMenuDiem(int x, int y, int viTriChon);
int  XuLyMenuDiem();
void MenuQuanLyDiem(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);

#endif // QUANLYDIEM_H


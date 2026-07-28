#ifndef QUANLYDIEM_H
#define QUANLYDIEM_H

#include "CauTruc.h"
#include "GiaoDien.h"
#include "Monhoc.h"
#include "Loptinchi.h"
#include "SinhVien.h"

void NhapDiem(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);
void InBangDiemMonHoc(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);
void InBangDiemTrungBinhTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);
void InBangDiemTongKetTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root);

#endif

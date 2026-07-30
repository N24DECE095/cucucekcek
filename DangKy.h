#ifndef DANGKY_H
#define DANGKY_H

#include "CauTruc.h"
#include "MonHoc.h"
#include "LopTinChi.h"
#include "SinhVien.h"
#include "GiaoDien.h"

//=====================================================
// QU?N LÝ ÐANG KÝ
//=====================================================

// Tìm sinh viên dã dang ký trong 1 l?p tín ch?
PTRDK TimDangKyTheoMASV(PTRDK First,
                        const string &masv);

// Thêm dang ký
bool DangKyLopTinChi(Loptinchi &ltc,
                     const string &masv);

// H?y dang ký
bool HuyDangKyLopTinChi(Loptinchi &ltc,
                        const string &masv);

// Ð?m s? sinh viên dang ký (không tính dã h?y)
int DemSVDangKy(PTRDK First);

// Gi?i phóng danh sách dang ký
void GiaiPhongDSDangKy(PTRDK &First);

//=====================================================
// CH?C NANG (b)
//=====================================================

// In danh sách SV dã dang ký theo:
// Niên khóa - H?c k? - Mã MH - Nhóm
void InDSSVDangKy(DS_LTC &dsltc,
                  DS_LOP &dslop,
                  treeMH root);

//=====================================================
// CH?C NANG (g)
//=====================================================

// Ðang ký l?p tín ch?
void MenuDangKyLopTinChi(DS_LTC &dsltc,
                         DS_LOP &dslop,
                         treeMH root);

#endif

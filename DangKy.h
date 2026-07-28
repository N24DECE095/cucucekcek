#ifndef DANGKY_H
#define DANGKY_H

#include "CauTruc.h"
#include "SinhVien.h"
#include "Loptinchi.h"
#include "Monhoc.h"

//==================== KI?M TRA ====================

// ki?m tra sinh viên có t?n t?i
bool SinhVienTonTai(DS_LOP &dslop,
                    const string &masv);

// sinh viên dã dang ký l?p tín ch? chua
bool DaDangKyLTC(Loptinchi *ltc,
                 const string &masv);

//==================== ÐANG KÝ ====================

bool DangKyLopTinChi(DS_LTC &dsltc,
                     DS_LOP &dslop,
                     int maloptc,
                     const string &masv);

bool HuyDangKyLopTinChi(DS_LTC &dsltc,
                        int maloptc,
                        const string &masv);

//==================== CH?C NANG B ====================

void InDSSVDangKy(DS_LTC &dsltc,
                  DS_LOP &dslop,
                  treeMH root,
                  const string &nienkhoa,
                  int hocky,
                  const string &mamh,
                  int nhom);

//==================== MENU ====================

void MenuDangKyHocPhan(DS_LTC &dsltc,
                       DS_LOP &dslop,
                       treeMH root);

#endif

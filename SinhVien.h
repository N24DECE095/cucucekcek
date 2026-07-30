#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "CauTruc.h"
#include "GiaoDien.h"

//=====================================================
// QU?N LÝ L?P
//=====================================================

int TimLopTheoMa(DS_LOP &ds, const string &malop);

Lop* LayLop(DS_LOP &ds, const string &malop);

PTRSV LaySinhVien(DS_LOP &ds,
                  const string &masv);

bool ThemLop(DS_LOP &ds, const Lop &lop);

bool XoaLop(DS_LOP &ds,
            const string &malop);

bool SuaLop(DS_LOP &ds,
            const string &malop,
            const Lop &moi);

void GiaiPhongDSLop(DS_LOP &ds);

void InDSLop(DS_LOP &ds);

//=====================================================
// QU?N LÝ SINH VIÊN
//=====================================================

PTRSV TaoNodeSV(const Sinhvien &sv);

PTRSV TimSVTheoMASV(PTRSV First,
                    const string &masv);

void Insert_Order_SV(PTRSV &First,
                     const Sinhvien &sv);

bool ThemSinhVien(Lop &lop,
                  const Sinhvien &sv);

bool XoaSinhVien(Lop &lop,
                 const string &masv);

bool HieuChinhSinhVien(Lop &lop,
                       const string &masv,
                       const Sinhvien &moi);

int DemSinhVien(PTRSV First);

void GiaiPhongDSSV(PTRSV &First);

void MenuNhapSinhVien(DS_LOP &dslop);

//================== CH?C NANG C ==================

void NhapSinhVienChoLop(Lop &lop,
                        int chucnang);

//================== CH?C NANG D ==================

void InDanhSachSinhVienAlphabet(Lop &lop);

#endif


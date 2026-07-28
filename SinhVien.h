#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "CauTruc.h"
#include "GiaoDien.h"

//=====================================================
// QU?N LÝ L?P
//=====================================================

// tìm v? trí l?p theo mã
int TimLopTheoMa(DS_LOP &ds, const string &malop);

// l?y con tr? l?p
Lop* LayLop(DS_LOP &ds, const string &malop);

// CRUD
bool ThemLop(DS_LOP &ds, const Lop &lop);
bool XoaLop(DS_LOP &ds, const string &malop);
bool SuaLop(DS_LOP &ds,
            const string &malop,
            const Lop &moi);

// gi?i phóng
void GiaiPhongDSLop(DS_LOP &ds);

// in
void InDSLop(DS_LOP &ds);

//=====================================================
// QU?N LÝ SINH VIÊN
//=====================================================

// t?o node
PTRSV TaoNodeSV(const Sinhvien &sv);

// tìm
PTRSV TimSVTheoMASV(PTRSV First,
                    const string &masv);
                    
PTRSV LaySinhVien(DS_LOP &ds,
                  const string &masv);
// chèn tang theo MASV
void Insert_Order_SV(PTRSV &First,
                     const Sinhvien &sv);

// CRUD
bool ThemSinhVien(Lop &lop,
                  const Sinhvien &sv);

bool XoaSinhVien(Lop &lop,
                 const string &masv);

bool HieuChinhSinhVien(Lop &lop,
                       const string &masv,
                       const Sinhvien &moi);

// d?m
int DemSinhVien(PTRSV First);

// gi?i phóng
void GiaiPhongDSSV(PTRSV &First);

// in
void InDanhSachSinhVien(PTRSV First);

#endifs

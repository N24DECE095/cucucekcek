#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "CauTruc.h"
#include "GiaoDien.h"

//=====================================================
// QU?N L� L?P
//=====================================================

// t�m v? tr� l?p theo m�
int TimLopTheoMa(DS_LOP &ds, const string &malop);

// l?y con tr? l?p
Lop* LayLop(DS_LOP &ds, const string &malop);

// CRUD
bool ThemLop(DS_LOP &ds, const Lop &lop);
bool XoaLop(DS_LOP &ds, const string &malop);
bool SuaLop(DS_LOP &ds,
            const string &malop,
            const Lop &moi);

// gi?i ph�ng
void GiaiPhongDSLop(DS_LOP &ds);

// in
void InDSLop(DS_LOP &ds);

//=====================================================
// QU?N L� SINH VI�N
//=====================================================

// t?o node
PTRSV TaoNodeSV(const Sinhvien &sv);

// t�m
PTRSV TimSVTheoMASV(PTRSV First,
                    const string &masv);
                    
PTRSV LaySinhVien(DS_LOP &ds,
                  const string &masv);
// ch�n tang theo MASV
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

// gi?i ph�ng
void GiaiPhongDSSV(PTRSV &First);

// in
void InDanhSachSinhVien(PTRSV First);

#endif

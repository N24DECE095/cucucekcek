/*
    File: XuLyFile.h
    Tác giả: TV1 - Nguyễn Việt Anh
    Mô tả: Đọc / ghi 2 file text cho Task 1:
        - data_monhoc.txt   — cây môn học (ghi PreOrder để tái tạo shape)
        - data_ltc.txt      — lớp tín chỉ + DSLK đăng ký lồng nhau

    Định dạng file text — dùng delimiter `|`:

    data_monhoc.txt:
        <soRecord>
        MAMH|TENMH|STC_LT|STC_TH
        ...

    data_ltc.txt:
        <soLTC>
        MALOPTC|MAMH|NIENKHOA|HOCKY|NHOM|SOSVMIN|SOSVMAX|HUYLOP|<soDK>
        MASV|DIEM|HUYDK
        MASV|DIEM|HUYDK
        ...
        MALOPTC|...|<soDK>
        ...
*/

#ifndef XULYFILE_H
#define XULYFILE_H

#include "CauTruc.h"

#define FILE_MONHOC "data_monhoc.txt"
#define FILE_LTC    "data_ltc.txt"
#define FILE_LOP    "data_lop.txt"

// ===================== MÔN HỌC =====================
void LuuFileMonhoc(treeMH root, const char *file = FILE_MONHOC);
void DocFileMonhoc(treeMH &root, const char *file = FILE_MONHOC);

// ===================== LỚP TÍN CHỈ (kèm Dangky lồng) =====================
void LuuFileLTC(DS_LTC &ds, const char *file = FILE_LTC);
void DocFileLTC(DS_LTC &ds, const char *file = FILE_LTC);

// ===================== LỚP HÀNH CHÍNH & SINH VIÊN =====================
void LuuFileLop(DS_LOP &ds, const char *file = FILE_LOP);
void DocFileLop(DS_LOP &ds, const char *file = FILE_LOP);

// ===================== MASTER =====================
void LuuTatCa(treeMH root, DS_LTC &dsLtc, DS_LOP &dsLop);
void DocTatCa(treeMH &root, DS_LTC &dsLtc, DS_LOP &dsLop);

#endif // XULYFILE_H

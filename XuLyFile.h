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

// ===================== MÔN HỌC =====================
void LuuFileMonhoc(treeMH root, const char *file = FILE_MONHOC);
void DocFileMonhoc(treeMH &root, const char *file = FILE_MONHOC);

// ===================== LỚP TÍN CHỈ (kèm Dangky lồng) =====================
void LuuFileLTC(DS_LTC &ds, const char *file = FILE_LTC);
void DocFileLTC(DS_LTC &ds, const char *file = FILE_LTC);

// ===================== MASTER =====================
void LuuTatCa(treeMH root, DS_LTC &dsLtc);
void DocTatCa(treeMH &root, DS_LTC &dsLtc);

#endif // XULYFILE_H

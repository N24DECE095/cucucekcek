/*
    File: Monhoc.h
    Tác giả: TV1 - Nguyễn Việt Anh
    Mô tả: Module Môn học — BST theo MAMH + hàm TỰ CÂN BẰNG (InOrder-Rebuild).
    Chức năng đề bài:
        (e) Nhập môn học: Thêm / Xoá / Hiệu chỉnh
        (f) In DS môn học tăng dần theo TÊN môn

    Kỹ thuật thầy dạy được dùng (Chương 5 giáo trình):
        - BST cơ bản: Insert / Delete 3 case / Search / Duyệt InOrder
        - THUẬT TOÁN TỰ CÂN BẰNG (theo yêu cầu miệng của thầy):
            Bước 1: Duyệt In-Order → mảng đã sắp theo MAMH tăng dần
            Bước 2: Xây lại cây từ giữa mảng (đệ qui chia đôi)
                    → cây cân bằng hoàn hảo (perfect balance)
            Được gọi tự động sau mỗi Thêm / Xoá / đọc file
        - Selection Sort để sắp theo TENMH cho chức năng (f) — Chương 6
        - Giải phóng cây bằng LRN (Postorder)
*/

#ifndef MONHOC_H
#define MONHOC_H

#include "CauTruc.h"
#include "GiaoDien.h"

// Forward declaration để dùng trong hàm xoá MH (kiểm tra ràng buộc)
struct DS_LTC;

// ===================== HÀM CƠ BẢN BST =====================
// Tìm kiếm theo MAMH — trả NULL nếu không thấy
treeMH Search(treeMH root, const string &mamh);

// Đếm số node
int    DemMH(treeMH root);

// Lấy chiều cao cây (để đánh giá cây có bị lệch không)
int    LayChieuCaoCay(treeMH root);

// ===================== TỰ CÂN BẰNG CÂY =====================
// Cân bằng cây bằng thuật toán InOrder-Rebuild:
//   B1: Duyệt In-Order → mảng
//   B2: Xoá cây cũ + xây lại từ giữa mảng bằng đệ qui chia đôi
// Kết quả: cây cân bằng hoàn hảo, độ phức tạp O(n).
// Được gọi tự động sau mỗi Thêm/Xoá/đọc file.
void   CanBangCay(treeMH &root);

// ===================== THÊM / XOÁ / HIỆU CHỈNH =====================
// Thêm 1 node — trả về gốc cây con mới; ok = false nếu MAMH đã tồn tại
treeMH ThemMH(treeMH root, Monhoc mh, bool &ok);

// Xoá node theo MAMH — trả về gốc cây con mới; ok = false nếu không thấy
treeMH XoaMH(treeMH root, const string &mamh, bool &ok);

// Hiệu chỉnh — chỉ đổi TENMH, STC_LT, STC_TH (giữ MAMH vì là khoá)
bool   HieuChinhMH(treeMH root, const string &mamh,
                   const string &tenMoi, int lt, int th);

// ===================== DUYỆT & IN =====================
// Duyệt In-Order (LNR) → ghi ra mảng (theo thứ tự MAMH tăng dần)
void   DuyetInOrderMH(treeMH root, Monhoc *arr, int &idx);

// Chức năng (f): in DSMH tăng dần theo TÊN MH
void   InDSMHTheoTen(treeMH root);

// ===================== GIẢI PHÓNG =====================
// Giải phóng toàn bộ cây (LRN)
void   GiaiPhongCayMH(treeMH &root);

// ===================== RÀNG BUỘC TOÀN VẸN =====================
// Trả về true nếu MAMH đang được ít nhất 1 LTC dùng
// (khai báo ở đây, cài đặt trong Loptinchi.cpp vì cần DS_LTC)
bool   MonHocDangDuocDung(DS_LTC &dsLtc, const string &mamh);

#endif // MONHOC_H

/*
    File: CauTruc.h
    Mô tả: Khai báo TẤT CẢ struct dữ liệu của dự án QLDSV_HTC.
    Bám sát khai báo của thầy trong li_thuyet/Khaibao_QLDSV_HTC.cpp.
    Có bổ sung struct Lop (do đề bài yêu cầu nhưng khai báo thầy thiếu).
*/

#ifndef CAUTRUC_H
#define CAUTRUC_H

#include <string>
#include <cstdio>
using namespace std;

// ===================== HẰNG SỐ =====================
const int MAX_LOP = 10000;      // Số lớp học tối đa (bổ sung)
const int MAX_LTC = 10000;      // Số lớp tín chỉ tối đa (theo khai báo thầy)

// ===================== MÔN HỌC (BST + hàm tự cân bằng) =====================
// Theo Khaibao_QLDSV_HTC.cpp — có sửa typo: thêm ; sau TENMH.
// GIỮ NGUYÊN struct theo khai báo thầy. Cơ chế tự cân bằng được cài đặt bằng
// thuật toán RIÊNG (InOrder-Rebuild) trong Monhoc.cpp — KHÔNG cần thêm trường
// nào vào struct, tránh cho cây bị suy biến khi dữ liệu vào có thứ tự.
struct Monhoc {
    string MAMH;       // C10 — khóa chính, viết HOA
    string TENMH;      // C50
    int    STC_LT;     // Số tín chỉ lý thuyết
    int    STC_TH;     // Số tín chỉ thực hành
};
struct nodeMH {
    Monhoc  mh;
    nodeMH *left;
    nodeMH *right;
};
typedef nodeMH* treeMH;

// ===================== ĐĂNG KÝ (DSLK đơn nằm trong Loptinchi) =====================
// Theo khai báo thầy: bool HUYDK = false (in-class initialization — C++11)
struct Dangky {
    string MASV;
    float  DIEM;
    bool   HUYDK;
};
struct nodeDK {
    Dangky  dk;
    nodeDK *next;
};
typedef nodeDK* PTRDK;

// ===================== LỚP TÍN CHỈ (Mảng con trỏ) =====================
// Theo khai báo thầy: HUYLOP=false, dssvdk=NULL default
struct Loptinchi {
    int    MALOPTC;      // Auto-increment
    string MAMH;
    string NIENKHOA;     // Ví dụ "2025-2026"
    int    HOCKY;        // 1, 2, 3
    int    NHOM;
    int    SOSVMIN;
    int    SOSVMAX;
    bool   HUYLOP;
    PTRDK  dssvdk;       // DSLK các sinh viên đã đăng ký
};
struct DS_LTC {
    Loptinchi *nodes[MAX_LTC];
    int n;
};

// ===================== SINH VIÊN (DSLK đơn nằm trong Lop) =====================
// Theo khai báo thầy: giữ trường MALOP (back-reference về Lop)
struct Sinhvien {
    string MASV;       // C10 — khóa chính, viết HOA
    string HO;         // C30
    string TEN;        // C10
    string PHAI;       // "NAM" hoặc "NU"
    string SODT;
    string MALOP;      // back-reference về Lop.MALOP
};
struct nodeSV {
    Sinhvien sv;
    nodeSV  *next;
};
typedef nodeSV* PTRSV;

// ===================== LỚP HỌC (BỔ SUNG — theo đề bài) =====================
// Đề bài: Lop (MALOP, TENLOP, con trỏ dssv trỏ đến DSSV thuộc lớp đó)
struct Lop {
    string MALOP;      // C10 — khóa chính, viết HOA
    string TENLOP;     // C50
    PTRSV  dssv;       // DSLK sinh viên của lớp
};
struct DS_LOP {
    Lop *nodes[MAX_LOP];
    int n;
};

// ===================== HÀM KHỞI TẠO =====================
inline void KhoiTaoDSLTC(DS_LTC &ds) {
    ds.n = 0;
    for (int i = 0; i < MAX_LTC; i++) ds.nodes[i] = NULL;
}
inline void KhoiTaoDSLop(DS_LOP &ds) {
    ds.n = 0;
    for (int i = 0; i < MAX_LOP; i++) ds.nodes[i] = NULL;
}

// Khởi tạo 1 Loptinchi mới (đảm bảo dssvdk = NULL, HUYLOP = false)
inline void KhoiTaoLopTinChi(Loptinchi &ltc) {
    ltc.dssvdk = NULL;
    ltc.HUYLOP = false;
}

// Khởi tạo 1 Lop mới
inline void KhoiTaoLop(Lop &lop) {
    lop.dssv = NULL;
}

// Khởi tạo 1 Đăng ký mới
inline void KhoiTaoDangKy(Dangky &dk) {
    dk.DIEM = -1;      // -1 nghĩa là chưa nhập điểm
    dk.HUYDK = false;
}

#endif // CAUTRUC_H

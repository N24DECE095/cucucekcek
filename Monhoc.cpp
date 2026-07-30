/*
    File: Monhoc.cpp
    Tác giả: TV1 - Nguyễn Việt Anh
    Mô tả: Cài đặt module Môn học — BST + THUẬT TOÁN TỰ CÂN BẰNG.

    Cây được giữ nguyên struct theo khai báo thầy (không có trường `height`).
    Cơ chế tự cân bằng: sau mỗi Thêm/Xoá, gọi CanBangCay() để cân bằng lại.
    Thuật toán CanBangCay dùng InOrder-Rebuild — O(n):
        B1: Duyệt InOrder → mảng đã sắp
        B2: Xoá cây + xây lại từ middle mảng bằng đệ qui chia đôi
        → cây cân bằng hoàn hảo (chiều cao ≈ log2(n)).

    Vì sao không dùng AVL? — Vì AVL cần thêm trường `height` vào struct,
    khác với khai báo thầy. Cách này giữ khai báo nguyên vẹn.
*/

#include "Monhoc.h"

// ============================================================================
// TÌM KIẾM BST — đệ qui, O(log n) khi cây cân bằng
// ============================================================================
treeMH Search(treeMH root, const string &mamh) {
    if (root == NULL) return NULL;
    if (root->mh.MAMH == mamh) return root;
    if (mamh < root->mh.MAMH)
        return Search(root->left, mamh);
    return Search(root->right, mamh);
}

// ============================================================================
// ĐẾM SỐ NODE — dùng đệ qui InOrder
// ============================================================================
int DemMH(treeMH root) {
    if (root == NULL) return 0;
    return 1 + DemMH(root->left) + DemMH(root->right);
}

// ============================================================================
// THÊM NODE — BST insert (Chương 5)
// Việc cân bằng lại được thực hiện SAU KHI thêm, bằng cách gọi CanBangCay(root)
// từ hàm gọi (VD FormThemMH).
// ============================================================================
treeMH ThemMH(treeMH root, Monhoc mh, bool &ok) {
    if (root == NULL) {
        treeMH p = new nodeMH;
        p->mh = mh;
        p->left = p->right = NULL;
        ok = true;
        return p;
    }
    if (mh.MAMH == root->mh.MAMH) {
        ok = false;                     // Trùng khoá → không thêm
        return root;
    }
    if (mh.MAMH < root->mh.MAMH)
        root->left = ThemMH(root->left, mh, ok);
    else
        root->right = ThemMH(root->right, mh, ok);
    return root;
}

// ----- Hàm phụ: tìm node có MAMH nhỏ nhất trong cây (dùng cho xoá case 3) -----
static treeMH TimNodeNhoNhat(treeMH root) {
    while (root != NULL && root->left != NULL) root = root->left;
    return root;
}

// ============================================================================
// XOÁ NODE — 3 trường hợp: lá / 1 con / 2 con (Chương 5)
// Việc cân bằng lại được thực hiện SAU KHI xoá, bằng cách gọi CanBangCay(root)
// từ hàm gọi (VD FormXoaMH).
// ============================================================================
treeMH XoaMH(treeMH root, const string &mamh, bool &ok) {
    if (root == NULL) { ok = false; return NULL; }

    if (mamh < root->mh.MAMH) {
        root->left = XoaMH(root->left, mamh, ok);
    }
    else if (mamh > root->mh.MAMH) {
        root->right = XoaMH(root->right, mamh, ok);
    }
    else {
        // Đã tìm thấy node cần xoá

        // Trường hợp 1 & 2: lá hoặc chỉ có 1 con
        if (root->left == NULL) {
            treeMH tmp = root->right;
            delete root;
            ok = true;
            return tmp;
        }
        if (root->right == NULL) {
            treeMH tmp = root->left;
            delete root;
            ok = true;
            return tmp;
        }

        // Trường hợp 3: có 2 con — chép successor rồi xoá successor
        treeMH successor = TimNodeNhoNhat(root->right);
        root->mh = successor->mh;
        bool okTmp;
        root->right = XoaMH(root->right, successor->mh.MAMH, okTmp);
        ok = true;
    }
    return root;
}

// ============================================================================
// LẤY CHIỀU CAO CÂY — dùng để đánh giá cây có cân bằng hay không
// ============================================================================
int LayChieuCaoCay(treeMH root) {
    if (root == NULL) return 0;
    int hL = LayChieuCaoCay(root->left);
    int hR = LayChieuCaoCay(root->right);
    return 1 + (hL > hR ? hL : hR);
}

// ============================================================================
// CÂN BẰNG CÂY — THUẬT TOÁN INORDER-REBUILD
//   Bước 1: Duyệt In-Order → mảng arr[] đã sắp tăng dần theo MAMH
//   Bước 2: Giải phóng cây cũ
//   Bước 3: Xây lại cây từ middle mảng bằng đệ qui chia đôi
//           → cây cân bằng hoàn hảo, chiều cao ≈ log2(n)
// Độ phức tạp: O(n)
// ============================================================================
static treeMH XayCayTuMang(Monhoc *arr, int l, int r) {
    if (l > r) return NULL;
    int mid = (l + r) / 2;
    treeMH p = new nodeMH;
    p->mh = arr[mid];
    p->left  = XayCayTuMang(arr, l, mid - 1);
    p->right = XayCayTuMang(arr, mid + 1, r);
    return p;
}

void CanBangCay(treeMH &root) {
    int n = DemMH(root);
    if (n <= 2) return;                        // cây ≤ 2 node đã cân bằng sẵn

    // Bước 1: đưa cây ra mảng theo In-Order (tăng dần theo MAMH)
    Monhoc *arr = new Monhoc[n];
    int idx = 0;
    DuyetInOrderMH(root, arr, idx);

    // Bước 2: giải phóng cây cũ
    GiaiPhongCayMH(root);

    // Bước 3: xây lại từ middle → cây cân bằng
    root = XayCayTuMang(arr, 0, n - 1);

    delete[] arr;
}

// ============================================================================
// HIỆU CHỈNH — không thay đổi khoá MAMH
// ============================================================================
bool HieuChinhMH(treeMH root, const string &mamh,
                 const string &tenMoi, int lt, int th) {
    treeMH p = Search(root, mamh);
    if (p == NULL) return false;
    p->mh.TENMH  = tenMoi;
    p->mh.STC_LT = lt;
    p->mh.STC_TH = th;
    return true;
}

// ============================================================================
// DUYỆT IN-ORDER (LNR) → ghi ra mảng
// ============================================================================
void DuyetInOrderMH(treeMH root, Monhoc *arr, int &idx) {
    if (root == NULL) return;
    DuyetInOrderMH(root->left, arr, idx);
    arr[idx++] = root->mh;
    DuyetInOrderMH(root->right, arr, idx);
}

// ============================================================================
// CHỨC NĂNG (f): IN DSMH TĂNG DẦN THEO TÊN MÔN
// Bước 1: duyệt cây → mảng (tăng dần theo MAMH)
// Bước 2: Selection Sort mảng theo TENMH (kỹ thuật Chương 6)
// Bước 3: in ra bảng
// ============================================================================
void InDSMHTheoTen(treeMH root) {
    int n = DemMH(root);
    if (n == 0) {
        HienThongBaoLoi("Danh sach mon hoc rong!");
        return;
    }
    Monhoc *arr = new Monhoc[n];
    int idx = 0;
    DuyetInOrderMH(root, arr, idx);

    // Selection Sort theo TENMH (không phân biệt hoa/thường)
    for (int i = 0; i < n - 1; i++) {
        int imin = i;
        for (int j = i + 1; j < n; j++)
            if (StrCmpNoCase(arr[j].TENMH, arr[imin].TENMH) < 0)
                imin = j;
        if (imin != i) {
            Monhoc tam = arr[i];
            arr[i] = arr[imin];
            arr[imin] = tam;
        }
    }

    // In bảng
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "DANH SACH MON HOC (TANG DAN THEO TEN)");
    gotoxy(4, 3);
    SetColor(MAU_VANG_SANG);
    printf("%-5s  %-12s  %-45s  %-8s  %-8s",
           "STT", "MA MH", "TEN MON HOC", "STC LT", "STC TH");
    SetColor(MAU_TRANG);
    VeDuongNgang(4, 115, 4);

    int y = 5;
    for (int i = 0; i < n && i < 20; i++) {
        gotoxy(4, y + i);
        printf("%-5d  %-12s  %-45s  %-8d  %-8d",
               i + 1,
               arr[i].MAMH.c_str(),
               arr[i].TENMH.c_str(),
               arr[i].STC_LT,
               arr[i].STC_TH);
    }
    gotoxy(4, 26);
    SetColor(MAU_XANH_LA_SANG);
    printf("Tong so mon: %d", n);
    SetColor(MAU_TRANG);

    delete[] arr;
    ChoPhimBatKy();
}

// ============================================================================
// GIẢI PHÓNG CÂY — LRN (Postorder) để xoá con trước rồi mới xoá cha
// ============================================================================
void GiaiPhongCayMH(treeMH &root) {
    if (root == NULL) return;
    GiaiPhongCayMH(root->left);
    GiaiPhongCayMH(root->right);
    delete root;
    root = NULL;
}

// ============================================================================
// FORM NHẬP LIỆU — 3 form phục vụ chức năng (e)
// ============================================================================

// ---- Form Thêm MH ----
static void FormThemMH(treeMH &root) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "THEM MON HOC MOI");
    Monhoc mh;

    gotoxy(6, 5); printf("Ma mon hoc (toi da 10 ky tu, ENTER de huy): ");
    HienConTro();
    NhapMa(mh.MAMH, 10, true);
    AnConTro();
    if (mh.MAMH.empty()) return;

    if (Search(root, mh.MAMH) != NULL) {
        HienThongBaoLoi("Ma mon hoc da ton tai!");
        return;
    }

    gotoxy(6, 7); printf("Ten mon hoc (toi da 50 ky tu): ");
    HienConTro();
    NhapChuoi(mh.TENMH, 50);
    if (mh.TENMH.empty()) { AnConTro(); return; }

    gotoxy(6, 9);  printf("So tin chi ly thuyet: ");
    NhapSo(mh.STC_LT);

    gotoxy(6, 11); printf("So tin chi thuc hanh: ");
    NhapSo(mh.STC_TH);
    AnConTro();

    if (mh.STC_LT + mh.STC_TH == 0) {
        HienThongBaoLoi("Tong so tin chi phai > 0!");
        return;
    }

    bool ok;
    root = ThemMH(root, mh, ok);
    if (ok) {
        CanBangCay(root);           // Tự cân bằng lại cây sau khi thêm
        HienThongBaoThanhCong("Da them mon hoc va can bang cay.");
    } else {
        HienThongBaoLoi("Loi: Ma mon hoc bi trung.");
    }
}

// ---- Form Xoá MH ----
static void FormXoaMH(treeMH &root, DS_LTC &dsLtc) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "XOA MON HOC");
    string mamh;

    gotoxy(6, 5); printf("Nhap ma mon hoc can xoa (ENTER de huy): ");
    HienConTro();
    NhapMa(mamh, 10, true);
    AnConTro();
    if (mamh.empty()) return;

    treeMH p = Search(root, mamh);
    if (p == NULL) {
        HienThongBaoLoi("Khong tim thay ma mon hoc.");
        return;
    }
    gotoxy(6, 7); printf("Ten mon hoc: %s", p->mh.TENMH.c_str());
    gotoxy(6, 8); printf("So TC: %d (LT) + %d (TH)", p->mh.STC_LT, p->mh.STC_TH);

    // Ràng buộc toàn vẹn: không xoá nếu MH đang được LTC tham chiếu
    if (MonHocDangDuocDung(dsLtc, mamh)) {
        HienThongBaoLoi("Khong the xoa: Mon hoc dang duoc su dung boi 1 lop tin chi.");
        return;
    }

    gotoxy(6, 10);
    HienConTro();
    if (XacNhan("Ban co chac chan muon xoa mon hoc nay? (Y/N) ")) {
        bool ok;
        root = XoaMH(root, mamh, ok);
        if (ok) {
            CanBangCay(root);       // Tự cân bằng lại cây sau khi xoá
            HienThongBaoThanhCong("Da xoa mon hoc va can bang cay.");
        } else {
            HienThongBaoLoi("Xoa that bai.");
        }
    }
    AnConTro();
}

// ---- Form Hiệu chỉnh MH ----
static void FormHieuChinhMH(treeMH root) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "HIEU CHINH MON HOC");
    string mamh;

    gotoxy(6, 5); printf("Nhap ma mon hoc can sua (ENTER de huy): ");
    HienConTro();
    NhapMa(mamh, 10, true);
    AnConTro();
    if (mamh.empty()) return;

    treeMH p = Search(root, mamh);
    if (p == NULL) {
        HienThongBaoLoi("Khong tim thay ma mon hoc.");
        return;
    }
    gotoxy(6, 7); printf("Thong tin hien tai:");
    gotoxy(6, 8); printf("  Ten:   %s", p->mh.TENMH.c_str());
    gotoxy(6, 9); printf("  STC:   %d (LT) + %d (TH)", p->mh.STC_LT, p->mh.STC_TH);

    string tenMoi;
    int lt, th;
    gotoxy(6, 11); printf("Ten moi (max 50): ");
    HienConTro();
    NhapChuoi(tenMoi, 50);

    gotoxy(6, 13); printf("STC LT moi: ");
    NhapSo(lt);

    gotoxy(6, 15); printf("STC TH moi: ");
    NhapSo(th);
    AnConTro();

    if (lt + th == 0) {
        HienThongBaoLoi("Tong so tin chi phai > 0!");
        return;
    }

    if (HieuChinhMH(root, mamh, tenMoi, lt, th))
        HienThongBaoThanhCong("Da cap nhat.");
    else
        HienThongBaoLoi("Cap nhat that bai.");
}



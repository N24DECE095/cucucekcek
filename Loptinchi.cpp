/*
    File: Loptinchi.cpp
    Tác giả: TV1 - Nguyễn Việt Anh
    Mô tả: Cài đặt Lớp tín chỉ (mảng con trỏ) + DSLK đăng ký (Dangky).
*/

#include "Loptinchi.h"

// ============================================================================
// AUTO-INCREMENT MALOPTC — tìm giá trị max hiện có rồi + 1
// ============================================================================
int SinhMalopTCTiepTheo(DS_LTC &ds) {
    int max = 0;
    for (int i = 0; i < ds.n; i++)
        if (ds.nodes[i] != NULL && ds.nodes[i]->MALOPTC > max)
            max = ds.nodes[i]->MALOPTC;
    return max + 1;
}

// ============================================================================
// TÌM KIẾM
// ============================================================================
int TimLTCTheoMa(DS_LTC &ds, int maloptc) {
    for (int i = 0; i < ds.n; i++)
        if (ds.nodes[i] != NULL && ds.nodes[i]->MALOPTC == maloptc)
            return i;
    return -1;
}

Loptinchi* LayLTC(DS_LTC &ds, int maloptc) {
    int i = TimLTCTheoMa(ds, maloptc);
    return (i == -1) ? NULL : ds.nodes[i];
}

// ============================================================================
// THÊM LTC — thêm vào cuối mảng
// ============================================================================
bool ThemLopTinChi(DS_LTC &ds, const Loptinchi &ltc) {
    if (ds.n >= MAX_LTC) return false;
    Loptinchi *p = new Loptinchi;
    *p = ltc;                        // copy nội dung
    ds.nodes[ds.n++] = p;
    return true;
}

// ============================================================================
// XOÁ LTC — chỉ được xoá khi chưa có Dangky
// ============================================================================
bool XoaLopTinChi(DS_LTC &ds, int maloptc) {
    int i = TimLTCTheoMa(ds, maloptc);
    if (i == -1) return false;
    // Ràng buộc: không xoá LTC nếu đã có SV đăng ký
    if (ds.nodes[i]->dssvdk != NULL) return false;
    delete ds.nodes[i];
    // Dồn mảng về trái
    for (int j = i; j < ds.n - 1; j++)
        ds.nodes[j] = ds.nodes[j + 1];
    ds.nodes[--ds.n] = NULL;
    return true;
}

// ============================================================================
// HIỆU CHỈNH LTC — giữ nguyên MALOPTC và dssvdk, cập nhật các trường khác
// ============================================================================
bool HieuChinhLTC(DS_LTC &ds, int maloptc, const Loptinchi &moi) {
    int i = TimLTCTheoMa(ds, maloptc);
    if (i == -1) return false;

    PTRDK giuDSSVDK   = ds.nodes[i]->dssvdk;
    int   giuMALOPTC  = ds.nodes[i]->MALOPTC;
    *(ds.nodes[i])    = moi;
    ds.nodes[i]->MALOPTC = giuMALOPTC;
    ds.nodes[i]->dssvdk  = giuDSSVDK;
    return true;
}

// ============================================================================
// GIẢI PHÓNG TOÀN BỘ DS_LTC (bao gồm cả DSLK Dangky bên trong)
// ============================================================================
void HuyDSLTC(DS_LTC &ds) {
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != NULL) {
            GiaiPhongDSDK(ds.nodes[i]->dssvdk);
            delete ds.nodes[i];
            ds.nodes[i] = NULL;
        }
    }
    ds.n = 0;
}

// ============================================================================
// DSLK ĐĂNG KÝ — CHÈN CÓ THỨ TỰ (STYLE INSERT_ORDER CỦA THẦY, 2 CON TRỎ t, s)
// Tham chiếu: li_thuyet/DSLK_SV_Undo.cpp — hàm Insert_Order
// ============================================================================
void Insert_Order_DK(PTRDK &First, Dangky dk) {
    PTRDK p = new nodeDK;
    p->dk = dk;

    PTRDK t = NULL, s;
    // t đứng trước s trong quá trình quét
    for (s = First; s != NULL && s->dk.MASV < dk.MASV; t = s, s = s->next);

    if (s == First) {
        // Chèn đầu danh sách
        p->next = First;
        First = p;
    } else {
        // Chèn giữa hoặc cuối
        p->next = s;
        t->next = p;
    }
}

// ============================================================================
// TÌM KIẾM ĐĂNG KÝ THEO MASV
// ============================================================================
PTRDK TimDKTheoMASV(PTRDK First, const string &masv) {
    for (PTRDK p = First; p != NULL; p = p->next)
        if (p->dk.MASV == masv) return p;
    return NULL;
}

// ============================================================================
// ĐẾM — chỉ đếm SV chưa huỷ đăng ký (dùng cho chức năng h)
// ============================================================================
int DemSVDangKy(PTRDK First) {
    int dem = 0;
    for (PTRDK p = First; p != NULL; p = p->next)
        if (!p->dk.HUYDK) dem++;
    return dem;
}

// Đếm tổng cả những SV đã huỷ (dùng khi lưu file để biết số dòng cần ghi)
int DemSVDangKy_KyCaHuy(PTRDK First) {
    int dem = 0;
    for (PTRDK p = First; p != NULL; p = p->next) dem++;
    return dem;
}

// ============================================================================
// GIẢI PHÓNG DSLK ĐĂNG KÝ
// ============================================================================
void GiaiPhongDSDK(PTRDK &First) {
    while (First != NULL) {
        PTRDK p = First;
        First = First->next;
        delete p;
    }
}

// ============================================================================
// RÀNG BUỘC — kiểm tra MH có LTC đang tham chiếu không
// (Được khai báo trong Monhoc.h, cài đặt ở đây vì cần biết struct DS_LTC)
// ============================================================================
bool MonHocDangDuocDung(DS_LTC &dsLtc, const string &mamh) {
    for (int i = 0; i < dsLtc.n; i++)
        if (dsLtc.nodes[i] != NULL && dsLtc.nodes[i]->MAMH == mamh)
            return true;
    return false;
}

// ============================================================================
// IN DANH SÁCH LTC — kèm tên môn (tra bằng cây MH)
// ============================================================================
void InDSLopTinChi(DS_LTC &ds, treeMH root) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "DANH SACH LOP TIN CHI");
    gotoxy(4, 3);
    SetColor(MAU_VANG_SANG);
    printf("%-6s  %-10s  %-25s  %-11s  %-3s  %-5s  %-5s  %-5s  %-4s",
           "MALTC", "MAMH", "TEN MH", "NIEN KHOA", "HK",
           "NHOM", "SVDK", "MAX", "HUY");
    SetColor(MAU_TRANG);
    VeDuongNgang(4, 115, 4);

    int y = 5, cnt = 0;
    for (int i = 0; i < ds.n && cnt < 20; i++) {
        Loptinchi *l = ds.nodes[i];
        treeMH mh = Search(root, l->MAMH);
        string tenMH = (mh != NULL) ? mh->mh.TENMH : "(MH khong ton tai)";
        if ((int)tenMH.size() > 24) tenMH = tenMH.substr(0, 24);
        gotoxy(4, y + cnt);
        printf("%-6d  %-10s  %-25s  %-11s  %-3d  %-5d  %-5d  %-5d  %-4s",
               l->MALOPTC, l->MAMH.c_str(), tenMH.c_str(),
               l->NIENKHOA.c_str(), l->HOCKY, l->NHOM,
               DemSVDangKy(l->dssvdk), l->SOSVMAX,
               l->HUYLOP ? "YES" : "NO");
        cnt++;
    }
    gotoxy(4, 26);
    SetColor(MAU_XANH_LA_SANG);
    printf("Tong so LTC: %d", ds.n);
    SetColor(MAU_TRANG);
    ChoPhimBatKy();
}

// ============================================================================
// FORM NHẬP LIỆU CHỨC NĂNG (a)
// ============================================================================

// ---- Form THÊM LTC ----
static void FormThemLTC(DS_LTC &dsltc, treeMH root) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "MO LOP TIN CHI MOI (Chuc nang a)");
    Loptinchi ltc;
    KhoiTaoLopTinChi(ltc);
    ltc.MALOPTC = SinhMalopTCTiepTheo(dsltc);

    gotoxy(6, 4);
    SetColor(MAU_XANH_LA_SANG);
    printf("Ma lop tin chi (tu dong sinh): %d", ltc.MALOPTC);
    SetColor(MAU_TRANG);

    gotoxy(6, 6); printf("Ma mon hoc: ");
    HienConTro();
    NhapMa(ltc.MAMH, 10, true);
    if (ltc.MAMH.empty()) { AnConTro(); return; }

    treeMH mh = Search(root, ltc.MAMH);
    if (mh == NULL) {
        AnConTro();
        HienThongBaoLoi("Ma mon hoc khong ton tai. Vao Menu 1 tao MH truoc.");
        return;
    }
    gotoxy(6, 7);
    SetColor(MAU_XAM);
    printf("--> %s (LT %d, TH %d)",
           mh->mh.TENMH.c_str(), mh->mh.STC_LT, mh->mh.STC_TH);
    SetColor(MAU_TRANG);

    gotoxy(6, 9); printf("Nien khoa (VD 2025-2026): ");
    NhapMa(ltc.NIENKHOA, 15, false);

    gotoxy(6, 11); printf("Hoc ky (1-3): "); NhapSo(ltc.HOCKY);
    if (ltc.HOCKY < 1) ltc.HOCKY = 1;
    if (ltc.HOCKY > 3) ltc.HOCKY = 3;

    gotoxy(6, 13); printf("Nhom: ");        NhapSo(ltc.NHOM);
    gotoxy(6, 15); printf("So SV min: ");    NhapSo(ltc.SOSVMIN);
    gotoxy(6, 17); printf("So SV max: ");    NhapSo(ltc.SOSVMAX);
    AnConTro();

    if (ltc.SOSVMAX < ltc.SOSVMIN) {
        HienThongBaoLoi("SO SV MAX phai >= SO SV MIN.");
        return;
    }
    if (ltc.SOSVMIN <= 0 || ltc.SOSVMAX <= 0) {
        HienThongBaoLoi("So SV min/max phai > 0.");
        return;
    }

    if (ThemLopTinChi(dsltc, ltc))
        HienThongBaoThanhCong("Da mo lop tin chi.");
    else
        HienThongBaoLoi("Danh sach lop tin chi day.");
}

// ---- Form XOÁ LTC ----
static void FormXoaLTC(DS_LTC &dsltc) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "XOA LOP TIN CHI");
    int maloptc;
    gotoxy(6, 5); printf("Ma lop tin chi can xoa: ");
    HienConTro();
    NhapSo(maloptc);
    AnConTro();

    Loptinchi *ltc = LayLTC(dsltc, maloptc);
    if (ltc == NULL) {
        HienThongBaoLoi("Khong tim thay ma LTC.");
        return;
    }
    gotoxy(6, 7); printf("MH: %s | NK: %s | HK: %d | Nhom: %d",
                         ltc->MAMH.c_str(), ltc->NIENKHOA.c_str(),
                         ltc->HOCKY, ltc->NHOM);
    gotoxy(6, 8); printf("So SV da DK: %d", DemSVDangKy(ltc->dssvdk));

    if (ltc->dssvdk != NULL) {
        HienThongBaoLoi("Khong the xoa: Da co SV dang ky.");
        return;
    }
    gotoxy(6, 10);
    HienConTro();
    if (XacNhan("Xac nhan xoa? (Y/N) ")) {
        if (XoaLopTinChi(dsltc, maloptc))
            HienThongBaoThanhCong("Da xoa lop tin chi.");
    }
    AnConTro();
}

// ---- Form HIỆU CHỈNH LTC ----
static void FormHieuChinhLTC(DS_LTC &dsltc, treeMH root) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "HIEU CHINH LOP TIN CHI");
    int maloptc;
    gotoxy(6, 5); printf("Ma LTC can sua: ");
    HienConTro();
    NhapSo(maloptc);
    AnConTro();

    Loptinchi *cu = LayLTC(dsltc, maloptc);
    if (cu == NULL) { HienThongBaoLoi("Khong tim thay ma LTC."); return; }

    Loptinchi moi = *cu;
    gotoxy(6, 7); printf("MH hien tai: %s | Nhap MH moi (ENTER giu nguyen): ",
                        cu->MAMH.c_str());
    HienConTro();
    string tmp;
    NhapMa(tmp, 10, true);
    if (!tmp.empty()) {
        if (Search(root, tmp) == NULL) {
            AnConTro();
            HienThongBaoLoi("Ma mon hoc khong ton tai.");
            return;
        }
        moi.MAMH = tmp;
    }

    gotoxy(6, 9); printf("NK hien tai %s | Nhap moi (ENTER giu): ",
                        cu->NIENKHOA.c_str());
    NhapMa(tmp, 15, true);
    if (!tmp.empty()) moi.NIENKHOA = tmp;

    gotoxy(6, 11); printf("HK hien tai %d | Nhap moi: ", cu->HOCKY);
    NhapSo(moi.HOCKY);

    gotoxy(6, 13); printf("Nhom hien tai %d | Nhap moi: ", cu->NHOM);
    NhapSo(moi.NHOM);

    gotoxy(6, 15); printf("Min hien tai %d | Nhap moi: ", cu->SOSVMIN);
    NhapSo(moi.SOSVMIN);

    gotoxy(6, 17); printf("Max hien tai %d | Nhap moi: ", cu->SOSVMAX);
    NhapSo(moi.SOSVMAX);
    AnConTro();

    if (moi.SOSVMAX < moi.SOSVMIN) {
        HienThongBaoLoi("MAX phai >= MIN.");
        return;
    }

    if (HieuChinhLTC(dsltc, maloptc, moi))
        HienThongBaoThanhCong("Da cap nhat LTC.");
}

// ============================================================================
// MENU CHÍNH — Chức năng (a)
// ============================================================================
void MenuMoLopTinChi(DS_LTC &dsltc, treeMH root) {
    const char *items[5] = {
        "1. Mo lop tin chi moi                        ",
        "2. Xoa lop tin chi                           ",
        "3. Hieu chinh lop tin chi                    ",
        "4. In danh sach lop tin chi                  ",
        "5. Quay lai                                  "
    };
    while (true) {
        clrscr();
        VeKhungCoTieuDe(2, 1, 118, 28, "QUAN LY LOP TIN CHI (Chuc nang a)");
        gotoxy(30, 5);
        SetColor(MAU_VANG_SANG);
        printf("Tong so LTC: %d", dsltc.n);
        SetColor(MAU_TRANG);
        int chon = XuLyMenu(30, 8, items, 5);
        if (chon == -1 || chon == 4) return;
        switch (chon) {
            case 0: FormThemLTC(dsltc, root); break;
            case 1: FormXoaLTC(dsltc); break;
            case 2: FormHieuChinhLTC(dsltc, root); break;
            case 3: InDSLopTinChi(dsltc, root); break;
        }
    }
}

// ============================================================================
// CHỨC NĂNG (h) — HUỶ LTC TỰ ĐỘNG KHI SỐ SV ĐĂNG KÝ < SOSVMIN
// User nhập NK + HK → chương trình lọc các LTC trong NK+HK
// đó, đếm số SV chưa huỷ, nếu < SOSVMIN thì đưa vào danh sách cần huỷ.
// Trước khi thực sự huỷ, YÊU CẦU USER XÁC NHẬN (theo đề bài).
// ============================================================================
void HuyLopTinChiTuDong(DS_LTC &dsltc) {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "HUY LOP TIN CHI TU DONG (Chuc nang h)");
    string nk;
    int hk;
    gotoxy(6, 3); printf("Nien khoa: ");
    HienConTro();
    NhapMa(nk, 15, true);
    if (nk.empty()) { AnConTro(); return; }

    gotoxy(6, 5); printf("Hoc ky: ");
    NhapSo(hk);
    AnConTro();

    // Lọc và liệt kê các LTC cần huỷ
    int idxCanHuy[MAX_LTC];
    int soHuy = 0;

    gotoxy(4, 8);
    SetColor(MAU_VANG_SANG);
    printf("Cac LTC co SO SV DK < SO SV MIN va CHUA BI HUY:");
    SetColor(MAU_TRANG);

    int y = 10;
    for (int i = 0; i < dsltc.n; i++) {
        Loptinchi *l = dsltc.nodes[i];
        if (l == NULL) continue;
        if (l->NIENKHOA != nk || l->HOCKY != hk) continue;
        if (l->HUYLOP) continue;              // đã huỷ trước đó thì bỏ qua

        int daDK = DemSVDangKy(l->dssvdk);
        if (daDK < l->SOSVMIN) {
            idxCanHuy[soHuy++] = i;
            gotoxy(4, y);
            printf("  MALTC %d - MH %s - Nhom %d - Da DK %d/%d (min %d)",
                   l->MALOPTC, l->MAMH.c_str(), l->NHOM,
                   daDK, l->SOSVMAX, l->SOSVMIN);
            y++;
            if (y >= 22) break;               // giới hạn hiển thị
        }
    }
    if (soHuy == 0) {
        HienThongBaoLoi("Khong co LTC nao can huy trong NK+HK nay.");
        return;
    }
    gotoxy(4, y + 2);
    HienConTro();
    char msg[100];
    sprintf(msg, "Xac nhan huy %d LTC tren? (Y/N) ", soHuy);
    if (XacNhan(msg)) {
        for (int k = 0; k < soHuy; k++)
            dsltc.nodes[idxCanHuy[k]]->HUYLOP = true;
        HienThongBaoThanhCong("Da huy cac LTC.");
    }
    AnConTro();
}

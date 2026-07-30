/*
    File: GiaoDien.cpp
    Mô tả: Cài đặt các hàm giao diện.
*/

#include "GiaoDien.h"
#include "QuanLyDiem.h"
#include "Monhoc.h"
#include "Loptinchi.h"
#include "SinhVien.h"

// ============================================================================
// VẼ KHUNG
// ============================================================================

void VeKhung(int x1, int y1, int x2, int y2) {
    // Góc
    gotoxy(x1, y1); printf("+");
    gotoxy(x2, y1); printf("+");
    gotoxy(x1, y2); printf("+");
    gotoxy(x2, y2); printf("+");
    // Cạnh ngang
    for (int i = x1 + 1; i < x2; i++) {
        gotoxy(i, y1); printf("-");
        gotoxy(i, y2); printf("-");
    }
    // Cạnh dọc
    for (int j = y1 + 1; j < y2; j++) {
        gotoxy(x1, j); printf("|");
        gotoxy(x2, j); printf("|");
    }
}

void VeKhungCoTieuDe(int x1, int y1, int x2, int y2, const char *tieuDe) {
    VeKhung(x1, y1, x2, y2);
    int len = strlen(tieuDe);
    int mid = (x1 + x2) / 2 - len / 2;
    gotoxy(mid - 1, y1);
    SetColor(MAU_VANG_SANG);
    printf(" %s ", tieuDe);
    SetColor(MAU_TRANG);
}

void VeDuongNgang(int x1, int x2, int y) {
    for (int i = x1; i <= x2; i++) {
        gotoxy(i, y);
        printf("-");
    }
}

// ============================================================================
// MENU THANH SÁNG
// ============================================================================

void VeMenu(int x, int y, const char *items[], int soMuc, int viTriChon) {
    for (int i = 0; i < soMuc; i++) {
        gotoxy(x, y + i);
        if (i == viTriChon) {
            SetColor(MAU_TRANG_SANG);
            SetBGColor(MAU_XANH_DUONG);
            printf(" > %s", items[i]);
            int len = strlen(items[i]);
            for (int k = len; k < 40; k++) printf(" ");
            SetBGColor(MAU_DEN);
        } else {
            SetColor(MAU_TRANG);
            printf("   %s", items[i]);
        }
    }
    SetColor(MAU_TRANG);
}

int XuLyMenu(int x, int y, const char *items[], int soMuc) {
    int chon = 0;
    VeMenu(x, y, items, soMuc, chon);
    while (true) {
        int phim = DocPhim();
        if (phim == PHIM_ENTER) return chon;
        if (phim == PHIM_ESC) return -1;
        if (phim == PHIM_LEN) {
            chon = (chon - 1 + soMuc) % soMuc;
            VeMenu(x, y, items, soMuc, chon);
        }
        if (phim == PHIM_XUONG) {
            chon = (chon + 1) % soMuc;
            VeMenu(x, y, items, soMuc, chon);
        }
    }
}

// ============================================================================
// HÀM PHÍM
// ============================================================================

int DocPhim() {
    int c = getch();
    if (c == 0 || c == 224) {
        // Phím mở rộng
        c = getch();
        return c;
    }
    return c;
}

void ChoPhimBatKy() {
    gotoxy(2, 27);
    SetColor(MAU_XANH_LA_SANG);
    printf(" Nhan phim bat ky de tiep tuc...");
    SetColor(MAU_TRANG);
    getch();
}

int HienMenuCon(const char *tieuDe, const char *lc1, const char *lc2) {
    int x = 30, y = 10;
    VeKhungCoTieuDe(x, y, x + 50, y + 6, tieuDe);
    const char *items[2] = { lc1, lc2 };
    int chon = XuLyMenu(x + 4, y + 2, items, 2);
    return chon;
}

// ============================================================================
// THÔNG BÁO
// ============================================================================

void XoaDong(int y) {
    gotoxy(0, y);
    for (int i = 0; i < 120; i++) printf(" ");
    gotoxy(0, y);
}

void HienThongBaoLoi(const char *tb) {
    int x = wherex(), y = wherey();
    gotoxy(2, 27);
    SetColor(MAU_DO_SANG);
    printf(" [LOI] %s", tb);
    SetColor(MAU_TRANG);
    Sleep(2500);
    XoaDong(27);
    gotoxy(x, y);
}

void HienThongBaoThanhCong(const char *tb) {
    int x = wherex(), y = wherey();
    gotoxy(2, 27);
    SetColor(MAU_XANH_LA_SANG);
    printf(" [OK] %s", tb);
    SetColor(MAU_TRANG);
    Sleep(1500);
    XoaDong(27);
    gotoxy(x, y);
}

void HienThongBao(int x, int y, const char *tb, int mau) {
    gotoxy(x, y);
    SetColor(mau);
    printf("%s", tb);
    SetColor(MAU_TRANG);
}

// ============================================================================
// THIẾT LẬP CONSOLE
// ============================================================================

void ThietLapConsole() {
    SetColor(MAU_TRANG);
    SetBGColor(MAU_DEN);
    system("mode con: cols=120 lines=30");
    AnConTro();
}

void HienConTro() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 25;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(h, &info);
}

void AnConTro() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 25;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(h, &info);
}

void InCanGiua(const char *s, int doRong) {
    int len = strlen(s);
    int pad = (doRong - len) / 2;
    for (int i = 0; i < pad; i++) printf(" ");
    printf("%s", s);
    for (int i = 0; i < doRong - pad - len; i++) printf(" ");
}

// ============================================================================
// MÀN HÌNH CHÀO
// ============================================================================

void HienManHinhChao() {
    clrscr();
    SetColor(MAU_XANH_NHAT_SANG);
    VeKhungCoTieuDe(20, 5, 100, 22, "CHUONG TRINH QUAN LY DIEM SINH VIEN HE TIN CHI");
    SetColor(MAU_VANG_SANG);
    gotoxy(35, 9); printf("Do an mon: Cau truc Du lieu & Giai thuat");
    gotoxy(38, 11); printf("Khoa 2023+2024 - DHCQ CLC");
    gotoxy(35, 12); printf("Hoc ky 3 nam hoc 2025-2026");
    SetColor(MAU_TRANG);
    gotoxy(30, 16); printf("Sinh vien thuc hien:");
    SetColor(MAU_XANH_LA_SANG);
    gotoxy(32, 17); printf("Nguyen Viet Anh");
    SetColor(MAU_TRANG);
    gotoxy(30, 20); printf("Nhan phim bat ky de bat dau...");
    getch();
}

// ============================================================================
// MENU CHÍNH
// ============================================================================

int HienMenuChinh() {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "QUAN LY DIEM SINH VIEN HE TIN CHI - MENU CHINH");

    const char *items[8] = {
        "1. Quan ly Mon hoc                                    ",
        "2. Quan ly Lop                                        ",
        "3. Nhap sinh vien vao Lop                             ",
        "4. Quan ly Lop tin chi + Dang ky                      ",
        "5. Nhap diem / In bang diem                           ",
        "6. Luu du lieu vao file                               ",
        "7. Doc du lieu tu file                                ",
        "8. Thoat chuong trinh                                 "
    };

    gotoxy(30, 5);
    SetColor(MAU_VANG_SANG);
    printf("Su dung phim mui ten LEN/XUONG va ENTER de chon");
    SetColor(MAU_TRANG);

    int chon = XuLyMenu(30, 8, items, 8);
    return chon + 1;   // trả về 1..8
}

// ============================================================================
// PHÂN TRANG
// ============================================================================

int TinhTongTrang(int tongSoDong, int soDongMotTrang) {
    if (tongSoDong <= 0) return 1;
    return (tongSoDong + soDongMotTrang - 1) / soDongMotTrang;
}

void KhoiTaoPhanTrang(PhanTrang &pt, int tongSoDong, int soDongMotTrang) {
    pt.tongSoDong = tongSoDong;
    pt.soDongMotTrang = soDongMotTrang;
    pt.tongTrang = TinhTongTrang(tongSoDong, soDongMotTrang);
    pt.trangHienTai = 1;
}

bool TrangTruoc(PhanTrang &pt) {
    if (pt.trangHienTai > 1) { pt.trangHienTai--; return true; }
    return false;
}

bool TrangSau(PhanTrang &pt) {
    if (pt.trangHienTai < pt.tongTrang) { pt.trangHienTai++; return true; }
    return false;
}

int LayViTriBatDau(PhanTrang &pt) {
    return (pt.trangHienTai - 1) * pt.soDongMotTrang;
}

int LayViTriKetThuc(PhanTrang &pt) {
    int cuoi = pt.trangHienTai * pt.soDongMotTrang - 1;
    if (cuoi >= pt.tongSoDong) cuoi = pt.tongSoDong - 1;
    return cuoi;
}

void HienThiThongTinTrang(PhanTrang &pt, int x, int y) {
    gotoxy(x, y);
    SetColor(MAU_VANG_SANG);
    printf(" Trang %d/%d  (Tong %d ban ghi)  [F5]=Truoc  [F6]=Sau  [ESC]=Thoat ",
           pt.trangHienTai, pt.tongTrang, pt.tongSoDong);
    SetColor(MAU_TRANG);
}

// ============================================================================
// GIAO DIỆN GIẢNG VIÊN
// ============================================================================

void VeNutGiangVien(int x, int y, int width, int height, const char *label, bool isHover) {
    if (isHover) {
        SetBGColor(MAU_DO);
        SetColor(MAU_TRANG_SANG);
    } else {
        SetBGColor(MAU_XANH_DUONG);
        SetColor(MAU_TRANG_SANG);
    }

    int len = (int)strlen(label);
    int padLeft = (width - len) / 2;
    int padRight = width - len - padLeft;

    for (int h = 0; h < height; h++) {
        gotoxy(x, y + h);
        if (h == height / 2) {
            for (int i = 0; i < padLeft; i++) printf(" ");
            printf("%s", label);
            for (int i = 0; i < padRight; i++) printf(" ");
        } else {
            for (int i = 0; i < width; i++) printf(" ");
        }
    }

    SetBGColor(MAU_DEN);
    SetColor(MAU_TRANG);
}

void VeAsciiArtGiangVien(int x, int y) {
    SetColor(MAU_VANG_SANG);
    SetBGColor(MAU_DEN);
    const char *art[5] = {
        "  ____   ___     _     ___   __   _____ _____ _   _ ",
        " / ___| |_ _|   / \\   / _ \\  \\ \\ / / |_ _| ____| \\ | |",
        "| |  _   | |   / _ \\ | | | |  \\ V /   | | |  _| |  \\| |",
        "| |_| |  | |  / ___ \\| |_| |   | |    | | | |___| |\\  |",
        " \\____| |___|/_/   \\_\\___/     |_|   |___|_____|_| \\_|"
    };
    for (int i = 0; i < 5; i++) {
        gotoxy(x, y + i);
        printf("%s", art[i]);
    }
    SetColor(MAU_TRANG);
}

void VeFooterGiangVien(int y) {
    VeDuongNgang(2, 117, y - 1);
    gotoxy(2, y);
    SetColor(MAU_VANG_SANG);
    SetBGColor(MAU_DEN);
    printf("ENTER: Chon   ESC: Thoat   UP/DOWN: Len Xuong");
    SetColor(MAU_TRANG);
}

void VeMenuGiangVien(int viTriChon) {
    const char *items[5] = { "Lop", "LopTinChi", "MonHoc", "SinhVien", "Diem" };
    int width = 28;
    int height = 3;
    int x = 8;
    int startY = 4;
    int spacing = 4;

    for (int i = 0; i < 5; i++) {
        VeNutGiangVien(x, startY + i * spacing, width, height, items[i], i == viTriChon);
    }
}

int XuLyPhimGiangVien(int &viTriChon) {
    int phim = DocPhim();
    if (phim == PHIM_LEN) {
        viTriChon = (viTriChon - 1 + 5) % 5;
    } else if (phim == PHIM_XUONG) {
        viTriChon = (viTriChon + 1) % 5;
    }
    return phim;
}

int XuLyMenuGiangVien() {
    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "GIAO DIEN GIANG VIEN");
    
    int viTriChon = 0;
    VeMenuGiangVien(viTriChon);
    VeAsciiArtGiangVien(50, 10);
    VeFooterGiangVien(27);

    while (true) {
        int phim = XuLyPhimGiangVien(viTriChon);
        if (phim == PHIM_ESC) {
            return -1;
        }
        if (phim == PHIM_ENTER) {
            return viTriChon;
        }
        if (phim == PHIM_LEN || phim == PHIM_XUONG) {
            VeMenuGiangVien(viTriChon);
        }
    }
}

// ============================================================================
// GIAO DIỆN QUẢN LÝ MÔN HỌC
// ============================================================================

void VeMenuMonHoc(int x, int y, int viTriChon) {
    const char *items[4] = {
        " 1. Them mon hoc                 ",
        " 2. Sua thong tin mon hoc        ",
        " 3. Xoa mon hoc                  ",
        " 4. In danh sach mon hoc         "
    };

    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i * 2);
        if (i == viTriChon) {
            SetBGColor(MAU_DO);
            SetColor(MAU_TRANG_SANG);
        } else {
            SetBGColor(MAU_XANH_DUONG);
            SetColor(MAU_TRANG_SANG);
        }
        printf("%s", items[i]);
    }
    SetBGColor(MAU_DEN);
    SetColor(MAU_TRANG);
}

int XuLyMenuMonHoc() {
    clrscr();
    VeKhungCoTieuDe(35, 6, 85, 21, "MENU QUAN LY MON HOC");
    
    gotoxy(37, 19);
    SetColor(MAU_VANG_SANG);
    printf("UP/DOWN: Chon  ENTER: Dong y  ESC: Quay lai");
    SetColor(MAU_TRANG);

    int viTriChon = 0;
    int soMuc = 4;
    VeMenuMonHoc(40, 9, viTriChon);

    while (true) {
        int phim = DocPhim();
        if (phim == PHIM_ESC) {
            return -1;
        }
        if (phim == PHIM_ENTER) {
            return viTriChon;
        }
        if (phim == PHIM_LEN) {
            viTriChon = (viTriChon - 1 + soMuc) % soMuc;
            VeMenuMonHoc(40, 9, viTriChon);
        }
        else if (phim == PHIM_XUONG) {
            viTriChon = (viTriChon + 1) % soMuc;
            VeMenuMonHoc(40, 9, viTriChon);
        }
    }
}

void MenuMonHoc(treeMH &root, DS_LTC &ds_ltc) {
    while (true) {
        int chon = XuLyMenuMonHoc();
        if (chon == -1) {
            break;
        }
        switch (chon) {
            case 0: { // Thêm môn học
                clrscr();
                VeKhungCoTieuDe(2, 1, 118, 28, "THEM MON HOC MOI");
                Monhoc mh;

                gotoxy(6, 5); printf("Ma mon hoc (toi da 10 ky tu, ENTER de huy): ");
                HienConTro();
                NhapMa(mh.MAMH, 10, true);
                AnConTro();
                if (!mh.MAMH.empty()) {
                    if (Search(root, mh.MAMH) != NULL) {
                        HienThongBaoLoi("Ma mon hoc da ton tai!");
                    } else {
                        gotoxy(6, 7); printf("Ten mon hoc (toi da 50 ky tu): ");
                        HienConTro();
                        NhapChuoi(mh.TENMH, 50);
                        if (!mh.TENMH.empty()) {
                            gotoxy(6, 9); printf("So tin chi ly thuyet: ");
                            NhapSo(mh.STC_LT);
                            gotoxy(6, 11); printf("So tin chi thuc hanh: ");
                            NhapSo(mh.STC_TH);
                            AnConTro();

                            if (mh.STC_LT + mh.STC_TH == 0) {
                                HienThongBaoLoi("Tong so tin chi phai > 0!");
                            } else {
                                bool ok;
                                root = ThemMH(root, mh, ok);
                                if (ok) {
                                    CanBangCay(root);
                                    HienThongBaoThanhCong("Da them mon hoc va can bang cay thanh cong!");
                                } else {
                                    HienThongBaoLoi("Loi: Them mon hoc that bai.");
                                }
                            }
                        } else {
                            AnConTro();
                        }
                    }
                }
                gotoxy(6, 25);
                SetColor(MAU_TRANG_SANG);
                printf("Nhan phim bat ky de tiep tuc...");
                getch();
                break;
            }
            case 1: { // Sửa thông tin môn học
                clrscr();
                VeKhungCoTieuDe(2, 1, 118, 28, "SUA THONG TIN MON HOC");
                string mamh;

                gotoxy(6, 5); printf("Nhap ma mon hoc can sua (ENTER de huy): ");
                HienConTro();
                NhapMa(mamh, 10, true);
                AnConTro();
                if (!mamh.empty()) {
                    treeMH p = Search(root, mamh);
                    if (p == NULL) {
                        HienThongBaoLoi("Khong tim thay ma mon hoc!");
                    } else {
                        gotoxy(6, 7); printf("Thong tin hien tai:");
                        gotoxy(6, 8); printf("  Ten:   %s", p->mh.TENMH.c_str());
                        gotoxy(6, 9); printf("  STC:   %d (LT) + %d (TH)", p->mh.STC_LT, p->mh.STC_TH);

                        string tenMoi;
                        int lt = 0, th = 0;
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
                        } else {
                            if (HieuChinhMH(root, mamh, tenMoi, lt, th)) {
                                HienThongBaoThanhCong("Da cap nhat thong tin mon hoc!");
                            } else {
                                HienThongBaoLoi("Cap nhat that bai!");
                            }
                        }
                    }
                }
                gotoxy(6, 25);
                SetColor(MAU_TRANG_SANG);
                printf("Nhan phim bat ky de tiep tuc...");
                getch();
                break;
            }
            case 2: { // Xóa môn học
                clrscr();
                VeKhungCoTieuDe(2, 1, 118, 28, "XOA MON HOC");
                string mamh;

                gotoxy(6, 5); printf("Nhap ma mon hoc can xoa (ENTER de huy): ");
                HienConTro();
                NhapMa(mamh, 10, true);
                AnConTro();
                if (!mamh.empty()) {
                    treeMH p = Search(root, mamh);
                    if (p == NULL) {
                        HienThongBaoLoi("Khong tim thay ma mon hoc!");
                    } else {
                        gotoxy(6, 7); printf("Ten mon hoc: %s", p->mh.TENMH.c_str());
                        gotoxy(6, 8); printf("So TC: %d (LT) + %d (TH)", p->mh.STC_LT, p->mh.STC_TH);

                        if (MonHocDangDuocDung(ds_ltc, mamh)) {
                            HienThongBaoLoi("Khong the xoa: Mon hoc dang duoc su dung boi 1 Lop tin chi!");
                        } else {
                            gotoxy(6, 10);
                            HienConTro();
                            if (HienMenuCon("XAC NHAN XOA MON HOC", "Dong y Xoa", "Huy bo") == 0) {
                                bool ok;
                                root = XoaMH(root, mamh, ok);
                                if (ok) {
                                    CanBangCay(root);
                                    HienThongBaoThanhCong("Da xoa mon hoc va can bang cay thanh cong!");
                                } else {
                                    HienThongBaoLoi("Xoa that bai!");
                                }
                            }
                            AnConTro();
                        }
                    }
                }
                gotoxy(6, 25);
                SetColor(MAU_TRANG_SANG);
                printf("Nhan phim bat ky de tiep tuc...");
                getch();
                break;
            }
            case 3: { // In danh sách môn học
                clrscr();
                InDSMHTheoTen(root);
                gotoxy(4, 26);
                SetColor(MAU_TRANG_SANG);
                printf("Nhan phim bat ky de tiep tuc...");
                getch();
                break;
            }
        }
    }
}

// ============================================================================
// GIAO DIỆN QUẢN LÝ LỚP TÍN CHỈ
// ============================================================================

void VeMenuLopTinChi(int x, int y, int viTriChon) {
    const char *items[5] = {
        " 1. Them lop tin chi moi             ",
        " 2. Sua thong tin lop tin chi        ",
        " 3. Xoa lop tin chi                  ",
        " 4. In danh sach lop tin chi         ",
        " 5. Huy lop tin chi tu dong          "
    };

    for (int i = 0; i < 5; i++) {
        gotoxy(x, y + i * 2);
        if (i == viTriChon) {
            SetBGColor(MAU_DO);
            SetColor(MAU_TRANG_SANG);
        } else {
            SetBGColor(MAU_XANH_DUONG);
            SetColor(MAU_TRANG_SANG);
        }
        printf("%s", items[i]);
    }
    SetBGColor(MAU_DEN);
    SetColor(MAU_TRANG);
}

int XuLyMenuLopTinChi() {
    clrscr();
    VeKhungCoTieuDe(35, 6, 85, 21, "MENU QUAN LY LOP TIN CHI");
    
    gotoxy(37, 19);
    SetColor(MAU_VANG_SANG);
    printf("UP/DOWN: Chon  ENTER: Dong y  ESC: Quay lai");
    SetColor(MAU_TRANG);

    int viTriChon = 0;
    int soMuc = 5;
    VeMenuLopTinChi(40, 8, viTriChon);

    while (true) {
        int phim = DocPhim();
        if (phim == PHIM_ESC) {
            return -1;
        }
        if (phim == PHIM_ENTER) {
            return viTriChon;
        }
        if (phim == PHIM_LEN) {
            viTriChon = (viTriChon - 1 + soMuc) % soMuc;
            VeMenuLopTinChi(40, 8, viTriChon);
        }
        else if (phim == PHIM_XUONG) {
            viTriChon = (viTriChon + 1) % soMuc;
            VeMenuLopTinChi(40, 8, viTriChon);
        }
    }
}

void MenuLopTinChi(DS_LTC &ds_ltc, treeMH root) {
    while (true) {
        int chon = XuLyMenuLopTinChi();
        if (chon == -1) {
            break;
        }
        switch (chon) {
            case 0:
                FormThemLTC(ds_ltc, root);
                break;
            case 1:
                FormHieuChinhLTC(ds_ltc, root);
                break;
            case 2:
                FormXoaLTC(ds_ltc);
                break;
            case 3:
                InDSLopTinChi(ds_ltc, root);
                break;
            case 4:
                HuyLopTinChiTuDong(ds_ltc);
                break;
        }
        gotoxy(4, 26);
        SetColor(MAU_TRANG_SANG);
        printf("Nhan phim bat ky de tiep tuc...");
        getch();
    }
}

// ============================================================================
// GIAO DIỆN QUẢN LÝ SINH VIÊN
// ============================================================================

void VeMenuSinhVien(int x, int y, int viTriChon) {
    const char *items[4] = {
        " 1. Them sinh vien moi               ",
        " 2. Sua thong tin sinh vien          ",
        " 3. Xoa sinh vien                    ",
        " 4. In danh sach sinh vien Alphabet  "
    };

    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i * 2);
        if (i == viTriChon) {
            SetBGColor(MAU_DO);
            SetColor(MAU_TRANG_SANG);
        } else {
            SetBGColor(MAU_XANH_DUONG);
            SetColor(MAU_TRANG_SANG);
        }
        printf("%s", items[i]);
    }
    SetBGColor(MAU_DEN);
    SetColor(MAU_TRANG);
}

int XuLyMenuSinhVien() {
    clrscr();
    VeKhungCoTieuDe(35, 6, 85, 21, "MENU QUAN LY SINH VIEN");
    
    gotoxy(37, 19);
    SetColor(MAU_VANG_SANG);
    printf("UP/DOWN: Chon  ENTER: Dong y  ESC: Quay lai");
    SetColor(MAU_TRANG);

    int viTriChon = 0;
    int soMuc = 4;
    VeMenuSinhVien(40, 9, viTriChon);

    while (true) {
        int phim = DocPhim();
        if (phim == PHIM_ESC) {
            return -1;
        }
        if (phim == PHIM_ENTER) {
            return viTriChon;
        }
        if (phim == PHIM_LEN) {
            viTriChon = (viTriChon - 1 + soMuc) % soMuc;
            VeMenuSinhVien(40, 9, viTriChon);
        }
        else if (phim == PHIM_XUONG) {
            viTriChon = (viTriChon + 1) % soMuc;
            VeMenuSinhVien(40, 9, viTriChon);
        }
    }
}

void MenuSinhVien(DS_LOP &ds_lop) {
    while (true) {
        int chon = XuLyMenuSinhVien();
        if (chon == -1) {
            break;
        }

        // Nhập Mã lớp để chọn Lớp cần xử lý
        clrscr();
        VeKhungCoTieuDe(30, 8, 90, 16, "CHON LOP HOC");
        gotoxy(35, 11);
        printf("Nhap MA LOP: ");
        string malop;
        NhapMa(malop, 15);
        ToUpper(malop);

        if (malop.empty()) {
            continue;
        }

        Lop *lop = LayLop(ds_lop, malop);
        if (lop == NULL) {
            HienThongBaoLoi("Ma lop khong ton tai trong he thong!");
            gotoxy(4, 26);
            SetColor(MAU_TRANG_SANG);
            printf("Nhan phim bat ky de tiep tuc...");
            getch();
            continue;
        }

        switch (chon) {
            case 0: // Thêm sinh viên mới
                NhapSinhVienChoLop(*lop, 1);
                break;
            case 1: // Sửa thông tin sinh viên
                NhapSinhVienChoLop(*lop, 3);
                break;
            case 2: // Xóa sinh viên
                NhapSinhVienChoLop(*lop, 2);
                break;
            case 3: // In danh sách sinh viên Alphabet
                InDanhSachSinhVienAlphabet(*lop);
                break;
        }

        gotoxy(4, 26);
        SetColor(MAU_TRANG_SANG);
        printf("Nhan phim bat ky de tiep tuc...");
        getch();
    }
}

// ============================================================================
// GIAO DIỆN QUẢN LÝ LỚP HÀNH CHÍNH
// ============================================================================

void VeMenuLop(int x, int y, int viTriChon) {
    const char *items[3] = {
        " 1. Them lop moi                          ",
        " 2. In danh sach cac lop dang co          ",
        " 3. In danh sach sinh vien cua mot lop    "
    };

    for (int i = 0; i < 3; i++) {
        gotoxy(x, y + i * 2);
        if (i == viTriChon) {
            SetBGColor(MAU_DO);
            SetColor(MAU_TRANG_SANG);
        } else {
            SetBGColor(MAU_XANH_DUONG);
            SetColor(MAU_TRANG_SANG);
        }
        printf("%s", items[i]);
    }
    SetBGColor(MAU_DEN);
    SetColor(MAU_TRANG);
}

int XuLyMenuLop() {
    clrscr();
    VeKhungCoTieuDe(35, 6, 85, 20, "MENU QUAN LY LOP HANG CHINH");
    
    gotoxy(37, 18);
    SetColor(MAU_VANG_SANG);
    printf("UP/DOWN: Chon  ENTER: Dong y  ESC: Quay lai");
    SetColor(MAU_TRANG);

    int viTriChon = 0;
    int soMuc = 3;
    VeMenuLop(40, 9, viTriChon);

    while (true) {
        int phim = DocPhim();
        if (phim == PHIM_ESC) {
            return -1;
        }
        if (phim == PHIM_ENTER) {
            return viTriChon;
        }
        if (phim == PHIM_LEN) {
            viTriChon = (viTriChon - 1 + soMuc) % soMuc;
            VeMenuLop(40, 9, viTriChon);
        }
        else if (phim == PHIM_XUONG) {
            viTriChon = (viTriChon + 1) % soMuc;
            VeMenuLop(40, 9, viTriChon);
        }
    }
}

void MenuLop(DS_LOP &ds_lop) {
    while (true) {
        int chon = XuLyMenuLop();
        if (chon == -1) {
            break;
        }
        switch (chon) {
            case 0: { // Thêm lớp mới
                clrscr();
                VeKhungCoTieuDe(2, 1, 118, 28, "THEM LOP HOC MOI");
                Lop lop;
                lop.dssv = NULL;

                gotoxy(6, 5);
                printf("Nhap Ma lop (VD: DXXYYYY0X) (ENTER de huy): ");
                HienConTro();
                NhapMa(lop.MALOP, 15, true);
                AnConTro();

                if (!lop.MALOP.empty()) {
                    if (TimLopTheoMa(ds_lop, lop.MALOP) != -1) {
                        HienThongBaoLoi("Ma lop nay da ton tai trong he thong!");
                    } else {
                        gotoxy(6, 7);
                        printf("Ten lop: ");
                        HienConTro();
                        NhapChuoi(lop.TENLOP, 40);
                        AnConTro();

                        if (!lop.TENLOP.empty()) {
                            if (ThemLop(ds_lop, lop)) {
                                HienThongBaoThanhCong("Them lop hoc moi thanh cong!");
                            } else {
                                HienThongBaoLoi("Danh sach lop da day hoac loi khong the them!");
                            }
                        }
                    }
                }
                gotoxy(4, 26);
                SetColor(MAU_TRANG_SANG);
                printf("Nhan phim bat ky de tiep tuc...");
                getch();
                break;
            }
            case 1: { // In danh sách các lớp đang có
                InDSLop(ds_lop);
                gotoxy(4, 26);
                SetColor(MAU_TRANG_SANG);
                printf("Nhan phim bat ky de tiep tuc...");
                getch();
                break;
            }
            case 2: { // In danh sách sinh viên của một lớp
                clrscr();
                VeKhungCoTieuDe(30, 8, 90, 16, "IN DANH SACH SINH VIEN CUA LOP");
                gotoxy(35, 11);
                printf("Nhap Ma lop (VD: DXXYYYY0X) (X:so,Y:Chu cai): ");
                string malop;
                HienConTro();
                NhapMa(malop, 15, true);
                AnConTro();
                ToUpper(malop);

                if (!malop.empty()) {
                    Lop *pLop = LayLop(ds_lop, malop);
                    if (pLop == NULL) {
                        HienThongBaoLoi("Ma lop khong ton tai trong he thong!");
                    } else {
                        InDanhSachSinhVienAlphabet(*pLop);
                    }
                }
                gotoxy(4, 26);
                SetColor(MAU_TRANG_SANG);
                printf("Nhan phim bat ky de tiep tuc...");
                getch();
                break;
            }
        }
    }
}

void HienMenuGiangVien(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH &root) {
    const char *items[5] = { "Lop", "LopTinChi", "MonHoc", "SinhVien", "Diem" };
    while (true) {
        int chon = XuLyMenuGiangVien();
        if (chon == -1) {
            break;
        }
        if (chon == 0) {
            // Mục "Lop" -> Hiển thị Menu con Lớp Hành Chính
            MenuLop(ds_lop);
        } else if (chon == 1) {
            // Mục "LopTinChi" -> Hiển thị Menu con Lớp Tín Chỉ
            MenuLopTinChi(ds_ltc, root);
        } else if (chon == 2) {
            // Mục "MonHoc" -> Hiển thị Menu con Môn Học
            MenuMonHoc(root, ds_ltc);
        } else if (chon == 3) {
            // Mục "SinhVien" -> Hiển thị Menu con Quản Lý Sinh Viên
            MenuSinhVien(ds_lop);
        } else if (chon == 4) {
            // Mục "Diem" -> Hiển thị Menu con Quản Lý Điểm
            MenuQuanLyDiem(ds_ltc, ds_lop, root);
        }
    }
}



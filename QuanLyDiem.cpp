#include "QuanLyDiem.h"
#include <iomanip>

static Loptinchi* TimLTCTheoThongTin(DS_LTC &ds, const string &nienkhoa, int hocky, const string &mamh, int nhom) {
    string nk = nienkhoa;
    string mh = mamh;
    ToUpper(nk);
    ToUpper(mh);

    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] == NULL) continue;

        string curNK = ds.nodes[i]->NIENKHOA;
        string curMH = ds.nodes[i]->MAMH;
        ToUpper(curNK);
        ToUpper(curMH);

        if (curNK == nk && ds.nodes[i]->HOCKY == hocky && curMH == mh && ds.nodes[i]->NHOM == nhom) {
            return ds.nodes[i];
        }
    }
    return NULL;
}



// Hàm so sánh Sinh viên theo TÊN rồi tới HỌ (theo chuẩn tài liệu giảng viên)
static bool SoSanhSV(PTRSV sv1, PTRSV sv2) {
    if (sv1 == NULL) return false;
    if (sv2 == NULL) return true;
    int cmpTen = StrCmpNoCase(sv1->sv.TEN, sv2->sv.TEN);
    if (cmpTen < 0) return true;
    if (cmpTen > 0) return false;
    return StrCmpNoCase(sv1->sv.HO, sv2->sv.HO) < 0;
}

// Thuật toán sắp xếp danh sách đăng ký theo Tên + Họ sinh viên
static void SapXepDSRegistered(PTRDK *active, int total, DS_LOP &ds_lop) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            PTRSV sv1 = LaySinhVien(ds_lop, active[i]->dk.MASV);
            PTRSV sv2 = LaySinhVien(ds_lop, active[j]->dk.MASV);
            if (!SoSanhSV(sv1, sv2)) {
                PTRDK temp = active[i];
                active[i] = active[j];
                active[j] = temp;
            }
        }
    }
}

// ============================================================================
// CHỨC NĂNG QUẢN LÝ ĐIỂM
// ============================================================================

void NhapDiemLTC(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root) {
    clrscr();
    VeKhungCoTieuDe(30, 7, 90, 21, "NHAP / CAP NHAT DIEM LOP TIN CHI");

    string nienkhoa;
    int hocky = 0;
    int nhom = 0;
    string mamh;

    gotoxy(35, 10);
    printf("Nien khoa (vd: 2025-2026): ");
    NhapMa(nienkhoa, 15);
    ToUpper(nienkhoa);
    if (nienkhoa.empty()) return;

    gotoxy(35, 12);
    printf("Hoc ky (1-3)            : ");
    NhapSo(hocky);

    gotoxy(35, 14);
    printf("Nhom                    : ");
    NhapSo(nhom);

    gotoxy(35, 16);
    printf("Ma mon hoc              : ");
    NhapMa(mamh, 15);
    ToUpper(mamh);
    if (mamh.empty()) return;

    Loptinchi *ltc = TimLTCTheoThongTin(ds_ltc, nienkhoa, hocky, mamh, nhom);
    if (ltc == NULL) {
        HienThongBaoLoi("Khong tim thay Lop tin chi voi thong tin tren!");
        ChoPhimBatKy();
        return;
    }

    int total = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
        if (!p->dk.HUYDK) total++;
    }

    if (total == 0) {
        HienThongBaoLoi("Lop tin chi nay chua co sinh vien dang ky!");
        ChoPhimBatKy();
        return;
    }

    PTRDK *active = new PTRDK[total];
    int idx = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
        if (!p->dk.HUYDK) {
            active[idx++] = p;
        }
    }

    // Sắp xếp danh sách SV theo Tên + Họ theo chuẩn tài liệu giảng viên
    SapXepDSRegistered(active, total, ds_lop);

    PhanTrang pt;
    KhoiTaoPhanTrang(pt, total, 10);
    int selectedIndex = 0;

    while (true) {
        int start = LayViTriBatDau(pt);
        int end = LayViTriKetThuc(pt);

        if (selectedIndex < start) selectedIndex = start;
        if (selectedIndex > end) selectedIndex = end;

        clrscr();
        VeKhungCoTieuDe(2, 1, 118, 28, "CAP NHAT DIEM LOP TIN CHI");

        treeMH nodeMH = Search(root, ltc->MAMH);
        string tenMon = (nodeMH != NULL) ? nodeMH->mh.TENMH : "";
        gotoxy(4, 3);
        SetColor(MAU_XANH_LA_SANG);
        printf("Mon: %s (%s) | NK: %s | HK: %d | Nhom: %d",
               tenMon.c_str(), ltc->MAMH.c_str(), ltc->NIENKHOA.c_str(), ltc->HOCKY, ltc->NHOM);
        SetColor(MAU_TRANG);

        gotoxy(4, 5);
        SetColor(MAU_XANH_NHAT_SANG);
        printf("%-5s | %-12s | %-45s | %-10s", "STT", "MASV", "HO TEN SINH VIEN", "DIEM");
        SetColor(MAU_TRANG);

        VeDuongNgang(4, 115, 6);

        int y = 7;
        for (int i = start; i <= end; i++) {
            PTRDK p = active[i];
            PTRSV sv = LaySinhVien(ds_lop, p->dk.MASV);
            string hoTen = (sv != NULL) ? (sv->sv.HO + " " + sv->sv.TEN) : "--";

            char diemStr[20];
            if (p->dk.DIEM < 0) {
                sprintf(diemStr, "Chua nhap");
            } else {
                sprintf(diemStr, "%.1f", p->dk.DIEM);
            }

            gotoxy(4, y);
            if (i == selectedIndex) {
                SetBGColor(MAU_DO);
                SetColor(MAU_TRANG_SANG);
            } else {
                SetBGColor(MAU_DEN);
                SetColor(MAU_TRANG);
            }

            printf("%-5d | %-12s | %-45s | %-10s",
                   i + 1, p->dk.MASV.c_str(), hoTen.c_str(), diemStr);

            SetBGColor(MAU_DEN);
            SetColor(MAU_TRANG);
            y++;
        }

        gotoxy(2, 26);
        SetColor(MAU_VANG_SANG);
        printf("UP/DOWN: Chon SV   ENTER: Nhap/Sua diem   LEFT/RIGHT: Chuyen trang   ESC: Hoan tat");
        SetColor(MAU_TRANG);

        gotoxy(2, 27);
        printf("Trang %d/%d (Tong %d SV)", pt.trangHienTai, pt.tongTrang, total);

        int key = DocPhim();
        if (key == PHIM_ESC) {
            break;
        } else if (key == PHIM_TRAI) {
            TrangTruoc(pt);
        } else if (key == PHIM_PHAI) {
            TrangSau(pt);
        } else if (key == PHIM_LEN) {
            if (selectedIndex > 0) {
                selectedIndex--;
                if (selectedIndex < LayViTriBatDau(pt)) TrangTruoc(pt);
            }
        } else if (key == PHIM_XUONG) {
            if (selectedIndex < total - 1) {
                selectedIndex++;
                if (selectedIndex > LayViTriKetThuc(pt)) TrangSau(pt);
            }
        } else if (key == PHIM_ENTER) {
            int startPage = LayViTriBatDau(pt);
            int yPos = 7 + (selectedIndex - startPage);
            
            gotoxy(72, yPos);
            SetColor(MAU_VANG_SANG);
            SetBGColor(MAU_DO);
            printf(" Nhap (0-10): ");
            
            HienConTro();
            float newDiem = (active[selectedIndex]->dk.DIEM >= 0) ? active[selectedIndex]->dk.DIEM : 0.0f;
            NhapDiem(newDiem);
            AnConTro();

            if (newDiem >= 0.0f && newDiem <= 10.0f) {
                active[selectedIndex]->dk.DIEM = newDiem;
                HienThongBaoThanhCong("Da cap nhat diem!");
            } else {
                HienThongBaoLoi("Diem khong hop le! (Phai tu 0.0 den 10.0)");
            }

            if (selectedIndex < total - 1) {
                selectedIndex++;
                if (selectedIndex > LayViTriKetThuc(pt)) TrangSau(pt);
            }
        }
    }

    delete [] active;
}

void InBangDiemLTC(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root) {
    clrscr();
    VeKhungCoTieuDe(30, 7, 90, 21, "IN BANG DIEM LOP TIN CHI");

    string nienkhoa;
    int hocky = 0;
    int nhom = 0;
    string mamh;

    gotoxy(35, 10);
    printf("Nien khoa (vd: 2025-2026): ");
    NhapMa(nienkhoa, 15);
    ToUpper(nienkhoa);
    if (nienkhoa.empty()) return;

    gotoxy(35, 12);
    printf("Hoc ky (1-3)            : ");
    NhapSo(hocky);

    gotoxy(35, 14);
    printf("Nhom                    : ");
    NhapSo(nhom);

    gotoxy(35, 16);
    printf("Ma mon hoc              : ");
    NhapMa(mamh, 15);
    ToUpper(mamh);
    if (mamh.empty()) return;

    Loptinchi *ltc = TimLTCTheoThongTin(ds_ltc, nienkhoa, hocky, mamh, nhom);
    if (ltc == NULL) {
        HienThongBaoLoi("Khong tim thay Lop tin chi!");
        ChoPhimBatKy();
        return;
    }

    int total = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
        if (!p->dk.HUYDK) total++;
    }

    if (total == 0) {
        HienThongBaoLoi("Lop tin chi nay chua co sinh vien dang ky!");
        ChoPhimBatKy();
        return;
    }

    PTRDK *active = new PTRDK[total];
    int idx = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
        if (!p->dk.HUYDK) {
            active[idx++] = p;
        }
    }

    // Sắp xếp danh sách SV theo Tên + Họ theo chuẩn tài liệu giảng viên
    SapXepDSRegistered(active, total, ds_lop);

    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "BANG DIEM LOP TIN CHI");

    treeMH nodeMH = Search(root, ltc->MAMH);
    string tenMon = (nodeMH != NULL) ? nodeMH->mh.TENMH : "";
    gotoxy(4, 3);
    SetColor(MAU_XANH_LA_SANG);
    printf("Mon: %s (%s) | NK: %s | HK: %d | Nhom: %d",
           tenMon.c_str(), ltc->MAMH.c_str(), ltc->NIENKHOA.c_str(), ltc->HOCKY, ltc->NHOM);
    SetColor(MAU_TRANG);

    gotoxy(4, 5);
    SetColor(MAU_XANH_NHAT_SANG);
    printf("%-5s | %-12s | %-25s | %-15s | %-8s", "STT", "MASV", "HO", "TEN", "DIEM");
    SetColor(MAU_TRANG);

    VeDuongNgang(4, 115, 6);

    int y = 7;
    for (int i = 0; i < total; i++) {
        PTRDK p = active[i];
        PTRSV sv = LaySinhVien(ds_lop, p->dk.MASV);
        string ho = (sv != NULL) ? sv->sv.HO : "--";
        string ten = (sv != NULL) ? sv->sv.TEN : "--";

        char diemStr[20];
        if (p->dk.DIEM < 0) {
            sprintf(diemStr, "Chua nhap");
        } else {
            sprintf(diemStr, "%.1f", p->dk.DIEM);
        }

        gotoxy(4, y + i);
        printf("%-5d | %-12s | %-25s | %-15s | %-8s",
               i + 1, p->dk.MASV.c_str(), ho.c_str(), ten.c_str(), diemStr);
    }

    delete [] active;
    ChoPhimBatKy();
}

// Helper: Lấy điểm thi cao nhất của 1 SV đối với 1 môn học cụ thể
static float LayDiemCaoNhatMonHoc(DS_LTC &ds_ltc, const string &masv, const string &mamh) {
    float maxDiem = -1.0f;
    for (int i = 0; i < ds_ltc.n; i++) {
        Loptinchi *ltc = ds_ltc.nodes[i];
        if (ltc == NULL || ltc->HUYLOP) continue;
        if (ltc->MAMH != mamh) continue;

        for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
            if (p->dk.MASV == masv && !p->dk.HUYDK && p->dk.DIEM >= 0.0f) {
                if (p->dk.DIEM > maxDiem) {
                    maxDiem = p->dk.DIEM;
                }
            }
        }
    }
    return maxDiem;
}

// Helper: Tính điểm trung bình tích lũy theo số tín chỉ của 1 sinh viên
static float TinhDiemTBKhoaHoc(DS_LTC &ds_ltc, treeMH root, const string &masv) {
    int maxLTC = ds_ltc.n;
    if (maxLTC <= 0) return -1.0f;

    Dangky *dsMon = new Dangky[maxLTC];
    int countMon = 0;

    for (int i = 0; i < ds_ltc.n; i++) {
        Loptinchi *ltc = ds_ltc.nodes[i];
        if (ltc == NULL || ltc->HUYLOP) continue;

        for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
            if (p->dk.MASV == masv && !p->dk.HUYDK && p->dk.DIEM >= 0.0f) {
                int foundIdx = -1;
                for (int j = 0; j < countMon; j++) {
                    if (dsMon[j].MASV == ltc->MAMH) {
                        foundIdx = j;
                        break;
                    }
                }

                if (foundIdx != -1) {
                    if (p->dk.DIEM > dsMon[foundIdx].DIEM) {
                        dsMon[foundIdx].DIEM = p->dk.DIEM;
                    }
                } else {
                    dsMon[countMon].MASV = ltc->MAMH;
                    dsMon[countMon].DIEM = p->dk.DIEM;
                    countMon++;
                }
            }
        }
    }

    if (countMon == 0) {
        delete [] dsMon;
        return -1.0f;
    }

    float tongDiemXSTC = 0.0f;
    int tongTC = 0;

    for (int i = 0; i < countMon; i++) {
        treeMH node = Search(root, dsMon[i].MASV);
        if (node != NULL) {
            int stc = node->mh.STC_LT + node->mh.STC_TH;
            if (stc > 0) {
                tongDiemXSTC += dsMon[i].DIEM * stc;
                tongTC += stc;
            }
        }
    }

    delete [] dsMon;

    if (tongTC <= 0) return -1.0f;
    return tongDiemXSTC / tongTC;
}

// ============================================================================
// CHỨC NĂNG (k): IN ĐIỂM TRUNG BÌNH KẾT THÚC KHÓA HỌC CỦA 1 LỚP THEO TÍN CHỈ
// ============================================================================
void InBangDiemTrungBinhTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root) {
    clrscr();
    VeKhungCoTieuDe(30, 8, 90, 16, "THONG KE DIEM TRUNG BINH KHOA HOC");

    gotoxy(35, 11);
    printf("Nhap MA LOP: ");
    string malop;
    NhapMa(malop, 15);
    ToUpper(malop);
    if (malop.empty()) return;

    Lop *lop = LayLop(ds_lop, malop);
    if (lop == NULL) {
        HienThongBaoLoi("Khong tim thay Lop hoc voi ma nay!");
        ChoPhimBatKy();
        return;
    }

    if (lop->dssv == NULL) {
        HienThongBaoLoi("Lop hoc nay chua co sinh vien nao!");
        ChoPhimBatKy();
        return;
    }

    int countSV = DemSinhVien(lop->dssv);
    PTRSV *students = new PTRSV[countSV];
    int idx = 0;
    for (PTRSV p = lop->dssv; p != NULL; p = p->next) {
        students[idx++] = p;
    }

    // Sắp xếp sinh viên theo Tên + Họ
    for (int i = 0; i < countSV - 1; i++) {
        for (int j = i + 1; j < countSV; j++) {
            if (!SoSanhSV(students[i], students[j])) {
                PTRSV temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "BANG THONG KE DIEM TRUNG BINH KHOA HOC");

    gotoxy(4, 3);
    SetColor(MAU_XANH_LA_SANG);
    printf("Lop: %-15s  Ten lop: %s  (Si so: %d)",
           lop->MALOP.c_str(), lop->TENLOP.c_str(), countSV);
    SetColor(MAU_TRANG);

    gotoxy(4, 5);
    SetColor(MAU_XANH_NHAT_SANG);
    printf("%-5s | %-12s | %-30s | %-15s | %-10s", "STT", "MASV", "HO", "TEN", "Diem TB");
    SetColor(MAU_TRANG);

    VeDuongNgang(4, 115, 6);

    int y = 7;
    for (int i = 0; i < countSV; i++) {
        float dtb = TinhDiemTBKhoaHoc(ds_ltc, root, students[i]->sv.MASV);
        char dtbStr[20];
        if (dtb < 0) {
            sprintf(dtbStr, "Chua co");
        } else {
            sprintf(dtbStr, "%.1f", dtb);
        }

        gotoxy(4, y + i);
        printf("%-5d | %-12s | %-30s | %-15s | %-10s",
               i + 1,
               students[i]->sv.MASV.c_str(),
               students[i]->sv.HO.c_str(),
               students[i]->sv.TEN.c_str(),
               dtbStr);
    }

    delete [] students;
    ChoPhimBatKy();
}

// ============================================================================
// CHỨC NĂNG (l): IN BẢNG ĐIỂM TỔNG KẾT CÁC MÔN CỦA CÁC SINH VIÊN THUỘC 1 LỚP
// ============================================================================
void InBangDiemTongKetTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root) {
    clrscr();
    VeKhungCoTieuDe(30, 8, 90, 16, "IN BANG DIEM TONG KET CAC MON CUA LOP");

    gotoxy(35, 11);
    printf("Nhap MA LOP: ");
    string malop;
    NhapMa(malop, 15);
    ToUpper(malop);
    if (malop.empty()) return;

    Lop *lop = LayLop(ds_lop, malop);
    if (lop == NULL) {
        HienThongBaoLoi("Khong tim thay Lop hoc voi ma nay!");
        ChoPhimBatKy();
        return;
    }

    if (lop->dssv == NULL) {
        HienThongBaoLoi("Lop hoc nay chua co sinh vien nao!");
        ChoPhimBatKy();
        return;
    }

    int countSV = DemSinhVien(lop->dssv);
    PTRSV *students = new PTRSV[countSV];
    int idx = 0;
    for (PTRSV p = lop->dssv; p != NULL; p = p->next) {
        students[idx++] = p;
    }

    // Sắp xếp sinh viên theo Tên + Họ
    for (int i = 0; i < countSV - 1; i++) {
        for (int j = i + 1; j < countSV; j++) {
            if (!SoSanhSV(students[i], students[j])) {
                PTRSV temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    // Thu thập tất cả các môn duy nhất mà các SV thuộc Lớp này đã học
    string *subjects = new string[1000];
    int countMH = 0;

    for (int i = 0; i < countSV; i++) {
        string masv = students[i]->sv.MASV;
        for (int k = 0; k < ds_ltc.n; k++) {
            Loptinchi *ltc = ds_ltc.nodes[k];
            if (ltc == NULL || ltc->HUYLOP) continue;

            for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
                if (p->dk.MASV == masv && !p->dk.HUYDK && p->dk.DIEM >= 0.0f) {
                    bool exists = false;
                    for (int m = 0; m < countMH; m++) {
                        if (subjects[m] == ltc->MAMH) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        subjects[countMH++] = ltc->MAMH;
                    }
                }
            }
        }
    }

    if (countMH == 0) {
        delete [] students;
        delete [] subjects;
        HienThongBaoLoi("Lop chua co sinh vien nao co diem mon hoc!");
        ChoPhimBatKy();
        return;
    }

    // Sắp xếp mã môn học tăng dần theo MAMH
    for (int i = 0; i < countMH - 1; i++) {
        for (int j = i + 1; j < countMH; j++) {
            if (subjects[i] > subjects[j]) {
                string temp = subjects[i];
                subjects[i] = subjects[j];
                subjects[j] = temp;
            }
        }
    }

    // Tạo ma trận điểm [countSV x countMH]
    float **matrix = new float*[countSV];
    for (int i = 0; i < countSV; i++) {
        matrix[i] = new float[countMH];
        for (int j = 0; j < countMH; j++) {
            matrix[i][j] = LayDiemCaoNhatMonHoc(ds_ltc, students[i]->sv.MASV, subjects[j]);
        }
    }

    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "BANG DIEM TONG KET CÁC MÔN HỌC");

    gotoxy(4, 3);
    SetColor(MAU_XANH_LA_SANG);
    printf("Lop: %-15s  Ten lop: %s", lop->MALOP.c_str(), lop->TENLOP.c_str());
    SetColor(MAU_TRANG);

    // In tiêu đề ma trận (cột)
    gotoxy(4, 5);
    SetColor(MAU_XANH_NHAT_SANG);
    printf("%-4s | %-10s | %-22s", "STT", "MASV", "HO TEN");
    for (int j = 0; j < countMH && j < 6; j++) {
        printf(" | %-8s", subjects[j].c_str());
    }
    SetColor(MAU_TRANG);

    VeDuongNgang(4, 115, 6);

    int y = 7;
    for (int i = 0; i < countSV; i++) {
        gotoxy(4, y + i);
        string hoTen = students[i]->sv.HO + " " + students[i]->sv.TEN;
        printf("%-4d | %-10s | %-22.22s",
               i + 1,
               students[i]->sv.MASV.c_str(),
               hoTen.c_str());

        for (int j = 0; j < countMH && j < 6; j++) {
            if (matrix[i][j] < 0) {
                printf(" | %-8s", "-");
            } else {
                printf(" | %-8.1f", matrix[i][j]);
            }
        }
    }

    // Giải phóng bộ nhớ
    for (int i = 0; i < countSV; i++) {
        delete [] matrix[i];
    }
    delete [] matrix;
    delete [] students;
    delete [] subjects;

    ChoPhimBatKy();
}

void VeMenuDiem(int x, int y, int viTriChon) {
    const char *items[5] = {
        "  Nhap / Cap nhat diem Lop tin chi            ",
        "  In Bang diem Lop tin chi                    ",
        "  In Bang diem trung binh khoa hoc ",
        "  In Bang diem tong ket cac mon   ",
        "  Quay lai Menu chinh                         "
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

int XuLyMenuDiem() {
    clrscr();
    VeKhungCoTieuDe(28, 6, 95, 21, "MENU QUAN LY DIEM");
    
    gotoxy(35, 19);
    SetColor(MAU_VANG_SANG);
    printf("UP/DOWN: Chon  ENTER: Dong y  ESC: Quay lai");
    SetColor(MAU_TRANG);

    int viTriChon = 0;
    int soMuc = 5;
    VeMenuDiem(30, 8, viTriChon);

    while (true) {
        int phim = DocPhim();
        if (phim == PHIM_ESC) {
            return 4;
        }
        if (phim == PHIM_ENTER) {
            return viTriChon;
        }
        if (phim == PHIM_LEN) {
            viTriChon = (viTriChon - 1 + soMuc) % soMuc;
            VeMenuDiem(30, 8, viTriChon);
        }
        else if (phim == PHIM_XUONG) {
            viTriChon = (viTriChon + 1) % soMuc;
            VeMenuDiem(30, 8, viTriChon);
        }
    }
}

void MenuQuanLyDiem(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root) {
    while (true) {
        int chon = XuLyMenuDiem();
        if (chon == 4 || chon == -1) {
            break;
        }
        switch (chon) {
            case 0:
                NhapDiemLTC(ds_ltc, ds_lop, root);
                break;
            case 1:
                InBangDiemLTC(ds_ltc, ds_lop, root);
                break;
            case 2:
                InBangDiemTrungBinhTheoLop(ds_ltc, ds_lop, root);
                break;
            case 3:
                InBangDiemTongKetTheoLop(ds_ltc, ds_lop, root);
                break;
        }
    }
}

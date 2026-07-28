#include "QuanLyDiem.h"
#include <iomanip>

static string TrimString(const string &s)
{
    int start = 0;
    int end = (int)s.size() - 1;
    while (start <= end && (s[start] == ' ' || s[start] == '\t')) start++;
    while (end >= start && (s[end] == ' ' || s[end] == '\t')) end--;
    if (start > end) return "";
    return s.substr(start, end - start + 1);
}

static string CatChuoi(const string &s, int maxLen)
{
    if ((int)s.size() <= maxLen)
        return s;
    if (maxLen <= 3)
        return s.substr(0, maxLen);
    return s.substr(0, maxLen - 3) + "...";
}

static Loptinchi* TimLTCTheoThongTin(DS_LTC &ds,
                                     const string &nienkhoa,
                                     int hocky,
                                     const string &mamh,
                                     int nhom)
{
    string nk = TrimString(nienkhoa);
    string mh = TrimString(mamh);
    ToUpper(nk);
    ToUpper(mh);

    for (int i = 0; i < ds.n; i++)
    {
        if (ds.nodes[i] == NULL)
            continue;

        string curNK = TrimString(ds.nodes[i]->NIENKHOA);
        string curMH = TrimString(ds.nodes[i]->MAMH);
        ToUpper(curNK);
        ToUpper(curMH);

        if (curNK == nk &&
            ds.nodes[i]->HOCKY == hocky &&
            curMH == mh &&
            ds.nodes[i]->NHOM == nhom)
        {
            return ds.nodes[i];
        }
    }

    return NULL;
}

static string TimTenMonHoc(treeMH root, const string &mamh)
{
    if (root == NULL)
        return "";

    if (root->mh.MAMH == mamh)
        return root->mh.TENMH;

    if (mamh < root->mh.MAMH)
        return TimTenMonHoc(root->left, mamh);

    return TimTenMonHoc(root->right, mamh);
}

static string ChuyenDiemSangChuoi(float diem)
{
    char buf[20];
    if (diem < 0)
        return " ";
    sprintf(buf, "%.1f", diem);
    return string(buf);
}

static void VeKhungNho(int x1, int y1, int x2, int y2, const char *title)
{
    VeKhung(x1, y1, x2, y2);
    if (title != NULL)
    {
        int len = strlen(title);
        int mid = (x1 + x2) / 2 - len / 2;
        gotoxy(mid, y1);
        SetColor(MAU_VANG_SANG);
        cout << title;
        SetColor(MAU_TRANG);
    }
}

static void VeBangLopTinChi(DS_LTC &ds_ltc,
                            treeMH root,
                            PhanTrang &pt,
                            int x1, int y1, int x2, int y2)
{
    VeKhung(x1, y1, x2, y2);

    gotoxy(x1 + 2, y1 + 1);
    SetColor(MAU_XANH_NHAT_SANG);
    cout << "BANG TRA CUU LTC";
    SetColor(MAU_TRANG);

    gotoxy(x1 + 2, y1 + 3);
    cout << left << setw(8) << "MALTC";
    gotoxy(x1 + 10, y1 + 3);
    cout << left << setw(10) << "MAMH";
    gotoxy(x1 + 20, y1 + 3);
    cout << left << setw(35) << "TEN MH";
    gotoxy(x1 + 56, y1 + 3);
    cout << left << setw(16) << "NIENKHOA";
    gotoxy(x1 + 72, y1 + 3);
    cout << left << setw(6) << "HK";
    gotoxy(x1 + 80, y1 + 3);
    cout << left << setw(8) << "NHOM";

    int total = 0;
    for (int i = 0; i < ds_ltc.n; i++)
    {
        if (ds_ltc.nodes[i] != NULL)
            total++;
    }

    if (total == 0)
    {
        gotoxy(x1 + 2, y1 + 6);
        cout << "Chua co lop tin chi nao.";
        return;
    }

    int start = LayViTriBatDau(pt);
    int end = LayViTriKetThuc(pt);

    int idx = 0;
    int row = 5;
    for (int i = 0; i < ds_ltc.n; i++)
    {
        if (ds_ltc.nodes[i] == NULL)
            continue;

        if (idx < start || idx > end)
        {
            idx++;
            continue;
        }

        Loptinchi *ltc = ds_ltc.nodes[i];
        string tenmh = TimTenMonHoc(root, ltc->MAMH);

        gotoxy(x1 + 2, y1 + row);
        cout << left << setw(8) << ltc->MALOPTC;
        gotoxy(x1 + 10, y1 + row);
        cout << left << setw(10) << ltc->MAMH;
        gotoxy(x1 + 20, y1 + row);
        cout << left << setw(35) << CatChuoi(tenmh, 32);
        gotoxy(x1 + 56, y1 + row);
        cout << left << setw(16) << ltc->NIENKHOA;
        gotoxy(x1 + 72, y1 + row);
        cout << left << setw(6) << ltc->HOCKY;
        gotoxy(x1 + 80, y1 + row);
        cout << left << setw(8) << ltc->NHOM;

        row++;
        idx++;
    }

    gotoxy(x1 + 2, y2 - 2);
    SetColor(MAU_VANG_SANG);
    cout << "Trang " << pt.trangHienTai << "/" << pt.tongTrang;
    SetColor(MAU_TRANG);
}

static void VeMenuThaoTac(int x1, int y1, int x2, int y2, int selectedIndex)
{
    VeKhung(x1, y1, x2, y2);

    gotoxy(x1 + 2, y1 + 1);
    SetColor(MAU_XANH_NHAT_SANG);
    cout << "THAO TAC";
    SetColor(MAU_TRANG);

    const char *items[] = {
        "1. Nhap diem",
        "2. In bang diem",
        "3. Quay lai"
    };

    int baseY = y1 + 4;
    for (int i = 0; i < 3; i++)
    {
        gotoxy(x1 + 3, baseY + i * 4);
        if (i == selectedIndex)
        {
            SetBGColor(MAU_XANH_DUONG_SANG);
            SetColor(MAU_TRANG);
        }
        else
        {
            SetBGColor(MAU_DEN);
            SetColor(MAU_TRANG);
        }
        cout << items[i];
    }

    SetBGColor(MAU_DEN);
    SetColor(MAU_TRANG);
}

static bool NhapThamSoLopTinChi(string &nienkhoa, int &hocky, string &mamh, int &nhom)
{
    // FIX: tăng khoảng cách và đẩy khung nhập sang phải để tách biệt với bảng tra cứu.
    VeKhungNho(92, 18, 118, 27, "NHAP THONG TIN");

    gotoxy(95, 20);
    cout << "NIEN KHOA: ";
    NhapMa(nienkhoa, 20);
    ToUpper(nienkhoa);

    gotoxy(95, 21);
    cout << "HOCKY    : ";
    NhapSo(hocky);

    gotoxy(95, 22);
    cout << "MAMH     : ";
    NhapMa(mamh, 20);
    ToUpper(mamh);

    gotoxy(95, 23);
    cout << "NHOM     : ";
    NhapSo(nhom);

    return true;
}

static void VeBangDiemSinhVien(Loptinchi *ltc,
                               DS_LOP &ds_lop,
                               PTRDK *active,
                               int total,
                               PhanTrang &pt,
                               int selectedIndex,
                               bool allowEdit)
{
    clrscr();

    VeKhungCoTieuDe(2, 1, 118, 28, "DANH SACH SINH VIEN");

    gotoxy(4, 3);
    cout << "Nien khoa : " << ltc->NIENKHOA;
    gotoxy(32, 3);
    cout << "Hoc ky    : " << ltc->HOCKY;
    gotoxy(52, 3);
    cout << "Mon hoc   : " << ltc->MAMH;
    gotoxy(84, 3);
    cout << "Nhom      : " << ltc->NHOM;

    gotoxy(4, 5);
    cout << "STT";
    gotoxy(12, 5);
    cout << "MASV";
    gotoxy(28, 5);
    cout << "HO";
    gotoxy(56, 5);
    cout << "TEN";
    gotoxy(90, 5);
    cout << "DIEM";

    VeDuongNgang(4, 115, 6);

    int start = LayViTriBatDau(pt);
    int end = LayViTriKetThuc(pt);
    int y = 8;

    for (int i = start; i <= end; i++)
    {
        PTRDK p = active[i];
        if (p == NULL)
            continue;

        PTRSV sv = LaySinhVien(ds_lop, p->dk.MASV);
        string ho = "--";
        string ten = "--";
        if (sv != NULL)
        {
            ho = sv->sv.HO;
            ten = sv->sv.TEN;
        }

        string diemText = ChuyenDiemSangChuoi(p->dk.DIEM);

        if (i == selectedIndex)
        {
            SetBGColor(MAU_XANH_DUONG_SANG);
            SetColor(MAU_TRANG);
        }
        else
        {
            SetBGColor(MAU_DEN);
            SetColor(MAU_TRANG);
        }

        gotoxy(4, y);
        printf("%-5d %-15s %-20s %-15s %-8s",
               i + 1,
               p->dk.MASV.c_str(),
               ho.c_str(),
               ten.c_str(),
               diemText.c_str());

        y += 2;
    }

    SetBGColor(MAU_DEN);
    SetColor(MAU_TRANG);

    gotoxy(2, 26);
    SetColor(MAU_VANG_SANG);
    if (allowEdit)
        cout << "ESC: QUAY LAI   <-/->: DOI TRANG   ^/v: CHON DONG   ENTER: NHAP DIEM";
    else
        cout << "ESC: QUAY LAI   <-/->: DOI TRANG   ^/v: CHON DONG";
    SetColor(MAU_TRANG);

    gotoxy(2, 27);
    cout << "Trang " << pt.trangHienTai << "/" << pt.tongTrang
         << "   (Tong " << total << " SV)";
}

static bool HienThiBangDiem(DS_LTC &ds_ltc,
                            DS_LOP &ds_lop,
                            treeMH root,
                            const string &nienkhoa,
                            int hocky,
                            const string &mamh,
                            int nhom,
                            bool allowEdit)
{
    Loptinchi *ltc = TimLTCTheoThongTin(ds_ltc, nienkhoa, hocky, mamh, nhom);
    if (ltc == NULL)
    {
        HienThongBaoLoi("Khong tim thay lop tin chi.");
        ChoPhimBatKy();
        return false;
    }

    int total = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next)
    {
        if (!p->dk.HUYDK)
            total++;
    }

    if (total == 0)
    {
        HienThongBaoLoi("Khong co sinh vien dang ky hien tai.");
        ChoPhimBatKy();
        return false;
    }

    PTRDK *active = new PTRDK[total + 1];
    int idx = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next)
    {
        if (!p->dk.HUYDK)
        {
            active[idx] = p;
            idx++;
        }
    }

    PhanTrang pt;
    KhoiTaoPhanTrang(pt, total, 10);
    int selectedIndex = 0;

    while (true)
    {
        int start = LayViTriBatDau(pt);
        int end = LayViTriKetThuc(pt);
        if (selectedIndex < start)
            selectedIndex = start;
        if (selectedIndex > end)
            selectedIndex = end;

        VeBangDiemSinhVien(ltc, ds_lop, active, total, pt, selectedIndex, allowEdit);

        int key = DocPhim();
        if (key == 27)
        {
            delete [] active;
            return true;
        }
        else if (key == PHIM_TRAI)
        {
            TrangTruoc(pt);
        }
        else if (key == PHIM_PHAI)
        {
            TrangSau(pt);
        }
        else if (key == PHIM_LEN)
        {
            if (selectedIndex > 0)
                selectedIndex--;
        }
        else if (key == PHIM_XUONG)
        {
            if (selectedIndex < total - 1)
                selectedIndex++;
        }
        else if (allowEdit && (key == 13 || key == 8 || key == 46 || key == 45 ||
                                (key >= '0' && key <= '9')))
        {
            int startPage = LayViTriBatDau(pt);
            int yPos = 8 + 2 * (selectedIndex - startPage);
            gotoxy(90, yPos);
            printf("         ");
            gotoxy(90, yPos);

            float newDiem = active[selectedIndex]->dk.DIEM;
            NhapDiem(newDiem);
            active[selectedIndex]->dk.DIEM = newDiem;
        }
    }
}

void InBangDiemMonHoc(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root)
{
    string nienkhoa;
    int hocky = 0;
    string mamh;
    int nhom = 0;

    NhapThamSoLopTinChi(nienkhoa, hocky, mamh, nhom);

    Loptinchi *ltc = TimLTCTheoThongTin(ds_ltc, nienkhoa, hocky, mamh, nhom);
    if (ltc == NULL)
    {
        HienThongBaoLoi("Khong tim thay lop tin chi.");
        ChoPhimBatKy();
        return;
    }

    int total = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next)
    {
        if (!p->dk.HUYDK)
            total++;
    }

    if (total == 0)
    {
        HienThongBaoLoi("Khong co sinh vien dang ky hien tai.");
        ChoPhimBatKy();
        return;
    }

    PTRDK *active = new PTRDK[total + 1];
    int idx = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next)
    {
        if (!p->dk.HUYDK)
        {
            active[idx] = p;
            idx++;
        }
    }

    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "BANG DIEM MON HOC");

    string tenMon = TimTenMonHoc(root, ltc->MAMH);
    gotoxy(4, 3);
    cout << "BANG DIEM MON HOC " << tenMon;
    gotoxy(4, 4);
    cout << "Nien khoa : " << ltc->NIENKHOA
         << "   Hoc ky : " << ltc->HOCKY
         << "   Nhom : " << ltc->NHOM;

    gotoxy(4, 6);
    cout << "STT | MASV | HO | TEN | DIEM";

    VeDuongNgang(4, 115, 7);

    int y = 8;
    for (int i = 0; i < total; i++)
    {
        PTRDK p = active[i];
        PTRSV sv = LaySinhVien(ds_lop, p->dk.MASV);
        string ho = "--";
        string ten = "--";
        if (sv != NULL)
        {
            ho = sv->sv.HO;
            ten = sv->sv.TEN;
        }

        string diemText = ChuyenDiemSangChuoi(p->dk.DIEM);
        gotoxy(4, y);
        printf("%-3d | %-10s | %-15s | %-10s | %-5s",
               i + 1,
               p->dk.MASV.c_str(),
               ho.c_str(),
               ten.c_str(),
               diemText.c_str());
        y++;
    }

    delete [] active;
    ChoPhimBatKy();
}

static int LaySoTinChiMonHoc(treeMH root, const string &mamh)
{
    treeMH node = Search(root, mamh);
    if (node == NULL)
        return 0;
    return node->mh.STC_LT + node->mh.STC_TH;
}

static float TinhDiemTBNguoiHoc(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root, const string &masv)
{
    float tong = 0.0f;
    int tongTC = 0;

    for (int i = 0; i < ds_ltc.n; i++)
    {
        Loptinchi *ltc = ds_ltc.nodes[i];
        if (ltc == NULL)
            continue;

        for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next)
        {
            if (p->dk.MASV != masv)
                continue;
            if (p->dk.HUYDK)
                continue;
            if (p->dk.DIEM < 0)
                continue;

            int tc = LaySoTinChiMonHoc(root, ltc->MAMH);
            if (tc <= 0)
                continue;

            tong += p->dk.DIEM * tc;
            tongTC += tc;
        }
    }

    if (tongTC <= 0)
        return -1.0f;

    return tong / tongTC;
}

void InBangDiemTrungBinhTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root)
{
    string malop;

    clrscr();
    VeKhungNho(30, 8, 90, 16, "NHAP MA LOP");
    gotoxy(34, 10);
    cout << "MA LOP: ";
    NhapMa(malop, 20);
    ToUpper(malop);

    Lop *lop = LayLop(ds_lop, malop);
    if (lop == NULL)
    {
        HienThongBaoLoi("Khong tim thay lop.");
        ChoPhimBatKy();
        return;
    }

    PTRSV firstSV = lop->dssv;
    if (firstSV == NULL)
    {
        HienThongBaoLoi("Lop chua co sinh vien nao.");
        ChoPhimBatKy();
        return;
    }

    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "BANG THONG KE DIEM TRUNG BINH KHOA HOC");

    gotoxy(4, 3);
    cout << "Lop     : " << malop;

    gotoxy(4, 5);
    cout << "STT | MASV | HO | TEN | Diem TB";

    VeDuongNgang(4, 115, 6);

    int y = 8;
    int stt = 1;

    for (PTRSV sv = firstSV; sv != NULL; sv = sv->next)
    {
        float diemTB = TinhDiemTBNguoiHoc(ds_ltc, ds_lop, root, sv->sv.MASV);
        char buf[20];
        if (diemTB < 0)
            sprintf(buf, "0.0");
        else
            sprintf(buf, "%.1f", diemTB);

        gotoxy(4, y);
        printf("%-3d | %-10s | %-15s | %-10s | %-5s",
               stt,
               sv->sv.MASV.c_str(),
               sv->sv.HO.c_str(),
               sv->sv.TEN.c_str(),
               buf);

        y++;
        stt++;
    }

    ChoPhimBatKy();
}

static bool DaCoMonHoc(string *subjects, int count, const string &mamh)
{
    for (int i = 0; i < count; i++)
    {
        if (subjects[i] == mamh)
            return true;
    }
    return false;
}

static void ThemMonHoc(string *&subjects, int &cap, int &count, const string &mamh)
{
    if (count == cap)
    {
        int newCap = cap * 2;
        string *newArr = new string[newCap];
        for (int i = 0; i < count; i++)
            newArr[i] = subjects[i];
        delete [] subjects;
        subjects = newArr;
        cap = newCap;
    }

    subjects[count++] = mamh;
}

static float LayDiemCaoNhatChoMon(DS_LTC &ds_ltc, const string &masv, const string &mamh)
{
    float best = -1.0f;

    for (int i = 0; i < ds_ltc.n; i++)
    {
        Loptinchi *ltc = ds_ltc.nodes[i];
        if (ltc == NULL)
            continue;

        if (ltc->MAMH != mamh)
            continue;

        for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next)
        {
            if (p->dk.MASV != masv)
                continue;
            if (p->dk.HUYDK)
                continue;
            if (p->dk.DIEM < 0)
                continue;
            if (p->dk.DIEM > best)
                best = p->dk.DIEM;
        }
    }

    return best;
}

void InBangDiemTongKetTheoLop(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root)
{
    string malop;

    clrscr();
    VeKhungNho(30, 8, 90, 16, "NHAP MA LOP");
    gotoxy(34, 10);
    cout << "MA LOP: ";
    NhapMa(malop, 20);
    ToUpper(malop);

    Lop *lop = LayLop(ds_lop, malop);
    if (lop == NULL)
    {
        HienThongBaoLoi("Khong tim thay lop.");
        ChoPhimBatKy();
        return;
    }

    PTRSV firstSV = lop->dssv;
    if (firstSV == NULL)
    {
        HienThongBaoLoi("Lop chua co sinh vien nao.");
        ChoPhimBatKy();
        return;
    }

    int studentCount = 0;
    for (PTRSV p = firstSV; p != NULL; p = p->next)
        studentCount++;

    string *subjects = new string[16];
    int subjectCap = 16;
    int subjectCount = 0;

    for (PTRSV p = firstSV; p != NULL; p = p->next)
    {
        for (int i = 0; i < ds_ltc.n; i++)
        {
            Loptinchi *ltc = ds_ltc.nodes[i];
            if (ltc == NULL)
                continue;

            for (PTRDK q = ltc->dssvdk; q != NULL; q = q->next)
            {
                if (q->dk.MASV != p->sv.MASV)
                    continue;
                if (q->dk.HUYDK)
                    continue;
                if (q->dk.DIEM < 0)
                    continue;
                if (!DaCoMonHoc(subjects, subjectCount, ltc->MAMH))
                    ThemMonHoc(subjects, subjectCap, subjectCount, ltc->MAMH);
            }
        }
    }

    if (subjectCount == 0)
    {
        delete [] subjects;
        HienThongBaoLoi("Lop chua co bat ky diem nao.");
        ChoPhimBatKy();
        return;
    }

    PTRSV *students = new PTRSV[studentCount];
    int idx = 0;
    for (PTRSV p = firstSV; p != NULL; p = p->next)
        students[idx++] = p;

    float **scores = new float*[studentCount];
    for (int i = 0; i < studentCount; i++)
    {
        scores[i] = new float[subjectCount];
        for (int j = 0; j < subjectCount; j++)
            scores[i][j] = -1.0f;
    }

    for (int i = 0; i < studentCount; i++)
    {
        for (int j = 0; j < subjectCount; j++)
        {
            float best = LayDiemCaoNhatChoMon(ds_ltc, students[i]->sv.MASV, subjects[j]);
            if (best >= 0)
                scores[i][j] = best;
        }
    }

    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, "BANG DIEM TONG KET");

    gotoxy(4, 3);
    cout << "Lop     : " << malop;

    gotoxy(4, 5);
    cout << "STT | MASV | HO TEN";
    for (int j = 0; j < subjectCount; j++)
    {
        cout << " | " << subjects[j];
    }

    VeDuongNgang(4, 115, 6);

    int y = 8;
    for (int i = 0; i < studentCount; i++)
    {
        string hoTen = students[i]->sv.HO + " " + students[i]->sv.TEN;
        char buf[20];

        gotoxy(4, y);
        printf("%-3d | %-10s | %-20s",
               i + 1,
               students[i]->sv.MASV.c_str(),
               hoTen.c_str());

        for (int j = 0; j < subjectCount; j++)
        {
            if (scores[i][j] < 0)
                sprintf(buf, "-");
            else
                sprintf(buf, "%.1f", scores[i][j]);
            cout << " | " << buf;
        }

        y++;
    }

    for (int i = 0; i < studentCount; i++)
        delete [] scores[i];
    delete [] scores;
    delete [] students;
    delete [] subjects;
    ChoPhimBatKy();
}

void NhapDiem(DS_LTC &ds_ltc, DS_LOP &ds_lop, treeMH root)
{
    int selectedAction = 0;

    while (true)
    {
        clrscr();

        PhanTrang pt;
        KhoiTaoPhanTrang(pt, 0, 10);

        int totalLtc = 0;
        for (int i = 0; i < ds_ltc.n; i++)
        {
            if (ds_ltc.nodes[i] != NULL)
                totalLtc++;
        }

        KhoiTaoPhanTrang(pt, totalLtc, 10);
        // FIX: tăng chiều rộng bảng trái và kéo khung phải sang phải để tạo khoảng trống rõ ràng.
        VeBangLopTinChi(ds_ltc, root, pt, 2, 2, 90, 28);
        VeMenuThaoTac(94, 4, 118, 18, selectedAction);

        int key = DocPhim();
        if (key == 27)
            return;
        else if (key == PHIM_LEN)
        {
            if (selectedAction > 0)
                selectedAction--;
        }
        else if (key == PHIM_XUONG)
        {
            if (selectedAction < 2)
                selectedAction++;
        }
        else if (key == PHIM_TRAI)
        {
            TrangTruoc(pt);
        }
        else if (key == PHIM_PHAI)
        {
            TrangSau(pt);
        }
        else if (key == 13)
        {
            if (selectedAction == 0)
            {
                string nienkhoa;
                int hocky = 0;
                string mamh;
                int nhom = 0;
                NhapThamSoLopTinChi(nienkhoa, hocky, mamh, nhom);
                HienThiBangDiem(ds_ltc, ds_lop, root, nienkhoa, hocky, mamh, nhom, true);
            }
            else if (selectedAction == 1)
            {
                InBangDiemMonHoc(ds_ltc, ds_lop, root);
            }
            else
            {
                return;
            }
        }
    }
}

#include "DangKy.h"
#include "GiaoDien.h"

#include <iostream>
#include <iomanip>

using namespace std;

//=====================================================
// KI?M TRA SINH VIÊN
//=====================================================

bool SinhVienTonTai(DS_LOP &dslop,
                    const string &masv)
{
    return LaySinhVien(dslop, masv) != NULL;
}

//=====================================================
// KI?M TRA ÐÃ ÐANG KÝ CHUA
//=====================================================

bool DaDangKyLTC(Loptinchi *ltc,
                 const string &masv)
{
    if (ltc == NULL)
        return false;

    PTRDK p = TimDKTheoMASV(ltc->dssvdk, masv);

    if (p == NULL)
        return false;

    return !p->dk.HUYDK;
}

//=====================================================
// ÐANG KÝ L?P TÍN CH?
//=====================================================

bool DangKyLopTinChi(DS_LTC &dsltc,
                     DS_LOP &dslop,
                     int maloptc,
                     const string &masv)
{
    // ki?m tra sinh viên
    if (!SinhVienTonTai(dslop, masv))
        return false;

    // tìm l?p tín ch?
    Loptinchi *ltc = LayLTC(dsltc, maloptc);

    if (ltc == NULL)
        return false;

    // l?p dã b? h?y
    if (ltc->HUYLOP)
        return false;

    // l?p dã d?y
    if (DemSVDangKy(ltc->dssvdk) >= ltc->SOSVMAX)
        return false;

    // ki?m tra sinh viên dã t?ng dang ký chua
    PTRDK p = TimDKTheoMASV(ltc->dssvdk, masv);

    if (p != NULL)
    {
        // n?u dã h?y thì m? l?i
        if (p->dk.HUYDK)
        {
            p->dk.HUYDK = false;
            p->dk.DIEM = -1;

            return true;
        }

        // dang dang ký r?i
        return false;
    }

    Dangky dk;

    KhoiTaoDangKy(dk);

    dk.MASV = masv;

    Insert_Order_DK(ltc->dssvdk, dk);

    return true;
}

//=====================================================
// H?Y ÐANG KÝ
//=====================================================

bool HuyDangKyLopTinChi(DS_LTC &dsltc,
                        int maloptc,
                        const string &masv)
{
    Loptinchi *ltc = LayLTC(dsltc, maloptc);

    if (ltc == NULL)
        return false;

    PTRDK p = TimDKTheoMASV(ltc->dssvdk, masv);

    if (p == NULL)
        return false;

    if (p->dk.HUYDK)
        return false;

    p->dk.HUYDK = true;

    return true;
}

//=====================================================
// CH?C NANG (b)
// IN DANH SÁCH SINH VIÊN ÐANG KÝ L?P TÍN CH?
//=====================================================

void InDSSVDangKy(DS_LTC &dsltc,
                  DS_LOP &dslop,
                  treeMH root,
                  const string &nienkhoa,
                  int hocky,
                  const string &mamh,
                  int nhom)
{
    Loptinchi *ltc = NULL;

    // Tìm l?p tín ch? theo 4 tham s?
    for (int i = 0; i < dsltc.n; i++)
    {
        if (dsltc.nodes[i] == NULL)
            continue;

        if (dsltc.nodes[i]->NIENKHOA != nienkhoa)
            continue;

        if (dsltc.nodes[i]->HOCKY != hocky)
            continue;

        if (dsltc.nodes[i]->MAMH != mamh)
            continue;

        if (dsltc.nodes[i]->NHOM != nhom)
            continue;

        ltc = dsltc.nodes[i];
        break;
    }

    if (ltc == NULL)
    {
        HienThongBaoLoi("Khong tim thay lop tin chi.");
        ChoPhimBatKy();
        return;
    }

    clrscr();

    VeKhungCoTieuDe(2,1,118,28,
        "DANH SACH SINH VIEN DANG KY");

    gotoxy(4,3);
    printf("Mon hoc    : %s", mamh.c_str());

    gotoxy(4,4);
    printf("Nien khoa  : %s", nienkhoa.c_str());

    gotoxy(40,4);
    printf("Hoc ky : %d", hocky);

    gotoxy(60,4);
    printf("Nhom : %d", nhom);

    VeDuongNgang(4,115,5);

    gotoxy(4,6);
    printf("%-15s %-35s %-15s",
           "MASV",
           "HO TEN",
           "TRANG THAI");

    VeDuongNgang(4,115,7);

    int y = 8;

    for (PTRDK p = ltc->dssvdk;
         p != NULL;
         p = p->next)
    {
        PTRSV sv = LaySinhVien(dslop,
                               p->dk.MASV);

        if (sv == NULL)
            continue;

        string hoten =
            sv->sv.HO + " " + sv->sv.TEN;

        gotoxy(4,y);

        printf("%-15s %-35s %-15s",
               sv->sv.MASV.c_str(),
               hoten.c_str(),
               p->dk.HUYDK ? "Da huy" : "Dang hoc");

        y++;
    }

    ChoPhimBatKy();
}

//=====================================================
// MENU ÐANG KÝ H?C PH?N
//=====================================================

void MenuDangKyHocPhan(DS_LTC &dsltc,
                       DS_LOP &dslop,
                       treeMH root)
{
    while (true)
    {
        clrscr();

        VeKhungCoTieuDe(20,2,100,26,
                        "DANG KY LOP TIN CHI");

        const char *items[] =
        {
            "1. Dang ky lop tin chi",
            "2. Huy dang ky lop tin chi",
            "3. In danh sach sinh vien dang ky",
            "4. Thoat"
        };

        int chon = XuLyMenu(28,6,items,4);

        if (chon == -1 || chon == 3)
            return;

        switch (chon)
        {
        //-------------------------------------------------
        // ÐANG KÝ
        //-------------------------------------------------
        case 0:
        {
            string masv;
            string nienkhoa;
            int hocky;
            int maloptc;

            clrscr();

            gotoxy(5,3);
            cout << "Nhap ma sinh vien: ";
            cin >> masv;

            PTRSV sv = LaySinhVien(dslop, masv);

            if (sv == NULL)
            {
                HienThongBaoLoi("Khong tim thay sinh vien.");
                ChoPhimBatKy();
                break;
            }

            gotoxy(5,5);
            cout << "Sinh vien: "
                 << sv->sv.HO << " "
                 << sv->sv.TEN;

            gotoxy(5,7);
            cout << "Nhap nien khoa: ";
            cin >> nienkhoa;

            gotoxy(5,8);
            cout << "Nhap hoc ky: ";
            cin >> hocky;

            clrscr();

            // Dùng hàm c?a TV1
            InDSLopTinChi(dsltc, root);

            cout << endl;
            cout << "Nhap ma lop tin chi: ";
            cin >> maloptc;

            if (DangKyLopTinChi(dsltc,
                                dslop,
                                maloptc,
                                masv))
            {
                HienThongBaoThanhCong("Dang ky thanh cong.");
            }
            else
            {
                HienThongBaoLoi("Dang ky that bai.");
            }

            ChoPhimBatKy();
            break;
        }

        //-------------------------------------------------
        // H?Y ÐANG KÝ
        //-------------------------------------------------
        case 1:
        {
            string masv;
            int maloptc;

            clrscr();

            gotoxy(5,3);
            cout << "Nhap ma sinh vien: ";
            cin >> masv;

            gotoxy(5,4);
            cout << "Nhap ma lop tin chi: ";
            cin >> maloptc;

            if (HuyDangKyLopTinChi(dsltc,
                                   maloptc,
                                   masv))
            {
                HienThongBaoThanhCong("Da huy dang ky.");
            }
            else
            {
                HienThongBaoLoi("Khong the huy dang ky.");
            }

            ChoPhimBatKy();
            break;
        }

        //-------------------------------------------------
        // IN DANH SÁCH ÐANG KÝ
        //-------------------------------------------------
        case 2:
        {
            string nienkhoa;
            string mamh;
            int hocky;
            int nhom;

            clrscr();

            gotoxy(5,3);
            cout << "Nhap nien khoa: ";
            cin >> nienkhoa;

            gotoxy(5,4);
            cout << "Nhap hoc ky: ";
            cin >> hocky;

            gotoxy(5,5);
            cout << "Nhap ma mon hoc: ";
            cin >> mamh;

            gotoxy(5,6);
            cout << "Nhap nhom: ";
            cin >> nhom;

            InDSSVDangKy(dsltc,
                         dslop,
                         root,
                         nienkhoa,
                         hocky,
                         mamh,
                         nhom);

            break;
        }
        }
    }
}

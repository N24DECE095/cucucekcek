#include "DangKy.h"
#include "GiaoDien.h"
#include "mylib.h"

#include <iostream>
#include <iomanip>

using namespace std;

//=====================================================
// KI?M TRA SINH VIÊN
//=====================================================

bool SinhVienTonTai(DS_LOP &dslop, const string &masv)
{
    return LaySinhVien(dslop, masv) != NULL;
}

//=====================================================
// KI?M TRA ?Ã ??NG KÝ CH?A
//=====================================================

bool DaDangKyLTC(Loptinchi *ltc, const string &masv)
{
    if (ltc == NULL)
        return false;

    PTRDK p = TimDKTheoMASV(ltc->dssvdk, masv);

    if (p == NULL)
        return false;

    return !p->dk.HUYDK;
}

//=====================================================
// ??NG KÝ L?P TÍN CH?
//=====================================================

bool DangKyLopTinChi(DS_LTC &dsltc, DS_LOP &dslop, int maloptc, const string &masv)
{
    // ki?m tra sinh viên
    if (!SinhVienTonTai(dslop, masv))
        return false;

    // tìm l?p tín ch?
    Loptinchi *ltc = LayLTC(dsltc, maloptc);

    if (ltc == NULL)
        return false;

    // l?p ?ã b? h?y
    if (ltc->HUYLOP)
        return false;

    // l?p ?ã ??y
    if (DemSVDangKy(ltc->dssvdk) >= ltc->SOSVMAX)
        return false;

    // ki?m tra sinh viên ?ã t?ng ??ng ký ch?a
    PTRDK p = TimDKTheoMASV(ltc->dssvdk, masv);

    if (p != NULL)
    {
        // n?u ?ã h?y thì m? l?i
        if (p->dk.HUYDK)
        {
            p->dk.HUYDK = false;
            p->dk.DIEM = -1;
            return true;
        }
        // ?ang ??ng ký r?i
        return false;
    }

    Dangky dk;

	dk.MASV = masv;
	dk.DIEM = -1;
	dk.HUYDK = false;

	Insert_Order_DK(ltc->dssvdk, dk);

    return true;
}

//=====================================================
// H?Y ??NG KÝ
//=====================================================

bool HUYDKLopTinChi(DS_LTC &dsltc, int maloptc, const string &masv)
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
// IN DANH SÁCH SINH VIÊN ??NG KÝ L?P TÍN CH?
//=====================================================

void InDSSVDangKy(DS_LTC &dsltc, DS_LOP &dslop, treeMH root, 
                  const string &nienkhoa, int hocky, const string &mamh, int nhom)
{
    Loptinchi *ltc = NULL;

    // Tìm l?p tín ch? theo 4 tham s?
    for (int i = 0; i < dsltc.n; i++)
    {
        if (dsltc.nodes[i] == NULL) continue;
        if (dsltc.nodes[i]->NIENKHOA != nienkhoa) continue;
        if (dsltc.nodes[i]->HOCKY != hocky) continue;
        if (dsltc.nodes[i]->MAMH != mamh) continue;
        if (dsltc.nodes[i]->NHOM != nhom) continue;

        ltc = dsltc.nodes[i];
        break;
    }

    if (ltc == NULL)
    {
        HienThongBaoLoi("Khong tim thay lop tin chi voi thong tin nay.");
        ChoPhimBatKy();
        return;
    }

    clrscr();
    VeKhungCoTieuDe(2, 1, 118, 28, " DANH SACH SINH VIEN DANG KY ");

    gotoxy(4, 3);
    printf("Mon hoc    : %s", mamh.c_str());

    gotoxy(4, 4);
    printf("Nien khoa  : %s", nienkhoa.c_str());

    gotoxy(40, 4);
    printf("Hoc ky : %d", hocky);

    gotoxy(60, 4);
    printf("Nhom : %d", nhom);

    VeDuongNgang(4, 115, 5);

    gotoxy(4, 6);
    SetColor(MAU_VANG_SANG);

    printf("%-5s %-15s %-40s %-15s",
       "STT",
       "MASV",
       "HO TEN",
       "TRANG THAI");
    SetColor(MAU_TRANG);

    VeDuongNgang(4, 115, 7);

    int y = 8;
    int count = 0;

    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next)
    {
	int stt=1;
        PTRSV sv = LaySinhVien(dslop, p->dk.MASV);
        if (sv == NULL) continue;

        string hoten = sv->sv.HO + " " + sv->sv.TEN;

        gotoxy(4, y);
        printf("%-5d %-15s %-40s %-15s",
       stt++,
       sv->sv.MASV.c_str(),
       hoten.c_str(),
       p->dk.HUYDK ? "HUY" : "DANG KY");
               
        y++;
        count++;
        
        // Tránh in tràn khung (n?u quá nhi?u SV, b?n có th? áp d?ng thêm Phân Trang ? ?ây)
        if (y > 26) {
            gotoxy(4, 27);
            SetColor(MAU_XANH_LA_SANG);
            printf("Danh sach con dai, nhan phim de xem tiep...");
            SetColor(MAU_TRANG);
            getch();
            XoaDong(27);
            // Xóa vùng hi?n th? ?? in trang sau
            for(int cl = 8; cl <= 26; cl++) { XoaDong(cl); VeKhung(2, 1, 118, 28); VeDuongNgang(4, 115, 5); VeDuongNgang(4, 115, 7);}
            y = 8;
        }
    }

    gotoxy(4, 27);
    SetColor(MAU_VANG_SANG);
    printf("Tong so sinh vien: %d", count);
    SetColor(MAU_TRANG);

    ChoPhimBatKy();
}

//=====================================================
// MENU ??NG KÝ H?C PH?N
//=====================================================

void MenuDangKyHocPhan(DS_LTC &dsltc, DS_LOP &dslop, treeMH root)
{
    while (true)
    {
        clrscr();
        VeKhungCoTieuDe(35, 5, 85, 15, " DANG KY LOP TIN CHI ");

        const char *items[] =
        {
            "1. Dang ky lop tin chi",
            "2. Huy dang ky lop tin chi",
            "3. In danh sach sinh vien dang ky",
            "4. Thoat"
        };

        // 40 là hoành ?? X, 8 là tung ?? Y c?a menu
        int chon = XuLyMenu(40, 8, items, 4);

        if (chon == -1 || chon == 3) // ESC ho?c ch?n Thoát
            return;

        switch (chon)
        {
        //-------------------------------------------------
        // ??NG KÝ
        //-------------------------------------------------
        case 0:
        {
            string masv;
            string nienkhoa;
            int hocky = 0;
            int maloptc = 0;

            clrscr();
            VeKhungCoTieuDe(25, 2, 95, 12, " THONG TIN DANG KY ");
            HienConTro(); // Hi?n con tr? ?? nh?p

            gotoxy(30, 4);
            cout << "Nhap ma sinh vien: ";
            NhapMa(masv, 15);

            PTRSV sv = LaySinhVien(dslop, masv);
            if (sv == NULL)
            {
                AnConTro();
                HienThongBaoLoi("Khong tim thay sinh vien hoac ma SV sai.");
                break;
            }

            gotoxy(30, 6);
            SetColor(MAU_XANH_LA_SANG);

		printf("Sinh vien: %s %s",
       			sv->sv.HO.c_str(),
       			sv->sv.TEN.c_str());

		SetColor(MAU_TRANG);

            gotoxy(30, 8);
            cout << "Nhap nien khoa (vd: 2023-2024): ";
            NhapMa(nienkhoa, 10);

            gotoxy(30, 10);
            cout << "Nhap hoc ky: ";
            NhapSo(hocky);

            AnConTro();
           clrscr();

		VeKhungCoTieuDe(2,1,118,28,
                "CAC LOP TIN CHI CO THE DANG KY");

		gotoxy(3,3);

		SetColor(MAU_VANG_SANG);

		printf("%-8s %-10s %-28s %-12s %-4s %-6s %-6s %-6s",
       			"MALTC",
       			"MAMH",
      			"TEN MON",
       			"NIEN KHOA",
       			"HK",
      			"NHOM",
       			"SVDK",
       			"CON");

		SetColor(MAU_TRANG);

		VeDuongNgang(3,115,4);

		int y=5;

		for(int i=0;i<dsltc.n;i++)
{
    Loptinchi *ltc=dsltc.nodes[i];

    if(ltc==NULL) continue;
    if(ltc->HUYLOP) continue;

    if(ltc->NIENKHOA!=nienkhoa)
        continue;

    if(ltc->HOCKY!=hocky)
        continue;

    treeMH mh=Search(root,ltc->MAMH);

    if(mh==NULL)
        continue;

    int svdk=DemSVDangKy(ltc->dssvdk);

    gotoxy(3,y++);

    printf("%-8d %-10s %-28s %-12s %-4d %-6d %-6d %-6d",
       ltc->MALOPTC,
       ltc->MAMH.c_str(),
       mh->mh.TENMH.c_str(),
       ltc->NIENKHOA.c_str(),
       ltc->HOCKY,
       ltc->NHOM,
       svdk,
       ltc->SOSVMAX - svdk);
}

            HienConTro();
            gotoxy(2, 27);
            cout << "Nhap ma lop tin chi ban muon dang ky: ";
            NhapSo(maloptc);
            AnConTro();

            if (DangKyLopTinChi(dsltc, dslop, maloptc, masv))
            {
                HienThongBaoThanhCong("Dang ky thanh cong vao lop tin chi!");
            }
            else
            {
                HienThongBaoLoi("Dang ky that bai (Lop day / Da DK / Huy lop).");
            }
            break;
        }

        //-------------------------------------------------
        // H?Y ??NG KÝ
        //-------------------------------------------------
        case 1:
        {
            string masv;
            int maloptc = 0;

            clrscr();
            VeKhungCoTieuDe(30, 5, 90, 15, " HUY DANG KY HOC PHAN ");
            HienConTro();

            gotoxy(35, 8);
            cout << "Nhap ma sinh vien: ";
            NhapMa(masv, 15);

            gotoxy(35, 11);
            cout << "Nhap ma lop tin chi: ";
            NhapSo(maloptc);
            AnConTro();

            if (HUYDKLopTinChi(dsltc, maloptc, masv))
            {
                HienThongBaoThanhCong("Huy dang ky thanh cong.");
            }
            else
            {
                HienThongBaoLoi("Huy that bai! (Chua dang ky hoac ma loi).");
            }
            break;
        }

        //-------------------------------------------------
        // IN DANH SÁCH ??NG KÝ
        //-------------------------------------------------
        case 2:
        {
            string nienkhoa;
            string mamh;
            int hocky = 0;
            int nhom = 0;

            clrscr();
            VeKhungCoTieuDe(25, 4, 95, 18, " THONG TIN LOP TIN CHI CAN IN ");
            HienConTro();

            gotoxy(30, 7);
            cout << "Nhap ma mon hoc: ";
            NhapMa(mamh, 15);

            gotoxy(30, 10);
            cout << "Nhap nien khoa: ";
            NhapMa(nienkhoa, 10);

            gotoxy(30, 13);
            cout << "Nhap hoc ky: ";
            NhapSo(hocky);

            gotoxy(30, 16);
            cout << "Nhap nhom: ";
            NhapSo(nhom);
            AnConTro();

            InDSSVDangKy(dsltc, dslop, root, nienkhoa, hocky, mamh, nhom);
            break;
        }
        }
    }
}



#include "SinhVien.h"
#include <algorithm>

//=====================================================
// QU?N LÝ L?P
//=====================================================

int TimLopTheoMa(DS_LOP &ds, const string &malop)
{
    for (int i = 0; i < ds.n; i++)
    {
        if (ds.nodes[i]->MALOP == malop)
            return i;
    }
    return -1;
}

Lop* LayLop(DS_LOP &ds, const string &malop)
{
    int vt = TimLopTheoMa(ds, malop);

    if (vt == -1)
        return NULL;

    return ds.nodes[vt];
}

PTRSV LaySinhVien(DS_LOP &ds, const string &masv)
{
    for (int i = 0; i < ds.n; i++)
    {
        PTRSV p = TimSVTheoMASV(ds.nodes[i]->dssv, masv);

        if (p != NULL)
            return p;
    }

    return NULL;
}

//=====================================================
// CRUD L?P
//=====================================================

bool ThemLop(DS_LOP &ds, const Lop &lop)
{
    if (ds.n >= MAX_LOP)
        return false;

    if (TimLopTheoMa(ds, lop.MALOP) != -1)
        return false;

    Lop *p = new Lop;
    *p = lop;
    p->dssv = NULL;

    ds.nodes[ds.n++] = p;

    return true;
}

bool XoaLop(DS_LOP &ds, const string &malop)
{
    int vt = TimLopTheoMa(ds, malop);

    if (vt == -1)
        return false;

    if (ds.nodes[vt]->dssv != NULL)
        return false;

    delete ds.nodes[vt];

    for (int i = vt; i < ds.n - 1; i++)
        ds.nodes[i] = ds.nodes[i + 1];

    ds.n--;

    return true;
}

bool SuaLop(DS_LOP &ds,
            const string &malop,
            const Lop &moi)
{
    Lop *lop = LayLop(ds, malop);

    if (lop == NULL)
        return false;

    lop->TENLOP = moi.TENLOP;

    return true;
}

void GiaiPhongDSLop(DS_LOP &ds)
{
    for (int i = 0; i < ds.n; i++)
    {
        GiaiPhongDSSV(ds.nodes[i]->dssv);
        delete ds.nodes[i];
    }

    ds.n = 0;
}

void InDSLop(DS_LOP &ds)
{
    clrscr();

    VeKhungCoTieuDe(2,1,118,28,"DANH SACH LOP");

    gotoxy(5,3);
    printf("%-12s %-40s %-10s",
           "MA LOP",
           "TEN LOP",
           "SO SV");

    VeDuongNgang(5,115,4);

    int y = 5;

    for(int i=0;i<ds.n;i++)
    {
        gotoxy(5,y+i);

        printf("%-12s %-40s %-10d",
               ds.nodes[i]->MALOP.c_str(),
               ds.nodes[i]->TENLOP.c_str(),
               DemSinhVien(ds.nodes[i]->dssv));
    }

    ChoPhimBatKy();
}

//=====================================================
// QU?N LÝ SINH VIÊN
//=====================================================

PTRSV TaonodeSV(const Sinhvien &sv)
{
    PTRSV p = new nodeSV;

    p->sv = sv;
    p->next = NULL;

    return p;
}

PTRSV TimSVTheoMASV(PTRSV First,
                    const string &masv)
{
    while (First != NULL)
    {
        if (First->sv.MASV == masv)
            return First;

        First = First->next;
    }

    return NULL;
}

void Insert_Order_SV(PTRSV &First,
                     const Sinhvien &sv)
{
    PTRSV p = TaonodeSV(sv);

    if (First == NULL ||
        sv.MASV < First->sv.MASV)
    {
        p->next = First;
        First = p;
        return;
    }

    PTRSV t = First;

    while (t->next != NULL &&
           t->next->sv.MASV < sv.MASV)
    {
        t = t->next;
    }

    p->next = t->next;
    t->next = p;
}

bool ThemSinhVien(Lop &lop,
                  const Sinhvien &sv)
{
    if (TimSVTheoMASV(lop.dssv, sv.MASV) != NULL)
        return false;

    Insert_Order_SV(lop.dssv, sv);

    return true;
}

bool XoaSinhVien(Lop &lop,
                 const string &masv)
{
    PTRSV p = lop.dssv;
    PTRSV truoc = NULL;

    while (p != NULL &&
           p->sv.MASV != masv)
    {
        truoc = p;
        p = p->next;
    }

    if (p == NULL)
        return false;

    if (truoc == NULL)
        lop.dssv = p->next;
    else
        truoc->next = p->next;

    delete p;

    return true;
}

bool HieuChinhSinhVien(Lop &lop,
                       const string &masv,
                       const Sinhvien &moi)
{
    PTRSV p = TimSVTheoMASV(lop.dssv, masv);

    if (p == NULL)
        return false;

    // Không s?a MASV vì danh sách ?ang s?p theo MASV
    p->sv.HO = moi.HO;
    p->sv.TEN = moi.TEN;
    p->sv.PHAI = moi.PHAI;
    p->sv.SODT = moi.SODT;

    return true;
}

int DemSinhVien(PTRSV First)
{
    int dem = 0;

    while (First != NULL)
    {
        dem++;
        First = First->next;
    }

    return dem;
}

void GiaiPhongDSSV(PTRSV &First)
{
    while (First != NULL)
    {
        PTRSV p = First;
        First = First->next;
        delete p;
    }
}


//=====================================================
// CH?C N?NG (c)
//=====================================================

void NhapSinhVienChoLop(Lop &lop, int chucnang)
{
    clrscr();

    if (chucnang == 1)
    {
        VeKhungCoTieuDe(2,1,118,28,"THEM SINH VIEN");

        Sinhvien sv;

        gotoxy(5,4);
        printf("Ma SV : ");
        NhapMa(sv.MASV,15,true);

        if(sv.MASV.empty())
            return;

        if(TimSVTheoMASV(lop.dssv,sv.MASV)!=NULL)
        {
            HienThongBaoLoi("Ma sinh vien da ton tai!");
            return;
        }

        gotoxy(5,6);
        printf("Ho : ");
        NhapChuoi(sv.HO,30);

        gotoxy(5,8);
        printf("Ten : ");
        NhapChuoi(sv.TEN,15);

        gotoxy(5,10);
        printf("Phai : ");
        NhapChuoi(sv.PHAI,5);

        gotoxy(5,12);
        printf("So DT : ");
        NhapSoDT(sv.SODT);

        ThemSinhVien(lop,sv);

        HienThongBaoThanhCong("Them sinh vien thanh cong!");
        return;
    }

    //----------------------------------------------------

    if(chucnang==2)
    {
        VeKhungCoTieuDe(2,1,118,28,"XOA SINH VIEN");

        string masv;

        gotoxy(5,4);
        printf("Ma SV can xoa : ");
        NhapMa(masv,15);

        if(XoaSinhVien(lop,masv))
            HienThongBaoThanhCong("Da xoa sinh vien!");
        else
            HienThongBaoLoi("Khong tim thay sinh vien!");

        return;
    }

    //----------------------------------------------------

    if(chucnang==3)
    {
        VeKhungCoTieuDe(2,1,118,28,"HIEU CHINH SINH VIEN");

        string masv;

        gotoxy(5,4);
        printf("Ma SV : ");
        NhapMa(masv,15);

        PTRSV p = TimSVTheoMASV(lop.dssv,masv);

        if(p==NULL)
        {
            HienThongBaoLoi("Khong tim thay sinh vien!");
            return;
        }

        Sinhvien moi;
        moi.MASV = masv;

        gotoxy(5,6);
        printf("Ho moi : ");
        NhapChuoi(moi.HO,30);

        gotoxy(5,8);
        printf("Ten moi : ");
        NhapChuoi(moi.TEN,15);

        gotoxy(5,10);
        printf("Phai : ");
        NhapChuoi(moi.PHAI,5);

        gotoxy(5,12);
        printf("So DT : ");
        NhapSoDT(moi.SODT);

        HieuChinhSinhVien(lop,masv,moi);

        HienThongBaoThanhCong("Cap nhat thanh cong!");
    }
}
//=====================================================
// CH?C N?NG (d)
// In DSSV theo alphabet TEN + HO
//=====================================================

static bool SoSanhAlphabet(const Sinhvien &a,
                           const Sinhvien &b)
{
    if (StrCmpNoCase(a.TEN, b.TEN) != 0)
        return StrCmpNoCase(a.TEN, b.TEN) < 0;

    if (StrCmpNoCase(a.HO, b.HO) != 0)
        return StrCmpNoCase(a.HO, b.HO) < 0;

    return StrCmpNoCase(a.MASV, b.MASV) < 0;
}

void InDanhSachSinhVienAlphabet(Lop &lop)
{
    clrscr();

    VeKhungCoTieuDe(2,1,118,28,"DANH SACH SINH VIEN");

    int n = DemSinhVien(lop.dssv);

    if(n==0)
    {
        HienThongBaoLoi("Lop chua co sinh vien!");
        return;
    }

    Sinhvien ds[1000];

    PTRSV p = lop.dssv;
    int i = 0;

    while(p!=NULL)
    {
        ds[i++] = p->sv;
        p = p->next;
    }

    //------------------ Sap xep Alphabet ------------------

    for(int i=0;i<n-1;i++)
    {
        int vt=i;

        for(int j=i+1;j<n;j++)
        {
            if(SoSanhAlphabet(ds[j],ds[vt]))
                vt=j;
        }

        if(vt!=i)
        {
            Sinhvien t=ds[i];
            ds[i]=ds[vt];
            ds[vt]=t;
        }
    }

    //------------------ Header ------------------

    SetColor(MAU_VANG_SANG);

    gotoxy(4,3);

    printf("%-5s %-12s %-22s %-15s %-8s %-12s",
           "STT",
           "MASV",
           "HO",
           "TEN",
           "PHAI",
           "SO DT");

    SetColor(MAU_TRANG);

    VeDuongNgang(4,115,4);

    //------------------ Data ------------------

    int y=5;

    for(i=0;i<n;i++)
    {
        gotoxy(4,y+i);

        printf("%-5d %-12s %-22s %-15s %-8s %-12s",
               i+1,
               ds[i].MASV.c_str(),
               ds[i].HO.c_str(),
               ds[i].TEN.c_str(),
               ds[i].PHAI.c_str(),
               ds[i].SODT.c_str());
    }

    //------------------ Footer ------------------

    VeDuongNgang(4,115,y+n);

    SetColor(MAU_XANH_LA_SANG);

    gotoxy(4,y+n+1);

    printf("Tong so sinh vien : %d",n);

    SetColor(MAU_TRANG);

    ChoPhimBatKy();
}
//=====================================================
// MENU NH?P SINH VIÊN
//=====================================================

void MenuNhapSinhVien(DS_LOP &dslop)
{
    string malop;

    clrscr();

    VeKhungCoTieuDe(2,1,118,28,"CAP NHAT SINH VIEN");

    gotoxy(5,4);
    printf("Nhap ma lop : ");
    NhapMa(malop,15);

    Lop *lop = LayLop(dslop,malop);

    if(lop==NULL)
    {
        HienThongBaoLoi("Ma lop khong ton tai!");
        return;
    }

    const char *menu[] =
    {
        "Them sinh vien",
        "Xoa sinh vien",
        "Hieu chinh sinh vien",
        "In DSSV Alphabet",
        "Thoat"
    };

    while(true)
    {
        clrscr();

        VeKhungCoTieuDe(2,1,118,28,"CAP NHAT SINH VIEN");

        gotoxy(5,3);
        SetColor(MAU_XANH_LA_SANG);
        printf("LOP: %s", lop->MALOP.c_str());

        gotoxy(35,3);
        printf("TEN LOP: %s", lop->TENLOP.c_str());

        gotoxy(95,3);
        printf("SO SV: %d", DemSinhVien(lop->dssv));

        SetColor(MAU_TRANG);

        int chon = XuLyMenu(35,8,menu,5);

        switch(chon)
        {
            case 0:
            {
                while(true)
                {
                    NhapSinhVienChoLop(*lop,1);

                    if(HienMenuCon("TIEP TUC ?", "Co", "Khong") != 0)
                        break;
                }
                break;
            }

            case 1:
                NhapSinhVienChoLop(*lop,2);
                break;

            case 2:
                NhapSinhVienChoLop(*lop,3);
                break;

            case 3:
                InDanhSachSinhVienAlphabet(*lop);
                break;

            case 4:
            case -1:
                return;
        }
    }
}


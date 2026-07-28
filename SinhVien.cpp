#include "SinhVien.h"

//=====================================================
// QU?N LÝ DANH SÁCH L?P
//=====================================================

int TimLopTheoMa(DS_LOP &ds, const string &malop)
{
    for (int i = 0; i < ds.n; i++)
    {
        if (ds.nodes[i] != NULL &&
            ds.nodes[i]->MALOP == malop)
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

bool ThemLop(DS_LOP &ds, const Lop &lop)
{
    if (ds.n >= MAX_LOP)
        return false;

    if (TimLopTheoMa(ds, lop.MALOP) != -1)
        return false;

    Lop *p = new Lop;

    KhoiTaoLop(*p);

    p->MALOP = lop.MALOP;
    p->TENLOP = lop.TENLOP;

    ds.nodes[ds.n] = p;
    ds.n++;

    return true;
}

bool XoaLop(DS_LOP &ds, const string &malop)
{
    int vt = TimLopTheoMa(ds, malop);

    if (vt == -1)
        return false;

    // Không cho xóa n?u còn sinh viên
    if (ds.nodes[vt]->dssv != NULL)
        return false;

    delete ds.nodes[vt];

    for (int i = vt; i < ds.n - 1; i++)
        ds.nodes[i] = ds.nodes[i + 1];

    ds.nodes[ds.n - 1] = NULL;
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

    // Ch? cho phép s?a tên l?p
    lop->TENLOP = moi.TENLOP;

    return true;
}

void GiaiPhongDSLop(DS_LOP &ds)
{
    for (int i = 0; i < ds.n; i++)
    {
        if (ds.nodes[i] != NULL)
        {
            GiaiPhongDSSV(ds.nodes[i]->dssv);

            delete ds.nodes[i];
            ds.nodes[i] = NULL;
        }
    }

    ds.n = 0;
}

void InDSLop(DS_LOP &ds)
{
    printf("\n");
    printf("==============================================================\n");
    printf("%-5s %-15s %-30s %-10s\n",
           "STT",
           "MA LOP",
           "TEN LOP",
           "SO SV");
    printf("==============================================================\n");

    for (int i = 0; i < ds.n; i++)
    {
        printf("%-5d %-15s %-30s %-10d\n",
               i + 1,
               ds.nodes[i]->MALOP.c_str(),
               ds.nodes[i]->TENLOP.c_str(),
               DemSinhVien(ds.nodes[i]->dssv));
    }

    printf("==============================================================\n");
}
//=====================================================
// QU?N LÝ DANH SÁCH SINH VIÊN
//=====================================================

PTRSV TaoNodeSV(const Sinhvien &sv)
{
   PTRSV p = new nodeSV;

      if (p == NULL)
        return NULL;

       p->sv = sv;
       p->next = NULL;

       return p;
}

PTRSV TimSVTheoMASV(PTRSV First,
                    const string &masv)
{
    for (PTRSV p = First; p != NULL; p = p->next)
    {
        if (p->sv.MASV == masv)
            return p;
    }

    return NULL;
}

PTRSV LaySinhVien(DS_LOP &ds,
                  const string &masv)
{
    for (int i = 0; i < ds.n; i++)
    {
        PTRSV p = TimSVTheoMASV(ds.nodes[i]->dssv, masv);

        if (p != NULL)
            return p;
    }

    return NULL;
}

void Insert_Order_SV(PTRSV &First,
                     const Sinhvien &sv)
{
    PTRSV p = TaoNodeSV(sv);

    if (First == NULL ||
        sv.MASV < First->sv.MASV)
    {
        p->next = First;
        First = p;
        return;
    }

    PTRSV t = First;
    PTRSV s = First->next;

    while (s != NULL &&
           s->sv.MASV < sv.MASV)
    {
        t = s;
        s = s->next;
    }

    p->next = s;
    t->next = p;
}

bool ThemSinhVien(Lop &lop,
                  const Sinhvien &sv)
{
    if (TimSVTheoMASV(lop.dssv,
                      sv.MASV) != NULL)
        return false;

    Sinhvien moi = sv;
    moi.MALOP = lop.MALOP;

    Insert_Order_SV(lop.dssv,
                    moi);

    return true;
}

bool XoaSinhVien(Lop &lop,
                 const string &masv)
{
    if (lop.dssv == NULL)
        return false;

    PTRSV p = lop.dssv;

    if (p->sv.MASV == masv)
    {
        lop.dssv = p->next;

        delete p;

        return true;
    }

    PTRSV truoc = p;
    p = p->next;

    while (p != NULL)
    {
        if (p->sv.MASV == masv)
        {
            truoc->next = p->next;

            delete p;

            return true;
        }

        truoc = p;
        p = p->next;
    }

    return false;
}

bool HieuChinhSinhVien(Lop &lop,
                       const string &masv,
                       const Sinhvien &moi)
{
    PTRSV p = TimSVTheoMASV(lop.dssv, masv);

    if (p == NULL)
        return false;

    // Không s?a MASV và MALOP
    p->sv.HO    = moi.HO;
    p->sv.TEN   = moi.TEN;
    p->sv.PHAI  = moi.PHAI;
    p->sv.SODT  = moi.SODT;

    return true;
}

int DemSinhVien(PTRSV First)
{
    int dem = 0;

    for (PTRSV p = First;
         p != NULL;
         p = p->next)
    {
        dem++;
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

void InDanhSachSinhVien(PTRSV First)
{
    printf("\n");

    printf("=========================================================================================\n");
    printf("%-5s %-12s %-25s %-12s %-8s %-15s\n",
           "STT",
           "MASV",
           "HO",
           "TEN",
           "PHAI",
           "SDT");
    printf("=========================================================================================\n");

    int stt = 1;

    for (PTRSV p = First;
         p != NULL;
         p = p->next)
    {
        printf("%-5d %-12s %-25s %-12s %-8s %-15s\n",
               stt++,
               p->sv.MASV.c_str(),
               p->sv.HO.c_str(),
               p->sv.TEN.c_str(),
               p->sv.PHAI.c_str(),
               p->sv.SODT.c_str());
    }

    printf("=========================================================================================\n");
}

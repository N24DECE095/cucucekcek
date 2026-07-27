/*
    File: XuLyFile.cpp
    Tác giả: TV1 - Nguyễn Việt Anh
    Mô tả: Cài đặt đọc / ghi 2 file text.
*/

#include "XuLyFile.h"
#include "Monhoc.h"
#include "Loptinchi.h"
#include <fstream>

// ============================================================================
// UTILITY — tách dòng theo `|`
// ============================================================================
static void SplitLine(const string &line, string tokens[], int maxTok, int &nTok) {
    nTok = 0;
    string cur = "";
    for (int i = 0; i < (int)line.size() && nTok < maxTok; i++) {
        if (line[i] == '|') {
            tokens[nTok++] = cur;
            cur = "";
        } else {
            cur.push_back(line[i]);
        }
    }
    if (nTok < maxTok) tokens[nTok++] = cur;
}

// ============================================================================
// GHI CÂY MH — dùng đệ qui PreOrder để khi đọc lại sẽ tái tạo đúng shape cây
// ============================================================================
static void GhiPreOrder(treeMH root, ofstream &f) {
    if (root == NULL) return;
    f << root->mh.MAMH  << "|" << root->mh.TENMH  << "|"
      << root->mh.STC_LT << "|" << root->mh.STC_TH << "\n";
    GhiPreOrder(root->left,  f);
    GhiPreOrder(root->right, f);
}

void LuuFileMonhoc(treeMH root, const char *file) {
    ofstream f(file);
    if (!f.is_open()) return;
    f << DemMH(root) << "\n";
    GhiPreOrder(root, f);
    f.close();
}

void DocFileMonhoc(treeMH &root, const char *file) {
    GiaiPhongCayMH(root);
    ifstream f(file);
    if (!f.is_open()) return;
    int n;
    f >> n;
    f.ignore();
    for (int i = 0; i < n; i++) {
        string line;
        if (!getline(f, line)) break;
        string tok[6];
        int nt;
        SplitLine(line, tok, 6, nt);
        if (nt < 4) continue;
        Monhoc mh;
        mh.MAMH   = tok[0];
        mh.TENMH  = tok[1];
        mh.STC_LT = atoi(tok[2].c_str());
        mh.STC_TH = atoi(tok[3].c_str());
        bool ok;
        root = ThemMH(root, mh, ok);
    }
    f.close();
    // Sau khi đọc xong, tự cân bằng lại cây để tối ưu tìm kiếm.
    // Vì thứ tự dữ liệu trong file có thể làm cây bị lệch.
    CanBangCay(root);
}

// ============================================================================
// GHI DS_LTC — mỗi LTC 1 dòng header + N dòng Dangky
// ============================================================================
void LuuFileLTC(DS_LTC &ds, const char *file) {
    ofstream f(file);
    if (!f.is_open()) return;
    f << ds.n << "\n";
    for (int i = 0; i < ds.n; i++) {
        Loptinchi *l = ds.nodes[i];
        int soDK = DemSVDangKy_KyCaHuy(l->dssvdk);
        f << l->MALOPTC << "|" << l->MAMH     << "|" << l->NIENKHOA << "|"
          << l->HOCKY   << "|" << l->NHOM     << "|"
          << l->SOSVMIN << "|" << l->SOSVMAX  << "|"
          << (l->HUYLOP ? 1 : 0) << "|" << soDK << "\n";
        for (PTRDK p = l->dssvdk; p != NULL; p = p->next) {
            f << p->dk.MASV << "|" << p->dk.DIEM << "|"
              << (p->dk.HUYDK ? 1 : 0) << "\n";
        }
    }
    f.close();
}

void DocFileLTC(DS_LTC &ds, const char *file) {
    HuyDSLTC(ds);
    KhoiTaoDSLTC(ds);
    ifstream f(file);
    if (!f.is_open()) return;
    int n;
    f >> n;
    f.ignore();
    for (int i = 0; i < n; i++) {
        string line;
        if (!getline(f, line)) break;
        string tok[12];
        int nt;
        SplitLine(line, tok, 12, nt);
        if (nt < 9) continue;

        Loptinchi *l = new Loptinchi;
        KhoiTaoLopTinChi(*l);
        l->MALOPTC  = atoi(tok[0].c_str());
        l->MAMH     = tok[1];
        l->NIENKHOA = tok[2];
        l->HOCKY    = atoi(tok[3].c_str());
        l->NHOM     = atoi(tok[4].c_str());
        l->SOSVMIN  = atoi(tok[5].c_str());
        l->SOSVMAX  = atoi(tok[6].c_str());
        l->HUYLOP   = (tok[7] == "1");
        int soDK    = atoi(tok[8].c_str());
        ds.nodes[ds.n++] = l;

        for (int k = 0; k < soDK; k++) {
            string dkLine;
            if (!getline(f, dkLine)) break;
            string dkTok[4];
            int nk;
            SplitLine(dkLine, dkTok, 4, nk);
            if (nk < 3) continue;
            Dangky dk;
            dk.MASV  = dkTok[0];
            dk.DIEM  = (float)atof(dkTok[1].c_str());
            dk.HUYDK = (dkTok[2] == "1");
            Insert_Order_DK(l->dssvdk, dk);
        }
    }
    f.close();
}

// ============================================================================
// MASTER — gộp 2 file
// ============================================================================
void LuuTatCa(treeMH root, DS_LTC &dsLtc) {
    LuuFileMonhoc(root);
    LuuFileLTC(dsLtc);
}

void DocTatCa(treeMH &root, DS_LTC &dsLtc) {
    DocFileMonhoc(root);
    DocFileLTC(dsLtc);
}

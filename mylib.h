
#ifndef MYLIB_H
#define MYLIB_H

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

#define Enter 13

// ============================================================================
// CONSOLE API 
// ============================================================================

inline void gotoxy(short x, short y) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}

inline int wherex() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    return info.dwCursorPosition.X;
}

inline int wherey() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    return info.dwCursorPosition.Y;
}

inline void clreol() {
    COORD coord;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    coord.X = info.dwCursorPosition.X;
    coord.Y = info.dwCursorPosition.Y;
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ',
        info.dwSize.X - info.dwCursorPosition.X, coord, &written);
    gotoxy(info.dwCursorPosition.X, info.dwCursorPosition.Y);
}

inline void clrscr() {
    system("cls");
}

inline void SetColor(WORD color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    WORD attr = info.wAttributes;
    color &= 0x000f;
    attr &= 0xfff0;
    attr |= color;
    SetConsoleTextAttribute(h, attr);
}

inline void SetBGColor(WORD color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    WORD attr = info.wAttributes;
    color &= 0x000f;
    color <<= 4;
    attr &= 0xff0f;
    attr |= color;
    SetConsoleTextAttribute(h, attr);
}

// ============================================================================
// HÀM XÁC NHẬN Y/N —
// ============================================================================
// Hỏi Y/N, trả về 1 nếu Y, 0 nếu N. Dùng getch + toupper cho input tức thời.
inline int XacNhan(const char *thongBao) {
    cout << thongBao;
    char kt;
    do {
        kt = toupper(getch());
    } while (kt != 'Y' && kt != 'N');
    cout << kt;
    return (kt == 'Y') ? 1 : 0;
}

// ============================================================================
// HÀM BÁO LỖI 
// ============================================================================
inline void BaoLoi(const char *s) {
    int x = wherex(), y = wherey();
    gotoxy(10, 24);
    SetColor(12); // đỏ
    cout << s;
    SetColor(15);
    Sleep(3000);
    gotoxy(10, 24);
    clreol();
    gotoxy(x, y);
}

// ============================================================================
// CÁC HÀM NHẬP CHUẨN 
// ============================================================================

// 1. Nhập số nguyên không âm (nhập từng ký số, bắt lỗi)
//    Chỉ chấp nhận '0'-'9'. Enter để kết thúc (cần ít nhất 1 ký số).
//    Backspace để xóa. Giới hạn 9 ký số để tránh tràn int.
inline void NhapSo(int &n) {
    string s = "";
    char c;
    while (true) {
        c = getch();
        // Bỏ qua phím mở rộng (F1-F12, mũi tên có 2 byte)
        if (c == 0 || c == (char)224) {
            getch();
            continue;
        }
        // Enter — kết thúc (yêu cầu ít nhất 1 ký số)
        if (c == Enter) {
            if (!s.empty()) break;
        }
        // Backspace
        else if (c == 8) {
            if (!s.empty()) {
                s.pop_back();
                printf("\b \b");
            }
        }
        // Chỉ nhận '0'-'9', giới hạn 9 ký số
        else if (c >= '0' && c <= '9' && (int)s.size() < 9) {
            s.push_back(c);
            printf("%c", c);
        }
    }
    n = atoi(s.c_str());
}

// 2. Nhập chuỗi chỉ nhận chữ cái (a-z, A-Z) và khoảng trắng
//    Tự động in HOA ký tự đầu chuỗi và ký tự ngay sau khoảng trắng
//    Chặn khoảng trắng đầu và liên tiếp. Tự cắt khoảng trắng cuối khi Enter.
inline void NhapChuoi(string &s, int maxLen) {
    s = "";
    char c;
    while (true) {
        c = getch();
        if (c == 0 || c == (char)224) {
            getch();
            continue;
        }
        if (c == Enter) {
            if (!s.empty()) break;
        }
        else if (c == 8) {
            if (!s.empty()) {
                s.pop_back();
                printf("\b \b");
            }
        }
        else if ((int)s.size() < maxLen) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ') {
                // Chặn khoảng trắng đầu và liên tiếp
                if (c == ' ' && (s.empty() || s.back() == ' ')) continue;
                // Tự động in HOA đầu từ
                if (c != ' ') {
                    if (s.empty() || s.back() == ' ') {
                        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
                    } else {
                        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
                    }
                }
                s.push_back(c);
                printf("%c", c);
            }
        }
    }
    // Cắt khoảng trắng cuối
    while (!s.empty() && s.back() == ' ') {
        s.pop_back();
        printf("\b \b");
    }
}

// 3. Nhập mã: chỉ nhận chữ cái, ký số, '-', '_'
//    Tự động in HOA. Khống chế chiều dài.
//    Cho phép Enter với chuỗi rỗng nếu allowEmpty = true (dùng cho chức năng dừng nhập SV)
inline void NhapMa(string &s, int maxLen, bool allowEmpty = false) {
    s = "";
    char c;
    while (true) {
        c = getch();
        if (c == 0 || c == (char)224) {
            getch();
            continue;
        }
        if (c == Enter) {
            if (!s.empty() || allowEmpty) break;
        }
        else if (c == 8) {
            if (!s.empty()) {
                s.pop_back();
                printf("\b \b");
            }
        }
        else if ((int)s.size() < maxLen) {
            if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_') {
                s.push_back(c);
                printf("%c", c);
            }
        }
    }
}

// 4. Nhập số điện thoại: chỉ nhận số, 10-11 ký số
inline void NhapSoDT(string &s) {
    s = "";
    char c;
    while (true) {
        c = getch();
        if (c == 0 || c == (char)224) { getch(); continue; }
        if (c == Enter) {
            if ((int)s.size() >= 10) break;
        }
        else if (c == 8) {
            if (!s.empty()) {
                s.pop_back();
                printf("\b \b");
            }
        }
        else if (c >= '0' && c <= '9' && (int)s.size() < 11) {
            s.push_back(c);
            printf("%c", c);
        }
    }
}

// 5. Nhập điểm float (0.0 - 10.0), chấp nhận số nguyên hoặc số có dấu chấm
inline void NhapDiem(float &d) {
    string s = "";
    char c;
    bool hasDot = false;
    while (true) {
        c = getch();
        if (c == 0 || c == (char)224) { getch(); continue; }
        if (c == Enter) {
            if (!s.empty()) break;
        }
        else if (c == 8) {
            if (!s.empty()) {
                if (s.back() == '.') hasDot = false;
                s.pop_back();
                printf("\b \b");
            }
        }
        else if (c == '.' && !hasDot && !s.empty()) {
            s.push_back(c);
            hasDot = true;
            printf("%c", c);
        }
        else if (c >= '0' && c <= '9' && (int)s.size() < 4) {
            s.push_back(c);
            printf("%c", c);
        }
    }
    d = atof(s.c_str());
    if (d < 0) d = 0;
    if (d > 10) d = 10;
}

// ============================================================================
// HÀM HỖ TRỢ CHUỖI
// ============================================================================

// Chuyển toàn bộ chuỗi thành CHỮ HOA (chỉ cho ASCII)
inline void ToUpper(string &s) {
    for (int i = 0; i < (int)s.size(); i++)
        if (s[i] >= 'a' && s[i] <= 'z') s[i] = s[i] - 'a' + 'A';
}

// So sánh 2 chuỗi không phân biệt hoa/thường (giống stricmp)
inline int StrCmpNoCase(const string &a, const string &b) {
    int la = a.size(), lb = b.size();
    int n = la < lb ? la : lb;
    for (int i = 0; i < n; i++) {
        char ca = a[i], cb = b[i];
        if (ca >= 'a' && ca <= 'z') ca = ca - 'a' + 'A';
        if (cb >= 'a' && cb <= 'z') cb = cb - 'a' + 'A';
        if (ca != cb) return ca - cb;
    }
    return la - lb;
}

#endif // MYLIB_H

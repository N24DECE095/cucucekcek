@echo off
REM Build Task 1 (Mon hoc + Lop tin chi)
REM Chi can double-click hoac chay trong CMD

echo ========================================
echo   BUILD TASK 1 - MH + LTC
echo ========================================
echo.

REM Xoa file cache neu co
if exist CauTruc.h.gch del CauTruc.h.gch
if exist mylib.h.gch del mylib.h.gch

REM Bien dich
g++ -std=c++11 -o Task1.exe main.cpp Monhoc.cpp Loptinchi.cpp GiaoDien.cpp XuLyFile.cpp SinhVien.cpp DangKy.cpp NhapDiem.cpp

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [LOI] Bien dich that bai! Xem loi phia tren.
    pause
    exit /b 1
)

echo.
echo [OK] Bien dich thanh cong! Da tao Task1.exe
echo.
echo Chay chuong trinh: Task1.exe hoac double-click run.bat
echo.
pause

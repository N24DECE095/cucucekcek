@echo off
REM Chay Task 1 trong cua so CMD moi (khong dung VS Code terminal)

if not exist Task1.exe (
    echo Chua co Task1.exe. Dang build...
    call build.bat
    if not exist Task1.exe exit /b 1
)

start "Task1 - MH + LTC" cmd /c "Task1.exe & pause"

@echo off
:: 1. 强制设置CMD终端为UTF-8编码（Windows 10/11支持）
chcp 65001 >nul
:: 2. 设置终端字体为支持UTF-8的字体（解决方块乱码）
reg add "HKCU\Console\%~dpnx0" /v "FaceName" /t REG_SZ /d "Consolas" /f >nul 2>&1
reg add "HKCU\Console\%~dpnx0" /v "CodePage" /t REG_DWORD /d 65001 /f >nul 2>&1
:: 3. 编译：源码UTF-8，输出也强制UTF-8（匹配终端）
gcc main.c common.c user.c post.c join.c -o main.exe -finput-charset=utf-8 -fexec-charset=utf-8
:: 4. 运行程序
if %errorlevel% equ 0 (
    echo 编译成功！启动程序...
    echo ---------------------------
    main.exe
) else (
    echo 编译失败！请检查代码
    pause
)
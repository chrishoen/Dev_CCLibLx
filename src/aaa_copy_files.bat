cd C:\Prime\DevelopLinux\Dev_CCLibLx\CCLib
del *.h
del *.cpp
copy /y C:\Prime\DevelopLib\Dev_CCLib\CCLib\Source\*.* .
del *_win.*
copy ..\src\*.h .
copy ..\src\*.cpp .
pause

@echo off
cd build\bin
Sandbox.exe
echo.
echo Exit code: %ERRORLEVEL%
pause

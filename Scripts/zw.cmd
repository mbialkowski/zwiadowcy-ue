@echo off
rem Skrot do Scripts\zw.ps1 (omija blokade uruchamiania skryptow PowerShell)
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0zw.ps1" %*
exit /b %ERRORLEVEL%

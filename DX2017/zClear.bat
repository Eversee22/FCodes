@echo off
rem fx
del *.cod /a /s
del *.fxo /a /s

for /f "delims=" %%i in ('dir /ad /b /s')  do (
	echo %%i
	if "%%~xi"==".vs" rd /s /q "%%i"
	if "%%~ni"=="Debug" rd /s /q "%%i"
	if "%%~ni"=="Release"  rd /s /q "%%i"
)
pause
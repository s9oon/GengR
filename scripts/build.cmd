@echo off
rem Builds GengR with MSVC + Ninja.
rem Usage: scripts\build.cmd [debug|release]   (default: debug)
setlocal

set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=debug
set PRESET=x64-%CONFIG%

rem MSVC's compiler only works inside its developer environment, so find Visual Studio and load that first
set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`%VSWHERE% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set VSPATH=%%i
if not defined VSPATH (
    echo error: couldn't find Visual Studio Build Tools with the C++ workload
    exit /b 1
)
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul || exit /b 1

rem run from the repo root (the folder above scripts\)
cd /d "%~dp0.."

cmake --preset %PRESET% >nul || exit /b 1
cmake --build --preset %PRESET% || exit /b 1

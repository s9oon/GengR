@echo off
rem Builds, then runs the client.
rem Usage: scripts\run-client.cmd [debug|release]   (default: debug)
setlocal

set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=debug

call "%~dp0build.cmd" %CONFIG% || exit /b 1

rem CMake puts each exe in the build folder matching its source folder (apps\client -> build\...\apps\client)
"%~dp0..\build\x64-%CONFIG%\apps\client\GengRClient.exe"

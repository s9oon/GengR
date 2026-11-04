@echo off
rem Builds, then runs the server.
rem Usage: scripts\run-server.cmd [debug|release]   (default: debug)
setlocal

set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=debug

call "%~dp0build.cmd" %CONFIG% || exit /b 1

"%~dp0..\build\x64-%CONFIG%\apps\server\GengRServer.exe"

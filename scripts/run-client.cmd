@echo off
rem Builds, then runs the client.
rem Usage: scripts\run-client.cmd [debug|release]   (default: debug)
setlocal

set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=debug

call "%~dp0build.cmd" %CONFIG% || exit /b 1

rem exe sits next to SDL3.dll in build\x64-<config>\<config>\ (CMAKE_RUNTIME_OUTPUT_DIRECTORY)
"%~dp0..\build\x64-%CONFIG%\%CONFIG%\GengRClient.exe"

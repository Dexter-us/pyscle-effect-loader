@echo off
REM Windows Setup Script for Psycle Adapter VST3 Plugin
REM This script sets up the entire build environment for Windows

echo ==========================================
echo Psycle Adapter - Windows Build Setup
echo ==========================================
echo.

REM Check for Git
where git >nul 2>nul
if errorlevel 1 (
    echo ERROR: Git is not installed or not in PATH
    echo Please install Git from: https://git-scm.com/download/win
    pause
    exit /b 1
)
echo [OK] Git found

REM Check for CMake
where cmake >nul 2>nul
if errorlevel 1 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from: https://cmake.org/download/
    echo Or: choco install cmake
    pause
    exit /b 1
)
echo [OK] CMake found

REM Check for Visual Studio 2022
if not exist "C:\Program Files\Microsoft Visual Studio\2022" (
    echo WARNING: Visual Studio 2022 not found in default location
    echo Please ensure Visual Studio 2022 Community/Professional is installed
    echo Download from: https://visualstudio.microsoft.com/downloads/
    echo.
)
echo [OK] Visual Studio 2022 setup assumed

REM Initialize Git submodules
echo.
echo Initializing Git submodules...
git submodule update --init --recursive
if errorlevel 1 (
    echo ERROR: Failed to initialize submodules
    pause
    exit /b 1
)
echo [OK] Submodules initialized

REM Create build directory
echo.
echo Creating build directory...
if not exist "build" mkdir build
echo [OK] Build directory created

REM Run CMake configuration
echo.
echo Running CMake configuration for Windows VS2022...
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed
    cd ..
    pause
    exit /b 1
)
cd ..
echo [OK] CMake configuration complete

echo.
echo ==========================================
echo Setup Complete!
echo ==========================================
echo.
echo Next steps:
echo   1. Open: build\iPlug2OOS.sln in Visual Studio 2022
echo   2. Set TemplateProject as the startup project
echo   3. Build the solution
echo.
echo Alternative (command line):
echo   cmake --build build --config Release
echo.
pause

@echo off
set root_folder=build
set dll_folder=build

if exist build (
  echo Deleting existing build directory...
  rd /s /q build
)

set CMAKE_COMMAND=cmake

echo Creating build directory...
mkdir build

echo Running CMake...
%CMAKE_COMMAND% -S . -B build

echo Building project...
%CMAKE_COMMAND% --build build

if not exist "%dll_folder%" (
  echo DLL folder not found.
  pause
  exit
)

if not exist "%root_folder%" (
  echo Root folder not found.
  pause
  exit
)

echo Searching for DLL files in %dll_folder%...
for /r "%dll_folder%" %%i in (*.dll) do (
  echo Copying "%%~nxi" to %root_folder%...
  copy "%%i" "%root_folder%"
)

echo DLL files copied successfully to %root_folder%.

echo Start Engine
cd build
.\Editor.exe ../demo
pause

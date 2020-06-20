@echo off

echo Current directory is %CD%
echo Please move this .bat file if this isn't the location of the .frag files or .vert file
echo.

echo Please enter the Vulkan SDK Path (Root dir) (With version number)
set /p VULKANPATH=

echo %VULKANPATH% is set as the SDK Path

echo 1. Use x64 architecture
echo 2. Use x86 architecture
echo Select 1 or 2 to continue

set /p BITFORMAT=
echo %BITFORMAT% was selected as format
echo.

for %%f in (*.*) do (
  if "%%~xf"==".frag" set ext="True"
  if "%%~xf"==".vert" set ext="True"

  if defined ext (
    if "%BITFORMAT%"=="1" %VULKANPATH%\Bin\glslangValidator.exe -V %%f)
    if "%BITFORMAT%"=="2" %VULKANPATH%\Bin32\glslangValidator.exe -V %%f)
)

echo.
echo Press any key to close
set /p anyKey=

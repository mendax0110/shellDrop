@echo off
setlocal

set "zip_executable=C:\Program Files\7-Zip\7z.exe"

if not exist "%zip_executable%" (
    echo Error: 7-Zip executable not found at "%zip_executable%"
    exit /b 1
)

if "%~1"=="" (
    echo Usage: %0 ^<input_executable^>
    exit /b 1
)

set "input_executable=%~1"
set "packed_executable=%input_executable%.zip"

"%zip_executable%" a "%packed_executable%" "%input_executable%"

echo Executable packed to: %packed_executable%

endlocal

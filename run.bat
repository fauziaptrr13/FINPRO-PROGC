@echo off
gcc main.c engineer_functions.c nakes_functions.c equipment_data.c shared_functions.c utils.c -o medical_system.exe

if %errorlevel% == 0 (
    medical_system.exe
) else (
    echo Compilation failed.
)
pause


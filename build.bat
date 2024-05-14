@echo off

set compiler=cl
set console=/DBUILD_CONSOLE_OUTPUT=1
set cl_flags=/Zi /nologo /FC /I..\code\ /I..\local\

if not exist build mkdir build

pushd build
@REM Windows grow significantly their pdb size over time
del *.pdb > NUL 2> NUL 

set CommonCompilerFlags= /I..\src\ /nologo /FC /std:c11
set CommonLinkerFlags= /INCREMENTAL:NO
set warn_disabled=-wd4201 -wd4505 -wd4100
cl /I..\src\ -DBUILD_CONSOLE_OUTPUT=1 -Zi -W4  %warn_disabled% -nologo ..\src\vault\main.c -Fevault_dm.exe /link %CommonLinkerFlags%
cl %CommonCompilerFlags% %warn_disabled% -nologo ..\src\vault\main.c -Fevault_rm.exe /link %CommonLinkerFlags%

@REM if "%dev%"=="1"       %compiler% %cl_flags% %gfx% %net%                               ..\code\samples\samples_app_template_main.c ..\code\render\d3d11\render_d3d11_main.cpp ..\code\font_provider\dwrite\font_provider_dwrite_main.cpp                        /link /out:samples_app_template.exe

popd

if "%1" == "" goto demo

if %1 == dbg ..\..\..\preprocessor\bin\templet.exe order.cpp -r TEMPLET_DBG -i dbg/tet.h
exit

:demo
call ..\..\..\preprocessor\bin\templet.exe order.cpp -d -r TEMPLET_DBG -i dbg/tet.h
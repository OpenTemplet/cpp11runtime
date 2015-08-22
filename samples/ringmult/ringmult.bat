if "%1" == "" goto demo

if %1 == dbg ..\..\..\preprocessor\bin\templet.exe ringmult.cpp -r TEMPLET_DBG -i dbg/tet.h
if %1 == par ..\..\..\preprocessor\bin\templet.exe ringmult.cpp -r TEMPLET_PAR -i par/tet.h
if %1 == seq ..\..\..\preprocessor\bin\templet.exe ringmult.cpp -r TEMPLET_SEQ -i seq/tet.h
if %1 == sim ..\..\..\preprocessor\bin\templet.exe ringmult.cpp -r TEMPLET_SIM -i sim/tet.h
exit

:demo
call ..\..\..\preprocessor\bin\templet.exe ringmult.cpp -d -r TEMPLET_DBG -i dbg/tet.h
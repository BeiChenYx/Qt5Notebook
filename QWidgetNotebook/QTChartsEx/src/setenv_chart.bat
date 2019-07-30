echo "Qt入门与提高-GUI产品开发 环境变量设置"

set QTDIR=C:\Qt\5.11.1
set QMAKESPEC=win32-msvc
set TRAINDEVHOME=e:\xingdianketang\project\charts
set TRAINBUILDBIT=64
set TRAINBUILDTYPE=all
set path=%TRAINDEVHOME%\lib;%TRAINDEVHOME%\bin;%QTDIR%\bin;C:\qt\5.11.1\qtcharts\examples\charts\chartthemes\debug;%QTDIR%\lib;C:\qt\qtcreator-4.7.2\bin;%path%
echo %QTDIR%
echo %QMAKESPEC%
echo %TRAINDEVHOME%
cd %TRAINDEVHOME%
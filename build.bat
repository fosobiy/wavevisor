cd /D tgl
call build.bat
cd /D ..

ktcc\kos32-tcc -Itgl\include main.c ^
	tgl\obj\api.o tgl\obj\arrays.o tgl\obj\clear.o tgl\obj\clip.o tgl\obj\error.o tgl\obj\get.o tgl\obj\glu.o tgl\obj\image_util.o tgl\obj\init.o tgl\obj\kosgl.o tgl\obj\light.o tgl\obj\list.o tgl\obj\matrix.o tgl\obj\misc.o tgl\obj\msghandling.o tgl\obj\select.o tgl\obj\specbuf.o tgl\obj\texture.o tgl\obj\vertex.o tgl\obj\zbuffer.o tgl\obj\zdither.o tgl\obj\zline.o tgl\obj\zmath.o tgl\obj\ztriangle.o ^
	-o examplebin
python pcdl-test.py
pause
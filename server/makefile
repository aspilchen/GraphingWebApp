swig: algos.h algos.c algos.i
	swig -python -py3 -modern algos.i
	gcc -fPIC -c algos.c algos_wrap.c -I/usr/include/python3.6
	ld -shared algos.o algos_wrap.o -o _algos.so

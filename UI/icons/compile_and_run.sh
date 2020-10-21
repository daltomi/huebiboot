#!/bin/bash
echo "[*] Compilando..."
chmod -w compile_and_run.sh png2h.c
g++ -c png2h.c $(fltk-config --cxxflags) || exit 1
g++ png2h.o -o png2h $(fltk-config --ldflags --use-images) || exit 1
rm png2h.o

echo "[*] Eliminando icons.h ..."
rm icons.h

echo "[*] Generando icons.h ..."
for i in *.png; do
	./png2h $i >> icons.h
done

rm png2h
echo "[*] Done..."

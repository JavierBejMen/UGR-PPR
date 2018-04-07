#!/bin/bash
rm 2d.txt
clear

echo "Floyd_2d:" >> 2d.txt
echo input40 >> 2d.txt
mpirun -np 4 ./bin/main data/input40 >> 2d.txt

echo input80 >> 2d.txt
mpirun -np 4 ./bin/main data/input80 >> 2d.txt

echo input160 >> 2d.txt
mpirun -np 4 ./bin/main data/input160 >> 2d.txt

echo input220 >> 2d.txt
mpirun -np 4 ./bin/main data/input220 >> 2d.txt

echo input320 >> 2d.txt
mpirun -np 4 ./bin/main data/input320 >> 2d.txt

echo input420 >> 2d.txt
mpirun -np 4 ./bin/main data/input420 >> 2d.txt

echo input520 >> 2d.txt
mpirun -np 4 ./bin/main data/input520 >> 2d.txt

echo input620 >> 2d.txt
mpirun -np 4 ./bin/main data/input620 >> 2d.txt

echo input720 >> 2d.txt
mpirun -np 4 ./bin/main data/input720 >> 2d.txt

echo input920 >> 2d.txt
mpirun -np 4 ./bin/main data/input920 >> 2d.txt

echo input1200 >> 2d.txt
mpirun -np 4 ./bin/main data/input1200 >> 2d.txt

echo input1400 >> 2d.txt
mpirun -np 4 ./bin/main data/input1400 >> 2d.txt

echo input1500 >> 2d.txt
mpirun -np 4 ./bin/main data/input1500 >> 2d.txt


echo "Floyd_1d:" >> 2d.txt
echo input40 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input40 >> 2d.txt

echo input80 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input80 >> 2d.txt

echo input160 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input160 >> 2d.txt

echo input220 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input220 >> 2d.txt

echo input320 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input320 >> 2d.txt

echo input420 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input420 >> 2d.txt

echo input520 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input520 >> 2d.txt

echo input620 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input620 >> 2d.txt

echo input720 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input720 >> 2d.txt

echo input920 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input920 >> 2d.txt

echo input1200 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input1200 >> 2d.txt

echo input1400 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input1400 >> 2d.txt

echo input1500 >> 2d.txt
mpirun -np 4 ./bin/floyd_1d data/input1500 >> 2d.txt



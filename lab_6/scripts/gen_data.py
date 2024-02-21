import os

sizes = [i for i in range(50, 1000, 50)]
threads = [2, 3, 4, 5, 6, 7, 8, 9, 10]
types_block = ['blocking', 'nonblocking']
for s in sizes:
    for t in threads:
        for tb in types_block:
            os.system(f'mpiexec -np {t} ../cmake-build-release/Lab6 -s {s} -t {tb}')
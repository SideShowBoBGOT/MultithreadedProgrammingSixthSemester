import os

sizes = [i for i in range(50, 1000, 50)]
threads = [2, 3, 4, 5, 6, 7, 8, 9, 10]
typesBlock = ['blocking', 'nonblocking']
for s in sizes:
    for t in threads:
        for tb in typesBlock:
            os.system(f'mpirun -np {t} ../cmake-build-release/Lab6 {s} {tb}')
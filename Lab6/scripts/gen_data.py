import os

processes = [2, 3, 4, 5, 6]
sizes = [i for i in range(50, 1000, 50)]
types_block = ['blocking', 'nonblocking']
for t in processes:
    for s in sizes:
        for tb in types_block:
            os.system(f'mpiexec -np {t} ../cmake-build-release/lab_6 -s {s} -t {tb}')
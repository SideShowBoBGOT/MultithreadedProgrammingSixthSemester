import os

sizes = [100, 200, 300, 400, 500, 600, 700]
threads = [2, 3, 4]
typesBlock = ['blocking', 'nonblocking']
for s in sizes:
    for t in threads:
        for tb in typesBlock:
            os.system(f'mpirun -np {t} bin {s} {tb}')
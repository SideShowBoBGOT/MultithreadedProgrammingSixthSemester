import os

sizes = [i for i in range(50, 750, 50)]
threads = [2, 3, 4]
typesBlock = ['one_to_many', 'many_to_many']
for s in sizes:
    for t in threads:
        for tb in typesBlock:
            os.system(f'mpirun -np {t} bin {s} {tb}')
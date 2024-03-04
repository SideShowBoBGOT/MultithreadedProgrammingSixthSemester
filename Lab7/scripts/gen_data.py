import os

processes = [3, 4, 5, 6]
sizes = [i for i in range(1000, 2600, 100)]
types_block = ['OneToMany', 'ManyToOne', 'ManyToMany']
for _ in range(5):
    for t in processes:
        for s in sizes:
            for tb in types_block:
                os.system(f'mpiexec -np {t} ../cmake-build-release/lab_7 -s {s} -t {tb}')

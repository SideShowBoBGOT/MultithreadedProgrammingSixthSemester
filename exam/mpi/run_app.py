import os

procs_num = 2
os.system(f'mpiexec -np {procs_num} cmake-build-release/exam_mpi')
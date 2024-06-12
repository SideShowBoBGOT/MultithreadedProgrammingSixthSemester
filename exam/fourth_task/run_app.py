import os
import random

procs_num = 6
numbers = [random.randint(0, 100) for _ in range(20)]
numbers_str = ' '.join([f'{n}' for n in numbers])

print(f'Procs num: {procs_num}')
print(f'numbers: {numbers}')
print('Sum in MPI:')
os.system(f'mpiexec -np {procs_num} cmake-build-release/exam_mpi --vector {numbers_str}')
print()
print(f'Sum in Python: {sum(numbers)}')

# sideshowbobgot@localhost:~/university/MultithreadedProgrammingSixthSemester/exam/fourth_task$ python run_app.py
# Procs num: 2
# numbers: [45, 62, 65, 22, 88, 84, 85, 15, 4, 70, 73, 47, 46, 43, 58, 89, 55, 78, 80, 7]
# Sum in MPI:
# 1116
# Sum in Python: 1116
# sideshowbobgot@localhost:~/university/MultithreadedProgrammingSixthSemester/exam/fourth_task$ python run_app.py
# Procs num: 3
# numbers: [10, 22, 86, 8, 84, 55, 57, 15, 52, 15, 100, 55, 90, 53, 67, 63, 84, 27, 49, 82]
# Sum in MPI:
# 1074
# Sum in Python: 1074
# sideshowbobgot@localhost:~/university/MultithreadedProgrammingSixthSemester/exam/fourth_task$ python run_app.py
# Procs num: 4
# numbers: [56, 62, 79, 41, 53, 53, 68, 96, 97, 37, 66, 84, 67, 78, 49, 23, 89, 47, 27, 83]
# Sum in MPI:
# 1255
# Sum in Python: 1255
# sideshowbobgot@localhost:~/university/MultithreadedProgrammingSixthSemester/exam/fourth_task$ python run_app.py
# Procs num: 5
# numbers: [72, 14, 77, 30, 13, 34, 97, 48, 69, 45, 68, 53, 5, 39, 55, 13, 23, 49, 68, 68]
# Sum in MPI:
# 940
# Sum in Python: 940
# sideshowbobgot@localhost:~/university/MultithreadedProgrammingSixthSemester/exam/fourth_task$

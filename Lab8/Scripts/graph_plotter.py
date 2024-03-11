import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV file
data = pd.read_csv('records.csv')

# Set of unique thread numbers and algorithm types
thread_nums = data['ThreadsNum'].unique()
alg_types = data['AlgType'].unique()

# Determine grid size for the plots
num_thread_nums = len(thread_nums)
grid_size = int(np.ceil(np.sqrt(num_thread_nums)))

rows = 6
cols = 2
figure, axis = plt.subplots(rows, cols, figsize=(20, 15))

for index, thread_num in enumerate(sorted(thread_nums), start=0):
    row = int(index / cols)
    col = index % cols
    ax = axis[row, col]

    filtered_data = data[data['ThreadsNum'] == thread_num]

    # Plot lines for each algorithm type
    for alg_type in alg_types:
        # Filter data for this specific algorithm type
        alg_data = filtered_data[filtered_data['AlgType'] == alg_type]

        # Plot MillisServer and MillisClient for this algorithm type
        ax.plot(alg_data['Size'], alg_data['MillisServer'], marker='o', label=f'{alg_type} - Server')
        ax.plot(alg_data['Size'], alg_data['MillisClient'], marker='x', label=f'{alg_type} - Client')

    # Add titles, labels, and legend
    ax.set_title(f'{thread_num} Threads')
    ax.set_xlabel('Data Size')
    ax.set_ylabel('Time (ms)')

handles, labels = axis[0, 0].get_legend_handles_labels()
figure.legend(handles, labels, loc='upper center')
figure.tight_layout()
figure.savefig(f"stat.png")

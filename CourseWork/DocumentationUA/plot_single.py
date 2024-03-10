import pandas as pd
import matplotlib.pyplot as plt
import json

# Define the json strings
json_data = [
    { "name": "Sequential", "nodes_number": 6250000, "milliseconds": 977 },
    { "name": "Sequential", "nodes_number": 6890625, "milliseconds": 1162 },
    { "name": "Sequential", "nodes_number": 7562500, "milliseconds": 1273 },
    { "name": "Sequential", "nodes_number": 8265625, "milliseconds": 1412 },
    { "name": "Sequential", "nodes_number": 9000000, "milliseconds": 1534 },
    { "name": "Sequential", "nodes_number": 9765625, "milliseconds": 1872 },
    { "name": "Sequential", "nodes_number": 10562500, "milliseconds": 2059 },
    { "name": "Sequential", "nodes_number": 12250000, "milliseconds": 2653 },
    { "name": "Sequential", "nodes_number": 13213225, "milliseconds": 3137 },
    { "name": "Sequential", "nodes_number": 14062500, "milliseconds": 3595 },
    { "name": "Sequential", "nodes_number": 15015625, "milliseconds": 3659 },
    { "name": "Sequential", "nodes_number": 16000000, "milliseconds": 3809 }
]

# Convert JSON data into DataFrame
df = pd.DataFrame(json_data)

# Multiply every value in 'nodes_number' by 2
# Plotting
plt.figure(figsize=(10, 6))
plt.plot(df['nodes_number'], df['milliseconds'], marker='o', linestyle='-', color='b')
plt.xlabel('Nodes Number')
plt.ylabel('Milliseconds')
plt.grid(True)
plt.show()
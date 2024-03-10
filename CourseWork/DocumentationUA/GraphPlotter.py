import pandas as pd
import ast
import os
import matplotlib.pyplot as plt
import json

def main() -> None:
    os.chdir('../DocumentationUA')

    with open('Benchmark.txt', 'r') as file:
        data = list(map(lambda line: json.loads(line[:-1]), file))

    data = pd.DataFrame(data)
    data['size'] = data['size'] * data['size']

    print(data.columns)

    sequential = data[data['name'] == 'Sequential'][['size', 'milliseconds']].groupby(['size']).mean()
    communication = data[data['name'] == 'Communication'][['size', 'threadsNum', 'milliseconds', 'acceleration']].groupby(['size', 'threadsNum']).mean()

    data[data['name'] == 'Communication'].to_csv("communication.txt", index=False)

    rows = 6
    cols = 2

    for param in ['milliseconds', 'acceleration']:
        figure, axis = plt.subplots(rows, cols, figsize=(10, 10))
        for index in range(0, 11):
            threads_num = index + 2
            row = int(index / cols)
            col = index % cols
            ax = axis[row, col]
            sizes = sequential.index.values
            if param == 'milliseconds':
                ax.plot(sizes, sequential[param].values, label='Послідовний BFS')
            else:
                ax.axhline(y=1.2, color='r', linestyle='-', label='Прийнятна межа')
            for (df, name) in [(communication, 'Паралельний BFS')]:
                ax.plot(sizes, df[df.index.get_level_values(1) == threads_num][param], label=name)
            ax.set_title(f'Потоки {threads_num}')
            ax.set_xlabel('Розмір графа')
            if param == 'milliseconds':
                ax.set_ylabel('Час, мс')
            else:
                ax.set_ylabel('Рази')
            ax.set_ylim(ymin=0)
        handles, labels = axis[0, 0].get_legend_handles_labels()
        figure.legend(handles, labels, loc='upper center')
        figure.tight_layout(rect=(0, 0, 1, 0.9))
        figure.savefig(f"{param}.png")

if __name__ == '__main__':
    main()
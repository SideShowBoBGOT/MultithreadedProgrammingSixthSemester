import pandas as pd
import ast
import os
import matplotlib.pyplot as plt


def main() -> None:
    with open('log.txt', 'r') as file:
        data = list(map(lambda line: ast.literal_eval(line), file))

    pd.read_csv('log.txt', sep='\t')
    data = pd.DataFrame(data)

    one_to_many = data[data.alg_type == 'OneToMany'][['mat_size', 'procs_num', 'efficiency']].groupby(
        ['mat_size', 'procs_num']).mean()
    many_to_one = data[data.alg_type == 'ManyToOne'][['mat_size', 'procs_num', 'efficiency']].groupby(
        ['mat_size', 'procs_num']).mean()
    many_to_many = data[data.alg_type == 'ManyToMany'][['mat_size', 'procs_num', 'efficiency']].groupby(
        ['mat_size', 'procs_num']).mean()

    rows = 2
    cols = 2

    figure, axis = plt.subplots(rows, cols, figsize=(10, 10))
    for index in range(0, rows * cols):
        threads_num = index + 3
        row = int(index / cols)
        col = index % cols
        ax = axis[row, col]
        ax.axhline(y=1, color='r', linestyle='-', label='Прийнятна межа')
        for (df, name) in [(one_to_many, 'OneToMany'), (many_to_one, 'ManyToOne'), (many_to_many, 'ManyToMany')]:
            dd = df[df.index.get_level_values(1) == threads_num]
            sizes = dd.index.get_level_values(0).values
            ax.plot(sizes, dd.values, label=name)
        ax.set_title(f'Процеси {threads_num}')
        ax.set_xlabel('Розмір матриці')
        ax.set_ylabel('Рази')
        ax.set_ylim(ymin=0)
    handles, labels = axis[0, 0].get_legend_handles_labels()
    figure.legend(handles, labels, loc='upper center')
    #figure.tight_layout(rect=(0, 0, 1, 0.9))
    figure.savefig(f"efficiency.png")


if __name__ == '__main__':
    main()

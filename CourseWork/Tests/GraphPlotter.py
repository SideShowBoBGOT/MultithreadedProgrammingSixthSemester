import pandas as pd
import ast
import os
import matplotlib.pyplot as plt

def main() -> None:
    with open('Benchmark.txt', 'r') as file:
        data = list(map(lambda line: ast.literal_eval(line), file))
    data = pd.DataFrame(data)
    sequential = data[data.name == 'Sequential'][['size', 'milliseconds']].groupby(['size']).mean()
    shared = data[data.name == 'Shared'][['size', 'threadsNum', 'milliseconds', 'acceleration']].groupby(['size', 'threadsNum']).mean()
    communication = data[data.name == 'Communication'][['size', 'threadsNum', 'milliseconds', 'acceleration']].groupby(['size', 'threadsNum']).mean()
    os.environ["XDG_SESSION_TYPE"] = "xcb"
    sequential.plot().get_figure().savefig("Sequential.png")
    print(sequential)

if __name__ == '__main__':
    main()
import pandas as pd
import ast

def main() -> None:
    with open('../cmake-build-debug/Tests/Benchmark.txt', 'r') as file:
        data = list(map(lambda line: ast.literal_eval(line), file))
    data = pd.DataFrame(data)
    sequential = data[data.name == 'Sequential'][['size', 'milliseconds']].groupby(['size']).mean()
    shared = data[data.name == 'Shared'][['size', 'threadsNum', 'milliseconds', 'acceleration']].groupby(['size', 'threadsNum']).mean()
    communication = data[data.name == 'Communication'][['size', 'threadsNum', 'milliseconds', 'acceleration']].groupby(['size', 'threadsNum']).mean()
    print(communication)

if __name__ == '__main__':
    main()
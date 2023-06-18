import matplotlib.pyplot as plt
import pandas as pd

vals = []
with open('log.log', 'r') as file:
    for line in file.readlines():
        vals.append(line.split('\t'))

df = pd.DataFrame(vals)
df.columns = ['tblock', 'matsize', 'tnum', 'millis']
for col in ['matsize', 'tnum', 'millis']:
    df[col] = df[col].astype('int')

tblock = list(set(df.tblock.to_list()))
tnum = list(set(df.tnum.to_list()))

fig, ax = plt.subplots(1, 1, figsize=(7, 7))
for tb in tblock:
    for tn in tnum:
        dd = df[(df.tblock == tb) & (df.tnum == tn)]
        plt.plot(dd.matsize, dd.millis, label=f'{tb} {tn}')

plt.ylabel('Milliseconds')
plt.xlabel('Matrix size')
plt.legend(loc='upper left')
plt.savefig('plot.png')
#plt.show()



        
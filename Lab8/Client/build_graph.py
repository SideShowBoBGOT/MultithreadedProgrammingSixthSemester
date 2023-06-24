import matplotlib.pyplot as plt
import pandas as pd

vals = []
with open('log.log', 'r') as file:
    for line in file.readlines():
        vals.append(line.split('\t'))

df = pd.DataFrame(vals)
df.columns = ['rtype', 'matsize', 'tnum', 'millis']
for col in ['matsize', 'tnum']:
    df[col] = df[col].astype('int')
    
for col in ['millis']:
    df[col] = df[col].astype('double')

rtype = list(set(df.rtype.to_list()))
tnum = list(set(df.tnum.to_list()))

fig, ax = plt.subplots(1, 1, figsize=(7, 7))
for tb in rtype:
    for tn in tnum:
        dd = df[(df.rtype == tb) & (df.tnum == tn)]
        plt.plot(dd.matsize, dd.millis, label=f'{tb} {tn}')

plt.ylabel('Milliseconds')
plt.xlabel('Matrix size')
plt.legend(loc='upper left')
plt.savefig('plot.png')
#plt.show()



        
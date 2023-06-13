import os
with open('./Lab4/code.txt', 'w') as output:
    for root, dirs, files in os.walk("./Lab4"):
        for file in files:
            if file.endswith('.java'):
                path = root + os.sep + file
                output.write('// ' + path + '\n\n')
                with open(path, 'r') as ff:
                    output.write(''.join(ff.readlines()))
                output.write('\n\n')
            

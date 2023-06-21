import os
with open('./Lab7/code.txt', 'w') as output:
    for root, dirs, files in os.walk("./Lab7"):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.h'):
                path = root + os.sep + file
                output.write('// ' + path + '\n\n')
                with open(path, 'r') as ff:
                    output.write(''.join(ff.readlines()))
                output.write('\n\n')
            

# Automation of Lattice Reduction Process, including try-except for timeout

import subprocess
from time import time

def veclength():

    with open("matrixred", "r") as f:
        line = f.readline()
        line = line.strip('\n')
        line = line.strip('\t')
        line = line.strip(' []')
        vector = [int(x) for x in line.split()]

    norm = 0;
    for i in vector:
        norm += i**2

    norm = norm**(0.5)

    return norm

def reducematrix(mode, n):

    input_file = "svpchallengedim{}seed0.txt".format(n)
    matrixinput = open(input_file)

    args = ['./fplll', '-a']
    modestring = ''
    duration = 0
    for arg in mode:
        args.append(arg)
        modestring += arg

    process= subprocess.Popen(args, stdin=matrixinput, stdout=subprocess.PIPE,
                   cwd='/home/carter/Documents/bin')
    try:
        start = time()
        outs, errs = process.communicate(timeout=30)
        end = time()
        duration = end-start

        with open("matrixred", "w") as f:
            outs = outs.decode('utf-8')
            f.write(outs)

        vector_length = veclength()

        with open("outputlong.txt", "a") as f:
            f.write("Reduction Method: {0:15s} \t Length of first vector: {1:5.8f} \t Runtime: {2:2.15f}\n"\
            .format(modestring, vector_length, duration))

    except subprocess.TimeoutExpired:
        process.kill()

        with open("outputlong.txt", "a") as f:
            f.write("Reduction Method: {0:15s} \t Timed out\n".format(modestring))

def main():
    dimension_list = []
    for i in range(40, 100, 2):
        dimension_list.append(i)


    for n in dimension_list:
        with open("outputlong.txt", "a") as f:
            f.write("\nComparison of Reduction Methods for {} dim lattice\n".format(n))

        reduction_modes = [['lll'], ['bkz', '-b', '20'],
                           ['bkz', '-b', '25'], ['hkz'], ['sld','-b','20'], ['svp']]

        for mode in reduction_modes:
            reducematrix(mode, n)


if __name__ == '__main__':
    main()

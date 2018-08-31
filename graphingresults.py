import re
import matplotlib.pyplot as plt

dimension_list = []
lll_lengths = []
lll_times = []
bkz20_lengths = []
bkz20_times = []
bkz25_lengths = []
bkz25_times = []
hkz_lengths = []
hkz_times = []
sld20_lengths = []
sld20_times = []
svp_lengths = []
svp_times = []

new_dim_line = re.compile('Comparison of Reduction Methods for ([0-9]+) dim lattice')
result_line = re.compile('Reduction Method: ([a-z0-9-]+)\s*Length of first vector: ([0-9\.]*)\s*Runtime: ([0-9\.]*)')
timeout_line = re.compile('Reduction Method: ([a-z0-9-]+)\s*Timed out')

with open('shortoutput.txt', 'r') as f:
    for line in f.readlines():
        first_line = new_dim_line.match(line)
        if first_line:
            dim = first_line.group(1)
            dimension_list.append(dim)
            continue
        result = result_line.match(line)
        timeout = timeout_line.match(line)
        if result:
            method = result.group(1)
            length = float(result.group(2))
            runtime = float(result.group(3))

        if timeout:
            method = timeout.group(1)
            length = 0
            runtime = 0

        if result or timeout:

            if method == 'lll':
                lll_lengths.append(length)
                lll_times.append(runtime)
            elif method == 'bkz-b20':
                bkz20_lengths.append(length)
                bkz20_times.append(runtime)
            elif method == 'bkz-b25':
                bkz25_lengths.append(length)
                bkz25_times.append(runtime)
            elif method == 'hkz':
                hkz_lengths.append(length)
                hkz_times.append(runtime)
            elif method == 'sld-b20':
                sld20_lengths.append(length)
                sld20_times.append(runtime)
            elif method == 'svp':
                svp_lengths.append(length)
                svp_times.append(runtime)
            else:
                print("Unknown method: {}\n".format(method))

plt.figure(1)
plt.plot(dimension_list, bkz20_times, 'r', label='BKZ - 20')
plt.plot(dimension_list, bkz25_times, 'b', label='BKZ-25')
ax = plt.gca()
plt.setp(ax.get_xticklabels()[1::2], visible=False)
plt.suptitle('Runtime of BKZ-20/25 Reduction')
plt.xlabel('Dimension')
plt.ylabel('Runtime (s)')
plt.legend()
plt.show(block=False)

plt.figure(2)
plt.plot(dimension_list, lll_lengths, 'r', label="LLL Length")
plt.plot(dimension_list, bkz20_lengths, 'b', label="BKZ-20 Length")
plt.plot(dimension_list, bkz25_lengths, 'g', label="BKZ-25 Length")
ax = plt.gca()
plt.setp(ax.get_xticklabels()[1::2], visible=False)
plt.suptitle('Length of first vector in reduced bases')
plt.xlabel('Dimension')
plt.ylabel('Vector Length')
plt.legend()
plt.show()

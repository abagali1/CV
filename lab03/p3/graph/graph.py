#!/usr/bin/env python3

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import subprocess

IT = 20
N = [*range(int(1e4), int(1e6),100)] # number of points (x axis)
B = [] # time for brute force 
R = [] # time for recursive 

for i in N:
    subprocess.check_output(f"./a.out {i}", shell=True)
    B_avg, R_avg = [], []
    for j in range(IT):
        out = subprocess.check_output(f"../a.out", shell=True).decode("latin-1").splitlines()
        B_avg.append(int(out[0]))
        R_avg.append(int(out[1]))
    B.append(B_avg)
    R.append(R_avg)

plt.plot(N, [sum(x)/IT for x in B], 'r')
plt.plot(N, [sum(x)/IT for x in R], 'b')
plt.title("# of Points vs. Runtime for Initial and Final Recursive Algorithms")
plt.xlabel('# of Points')
plt.ylabel('Runtime (microseconds)')

plt.legend(handles=[mpatches.Patch(color='red', label='Initial Recursive Algorithm'), mpatches.Patch(color='blue', label='Final Recursive Algorithm')])

plt.show()
#plt.savefig('plot.png')

#!/usr/bin/env python3

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import subprocess


N = [*range(50,1000,100)] # number of points (x axis)
B = [] # time for brute force 
R = [] # time for recursive 

for i in N:
    out = subprocess.check_output(f"../a.out {i}", shell=True).decode("latin-1").splitlines()
    B.append(int(out[0]))
    R.append(int(out[1]))

plt.plot(N, B, 'r')
plt.plot(N, R, 'b')
plt.title("# of Points vs. Runtime for Brute Force and Recursive Algorithms")
plt.xlabel('# of Points')
plt.ylabel('Runtime (microseconds)')

plt.legend(handles=[mpatches.Patch(color='red', label='Brute Force Algorithm'), mpatches.Patch(color='blue', label='Recursive Algorithm')])

plt.savefig('plot.png')

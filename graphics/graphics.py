import os
import matplotlib.pyplot as plt
import numpy as np


plt.rcParams['figure.figsize'] = (10, 7)

file_name = "bench_search.txt"

dots_x, dots_y = list(), list()
with open(file_name) as f:
    for line in f.readlines():
        x, y = line.split()
        dots_x.append(float(x))
        dots_y.append(float(y))

dots_x_new, dots_y_new = list(), list()

dots_x_new.append(dots_x[0])
dots_y_new.append(dots_y[0])

for i in range(1, len(dots_y) - 1):
    if abs(dots_y[i] - (dots_y[i - 1] + dots_y[i + 1]) / 2) <= 10 * abs(dots_y[i - 1] - dots_y[i + 1]) / 2:
        dots_x_new.append(dots_x[i])
        dots_y_new.append(dots_y[i])

dots_x_new.append(dots_x[-1])
dots_y_new.append(dots_y[-1])

dots_y_new = np.asarray(dots_y_new)
for i in range(3):
    dots_y_new[1:-1] = 0.25 * (dots_y_new[:-2] + 2 * dots_y_new[1:-1] + dots_y_new[2:])

plt.plot(dots_x_new, dots_y_new, label='search')

plt.ylabel('Time, mcs')
plt.xlabel('Number of vertex')
plt.title(f'Time of search on graph')

# plt.savefig(os.path.join(graph_path, file_name) + ".png")
# plt.gcf().clear()
plt.legend()
# plt.show()
plt.grid()
plt.savefig(f"graphics_search.png")

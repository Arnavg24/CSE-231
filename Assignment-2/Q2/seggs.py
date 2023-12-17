#!/usr/bin/python3

import sys

import matplotlib.pyplot as plt

arg1 = float(sys.argv[1])
arg2 = float(sys.argv[2])
arg3 = float(sys.argv[3])
print(f"Argument 1: {arg1}")
print(f"Argument 2: {arg2}")
print(f"Argument 3: {arg3}")


data = [arg1, arg2, arg3]

# Configure the dark-optimized theme

process_labels = ["Process FIFO", "Process RR", "Process OTHER"]

# Configure the dark-optimized theme
plt.style.use('dark_background')

# Create a histogram
plt.bar(process_labels, data, color='cyan', alpha=0.7)

# Customize the plot
plt.title('Histogram of Times taken by different scheduling params')
plt.xlabel('Processes')
plt.ylabel('time stamps')

# Display the value labels on top of each bar

for i, v in enumerate(data):
    plt.text(process_labels[i], v + 0.00005,
             f'{v:.6f}', ha='center', va='bottom', fontsize=10)


# Show the plot
plt.show()

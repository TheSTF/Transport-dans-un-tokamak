# scripts/plot_mask.py
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("mask.csv", delimiter=",", skiprows=2)
R = np.unique(data[:, 0])
Z = np.unique(data[:, 1])
M = data[:, 2].reshape(len(Z), len(R))

plt.figure(figsize=(5, 7))
plt.pcolormesh(R, Z, M, shading="nearest", cmap="viridis")
plt.gca().set_aspect("equal")
plt.xlabel("R [m]")
plt.ylabel("Z [m]")
plt.title("Masque plasma (65x65)")
plt.tight_layout()
plt.savefig("mask.png", dpi=150)
from fgen import ndtype_call, dtype_call
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import numpy as np
import sys


def distance(x, y):
    d = np.sqrt(((x - y) ** 2).sum())
    if d == 0.0:
        return sys.float_info.min
    else:
        return d

class NDFunction:
    def __init__(self, M, rho, gamma, t, globl):
        """
        Create an ND-type function with the specified parameters.

        :param M: An m x n matrix with coordinates for local minimizers, including the main paraboloid.
        :type M: m x n double matrix
        :param rho: A vector of length m with radii of local minimizers.
        :type M: float vector of length n
        :param gamma: A vector of length m with peak values.
        :type gamma: float vector of length n
        :param t: Minimum value of the main paraboloid.
        :type t: float
        :param globl: Minimum value of the global minimum.
        :type globl: float

        :returns: an instance of NDFunction.
        """
        self.M = M
        self.rho = rho
        self.gamma = gamma
        self.t = t
        self.globl = globl
        self.m = len(M)
        self.n = len(M[0])

    def __call__(self, x):
        return ndtype_call(self, x)

class DFunction(NDFunction):
    def __call__(self, x):
        return dtype_call(self, x)

def random_ndfunction(n, m, globl, radius, dist):
    """
    Generate an ND-type function with the specified parameters.

    :param n: Dimensionality of the function.
    :type n: int
    :param m: Number of local minimizers.
    :type m: int.
    :param global: Value of the global minimizer.
    :type global: float
    :param radius: Radius of the attraction region of the global minimizer.
    :type radius: float
    :param dist: Distance from the paraboloid vertex to the global minimizer.
    :type dist: float

    :returns: callable -- ND type function with the given parameters and other parameters randomized.
    """
    assert(n >= 2)
    assert(m >= 2)
    T = (np.random.random(n) - 0.5) * 2.0
    phis = np.zeros(n)
    phis[0] = np.random.random() * np.pi
    for i in range(1, n):
        phis[i] = np.random.random() * 2.0 * np.pi
    x_global = np.zeros(n)
    for i in range(n - 1):
        x_global[i] = T[i] + dist * np.cos(phis[i]) * np.array([np.sin(phis[j]) for j in range(i)]).prod()
    x_global[n - 1] = T[n - 1] + dist * np.array([np.sin(phis[j]) for j in range(n - 1)]).prod()
    # let's fix coordinates outside the solution space
    for i in range(n):
        if x_global[i] < -1.0 or x_global[i] > 1.0:
            x_global[i] = 2.0 * T[i] - x_global[i]
    M = [T, x_global]
    for i in range(2, m):
        cont = True
        while cont:
            x_local = (np.random.random(n) - 0.5) * 2.0
            if ((radius + radius) - distance(x_local, x_global)) < 0.0:
                cont = False
                M.append(x_local)
    rho = [0.0, radius]
    for i in range(2, m):
        distances = [distance(M[i], M[j]) for j in range(m) if i != j]
        rho.append(0.5 * min(distances))
    for i in range(2, m):
        distances = [distance(M[i], M[j]) - rho[j] for j in range(m) if i != j]
        rho[i] = max(rho[i], min(distances))
    for i in range(2, m):
        rho[i] *= 0.99
    gamma = [0.0, 0.0]
    for i in range(2, m):
        r1 = np.random.random()
        r2 = np.random.random()
        a1 = rho[i]
        b1 = 2.0 * rho[i]
        a2 = 0.0
        b2 = 0.0 - (-1.0)
        gamma.append(min(a1 + r1 * (b1 - a1), a2 + r2 * (b2 - a2)))
    nd_f = NDFunction(np.array(M), np.array(rho), np.array(gamma), 0.0, -1.0)
    nd_f.minimum = np.array(M[1])
    nd_f.tolerance = rho[1] / 2.0;
    d_f = DFunction(np.array(M), np.array(rho), np.array(gamma), 0.0, -1.0)
    d_f.minimum = np.array(M[1])
    d_f.tolerance = rho[1] / 2.0;
    return (nd_f, d_f)


if __name__ == '__main__':
    f1, f2 = random_ndfunction(n=2, m=10, globl=-1.0, radius=1.0/3.0, dist=2.0/3.0)
    X, Y = np.meshgrid(np.arange(-1.0, 1.0, 0.05), np.arange(-1.0, 1.0, 0.05))
    Z1 = np.array([[f1(np.array([x0, x1])) for x0, x1 in zip(row1, row2)] for row1, row2 in zip(X, Y)])
    Z2 = np.array([[f2(np.array([x0, x1])) for x0, x1 in zip(row1, row2)] for row1, row2 in zip(X, Y)])

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(X, Y, Z1)
    cset = ax.contour(X, Y, Z1, zdir='z', offset=-1.0, cmap=cm.Greys_r, levels=[-1.0 + x / 50.0 * 7 for x in range(50)])
    plt.show()

    ax.plot_surface(X, Y, Z2, rstride=1, cstride=1)
    plt.show()

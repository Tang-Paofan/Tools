import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def read_points(filename):
    points = []
    with open(filename, 'r') as file:
        cnt = 0
        for line in file:
            coords = [float(coord) for coord in line.split()]
            if cnt%1 == 0:
                points.append(coords)
            cnt+=1
    return np.array(points)

def plot_plane_and_points(point, normal, points):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    a, b, c = normal
    x0, y0, z0 = point
    
    d = - (a*x0 + b*y0 + c*z0)
    
    x_min, x_max = points[:, 0].min(), points[:, 0].max()
    y_min, y_max = points[:, 1].min(), points[:, 1].max()
    x, y = np.meshgrid(np.linspace(x_min, x_max, 100), np.linspace(y_min, y_max, 100))
    
    z = (-a*x - b*y - d) / c

    ax.plot_surface(x, y, z, alpha=0.5)

    ax.scatter(points[:, 0], points[:, 1], points[:, 2], color='red')

    ax.set_xlabel('X axis')
    ax.set_ylabel('Y axis')
    ax.set_zlabel('Z axis')

    plt.show()

if __name__ == "__main__":
    # point_on_plane = np.array([-0.885988, 2.67911,  93.0946]) 
    # normal_vector = np.array([ -0.0826476, 0.00724902,  -0.996553])  

    point_on_plane = np.array([-0.0598, -0.4462, 99.5605]) 
    normal_vector = np.array([ 0.165, 0.049, -0.985])  
    
    # point_on_plane = np.array([1.18448,2.6527,  92.3513]) 
    # normal_vector = np.array([-0.337736, 0.0107209, -0.94118])  
    
    points =read_points('in.asc')

    plot_plane_and_points(point_on_plane, normal_vector, points)

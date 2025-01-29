import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def read_ppm(filename):
    with open(filename, 'r') as file:
        # Leer encabezado
        assert file.readline().strip() == "P3"  # Verificar formato PPM
        cols, rows = map(int, file.readline().split())
        max_val = int(file.readline().strip())
        assert max_val == 255  # Verificar rango de color
        
        # Leer valores de píxeles
        data = np.loadtxt(file, dtype=int).reshape(rows, cols, 3)
        
        # Extraer valores originales
        r_channel = data[:, :, 0].astype(float)
        original_matrix = r_channel / 255.0
        
        return original_matrix

# Codigo
filename = "AlgoritmoGenProcedural/MapaDeCalor.ppm"
matrix = read_ppm(filename)

# Definir la matriz de alturas (z)
z = np.array(matrix)

# Crear coordenadas X, Y
x = np.arange(z.shape[1])
y = np.arange(z.shape[0])
X, Y = np.meshgrid(x, y)

# Crear figura 3D
fig = plt.figure(figsize=(6, 6))
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(X, Y, z, cmap='viridis')

plt.show()

Descripción del algoritmo 

El algoritmo se ejecuta en un espacio bidimensional, donde se posicionan varios puntos principales. 
Estos puntos principales deben cumplir con dos condiciones: deben ser al menos dos puntos y tener valores diferentes, de lo contrario sería toda la malla del mismo color. La matriz puede ser de cualquier tamaño, y la cantidad de puntos principales, así como sus valores entre [0,1], pueden ser ajustados según se desee.

A continuación, el algoritmo calcula el valor de todas las casillas que no son puntos principales, utilizando una fórmula matemática. Esta fórmula define el valor de cada casilla como la suma de los valores de los puntos principales multiplicados por su respectivo grado de influencia.

La suma de todos los grados de influencia debe ser igual a 1, ya que corresponde a un porcentaje.

El grado de influencia de un punto principal específico sobre una casilla se calcula de la siguiente manera:
1. Se obtiene la distancia entre el punto principal y la casilla.
2. La distancia se invierte, de modo que los puntos más cercanos tengan una mayor influencia.
3. El valor de influencia de un punto principal se calcula dividiendo la distancia invertida entre la suma total de las distancias invertidas de todos los puntos principales.

distanciaInvertida = (1 / distancia1)
distanciaInvertidaTotal = distanciaInvertida1 + distanaciaInvertida2 + ...
Valor de x casilla = valorCasillaPrincipal1 * (distanciaInvertida1 / distanciaInvertidaTotal) + valorCasillaPrincipal2 * (distanciaInvertida2 / distanciaInvertidaTotal) + ... 

El valor final de la casilla es una combinación ponderada de las influencias de todos los puntos principales, considerando su distancia a la casilla en cuestión.
De este modo, las casillas dependen de la distancia a los puntos principales, y su valor refleja la influencia de dichos puntos según la fórmula descrita.

Finalmente, el proceso de asignación de los puntos principales es clave. Estos puntos pueden ser asignados manualmente o mediante un sistema automatizado. En este caso, se utiliza un sistema basado en semillas, donde a partir de un número dado, se calcula cuántos puntos añadir, así como sus valores y posiciones.

En principio, el código genera un mapa de calor con formato .ppm, el cual ayuda a interpretarlo.
Gracias al script en Python 'Viewer3D.py' podemos generar la representación 3D de la imagen.ppm(MapaDeCalor.ppm).

Finalmente, comprobamos que el algoritmo funciona mejor en cuadrículas de pequeño tamaño, unos ejemplos:

Algoritmo:
![Ejemplo](ejemplos/3D(100x100)1.png) ![Ejemplo](ejemplos/3D(20x20).png) 

Random:
![Ejemplo](ejemplos/Random1.png) 

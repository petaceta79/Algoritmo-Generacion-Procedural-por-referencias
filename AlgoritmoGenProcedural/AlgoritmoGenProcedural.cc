// --Configuracion
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>  // Para srand() y rand()
#include <ctime>    // Para time()
#include <cmath>

using namespace std;

typedef vector< vector<double> > Matriz; 
// Los puntos se rigen en x=horizontal y=vertical
// La matriz [i][j]  i=filas=y=vertical  j=columnas=x=horizontal

// --Definimos structs necesarios
struct Punto {
    int x, y;
};


// --Funciones 
// Escribe la matriz en la cmd
void escribeMatrizCMD(Matriz &M) {
    int f = M.size();
    int c = M[0].size();
    
    cout.setf(ios::fixed);
    cout.precision(2);
    
    for (int i = 0; i < f; ++i) {
        for (int j = 0; j < c; ++j) {
            cout << M[i][j] << "   ";
        }
        cout << endl;
    }
    cout << endl;
}
// Dado la matriz, cordenadas y el valor añada el punto principal 
// M = mapa, PuntosImp = vector con los punots importantes, (x,y) = cordenadas, valor = el valor de la casilla
void annadirPuntoPrincipal(Matriz &M, vector<Punto> &PuntosImp, int x, int y, double valor) {
    M[y][x] = valor;

    Punto puntoNuevo;
    puntoNuevo.x = x;
    puntoNuevo.y = y;
    PuntosImp.push_back(puntoNuevo);
} 
// Dado una seed se genera un numero pseudoaleatorio
int numRandomSeed(int seed) {
    srand(seed);
    return rand();
}
// Dado una seed se genera un numeor pseudoaleatorio [0,1]
double numRandomSeedCompresed(int seed) {
    return (numRandomSeed(seed) % 10000) / 10000.0;
}
// Actualiza a semilla a otra nueva basada en la anterior
void actSeed(int &seed) {
    seed = ((seed * numRandomSeed(seed + 1)) % (200000 + int((50000) * numRandomSeedCompresed(seed + 3)))); 
    ++seed;
}
// Añade utilizando una seed los numeros iniciales ya asignados 
// Devuelve un vector con los puntos importantes
vector<Punto> annadirNumIniciales(Matriz &M, int &seed) {
    int f = M.size() - 1;
    int c = M[0].size() - 1;
    
    vector<Punto> PuntosImpor;
    // annadirPuntoPrincipal(M, PuntosImpor, 0, 0, 1); Para añadir puntos 

    actSeed(seed);

    double porcentajePuntos = (numRandomSeedCompresed(seed) * 20) + 10;
    int numPuntos = ceil(2 + ((f + c) / (2 * porcentajePuntos)));
    actSeed(seed);

    for (int i = 0; i < numPuntos; ++i) {
        int x, y;
        do {
            x = c * numRandomSeedCompresed(seed);
            actSeed(seed);
            y = f * numRandomSeedCompresed(seed + 1);
            actSeed(seed);
        } while (M[y][x] != -1);
        annadirPuntoPrincipal(M, PuntosImpor, x, y, numRandomSeedCompresed(seed));
        actSeed(seed);
        
    }

    return PuntosImpor;
}
// Dado 2 puntos, devuelve la distancia positiva de uno a otro (redondeada)
int dist(int x1, int y1, int x2, int y2) {
    return ceil(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}
// Crea un vector con todas las distancias
vector<int> VectorDist(int x, int y, vector<Punto> &P) {
    int numPuntos = P.size();
    vector<int> Dist(numPuntos);
    for (int i = 0; i < numPuntos; ++i) {
        Dist[i] = dist(x, y, P[i].x, P[i].y);
    }
    return Dist;
}
// Dado un vector devuelve la suma de todo 
double sumarVector(vector<double> &V) {
    int n = V.size();
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result += V[i];
    }
    return result;
}
vector<double> inversorV(vector<int> &V) {
    int n = V.size();
    
    vector<double> invV(n);
    
    for (int i = 0; i < n; ++i) {
        invV[i] = double(1.0/V[i]);
    }
    
    return invV;
}
// Dado un punto, la matriz y un vector con los puntos importantes
// Devuelve el valor que corresponde a ese punto 
double valorDeUnPunto(int x, int y, Matriz &M, vector<Punto> &P) {
    int numPuntos = P.size();
    
    // Crea un vector con todas las distancias
    vector<int> Dist = VectorDist(x, y, P);
    
    // Calcula su valor 
    double result = 0;
    
    vector<double> DistInverso = inversorV(Dist);
    double sumaDeTodas = sumarVector(DistInverso);
    
    for (int i = 0; i < numPuntos; ++i) {
        double denominador = DistInverso[i];
        double divisor = sumaDeTodas;
        result += M[P[i].y][P[i].x] * (denominador/divisor);
    }
    
    return result;
}
void debugPuntosPrincipales(Matriz &M,vector<Punto> &PuntosImpor ) {
    int numPuntos = PuntosImpor.size();
    // Debug posicon de los puntos importantes
    for (int i = 0; i < numPuntos; ++i) cout << PuntosImpor[i].x << ' ' << PuntosImpor[i].y << ' ' << M[PuntosImpor[i].y][PuntosImpor[i].x] << " - ";
    cout << endl;
}
// Dado un mapa con -1, menos x puntos principales y un vector con estos puntos 
// Devuelve el mapa lleno siguiendo el algoritmo
void rellenarMapa(Matriz &M, vector<Punto> &P) {
    int f = M.size();
    int c = M[0].size();
    
    for (int i = 0; i < f; ++i) {
        for (int j = 0; j < c; ++j) {
            if (M[i][j] == -1) {
                M[i][j] = valorDeUnPunto(j, i, M, P);
            }
        }
    }
}
// Creacion del mapa de calor con .ppm
void GeneraMapaDeCalorPPM(const string &filename, const vector< vector<double> > &matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    ofstream file(filename);
    file << "P3\n" << cols << " " << rows << "\n255\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double value = matrix[i][j]; // Valor entre 0 y 1
            int r = static_cast<int>(value * 255); // Rojo
            int g = static_cast<int>((1 - value) * 255); // Verde inverso
            int b = 128; // Azul constante
            file << r << " " << g << " " << b << " ";
        }
        file << "\n";
    }

    file.close();
}


// Codigo principal
int main()
{
    // Valores principales
    int ancho = 15; // modificables
    int alto = 15;
    int seed = time(0); // preferiblemente grande

    vector<Punto> PuntosPrincipales;
    
    // Creacion del mapa
    Matriz Mapa(alto, vector<double>(ancho, -1));
    PuntosPrincipales = annadirNumIniciales(Mapa, seed); // Los puntos principales para el algoritmo (Con los valores ya asignados en la matriz)
    rellenarMapa(Mapa, PuntosPrincipales); // Ejecuta el algorimo
    
    // Representacion
    //debugPuntosPrincipales(Mapa, PuntosPrincipales);
    //escribeMatrizCMD(Mapa); 
    GeneraMapaDeCalorPPM("MapaDeCalor.ppm", Mapa);

    cout << endl;
    cout << "Finalizado" << endl;

}
// --Configuracion
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

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

// Añade utilizando una seed los numeros iniciales ya asignados 
// Devuelve un vector con los puntos importantes
vector<Punto> annadirNumIniciales(Matriz &M, int seed) {
    int f = M.size() - 1;
    int c = M[0].size() - 1;
    
    vector<Punto> PuntosImpor;
    annadirPuntoPrincipal(M, PuntosImpor, 0, 0, 1);
    annadirPuntoPrincipal(M, PuntosImpor, c, 0, 0.15);
    annadirPuntoPrincipal(M, PuntosImpor, 0, f, 0.5);
    annadirPuntoPrincipal(M, PuntosImpor, c, f, 0.25);

    return PuntosImpor;
}
// Dado 2 puntos, devuelve la distancia de manhatan positiva de uno a otro
int distMan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}
// Crea un vector con todas las distancias
vector<int> VectorDist(int x, int y, vector<Punto> &P) {
    int numPuntos = P.size();
    vector<int> ManhatanDist(numPuntos);
    for (int i = 0; i < numPuntos; ++i) {
        ManhatanDist[i] = distMan(x, y, P[i].x, P[i].y);
    }
    return ManhatanDist;
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
    vector<int> ManhatanDist = VectorDist(x, y, P);
    
    // Calcula su valor 
    double result = 0;
    
    vector<double> ManhatanDistInverso = inversorV(ManhatanDist);
    double sumaDeTodas = sumarVector(ManhatanDistInverso);
    
    for (int i = 0; i < numPuntos; ++i) {
        double denominador = ManhatanDistInverso[i];
        double divisor = sumaDeTodas;
        result += M[P[i].y][P[i].x] * (denominador/divisor);
    }
    
    return result;
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
    int ancho = 1000; // modificables
    int alto = 1000;
    vector<Punto> PuntosPrincipales;
    
    // Creacion del mapa
    Matriz Mapa(alto, vector<double>(ancho, -1));
    PuntosPrincipales = annadirNumIniciales(Mapa, 1234);
    rellenarMapa(Mapa, PuntosPrincipales);
    
    //escribeMatrizCMD(Mapa); 

    GeneraMapaDeCalorPPM("MapaDeCalor.ppm", Mapa);

    cout << endl;
    cout << "Finalizado" << endl;

}
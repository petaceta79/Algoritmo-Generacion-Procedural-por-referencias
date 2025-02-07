// --Configuración
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>  // Para srand() y rand()
#include <ctime>    // Para time()
#include <cmath>

using namespace std;

typedef vector< vector<double> > Matriz; 
// Los puntos se rigen en x = horizontal, y = vertical  
// La matriz [i][j]: i = filas = y = vertical, j = columnas = x = horizontal  

// Definimos structs necesarios  
struct Punto {
    int x, y;
};


// --Funciones  
// Escribe la matriz en la consola  
void escribeMatrizCMD(Matriz &M) {
    int f = M.size();
    int c = M[0].size();
    
    cout.setf(ios::fixed);
    cout.precision(2);
    
    for (int i = 0; i < f; ++i) {
        for (int j = 0; j < c; ++j) {
            if (M[i][j] == -1) {
                cout << "nada" << "   ";
            }
            else cout << M[i][j] << "   ";
        }
        cout << endl;
    }
    cout << endl;
}
// Dada la matriz, coordenadas y el valor, añade el punto principal  
// M = mapa, PuntosImp = vector con los puntos importantes, (x, y) = coordenadas, valor = el valor de la casilla  
void annadirPuntoPrincipal(Matriz &M, vector<Punto> &PuntosImp, int x, int y, double valor) {
    M[y][x] = valor;

    Punto puntoNuevo;
    puntoNuevo.x = x;
    puntoNuevo.y = y;
    PuntosImp.push_back(puntoNuevo);
} 
// Dada una seed, se genera un número pseudoaleatorio  
int numRandomSeed(int seed) {
    srand(seed);
    return rand();
}
// Dada una seed, se genera un número pseudoaleatorio [0,1]  
double numRandomSeedCompresed(int seed) {
    return (numRandomSeed(seed) % 10000) / 10000.0;
}
// Actualiza la semilla a otra nueva basada en la anterior  
void actSeed(int &seed) {
    seed = ((seed * numRandomSeed(seed + 1)) % (200000 + int((50000) * numRandomSeedCompresed(seed + 3)))); 
    ++seed;
}
// Añade, utilizando una seed, los números iniciales ya asignados  
// Devuelve un vector con los puntos importantes  
// PuntosPrinciMax = El máximo de puntos principales  
vector<Punto> annadirNumIniciales(Matriz &M, int &seed, int PuntosPrinciMax) {
    int f = M.size() - 1;
    int c = M[0].size() - 1;
    
    vector<Punto> PuntosImpor;
    // annadirPuntoPrincipal(M, PuntosImpor, 0, 0, 1); Para añadir puntos 

    actSeed(seed);

    double porcentajePuntos = numRandomSeedCompresed(seed);
    int numPuntos = ceil(PuntosPrinciMax * porcentajePuntos);
    if (numPuntos < 2) numPuntos = 2;
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
// Dado un vector, devuelve la suma de todo  
double sumarVector(vector<double> &V) {
    int n = V.size();
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result += V[i];
    }
    return result;
}
// Invierte todos los valores del vector  
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
// Sirve para hacer debug de los principales  
void debugPuntosPrincipales(Matriz &M,vector<Punto> &PuntosImpor ) {
    int numPuntos = PuntosImpor.size();
    // Debug posición de los puntos importantes  
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
// Creación del mapa de calor con .ppm  
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
// Crea una casilla sola  
// ancho = lo que mide horizontalmente  
// alto = lo que mide verticalmente  
// seed = el valor del cual parten los números pseudoaleatorios  
// PuntosPrinciMax = El máximo de puntos principales  
Matriz CrearUnaCasillaRandom(int ancho, int alto, int seed, int PuntosPrinciMax ) {    
    actSeed(seed);
    // Creación del mapa  
    Matriz Mapa(alto, vector<double>(ancho, -1));
    vector<Punto> PuntosPrincipales;
    PuntosPrincipales = annadirNumIniciales(Mapa, seed, PuntosPrinciMax); // Los puntos principales para el algoritmo (con los valores ya asignados en la matriz)  
    rellenarMapa(Mapa, PuntosPrincipales); // Ejecuta el algoritmo  

    return Mapa;
}
// Crea una casilla con los puntos personalizados  
// ancho = lo que mide horizontalmente  
// alto = lo que mide verticalmente  
// seed = el valor del cual parten los números pseudoaleatorios  
// Modificar el script para añadir los puntos deseados  
Matriz CreaUnaCasillaPersonalizada(int ancho, int alto, int seed) {    
    actSeed(seed);
    // Creación del mapa  
    Matriz Mapa(alto, vector<double>(ancho, -1));
    vector<Punto> PuntosPrincipales;

    //annadirPuntoPrincipal(Mapa, PuntosPrincipales, "x", "y", "valor[0,1]");
    //annadirPuntoPrincipal(Mapa, PuntosPrincipales, 5, 4, 0.90); // Ejemplo: añade un punto en x = 5, y = 4, de valor 0.90  
    //annadirPuntoPrincipal(Mapa, PuntosPrincipales, 1, 0, 0.15); // Ejemplo: añade un punto en x = 1, y = 0, valor 0.15  
    // La x y y máximas son uno menos, pues empieza en el 0.  

    /*annadirPuntoPrincipal(Mapa, PuntosPrincipales, 0, 0, 0); // Una figura  
    annadirPuntoPrincipal(Mapa, PuntosPrincipales, 0, 29, 1);
    annadirPuntoPrincipal(Mapa, PuntosPrincipales, 29, 0, 1);
    annadirPuntoPrincipal(Mapa, PuntosPrincipales, 29, 29, 0);
    annadirPuntoPrincipal(Mapa, PuntosPrincipales, 15, 15, 0);*/





    rellenarMapa(Mapa, PuntosPrincipales); // Ejecuta el algoritmo  

    return Mapa;
}
// Funciona igual que CrearUnaCasillaRandom, pero dando ya un vector con algunos puntos y sus valores  
Matriz CrearUnaCasillaRandomConVector(int ancho, int alto, int seed, int PuntosPrinciMax, vector<Punto> &PuntsoPrincipales, vector<double> &ValorPUntosPrincipales) {
    // Creación del mapa  
    Matriz Mapa(alto, vector<double>(ancho, -1));
    vector<Punto> PuntosPrincipales;

    PuntosPrincipales = annadirNumIniciales(Mapa, seed, PuntosPrinciMax); // Los puntos principales para el algoritmo (con los valores ya asignados en la matriz)  

    int numPuntosAntes = PuntsoPrincipales.size();
    for (int i = 0; i < numPuntosAntes; ++i) {
        annadirPuntoPrincipal(Mapa, PuntosPrincipales, PuntsoPrincipales[i].x, PuntsoPrincipales[i].y, ValorPUntosPrincipales[i]);
    }

    //escribeMatrizCMD(Mapa); //debug

    rellenarMapa(Mapa, PuntosPrincipales); // Ejecuta el algoritmo  

    //escribeMatrizCMD(Mapa); //debug
    //cout << "---" << endl; //debug

    return Mapa;
}
// Fusiona la matriz grande y la pequeña según  
// ancho = el ancho de la pequeña (la grande es siempre más grande)  
// alto = alto de la pequeña  
// xInicio = cuántas casillas hay detrás de esta horizontalmente  
// yInicio = cuántas casillas hay detrás de esta verticalmente  
void sustituirMatriz(Matriz &Grande, Matriz &Pequenna, int ancho, int alto, int xInicio, int yInicio) {
    for (int x = 0; x < ancho; ++x) {
        for (int y = 0; y < alto; ++y) {
            Grande[y + (yInicio * alto)][x + (xInicio * ancho)] = Pequenna[y][x];
        }
    }
}
// Crea un mapa relacionando cada una de sus casillas, pero siendo independientes a la vez  
// NumeroInfluenciaCasillasLaterales = cada cuánto coge un valor de la casilla anterior  
// casillasHorizon = número de casillas horizontales por cada casilla  
// casillasVertical = número de casillas verticales por cada casilla  
// AnchoCasilla = lo que mide horizontalmente  
// AltoCasilla = lo que mide verticalmente  
// Seed = el valor del cual parten los números pseudoaleatorios  
// PuntosPrinciMax = el máximo de puntos principales, sin contar los añadidos para continuar la otra casilla  
Matriz CrearMapaRandom(int NumeroInfluenciaCasillasLaterales, int casillasHorizon, int casillasVertical, int anchoCasilla, int altoCasilla, int seed, int PuntosPrinciMax) {
    Matriz Mapa(casillasVertical * altoCasilla, vector<double>(casillasHorizon * anchoCasilla, -1));
    
    actSeed(seed);

    for (int i = 0; i < casillasVertical; ++i) {
        for (int j = 0; j < casillasHorizon; ++j) {
            vector<Punto> PuntosAAnnadir(0);
            vector<double> ValorDeLosPuntos(0);    

            // Si arriba o a la izquierda hay alguna otra casilla, se añaden valores para nivelarlo  
            if (i > 0) { // hay arriba
                for (int a = 0; a < anchoCasilla; a += NumeroInfluenciaCasillasLaterales) {
                    Punto PuntoNuevo;
                    PuntoNuevo.x = a;
                    PuntoNuevo.y = 0;
                    PuntosAAnnadir.push_back(PuntoNuevo);
                    ValorDeLosPuntos.push_back(Mapa[(altoCasilla * i) - 1][a + (anchoCasilla * j)]);
                }
            }
            if (j > 0) { // hay a la izquierda
                for (int a = 0; a < altoCasilla; a += NumeroInfluenciaCasillasLaterales) {
                    Punto PuntoNuevo;
                    PuntoNuevo.x = 0;
                    PuntoNuevo.y = a;
                    PuntosAAnnadir.push_back(PuntoNuevo);
                    ValorDeLosPuntos.push_back(Mapa[(altoCasilla * i) + a][(anchoCasilla * j) - 1]);
                }
            }


            Matriz casillaMomentania = CrearUnaCasillaRandomConVector(anchoCasilla, altoCasilla, seed, PuntosPrinciMax, PuntosAAnnadir, ValorDeLosPuntos);

            sustituirMatriz(Mapa, casillaMomentania, anchoCasilla, altoCasilla, j, i);

            actSeed(seed);
        }
    }

    return Mapa;
}
// Devuelve la media de los valores alrededor del punto  
double calculaMediaAlRededor(Matriz &M, int i, int j) {
    int f = M.size();
    int c = M[0].size();

    double result = 0;
    int numValores = 0;

    for (int a = i - 1; a < i + 1; ++a) {
        for (int b = j - 1; b < j + 1; ++b) {
            if (a >= 0 && b >= 0 && a < f && b < c && (a != i || b != j)) {
                ++ numValores;
                result += M[a][b];
            }
        }
    }

    return result / numValores;
}
// Asigna el valor según el factor de reducción  
double asignarValorSegunReduccion(double val, double avg, int factorReductor) {
    return (avg + val * factorReductor) / (factorReductor + 1);
}
// Analiza todos los valores contiguos y resta/suma la diferencia entre el factorReductor para aproximarlos  
// factorReductor = cuánta importancia tiene el pico al calcular su nuevo valor (cuanto mayor sea, más importancia)  
// diffMax = es la diferencia máxima a partir de la cual se aplica el filtro  
// vueltasDeSuavizado = cuántas vueltas máximo hace de suavizado  
void filtroReducirPorDiferencias(Matriz &M, int factorReductor, float diffMax, int vueltasDeSuavizado) {
    int f = M.size();
    int c = M[0].size();

    bool minimoUnCambio = false;

    do {
        minimoUnCambio = false;
        for (int i = 0; i < f; ++i) {
            for (int j = 0; j < c; ++j) {           
                double avg = calculaMediaAlRededor(M, i, j);
                if (abs(M[i][j] - avg) >= diffMax) {
                    M[i][j] = asignarValorSegunReduccion(M[i][j], avg, factorReductor);
                    minimoUnCambio = true;
                }
            }
        }
        --vueltasDeSuavizado;
    } while (minimoUnCambio && vueltasDeSuavizado > 0);  
}





// Código principal (explicación en el GitHub)  
int main()
{
    // Valores principales modificables  
    int ancho = 10; 
    int alto = 10;
    int MaximoPuntosPrincipales = 4;
    int seed = 86756767; // Preferiblemente grande  
    // Solo para cuando creas un mapa con varias casillas  
    int NumeroCasillasHorizonotales = 3; 
    int NumeroCasillasVerticales = 3;
    int NumeroInfluenciaCasillasLaterales = 3; // Cada cuánto mira un bloque de las casillas contiguas  


    // Generar mapa
    //Matriz Mapa = CrearUnaCasillaRandom(ancho, alto, seed, MaximoPuntosPrincipales);
    //Matriz Mapa = CreaUnaCasillaPersonalizada(ancho, alto, seed); // modificar funcion para annadir los deseados 
    Matriz Mapa = CrearMapaRandom(NumeroInfluenciaCasillasLaterales, NumeroCasillasHorizonotales, NumeroCasillasVerticales, ancho, alto, seed, MaximoPuntosPrincipales);


    // Filtro suavizador (cuando hay mucha diferencia, reduce la distancia) Leer especificaciones de la función  
    filtroReducirPorDiferencias(Mapa, 2, 0.05, 1000); // Reducir picos  


    // Representación  
    //debugPuntosPrincipales(Mapa, PuntosPrincipales);
    //escribeMatrizCMD(Mapa); 
    GeneraMapaDeCalorPPM("MapaDeCalor.ppm", Mapa);

    cout << endl;
    cout << "Seed: " << seed << endl;
    cout << "Finalizado" << endl;

}

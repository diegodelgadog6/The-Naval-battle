#include <iostream>
#include <string>

using namespace std;

class Nave {
private:
    string tipo;
    int tamaño;
    int posiciones[10][2];
    int partesRestantes;

public:
    Nave(string tipo, int tamaño) {
        this->tipo = tipo;
        this->tamaño = tamaño;
        partesRestantes = tamaño;
    }

    void setPosicion(int index, int x, int y) {
        if (index >= 0 && index < tamaño) {
            posiciones[index][0] = x;
            posiciones[index][1] = y;
        }
    }

    bool recibirAtaque(int x, int y) {
        for (int i = 0; i < tamaño; i++) {
            if (posiciones[i][0] == x && posiciones[i][1] == y) {
                posiciones[i][0] = -1;
                posiciones[i][1] = -1;
                partesRestantes--;
                return true;
            }
        }
        return false;
    }

    int getPartesRestantes() { return partesRestantes; }
    string getTipo() { return tipo; }
};

class Tablero {
private:
    char mapa[10][10];

public:
    Tablero() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                mapa[i][j] = '~';
            }
        }
    }

    void mostrarTablero() {
        cout << "   ";
        for (int j = 0; j < 10; j++) {
            cout << j << " ";
        }
        cout << endl;

        for (int i = 0; i < 10; i++) {
            cout << i << "  ";
            for (int j = 0; j < 10; j++) {
                cout << mapa[i][j] << " ";
            }
            cout << endl;
        }
    }

    void marcarCelda(int x, int y, char simbolo) {
        mapa[x][y] = simbolo;
    }

    char getCelda(int x, int y) { return mapa[x][y]; }
};

class Jugador {
private:
    string nombre;
    Tablero tablero;
    Nave barcos[3];

public:
    Jugador(string nombre) : nombre(nombre),
        barcos{Nave("Acorazado", 6), Nave("Submarino", 5), Nave("Lancha", 4)} {}

    void colocarBarcos() {
        barcos[0].setPosicion(0, 0, 0);
        barcos[0].setPosicion(1, 0, 1);
        barcos[0].setPosicion(2, 0, 2);
        barcos[0].setPosicion(3, 0, 3);
        barcos[0].setPosicion(4, 0, 4);
        barcos[0].setPosicion(5, 0, 5);

        barcos[1].setPosicion(0, 2, 2);
        barcos[1].setPosicion(1, 3, 2);
        barcos[1].setPosicion(2, 4, 2);
        barcos[1].setPosicion(3, 5, 2);
        barcos[1].setPosicion(4, 6, 2);

        barcos[2].setPosicion(0, 8, 0);
        barcos[2].setPosicion(1, 8, 1);
        barcos[2].setPosicion(2, 8, 2);
        barcos[2].setPosicion(3, 8, 3);
    }

    bool atacar(int x, int y) {
        for (int i = 0; i < 3; i++) {
            if (barcos[i].recibirAtaque(x, y)) {
                tablero.marcarCelda(x, y, 'X');
                cout << "Impacto!" << endl;
                if (barcos[i].getPartesRestantes() == 0) {
                    cout << barcos[i].getTipo() << " se ha hundido!" << endl;
                }
                return true;
            }
        }

        tablero.marcarCelda(x, y, 'O');
        cout << "Fallaste!" << endl;
        return false;
    }

    Tablero& getTablero() { return tablero; }

    bool todosBarcosHundidos() {
        for (int i = 0; i < 3; i++) {
            if (barcos[i].getPartesRestantes() > 0) {
                return false;
            }
        }
        return true;
    }
};

class Referee {
public:
    bool verificarVictoria(Jugador& jugador1, Jugador& jugador2) {
        if (jugador1.todosBarcosHundidos()) {
            cout << "El Jugador 2 ha ganado!" << endl;
            return true;
        }
        if (jugador2.todosBarcosHundidos()) {
            cout << "El Jugador 1 ha ganado!" << endl;
            return true;
        }
        return false;
    }
};

class Juego {
private:
    Jugador jugador1;
    Jugador jugador2;
    Referee referee;

public:
    Juego(string nombre1, string nombre2) : jugador1(nombre1), jugador2(nombre2) {}

    void inicializar() {
        jugador1.colocarBarcos();
        jugador2.colocarBarcos();
    }

    void jugar() {
        int turno = 1;

        while (true) {
            Jugador& atacante = (turno == 1) ? jugador1 : jugador2;
            Jugador& defensor = (turno == 1) ? jugador2 : jugador1;

            cout << "\nTurno del Jugador " << turno << endl;
            defensor.getTablero().mostrarTablero();

            int x, y;
            bool ataqueExitoso = false;

            while (!ataqueExitoso) {
                cout << "Ingresa las coordenadas del ataque (x y): ";
                cin >> x >> y;

                if (x < 0 || x >= 10 || y < 0 || y >= 10) {
                    cout << "Coordenadas fuera del tablero. Intenta de nuevo!" << endl;
                    continue;
                }

                char estadoCelda = defensor.getTablero().getCelda(x, y);
                if (estadoCelda == 'X' || estadoCelda == 'O') {
                    cout << "Ya atacaste esta posicion! Intenta de nuevo!" << endl;
                    continue;
                }

                ataqueExitoso = true;
                defensor.atacar(x, y);
            }

            if (referee.verificarVictoria(jugador1, jugador2)) {
                break;
            }

            turno = (turno == 1) ? 2 : 1;
        }
    }
};

int main() {
    cout << "=========================================" << endl;
    cout << "             JUEGO DE BATALLA NAVAL      " << endl;
    cout << "=========================================" << endl;
    cout << "1. El tablero es de 10x10." << endl;
    cout << "2. Los barcos tienen diferentes longitudes:" << endl;
    cout << "   - Acorazado: 6 casillas" << endl;
    cout << "   - Submarino: 5 casillas" << endl;
    cout << "   - Lancha: 4 casillas" << endl;
    cout << "3. Cada jugador atacara por turnos." << endl;
    cout << "4. Para atacar, introduce las coordenadas (x, y) separadas por un espacio." << endl;
    cout << "=========================================" << endl;

    Juego juego("Jugador 1", "Jugador 2");
    juego.inicializar();
    juego.jugar();

    return 0;
}
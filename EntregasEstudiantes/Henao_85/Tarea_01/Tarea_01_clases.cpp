#include <iostream>
#include <cmath>
#include<random>
#include <cstdlib>
#include <ctime>
#include <vector>



using namespace std;

const int N = 10;         // Número de partículas
const double R = 1.0;     // Radio de la circunferencia
const double DELTA = 0.1; // Rango de perturbación aleatoria


std::mt19937_64& global_rng() {
    static std::mt19937_64 rng{ /*semilla*/ };
    return rng;
}

double random_number(double a, double b) {
    std::uniform_real_distribution<double> dist(a, b);
    return dist(global_rng());
}

// Clase Partícula
class Particula {
private:
    double x, y;      // Posición
    double fx, fy;    // Fuerza

public:
    Particula() : x(0), y(0), fx(0), fy(0) {}

    void init_configuracion() {
        double theta = random_number(0, 2 * M_PI);
        x = cos(theta);
        y = sin(theta);
    }

    void perturbar(double delta) {
        double dx = random_number(-delta, delta);
        double dy = random_number(-delta, delta);
        x += dx;
        y += dy;
    }

    void asignar_fuerza() {
        fx = random_number(0, 1);
        fy = random_number(0, 1);
    }

    double get_x() const { return x; }
    double get_y() const { return y; }
    double get_fx() const { return fx; }
    double get_fy() const { return fy; }
};

// Clase Sistema
class Sistema {
private:
    std::vector<Particula> p;

public:
    Sistema(int N) : p(N) {}

    void inicializar() {
        for (auto& part : p)
            part.init_configuracion();
    }

    void perturbar_todas(double delta) {
        for (auto& part : p)
            part.perturbar(delta);
    }

    void asignar_fuerzas() {
        for (auto& part : p)
            part.asignar_fuerza();
    }

    double distancia_maxima() {
        double max_dist = 0.0;
        for (int i = 0; i < p.size(); ++i) {
            for (int j = i + 1; j < p.size(); ++j) {
                double dx = p[i].get_x() - p[j].get_x();
                double dy = p[i].get_y() - p[j].get_y();
                double d = sqrt(dx * dx + dy * dy);
                if (d > max_dist)
                    max_dist = d;
            }
        }
        return max_dist;
    }

    void fuerza_total(double& fx_total, double& fy_total, double& magnitud) {
        fx_total = fy_total = 0.0;
        for (const auto& part : p) {
            fx_total += part.get_fx();
            fy_total += part.get_fy();
        }
        magnitud = std::sqrt(fx_total * fx_total + fy_total * fy_total);
    }

    void imprimir_posiciones() {
        for (int i = 0; i < p.size(); ++i) {
            cout << "Partícula " << i
                << " -> (" << p[i].get_x() << ", " << p[i].get_y() << ")\n";
        }
    }
};

// Función principal
int main() {
    Sistema sistema(N);

    sistema.inicializar();
    sistema.perturbar_todas(DELTA);
    sistema.asignar_fuerzas();

    std::cout << "Posiciones después de la perturbación:\n";
    sistema.imprimir_posiciones();

    double dmax = sistema.distancia_maxima();
    std::cout << "Distancia máxima entre partículas: " << dmax << endl;

    double fx_total, fy_total, magnitud;
    sistema.fuerza_total(fx_total, fy_total, magnitud);
    std::cout << "Fuerza total:\n"
              << "  Fx = " << fx_total << "\n"
              << "  Fy = " << fy_total << "\n"
              << "  |F| = " << magnitud << endl;

    return 0;
}

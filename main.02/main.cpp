// ****** Inclusione delle librerie e dei file header necessari ******
#include "include/Caserma.hpp" // Per la classe Caserma
#include "include/Personale.hpp" // Per la classe Personale
#include "include/Mezzo.hpp" // Per le classi MezzoBase, MezzoTerrestre, MezzoAereo
#include "include/Risorsa.hpp" // Per CasermaException
#include "include/Colori.hpp" // Per i colori ANSI
// *******************************************************************


// ****** Inclusione delle librerie standard ******
#include <iostream> // Per input/output
#include <limits> // Per std::numeric_limits
#include <cstdlib> // Per rand() e srand()
#include <string> // Per std::string
#include <ctime>   // Per time()
#include <chrono> // Per std::chrono
#include <thread> // Per std::this_thread::sleep_for
// ************************************************


// ****** Dichiarazione delle funzioni ******
void menu();
Grado scegliGrado();
TipoMissione scegliTipoMissione();
MezzoBase* creaMezzo(int id);
void BarraCaricamento();
// ******************************************


// ****** Funzione principale ******
int main() {

    std::cout << BOLD << RED << "\n\nAccesso al database in corso...\n" << RESET;
    BarraCaricamento (); // Simula un caricamento
    std::cout << BOLD << GREEN << "\nAccesso completato!\n" << RESET << std::endl;

    srand(time(0)); // Inizializza il generatore di numeri casuali

    Caserma caserma; // Istanza della caserma
    int scelta = 0; // Variabile per la scelta del menu

    do {
            menu();
            std::cout << GREEN << BOLD << "\nScelta: " << RESET;
            std::cin >> scelta;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw CasermaException("Input non valido. Inserisci un numero.");
            }

            switch (scelta) {
                case 1: {
                    std::string nome;
                    std::cout << "Inserisci nome del militare: ";
                    std::cin.ignore();
                    std::getline(std::cin, nome);
                    Grado grado = scegliGrado();
                    int id = rand() % 1000 + 1;
                    caserma.aggiungiPersonale(Personale(id, nome, grado));
                    std::cout << "Personale aggiunto con ID " << id << "\n";
                    break;
                }

                case 2: {
                    int id = rand() % 1000 + 1;
                    caserma.aggiungiMezzo(creaMezzo(id));
                    std::cout << "Mezzo aggiunto con ID " << id << "\n";
                    break;
                }

                case 3: {
                    std::string descrizione;
                    TipoMissione tipo = scegliTipoMissione();
                    std::cout << "Descrizione missione: ";
                    std::cin.ignore();
                    std::getline(std::cin, descrizione);

                    caserma.mostraPersonale();
                    std::cout << "Inserisci ID del personale da assegnare (termina con -1): ";
                    std::vector<int> idPersonale;
                    int idp;
                    while (std::cin >> idp && idp != -1)
                        idPersonale.push_back(idp);

                    caserma.mostraMezzi();
                    std::cout << "Inserisci ID dei mezzi da assegnare (termina con -1): ";
                    std::vector<int> idMezzi;
                    int idm;
                    while (std::cin >> idm && idm != -1)
                        idMezzi.push_back(idm);

                    caserma.creaMissione(tipo, descrizione, idPersonale, idMezzi);
                    std::cout << "Missione creata con successo!\n";
                    break;
                }

                case 4:
                    caserma.mostraPersonale();
                    break;

                case 5:
                    caserma.mostraMezzi();
                    break;

                case 6:
                    caserma.mostraMissioni();
                    break;

                case 0:
                    std::cout << BOLD << RED << "\nLogout in corso...\n" << RESET;
                    BarraCaricamento (); // Simula un caricamento
                    std::cout << BOLD << GREEN << "\nLogout completato!\n" << RESET << std::endl;
                    break;

                default:
                    std::cout << RED << "Scelta non valida!\n" << RESET;
            }

    } while (scelta != 0);

    return 0;
}
// ***********************************************


// ****** Funzione menu ******
void menu() {
    std::cout << "\n--------- " << BOLD << YELLOW << "GESTIONE CASERMA" << RESET << " ---------" << std::endl;
    std::cout << BLUE << BOLD << "| 1." << RESET << " Aggiungi personale" << std::endl;
    std::cout << BLUE << BOLD <<"| 2." << RESET << " Aggiungi mezzo" << std::endl;
    std::cout << BLUE << BOLD << "| 3." << RESET<< " Crea missione" << std::endl;
    std::cout << BLUE << BOLD << "\n| 4." << RESET << " Mostra personale" << std::endl;
    std::cout << BLUE << BOLD << "| 5." << RESET << " Mostra mezzi" << std::endl;
    std::cout << BLUE << BOLD << "| 6." << RESET << " Mostra missioni" << std::endl;
    std::cout << BLUE << BOLD << "\n| 0." << RESET << " Esci" << std::endl;
}
// ********************************


// ****** Funzione scegliGrado ******
Grado scegliGrado() {
    int g;
    std::cout << BOLD << YELLOW << "\nScegli grado:\n" << RESET;
    std::cout << "1. Soldato\n2. Caporale\n3. Sergente\n4. Tenente\n5. Capitano\n6. Maggiore\n";
    std::cout << BOLD << GREEN << "\nScelta: " << RESET;
    std::cin >> g;

    switch (g) {
        case 1: return Grado::SOLDATO;
        case 2: return Grado::CAPORALE;
        case 3: return Grado::SERGENTE;
        case 4: return Grado::TENENTE;
        case 5: return Grado::CAPITANO;
        case 6: return Grado::MAGGIORE;
        default: return Grado::SOLDATO;
    }
}
// **********************************


// ****** Funzione scegliTipoMissione ******
TipoMissione scegliTipoMissione() {
    int t;
    std::cout << BOLD << "Scegli tipo di missione:\n" << RESET;
    std::cout << BLUE << BOLD << "| 1." << RESET << " Scorta (min 3 mezzi)\n";
    std::cout << BLUE << BOLD << "| 2." << RESET << " Assalto (max 1 mezzo ogni 5 persone)\n";
    std::cout << BLUE << BOLD << "| 3." << RESET << " Estrazione (min 2 mezzi terrestri e 5 persone)\n";
    std::cout << BOLD << GREEN << "Scelta: " << RESET;
    std::cin >> t;

    switch (t) {
        case 1: return TipoMissione::SCORTA;
        case 2: return TipoMissione::ASSALTO;
        case 3: return TipoMissione::ESTRAZIONE;
        default: return TipoMissione::SCORTA;
    }
}
// **********************************


// ****** Funzione creaMezzo ******
MezzoBase* creaMezzo(int id) {
    int t;
    std::string tipo;
    int min, max;
    bool valido = false;

    std::cout << "Scegli tipo di mezzo:\n";
    std::cout << "1. Terrestre\n2. Aereo\n";
    std::cout << GREEN << BOLD << "\nScelta: " << RESET;
    std::cin >> t;

    std::cout << "Inserisci nome/tipo di mezzo (es: Jeep, Elicottero): ";
    std::cin.ignore();
    std::getline(std::cin, tipo);

    // Ciclo di convalida per min e max
    while (!valido) {
        std::cout << "Inserisci equipaggio minimo (2-6): ";
        std::cin >> min;
        std::cout << "Inserisci equipaggio massimo (2-6): ";
        std::cin >> max;

        if (min < 2 || max > 6 || min > max) {
            std::cerr << RED << BOLD << "\nERRORE: Requisiti non validi!" << RESET << std::endl;
            std::cerr << "Ricorda: minimo >= 2, massimo <= 6, minimo <= massimo\n" << std::endl;
        } else {
            valido = true;
        }
    }

    if (t == 1) {
        return new MezzoTerrestre(id, tipo, min, max);
    } else if (t == 2) {
        return new MezzoAereo(id, tipo, min, max);
    } else {
        std::cerr << "Tipo di mezzo non valido. Creato Mezzo Terrestre di default.\n";
        return new MezzoTerrestre(id, tipo, 2, 4);
    }
}
// **********************************


// ****** Funzione BarraCaricamento ******
void BarraCaricamento (){
    int progress = 0;
    int total_steps = 100;

    for (progress = 0; progress <= total_steps; ++progress) {
        // Calcola la percentuale e la lunghezza della barra
        int bar_width = 40;
        float percentage = (float)progress / total_steps;
        int filled_width = (int)(bar_width * percentage);

        // Costruisci la stringa della barra di progresso
        std::string bar = "[";
        for (int i = 0; i < bar_width; ++i) {
            if (i < filled_width) {
                bar += "=";
            } else if (i == filled_width) {
                bar += ">";
            } else {
                bar += " ";
            }
        }
        bar += "]";

        // Stampa la barra e la percentuale sulla stessa riga usando '\r'
        std::cout << "\r" << bar << " " << (int)(percentage * 100) << "%";
        
        // Forza l'output immediato sul terminale
        std::cout.flush();

        // Simula un'operazione che richiede tempo (ritardo)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
// ***giori
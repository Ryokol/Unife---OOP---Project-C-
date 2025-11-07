#include "include/Caserma.hpp"
#include "include/Personale.hpp"
#include "include/Mezzo.hpp"
// #include "include/Risorsa.hpp" // Per CasermaException
#include <iostream>
#include <limits>
#include <cstdlib> // Per rand() e srand()
#include <ctime>   // Per time()
#include <chrono>   // Per std::chrono::milliseconds
#include <thread> 
#include <string>

const std::string RESET = "\x1b[0m";
const std::string RED = "\x1b[31m";
const std::string GREEN = "\x1b[32m";
const std::string YELLOW = "\x1b[33m";
const std::string CYAN = "\x1b[36m";
const std::string BOLD = "\x1b[1m";

void menu();
Grado scegliGrado();
TipoMissione scegliTipoMissione();
std::shared_ptr<MezzoBase> creaMezzo(int id);
void BarraCaricamento ();

int main() {
    int progress = 0;
    int total_steps = 100;

    std::cout << BOLD << RED << "\n\nAccesso al database in corso...\n" << RESET;
    BarraCaricamento ();
    std::cout << BOLD << GREEN << "\nAccesso completato!\n" << RESET << std::endl;

    srand(time(0)); // numero casuale iniziale

    Caserma caserma;
    int scelta = 0;

    do {
        try {
            menu();
            std::cout << BOLD << GREEN << "\nScelta: " << RESET;
            std::cin >> scelta;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw CasermaException(RED + "Input non valido. Inserisci un numero." + RESET);
            }

            switch (scelta) {
                case 1: {
                    std:: cout << " ------" << BOLD << YELLOW <<" AGGIUNGI PERSONALE " << RESET << "------ \n";
                    std::string nome;
                    std::cout << "\nInserisci nome del militare: ";
                    std::cin.ignore();
                    std::getline(std::cin, nome);
                    Grado grado = scegliGrado();
                    int id = rand() % 1000 + 1;
                    caserma.aggiungiPersonale(std::make_shared<Personale>(id, nome, grado));
                    std::cout << "Personale aggiunto con " << BOLD << GREEN << "ID " << id << RESET << "\n";
                    break;
                }

                case 2: {
                    int id = rand() % 1000 + 1;
                    caserma.aggiungiMezzo(creaMezzo(id));
                    std::cout << "Mezzo aggiunto con " << BOLD << GREEN << "ID " << id << RESET << "\n";
                    break;
                }

                case 3: {
                    std:: cout << " ------" << BOLD << YELLOW <<" INIZIA MISSIONE " << RESET << "------ \n";
                    std::string descrizione;
                    TipoMissione tipo = scegliTipoMissione();
                    std::cout << BOLD << "Descrizione missione: " << RESET;
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
                    std:: cout << " ------" << BOLD << YELLOW <<" LISTA PERSONALE " << RESET << "------ \n";
                    caserma.mostraPersonale();
                    break;

                case 5:
                    std:: cout << " ------" << BOLD << YELLOW <<" LISTA MEZZI " << RESET << "------ \n";
                    caserma.mostraMezzi();
                    break;

                case 6:
                    std:: cout << " ------" << BOLD << YELLOW <<" LISTA MISSIONI " << RESET << "------ \n";
                    caserma.mostraMissioni();
                    break;

                case 0:
                    std::cout << BOLD << RED << "\n\nUscita in corso...\n" << RESET;
                    BarraCaricamento ();
                    std::cout << BOLD << GREEN << "\nProgramma terminato!\n\n" << RESET << std::endl;

    srand(time(0)); // numero casuale iniziale
                    break;

                default:
                    std::cout << "Scelta non valida!\n";
            }
        } catch (const CasermaException& e) {
            std::cerr << BOLD << RED << "\nERRORE: "<< RESET << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "\nERRORE GENERICO: " << RESET << e.what() << "\n";
        }

    } while (scelta != 0);

    return 0;
}

void menu() {
    std::cout << "\n========== " << BOLD << YELLOW << "GESTIONE CASERMA REFACTORING" << RESET << " ==========" << std::endl;
    std::cout << "1." << " Aggiungi personale" << std::endl;
    std::cout << "2." << " Aggiungi mezzo" << std::endl;
    std::cout << "3." << " Crea missione" << std::endl;
    std::cout << "4." << " Mostra personale" << std::endl;
    std::cout << "5." << " Mostra mezzi" << std::endl;
    std::cout << "6." << " Mostra missioni" << std::endl;
    std::cout << "0. Esci" << std::endl;
}

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

TipoMissione scegliTipoMissione() {
    int t;
    std::cout << BOLD << "Scegli tipo di missione:\n" << RESET;
    std::cout << "1. Scorta (min 3 mezzi)\n";
    std::cout << "2. Assalto (max 1 mezzo ogni 5 persone)\n";
    std::cout << "3. Estrazione (min 2 mezzi terrestri e 5 persone)\n";
    std::cout << BOLD << GREEN << "Scelta: " << RESET;
    std::cin >> t;

    switch (t) {
        case 1: return TipoMissione::SCORTA;
        case 2: return TipoMissione::ASSALTO;
        case 3: return TipoMissione::ESTRAZIONE;
        default: return TipoMissione::SCORTA;
    }
}

std::shared_ptr<MezzoBase> creaMezzo(int id) {
    int t;
    std::string tipo;
    int min, max;

    std:: cout << " ------" << BOLD << YELLOW << " AGGIUNGI MEZZO " << RESET << "------ \n";
    std::cout << "Scegli tipo di mezzo:\n";
    std::cout << "1. Terrestre\n2. Aereo\n";
    std::cout << BOLD << GREEN <<"Scelta: " << RESET;
    std::cin >> t;

    std::cout << "Inserisci nome/tipo di mezzo (es: Jeep, Elicottero): ";
    std::cin.ignore();
    std::getline(std::cin, tipo);

    std::cout << "Inserisci equipaggio minimo (2-6): ";
    std::cin >> min;
    std::cout << "Inserisci equipaggio massimo (2-6): ";
    std::cin >> max;

    if (t == 1) {
        return std::make_shared<MezzoTerrestre>(id, tipo, min, max);
    } else if (t == 2) {
        return std::make_shared<MezzoAereo>(id, tipo, min, max);
    } else {
        std::cerr << BOLD << RED << "Errore... Processo Annullato.\n" << RESET << std:: endl;
        return nullptr;
    }
}

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
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

#include "../include/Missione.hpp"
#include <iostream>
#include <thread> 
#include <string>

const std::string RESET = "\x1b[0m";
const std::string RED = "\x1b[31m";
const std::string GREEN = "\x1b[32m";
const std::string YELLOW = "\x1b[33m";
const std::string CYAN = "\x1b[36m";
const std::string BOLD = "\x1b[1m";

void MissioneBase::assegnaPersonale(Personale* p) {
    personaleAssegnato.push_back(p);
    p->setDisponibile(false);
}

void MissioneBase::assegnaMezzo(MezzoBase* m) {
    mezziAssegnati.push_back(m);
    m->setDisponibile(false);
}

void MissioneBase::mostraDettagli() const {
    std::cout << "\n=== " << BOLD << "MISSIONE N° " << id << " (" << RED << getTipoMissione() << RESET << ")" << " ===\n\n";
    std::cout << BOLD << "Descrizione: " << RESET << descrizione << "\n";

    std::cout << BOLD <<"Personale assegnato" << RESET << " (" << RED << personaleAssegnato.size() << RESET << "):\n";
    for (auto p : personaleAssegnato)
        std::cout << " - " << p->getNome() << " (" << CYAN << p->gradoToString() << RESET << ")\n";

    std::cout << BOLD <<"Mezzi assegnati" << RESET << " (" << RED << mezziAssegnati.size() << RESET << "):\n";
    for (auto m : mezziAssegnati) {
        std::cout << " - " << m->getNome() << " (Tipo: " << (m->isTerrestre() ? "Terrestre" : "Aereo");
            if (m->getEquipaggioCorrente() == m->getEquipaggioMax()) {
                std::cout << ", Equipaggio: " << RED << m->getEquipaggioCorrente() << "/" << m->getEquipaggioMax() << RESET << ")\n";
            } else {
                std::cout << ", Equipaggio: " << GREEN << m->getEquipaggioCorrente() << "/" << m->getEquipaggioMax() << RESET << ")\n";    }
            }
}

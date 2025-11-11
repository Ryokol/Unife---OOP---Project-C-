#include "../include/Missione.hpp"
#include <iostream>

void MissioneBase::assegnaPersonale(Personale* p) {
    personaleAssegnato.push_back(p);
    p->setDisponibile(false);
}

void MissioneBase::assegnaMezzo(MezzoBase* m) {
    mezziAssegnati.push_back(m);
    m->setDisponibile(false);
}

void MissioneBase::mostraDettagli() const {
    std::cout << "=== Missione " << id << " (" << getTipoMissione() << ") ===\n";
    std::cout << "Descrizione: " << descrizione << "\n";

    std::cout << "Personale assegnato (" << personaleAssegnato.size() << "):\n";
    for (auto p : personaleAssegnato)
        std::cout << " - " << p->getNome() << " (" << p->gradoToString() << ")\n";

    std::cout << "Mezzi assegnati (" << mezziAssegnati.size() << "):\n";
    for (auto m : mezziAssegnati) {
        std::cout << " - " << m->getNome() << " (Tipo: " << (m->isTerrestre() ? "Terrestre" : "Aereo")
                  << ", Equipaggio: " << m->getEquipaggioCorrente() << "/" << m->getEquipaggioMax() << ")\n";
    }
}

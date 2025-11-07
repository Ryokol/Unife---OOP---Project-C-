#include "../include/Caserma.hpp"
#include <iostream>
#include <numeric>
#include <thread> 
#include <string>

const std::string RESET = "\x1b[0m";
const std::string RED = "\x1b[31m";
const std::string GREEN = "\x1b[32m";
const std::string YELLOW = "\x1b[33m";
const std::string CYAN = "\x1b[36m";
const std::string BOLD = "\x1b[1m";

// Funzioni di utilità per la ricerca
std::shared_ptr<Personale> Caserma::trovaPersonale(int id) const {
    for (const auto& p : personale) {
        if (p->getId() == id) {
            return p;
        }
    }
    return nullptr;
}

std::shared_ptr<MezzoBase> Caserma::trovaMezzo(int id) const {
    for (const auto& m : mezzi) {
        if (m->getId() == id) {
            return m;
        }
    }
    return nullptr;
}

void Caserma::aggiungiPersonale(std::shared_ptr<Personale> p) {
    // Controllo unicità ID
    if (trovaPersonale(p->getId())) {
        throw CasermaException("ID Personale gia' esistente: " + std::to_string(p->getId()));
    }
    personale.push_back(p);
}

void Caserma::aggiungiMezzo(std::shared_ptr<MezzoBase> m) {
    // Controllo unicità ID
    if (trovaMezzo(m->getId())) {
        throw CasermaException("ID Mezzo gia' esistente: " + std::to_string(m->getId()));
    }
    mezzi.push_back(m);
}

void Caserma::creaMissione(TipoMissione tipo,
                           const std::string& descrizione,
                           const std::vector<int>& idPersonale,
                           const std::vector<int>& idMezzi) {
    int id = missioni.size() + 1;
    std::unique_ptr<MissioneBase> nuovaMissione;

    // 1. Creazione dell'oggetto Missione in base al tipo
    switch (tipo) {
        case TipoMissione::SCORTA:
            nuovaMissione = std::make_unique<MissioneScorta>(id, descrizione);
            break;
        case TipoMissione::ASSALTO:
            nuovaMissione = std::make_unique<MissioneAssalto>(id, descrizione);
            break;
        case TipoMissione::ESTRAZIONE:
            nuovaMissione = std::make_unique<MissioneEstrazione>(id, descrizione);
            break;
        default:
            throw CasermaException("Tipo di missione non valido.");
    }

    // 2. Pre-assegnazione e verifica disponibilità
    std::vector<Personale*> personaleDaAssegnare;
    std::vector<MezzoBase*> mezziDaAssegnare;
    int personaleTotale = 0;

    // Verifica Personale
    for (int idP : idPersonale) {
        auto p = trovaPersonale(idP);
        if (!p) {
            throw CasermaException("Personale con ID " + std::to_string(idP) + " non trovato.");
        }
        if (!p->isDisponibile()) {
            throw CasermaException("Personale " + p->getNome() + " (ID " + std::to_string(idP) + ") non disponibile.");
        }
        personaleDaAssegnare.push_back(p.get());
        personaleTotale++;
    }

    // Verifica Mezzi e calcolo equipaggio
    int personaleRimanente = personaleTotale;
    for (int idM : idMezzi) {
        auto m = trovaMezzo(idM);
        if (!m) {
            throw CasermaException("Mezzo con ID " + std::to_string(idM) + " non trovato.");
        }
        if (!m->isDisponibile()) {
            throw CasermaException("Mezzo " + m->getNome() + " (ID " + std::to_string(idM) + ") non disponibile.");
        }

        // Assegnazione equipaggio al mezzo (greedy: assegna il massimo possibile)
        int equipaggioNecessario = m->getEquipaggioMin();
        int equipaggioMassimo = m->getEquipaggioMax();
        int equipaggioAssegnato = std::min(equipaggioMassimo, personaleRimanente);

        if (equipaggioAssegnato < equipaggioNecessario) {
            throw CasermaException("Personale insufficiente per equipaggiare il mezzo " + m->getNome() +
                                   ". Richiesto min: " + std::to_string(equipaggioNecessario) +
                                   ", Disponibile: " + std::to_string(personaleRimanente));
        }

        m->setEquipaggioCorrente(equipaggioAssegnato);
        personaleRimanente -= equipaggioAssegnato;
        mezziDaAssegnare.push_back(m.get());
    }

    // Verifica personale rimanente (se non tutto il personale è stato assegnato a un mezzo)
    if (personaleRimanente > 0) {
        // Se ci sono mezzi, il personale rimanente è considerato "di supporto"
        if (mezziDaAssegnare.empty()) {
             throw CasermaException("Nessun mezzo assegnato, ma personale richiesto.");
        }
    }

    // 3. Assegnazione finale alla missione
    for (auto p : personaleDaAssegnare) {
        nuovaMissione->assegnaPersonale(p);
    }
    for (auto m : mezziDaAssegnare) {
        nuovaMissione->assegnaMezzo(m);
    }

    // 4. Verifica Requisiti Missione
    if (!nuovaMissione->verificaRequisiti()) {
        // Se i requisiti non sono soddisfatti, si annulla l'assegnazione
        for (auto p : personaleDaAssegnare) {
            p->setDisponibile(true);
        }
        for (auto m : mezziDaAssegnare) {
            m->setDisponibile(true);
            m->setEquipaggioCorrente(0); // Reset equipaggio
        }
        throw CasermaException("Requisiti minimi per la missione di " + nuovaMissione->getTipoMissione() + " non soddisfatti.");
    }

    // 5. Aggiunta Missione
    missioni.push_back(std::move(nuovaMissione));
}

void Caserma::mostraPersonale() const {
    std::cout << "\n--- " << BOLD << YELLOW << "Personale" << RESET << " ---\n";
    for (const auto& p : personale)
        std::cout << p->getId() << " - " << p->getNome() << " (" << p->gradoToString()
                  << ") [" << (p->isDisponibile() ? GREEN + "Disponibile" + RESET : RED + "In missione" + RESET) << "]\n";
}

void Caserma::mostraMezzi() const {
    std::cout << "\n--- " << BOLD << YELLOW << "Mezzi" << RESET << " ---\n";
    for (const auto& m : mezzi)
        std::cout << m->getId() << " - " << m->getNome() << " (Tipo: " << (m->isTerrestre() ? "Terrestre" : "Aereo")
                  << ", Equipaggio: " << m->getEquipaggioCorrente() << "/" << m->getEquipaggioMax()
                  << ") [" << (m->isDisponibile() ? GREEN + "Disponibile" + RESET : RED + "In missione" + RESET) << "]\n";
}

void Caserma::mostraMissioni() const {
    for (const auto& m : missioni)
        m->mostraDettagli();
}

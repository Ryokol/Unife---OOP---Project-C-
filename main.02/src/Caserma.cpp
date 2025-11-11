#include "../include/Caserma.hpp"
#include <iostream>
#include <numeric>
#include "Caserma.hpp"

// Distruttore per la pulizia della memoria
Caserma::~Caserma() {
    for (auto m : mezzi) {
        delete m;
    }
    mezzi.clear();

    for (auto mission : missioni) {
        delete mission;
    }
    missioni.clear();
}

// Funzioni di utilità per la ricerca
Personale* Caserma::trovaPersonale(int id) {
    for (auto& p : personale) {
        if (p.getId() == id) {
            return &p;
        }
    }
    return nullptr;
}

MezzoBase* Caserma::trovaMezzo(int id) const {
    for (const auto& m : mezzi) {
        if (m->getId() == id) {
            return m;
        }
    }
    return nullptr;
}

void Caserma::aggiungiPersonale(const Personale& p) {
    if (trovaPersonale(p.getId())) {
        throw CasermaException("ID Personale gia' esistente: " + std::to_string(p.getId()));
    }
    personale.push_back(p);
}

void Caserma::aggiungiMezzo(MezzoBase* m) {
    if (trovaMezzo(m->getId())) {
        delete m; // Pulisce la memoria se l'ID è duplicato
        throw CasermaException("ID Mezzo gia' esistente: " + std::to_string(m->getId()));
    }
    mezzi.push_back(m);
}

void Caserma::creaMissione(TipoMissione tipo,
                           const std::string& descrizione,
                           const std::vector<int>& idPersonale,
                           const std::vector<int>& idMezzi) {
    int id = missioni.size() + 1;
    MissioneBase* nuovaMissione = nullptr;

    // 1. Creazione dell'oggetto Missione in base al tipo
    switch (tipo) {
        case TipoMissione::SCORTA:
            nuovaMissione = new MissioneScorta(id, descrizione);
            break;
        case TipoMissione::ASSALTO:
            nuovaMissione = new MissioneAssalto(id, descrizione);
            break;
        case TipoMissione::ESTRAZIONE:
            nuovaMissione = new MissioneEstrazione(id, descrizione);
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
        Personale* p = trovaPersonale(idP);
        if (!p) {
            delete nuovaMissione;
            throw CasermaException("Personale con ID " + std::to_string(idP) + " non trovato.");
        }
        if (!p->isDisponibile()) {
            delete nuovaMissione;
            throw CasermaException("Personale " + p->getNome() + " (ID " + std::to_string(idP) + ") non disponibile.");
        }
        personaleDaAssegnare.push_back(p);
        personaleTotale++;
    }

    // Verifica Mezzi e calcolo equipaggio
    int personaleRimanente = personaleTotale;
    for (int idM : idMezzi) {
        MezzoBase* m = trovaMezzo(idM);
        if (!m) {
            delete nuovaMissione;
            throw CasermaException("Mezzo con ID " + std::to_string(idM) + " non trovato.");
        }
        if (!m->isDisponibile()) {
            delete nuovaMissione;
            throw CasermaException("Mezzo " + m->getNome() + " (ID " + std::to_string(idM) + ") non disponibile.");
        }

        int equipaggioNecessario = m->getEquipaggioMin();
        int equipaggioMassimo = m->getEquipaggioMax();
        int equipaggioAssegnato = std::min(equipaggioMassimo, personaleRimanente);

        if (equipaggioAssegnato < equipaggioNecessario) {
            delete nuovaMissione;
            throw CasermaException("Personale insufficiente per equipaggiare il mezzo " + m->getNome() +
                                   ". Richiesto min: " + std::to_string(equipaggioNecessario) +
                                   ", Disponibile: " + std::to_string(personaleRimanente));
        }

        m->setEquipaggioCorrente(equipaggioAssegnato);
        personaleRimanente -= equipaggioAssegnato;
        mezziDaAssegnare.push_back(m);
    }

    if (personaleRimanente > 0 && mezziDaAssegnare.empty()) {
         delete nuovaMissione;
         throw CasermaException("Nessun mezzo assegnato, ma personale richiesto.");
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
        for (auto p : personaleDaAssegnare) {
            p->setDisponibile(true);
        }
        for (auto m : mezziDaAssegnare) {
            m->setDisponibile(true);
            m->setEquipaggioCorrente(0);
        }
        delete nuovaMissione;
        throw CasermaException("Requisiti minimi per la missione di " + nuovaMissione->getTipoMissione() + " non soddisfatti.");
    }

    // 5. Aggiunta Missione
    missioni.push_back(nuovaMissione);
}

void Caserma::mostraPersonale() const {
    std::cout << "\n--- Personale ---\n";
    for (const auto& p : personale)
        std::cout << p.getId() << " - " << p.getNome() << " (" << p.gradoToString()
                  << ") [" << (p.isDisponibile() ? "Disponibile" : "In missione") << "]\n";
}

void Caserma::mostraMezzi() const {
    std::cout << "\n--- Mezzi ---\n";
    for (const auto& m : mezzi)
        std::cout << m->getId() << " - " << m->getNome() << " (Tipo: " << (m->isTerrestre() ? "Terrestre" : "Aereo")
                  << ", Equipaggio: " << m->getEquipaggioCorrente() << "/" << m->getEquipaggioMax()
                  << ") [" << (m->isDisponibile() ? "Disponibile" : "In missione") << "]\n";
}

void Caserma::mostraMissioni() const {
    std::cout << "\n=== Elenco Missioni ===\n";
    for (const auto& m : missioni)
        m->mostraDettagli();
}

#ifndef MISSIONE_HPP
#define MISSIONE_HPP

#include "Personale.hpp"
#include "Mezzo.hpp"
#include "Risorsa.hpp"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

// Tipi di missione
enum class TipoMissione {
    SCORTA,
    ASSALTO,
    ESTRAZIONE
};

// Classe base astratta per le Missioni
class MissioneBase {
protected:
    int id;
    std::string descrizione;
    std::vector<Personale*> personaleAssegnato;
    std::vector<MezzoBase*> mezziAssegnati;

public:
    MissioneBase(int id, const std::string& descrizione)
        : id(id), descrizione(descrizione) {}
    virtual ~MissioneBase() = default;

    // Metodo virtuale puro per la verifica dei requisiti
    virtual bool verificaRequisiti() const = 0;
    virtual std::string getTipoMissione() const = 0;

    void assegnaPersonale(Personale* p);
    void assegnaMezzo(MezzoBase* m);

    void mostraDettagli() const;
    const std::vector<Personale*>& getPersonale() const { return personaleAssegnato; }
    const std::vector<MezzoBase*>& getMezzi() const { return mezziAssegnati; }
};

// 1 - Missione di Scorta (almeno 3 mezzi devono essere presenti)
class MissioneScorta : public MissioneBase {
public:
    MissioneScorta(int id, const std::string& descrizione)
        : MissioneBase(id, descrizione) {}

    bool verificaRequisiti() const override {
        return mezziAssegnati.size() >= 3;
    }
    std::string getTipoMissione() const override { return "Scorta"; }
};

// 2 - Missione di Assalto (massimo 1 mezzo ogni 5 elementi di personale)
class MissioneAssalto : public MissioneBase {
public:
    MissioneAssalto(int id, const std::string& descrizione)
        : MissioneBase(id, descrizione) {}

    bool verificaRequisiti() const override {
        // (personaleAssegnato.size() / 5) >= mezziAssegnati.size()
        // Esempio: 5 persone -> max 1 mezzo. 6 persone -> max 1 mezzo. 10 persone -> max 2 mezzi.
        return mezziAssegnati.size() <= (personaleAssegnato.size() / 5);
    }
    std::string getTipoMissione() const override { return "Assalto"; }
};

// 3 - Missione di Estrazione (almeno 2 mezzi terrestri e 5 elementi di personale)
class MissioneEstrazione : public MissioneBase {
public:
    MissioneEstrazione(int id, const std::string& descrizione)
        : MissioneBase(id, descrizione) {}

    bool verificaRequisiti() const override {
        int mezziTerrestri = 0;
        for (const auto& m : mezziAssegnati) {
            if (m->isTerrestre()) {
                mezziTerrestri++;
            }
        }
        return mezziTerrestri >= 2 && personaleAssegnato.size() >= 5;
    }
    std::string getTipoMissione() const override { return "Estrazione"; }
};

#endif // MISSIONE_HPP

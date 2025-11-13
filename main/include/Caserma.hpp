#ifndef CASERMA_HPP
#define CASERMA_HPP

#include "Personale.hpp"
#include "Mezzo.hpp"
#include "Missione.hpp"
#include "Risorsa.hpp"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

class Caserma {
private:
    // Uso di smart pointers per una gestione della memoria più sicura
    std::vector<std::shared_ptr<Personale>> personale;
    std::vector<std::shared_ptr<MezzoBase>> mezzi;
    std::vector<std::unique_ptr<MissioneBase>> missioni;

    // Funzioni di utilità per la ricerca
    std::shared_ptr<Personale> trovaPersonale(int id) const;
    std::shared_ptr<MezzoBase> trovaMezzo(int id) const;

public:
    void aggiungiPersonale(std::shared_ptr<Personale> p);
    void aggiungiMezzo(std::shared_ptr<MezzoBase> m);

    void creaMissione(TipoMissione tipo,
                      const std::string& descrizione,
                      const std::vector<int>& idPersonale,
                      const std::vector<int>& idMezzi);

    void mostraPersonale() const;
    void mostraMezzi() const;
    void mostraMissioni() const;
};

#endif // CASERMA_HPP

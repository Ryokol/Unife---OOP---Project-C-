#ifndef CASERMA_HPP
#define CASERMA_HPP

#include "Personale.hpp" // Per Personale
#include "Mezzo.hpp" // Per MezzoBase
#include "Missione.hpp" // Per MissioneBase e TipoMissione
#include "Risorsa.hpp" // Per CasermaException

#include <vector> // Per std::vector
#include <string> // Per std::string
#include <algorithm> // Per std::find_if

class Caserma { // Definizione della classe Caserma
public:
    ~Caserma();
private:
    // Uso di smart pointers per una gestione della memoria più sicura
    std::vector<Personale> personale;
    std::vector<MezzoBase*> mezzi; // Mezzi stored by raw pointer for polymorphism
    std::vector<MissioneBase*> missioni; // Missioni stored by raw pointer for polymorphism

    // Funzioni di utilità per la ricerca
    Personale *trovaPersonale(int id);
    MezzoBase *trovaMezzo(int id) const;

public:
    void aggiungiPersonale(const Personale& p); // Aggiunta di personale
    void aggiungiMezzo(MezzoBase* m); // Aggiunta di un mezzo

    void creaMissione( // Creazione di una nuova missione
        TipoMissione tipo,
                      const std::string& descrizione,
                      const std::vector<int>& idPersonale,
                      const std::vector<int>& idMezzi);

    void mostraPersonale() const; // Mostra tutto il personale
    void mostraMezzi() const; // Mostra tutti i mezzi
    void mostraMissioni() const; // Mostra tutte le missioni
};

#endif // CASERMA_HPP

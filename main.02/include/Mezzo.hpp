#ifndef MEZZO_HPP
#define MEZZO_HPP

#include "Risorsa.hpp" // Per CasermaException
#include "Colori.hpp" // Per i colori ANSI

#include <iostream> // Per std::cout
#include <string> // Per std::string
#include <stdexcept> // Per std::runtime_error


// Classe base astratta per i Mezzi
class MezzoBase : public Risorsa { // Eredita da Risorsa
protected:
    int id;
    std::string tipo;
    bool disponibile;
    int equipaggioMin;
    int equipaggioMax;
    int equipaggioCorrente;
    bool Check = false;

public:
    MezzoBase(int id, const std::string& tipo, int min, int max)
        : id(id), tipo(tipo), disponibile(true), equipaggioMin(min), equipaggioMax(max), equipaggioCorrente(0) {
    }

    // Implementazione dei metodi di Risorsa
    int getId() const override { return id; } // ID del mezzo
    std::string getNome() const override { return tipo; }
    bool isDisponibile() const override { return disponibile; } // Disponibilità del mezzo
    void setDisponibile(bool disp) override { disponibile = disp; } // Imposta disponibilità

    // Metodi specifici di Mezzo
    int getEquipaggioMin() const { return equipaggioMin; } // Equipaggio minimo richiesto
    int getEquipaggioMax() const { return equipaggioMax; } // Equipaggio massimo consentito
    int getEquipaggioCorrente() const { return equipaggioCorrente; } // Equipaggio attualmente assegnato
    void setEquipaggioCorrente(int count) { // Imposta equipaggio corrente con validazione
        if (count < equipaggioMin || count > equipaggioMax) { // Validazione
            throw CasermaException("Numero di personale non valido per l'equipaggio del mezzo.");
        }
        equipaggioCorrente = count;
    }

    // Metodo virtuale puro per la categorizzazione (utile per i requisiti di missione)
    virtual bool isTerrestre() const = 0;
};

// Classi concrete per i tipi di mezzo
class MezzoTerrestre : public MezzoBase { // Mezzo terrestre
public:
    MezzoTerrestre(int id, const std::string& tipo, int min, int max)
        : MezzoBase(id, tipo, min, max) {}
    bool isTerrestre() const override { return true; }
};

class MezzoAereo : public MezzoBase { // Mezzo aereo
public:
    MezzoAereo(int id, const std::string& tipo, int min, int max)
        : MezzoBase(id, tipo, min, max) {}
    bool isTerrestre() const override { return false; }
};

#endif // MEZZO_HPP

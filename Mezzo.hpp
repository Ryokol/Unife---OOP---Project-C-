#ifndef MEZZO_HPP
#define MEZZO_HPP

#include "Risorsa.hpp"
#include <string>
#include <stdexcept>

// Classe base astratta per i Mezzi
class MezzoBase : public Risorsa {
protected:
    int id;
    std::string tipo;
    bool disponibile;
    int equipaggioMin;
    int equipaggioMax;
    int equipaggioCorrente;

public:
    MezzoBase(int id, const std::string& tipo, int min, int max)
        : id(id), tipo(tipo), disponibile(true), equipaggioMin(min), equipaggioMax(max), equipaggioCorrente(0) {
        if (min < 2 || max > 6 || min > max) {
            throw CasermaException("Requisiti equipaggio non validi (min 2, max 6).");
        }
    }

    // Implementazione dei metodi di Risorsa
    int getId() const override { return id; }
    std::string getNome() const override { return tipo; } // Usiamo 'tipo' come nome
    bool isDisponibile() const override { return disponibile; }
    void setDisponibile(bool disp) override { disponibile = disp; }

    // Metodi specifici di Mezzo
    int getEquipaggioMin() const { return equipaggioMin; }
    int getEquipaggioMax() const { return equipaggioMax; }
    int getEquipaggioCorrente() const { return equipaggioCorrente; }
    void setEquipaggioCorrente(int count) {
        if (count < equipaggioMin || count > equipaggioMax) {
            throw CasermaException("Numero di personale non valido per l'equipaggio del mezzo.");
        }
        equipaggioCorrente = count;
    }

    // Metodo virtuale puro per la categorizzazione (utile per i requisiti di missione)
    virtual bool isTerrestre() const = 0;
};

// Classi concrete per i tipi di mezzo
class MezzoTerrestre : public MezzoBase {
public:
    MezzoTerrestre(int id, const std::string& tipo, int min, int max)
        : MezzoBase(id, tipo, min, max) {}
    bool isTerrestre() const override { return true; }
};

class MezzoAereo : public MezzoBase {
public:
    MezzoAereo(int id, const std::string& tipo, int min, int max)
        : MezzoBase(id, tipo, min, max) {}
    bool isTerrestre() const override { return false; }
};

#endif // MEZZO_HPP

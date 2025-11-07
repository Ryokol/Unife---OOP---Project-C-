#ifndef PERSONALE_HPP
#define PERSONALE_HPP

#include "Risorsa.hpp"
#include <string>

enum class Grado {
    SOLDATO,
    CAPORALE,
    SERGENTE,
    TENENTE,
    CAPITANO,
    MAGGIORE
};

class Personale : public Risorsa {
private:
    int id;
    std::string nome;
    Grado grado;
    bool disponibile;

public:
    Personale(int id, const std::string& nome, Grado grado);

    // Implementazione dei metodi di Risorsa
    int getId() const override { return id; }
    std::string getNome() const override { return nome; }
    bool isDisponibile() const override { return disponibile; }
    void setDisponibile(bool disp) override { disponibile = disp; }

    // Metodi specifici di Personale
    Grado getGrado() const;
    std::string gradoToString() const;
};

#endif // PERSONALE_HPP

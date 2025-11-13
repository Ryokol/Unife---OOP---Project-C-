#ifndef RISORSA_HPP
#define RISORSA_HPP

#include <string>
#include <stdexcept>

// Classe base astratta (Interfaccia) per tutte le risorse gestite dalla Caserma
class Risorsa {
public:
    virtual ~Risorsa() = default;
    virtual int getId() const = 0;
    virtual std::string getNome() const = 0;
    virtual bool isDisponibile() const = 0;
    virtual void setDisponibile(bool disp) = 0;
};

// Eccezione personalizzata per la gestione degli errori
class CasermaException : public std::runtime_error {
public:
    CasermaException(const std::string& message) : std::runtime_error(message) {}
};

#endif // RISORSA_HPP

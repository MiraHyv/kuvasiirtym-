#pragma once
#include "kuvanPiirtaja.h"
#include "kello.h"
#include "asetukset.h"

#include <string>


class siirtyma : public kuvanPiirtaja, public asetukset {
    kuva kuva1, kuva2, kuva3;
    kuva kartta1, kartta2, kartta3, kartta4, kartta5, kartta6;
    kuva tmp, tmp2;
    kuva sekoitus;
    kuva tulos;
    
    kello t, t0;
    
    int frame;
    float tavoiteFPS;
    
    bool lataaKuvat();
    
    void kerroAjat();
    
    std::string luoTiedostonimi();
    
public:
    
    void suorita();
    
    siirtyma();
};

#pragma once
#include "json.hpp"

#include <string>

class asetukset{
public:
    //oletukset
    int leveys = 3500;
    int korkeus = 2333;
    float fps_tulos = 60;
    float fps_esikatselu = 20;
    int marginaali = 60;
    int T1 = 1000;
    int T2 = 2000;
    bool tallennetaan = false;

    std::string kuvatiedostot[3] = {"", "", ""};
    std::string karttatiedostot[6] = {"", "", "", "", "", ""};
    
    //lataa tiedostosta
    bool lataaAsetukset(std::string tiedosto = "");
    
};

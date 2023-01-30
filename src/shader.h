#pragma once
#include "ikkuna.h"
#include <string>


const std::string OLETUS_VSHADER = "data/kokoruutu.vsh";
const std::string OLETUS_FSHADER = "data/default.fsh";
                                

class shader{
    GLuint tunnus;
public:
    shader();
    shader(std::string vTiedosto, std::string fTiedosto);
    
    bool lataa( std::string vTiedosto = OLETUS_VSHADER,
                std::string fTiedosto = OLETUS_FSHADER
              );
    
    GLuint haeTunnus();
    
    bool kayta();
    bool asetaUniform(std::string nimi, int arvo);
    bool asetaUniform(std::string nimi, float arvo);    
};

#include "shader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>

std::string lueTiedosto(std::string polku) {
    std::string tulos;
    std::ifstream tiedosto(polku.c_str(), std::ios::in);
    if(tiedosto.is_open() ) {
        std::stringstream ss;
        ss << tiedosto.rdbuf();
        tulos = ss.str();
        tiedosto.close();
    }
    else {
        std::cout << "  Ei voitu avata tiedostoa " << polku << "\n";
        return "";
    }
    return tulos;
}


shader::shader() {
    tunnus = 0;
}


shader::shader(std::string vTiedosto, std::string fTiedosto) {
    tunnus = 0;
    lataa(vTiedosto, fTiedosto);
}


bool shader::lataa(std::string vTiedosto, std::string fTiedosto) {
    
    //Jos ikkunaa ei ole luotu onnistuneesti:
    if(!haeIkkuna().otaKayttoon() )
        return false;
    
    //Jos tämä shader on jo ladattu:
    if(haeTunnus() != 0)
        return false;
    
    //Luodaan shader
    tunnus = glCreateProgram();
    
    unsigned int vShaderTunnus = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fShaderTunnus = glCreateShader(GL_FRAGMENT_SHADER);
    
    //lue lähdekoodit tiedostosta
    std::string vSrc = lueTiedosto(vTiedosto);
    const char* vSrc_c = vSrc.c_str();
    std::string fSrc = lueTiedosto(fTiedosto);
    const char* fSrc_c = fSrc.c_str();
    
    // kääntö: Kääntäjä voi tuottaa tekstipalautteen, joka tulostetaan ruudulle.
    // Käännön tulos ja tekstipalautteen pituus:
    int kTulos = GL_FALSE;
    int palautteenPituus;
    
    // annetaan lähdekoodit:
    glShaderSource(vShaderTunnus, 1, &vSrc_c, NULL);
    glShaderSource(fShaderTunnus, 1, &fSrc_c, NULL);
    
    //käännetään vertex shader
    glCompileShader(vShaderTunnus);
    glGetShaderiv(vShaderTunnus, GL_COMPILE_STATUS, &kTulos);
    glGetShaderiv(vShaderTunnus, GL_INFO_LOG_LENGTH, &palautteenPituus);
    if(palautteenPituus > 0) {
        std::vector<char> palaute(palautteenPituus+1);
        glGetShaderInfoLog( vShaderTunnus,
                            palautteenPituus,
                            NULL,
                            &palaute[0]
                          );
        printf("%s\n", &palaute[0]);
    }
    if(kTulos != GL_TRUE)
        return false;
    
    //käännetään fragment shader
    glCompileShader(fShaderTunnus);
    glGetShaderiv(fShaderTunnus, GL_COMPILE_STATUS, &kTulos);
    glGetShaderiv(fShaderTunnus, GL_INFO_LOG_LENGTH, &palautteenPituus);
    if(palautteenPituus > 0) {
        std::vector<char> palaute(palautteenPituus+1);
        glGetShaderInfoLog( fShaderTunnus,
                            palautteenPituus,
                            NULL,
                            &palaute[0]
                          );
        printf("%s\n", &palaute[0]);
    }
    if(kTulos != GL_TRUE)
        return false;
    
    //linkitetään ohjelma
    // TODO: tarkista ensin onko tunnus hyvä ja onko shader jo käännetty ja linkitetty
    glAttachShader(haeTunnus(), vShaderTunnus);
    glAttachShader(haeTunnus(), fShaderTunnus);
    glLinkProgram(haeTunnus());
    
    //vapautetaan tarpeettomat asiat
    glDetachShader(haeTunnus(), vShaderTunnus);
    glDetachShader(haeTunnus(), fShaderTunnus);
    glDeleteShader(vShaderTunnus);
    glDeleteShader(fShaderTunnus);
    
    //tarkastetaan onnistuiko
    int lTulos;
    glGetProgramiv(haeTunnus(), GL_LINK_STATUS, &lTulos);
    if(lTulos == GL_TRUE) {
        
        //Otetaan shader käyttöön ja asetetaan tekstuurien nimet
        glUseProgram(haeTunnus() );
        asetaUniform("texture0", 0);
        asetaUniform("texture1", 1);
        asetaUniform("texture2", 2);
        asetaUniform("texture3", 3);
        asetaUniform("texture4", 4);
        asetaUniform("texture5", 5);
        asetaUniform("texture6", 6);
        return true;
    }
    else
        return false;
}


GLuint shader::haeTunnus() {
    return tunnus;
}


bool shader::kayta() {
    //Jos ikkunaa ei ole luotu onnistuneesti:
    if(!haeIkkuna().otaKayttoon() )
        return false;
    if(haeTunnus() == 0)
        return false;

    glUseProgram(haeTunnus() );
    return true;
}


bool shader::asetaUniform(std::string nimi, int arvo) {
    if(!kayta() )
        return false;
    glUniform1i(glGetUniformLocation(haeTunnus(), nimi.c_str() ), arvo);
    return true;
}


bool shader::asetaUniform(std::string nimi, float arvo) {
    if(!kayta() )
        return false;
    glUniform1f(glGetUniformLocation(haeTunnus(), nimi.c_str() ), arvo);
    return true;
}

#pragma once
#include "GL.h"
#include <string>

//Kuvalla on tekstuuri ja FBO.
// FBO luodaan vasta tarvittaessa.
class kuva{
    GLuint tekstuuri;
    GLuint FBO;
    
    int leveys;
    int korkeus;
    
    bool asetaPikselit( int leveys, 
                        int korkeus, 
                        uint8_t* pikselit, 
                        GLenum muoto = GL_RGB,
                        GLenum tyyppi = GL_UNSIGNED_BYTE
                      );
    
    //Sido tämän kuvan tekstuuri framebufferiin
    bool asetaKohteeksi();
    bool asetaKohteeksi() const;

public:
    kuva();
    kuva(std::string tiedosto);
    
    //Nämä funktiot palauttavat GL-tunnuksen
    GLuint haeTekstuuri() const;
    GLuint haeFBO() const;
    
    int haeLeveys() const;
    int haeKorkeus() const;
    
    bool luoTekstuuri();
    bool luoFBO();
    
    bool lataa(std::string tiedosto);
    bool lataa_16(std::string tiedosto);
    bool tallenna(std::string tiedosto);
    
    bool asetaTyhjana(int leveys, int korkeus);
    
    bool kayta(int teksuuripaikka = 0) const;
    
    //Piirrä annettu kuva tähän kuvaan
    bool piirra(const kuva& src) const;
};

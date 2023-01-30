#pragma once
#include "GL.h"
#include "kuva.h"
#include <memory>
#include <string>

const int OLETUSLEVEYS = 512;
const int OLETUSKORKEUS = 512;

/*
 * GLFW-ikkuna
 * Vain yksi ikkuna sallitaan kerrallaan.
 */

class ikkuna{
private:
    //konstruktorit ja destruktori: private, estetään niiden käyttö
    ikkuna();
    ikkuna(const ikkuna& toinen);
    const ikkuna& operator=(const ikkuna& toinen);

    //Jäsenet
    int leveys, korkeus;
    GLFWwindow* ikkuna_p;

    GLuint VAO;
    
public:
    //Globaali pääsy: palauttaa viitteen ikkunaan
    static ikkuna& haeInstanssi();
    
    //luo ikkuna
    bool luo(int leveys_ = OLETUSLEVEYS, int korkeus_ = OLETUSKORKEUS);
    
    //onko ikkuna luotu onnistuneesti?
    bool onHyva() const;
    
    //käytä ikkunaa eli tee siitä aktiivinen konteksti
    bool otaKayttoon() const;
    
    //aseta ikkuna piirtokohteeksi eli sido framebufferiin 0
    bool asetaKohteeksi() const;
    
    //sulje ikkuna (glfwTerminate)
    void sulje();
    
    //piirrä annettu kuva ikkunaan
    bool piirra(const kuva& k) const;
    
    //Toteuta piirtotoiminnot ja katso käyttäjän syöte.
    // Palauta false, jos ikkuna halutaan sulkea tai sitä ei ole.
    bool paivita() const;
};

ikkuna& haeIkkuna();

ikkuna& luoIkkuna(int leveys = OLETUSLEVEYS, int korkeus = OLETUSKORKEUS);

void sulje();

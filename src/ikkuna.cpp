#include "GL.h"
#include "ikkuna.h"

#include <iostream>

ikkuna& haeIkkuna() {
    return ikkuna::haeInstanssi();
}


ikkuna& luoIkkuna(int leveys, int korkeus) {
    ikkuna::haeInstanssi().luo(leveys, korkeus);
    return ikkuna::haeInstanssi();
}


void sulje() {
    ikkuna::haeInstanssi().sulje();
}


ikkuna::ikkuna() {
    ikkuna_p = NULL;
}

    
ikkuna& ikkuna::haeInstanssi() {
    static std::unique_ptr<ikkuna> instanssi(new ikkuna);
    return *instanssi;
}


bool ikkuna::luo(int leveys_, int korkeus_) {
    if(ikkuna_p != NULL ) {
        std::cout << "Ikkuna on jo olemassa!\n";
        return false;
    }
    
    //luodaan GLFW-ikkuna
    
    glewExperimental = true;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    if(!glfwInit())
        return false;
    
    ikkuna_p = glfwCreateWindow(leveys_, korkeus_, "OpenGL-ohjelma", NULL, NULL);
    if(!ikkuna_p) {
        std::cout << "Ikkunan luominen ei onnistunut\n";
        return false;
    }
    
    glfwMakeContextCurrent(ikkuna_p);
    
    if(glewInit() != GLEW_OK) {
        std::cout << "GLEW:n alustus ei onnistunut\n";
        sulje();
        return false;
    }
    
    leveys = leveys_;
    korkeus = korkeus_;

    // Luodaan VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    return true;
}


bool ikkuna::onHyva() const {
    return(ikkuna_p != NULL);
}


bool ikkuna::otaKayttoon() const {
    if(!onHyva() )
        return false;

    glfwMakeContextCurrent(ikkuna_p);
    glBindVertexArray(VAO);
    
    return true;
}


bool ikkuna::asetaKohteeksi() const {
    if(!otaKayttoon() )
        return false;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, leveys, korkeus);
}


void ikkuna::sulje() {
    glfwTerminate();
    ikkuna_p = NULL;
}


bool ikkuna::piirra(const kuva& k) const {
    if(!k.kayta(0))
        return false;
    asetaKohteeksi();
    
    // Tässä kerrotaan ainoastaan, kuinka monta verteksiä piirretään, sillä
    // suorakulmion verteksit on hard-koodattu shaderiin
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

    
//palauta false jos ikkuna suljettiin tai sitä ei ole
bool ikkuna::paivita() const {
    
    if(!ikkuna_p)
        return false;
    
    glfwMakeContextCurrent(ikkuna_p);

    //Toteutetaan
    
    glfwSwapBuffers(ikkuna_p);
    glfwPollEvents();
    if(     glfwGetKey(ikkuna_p, GLFW_KEY_ESCAPE ) == GLFW_PRESS 
        ||  glfwWindowShouldClose(ikkuna_p) != 0
    )
        return false;
    else
        return true;
}

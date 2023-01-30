#include "kuva.h"

#include "ikkuna.h"
#include <opencv2/opencv.hpp>

kuva::kuva() {
    tekstuuri = 0;
    FBO = 0;
    leveys = 0;
    korkeus = 0;
}


kuva::kuva(std::string tiedosto) {
    tekstuuri = 0;
    FBO = 0;
    leveys = 0;
    korkeus = 0;
    lataa(tiedosto);
}


GLuint kuva::haeTekstuuri() const {
    return tekstuuri;
}


GLuint kuva::haeFBO() const {
    return FBO;
}


int kuva::haeLeveys() const {
    return leveys;
}


int kuva::haeKorkeus() const {
    return korkeus;
}


bool kuva::luoTekstuuri() {
    //Jos tekstuuri on jo olemassa, ei tehdä mitään
    if(haeTekstuuri() != 0)
        return false;
    
    if(!haeIkkuna().otaKayttoon() )
        return false;
    
    glGenTextures(1, &tekstuuri);
    
    if(haeTekstuuri() != 0)
        return true;
    else
        return false;
}


//Luodaan ja liitetään kuvan tekstuuri FBO:oon
// Huom. Tämän jälkeen piirtokohde on asetettuna ikkunaan
// eli framebuffer sidotaan nollaan
bool kuva::luoFBO() {
    if(haeFBO() != 0)
        return false;
    
    if(!haeIkkuna().otaKayttoon() )
        return false;
    
    glGenFramebuffers(1, &FBO);
    
    if(haeFBO() == 0)
        return false;
    
    //Liitetään kuvan tekstuuri FBO:oon
    
    glBindFramebuffer(GL_FRAMEBUFFER, haeFBO() );
    if(!this->kayta()) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;        
    }
    
    glFramebufferTexture(   GL_FRAMEBUFFER, 
                            GL_COLOR_ATTACHMENT0, 
                            haeTekstuuri(),
                            0
                        );
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout   << "Framebufferin liittäminen ei onnistunut! Tunnus: " 
                    << haeFBO() <<"\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}


bool kuva::asetaPikselit(int w, int h, uint8_t* pikselit, GLenum muoto, GLenum tyyppi) {
    if(!kayta())
        return false;
    
    std::cout << w << "x" << h << "\n";
    
    //asetetaan kuvadata
    glTexImage2D(   GL_TEXTURE_2D,
                    0,
                    GL_RGB32F,
                    w,
                    h,
                    0,
                    muoto,
                    tyyppi,
                    &pikselit[0]
                );
    
    leveys = w;
    korkeus = h;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    
    
    return true;
}


//Liitä kuvan FBO framebufferiin. Luo FBO tarvittaessa.
bool kuva::asetaKohteeksi() {
    if(haeFBO() == 0) {
        if(!luoFBO() )
            return false;
    }
    return asetaKohteeksi();
}


//Const-versio: ei luoda FBO:ta
bool kuva::asetaKohteeksi() const {
    if(!haeIkkuna().otaKayttoon())
        return false;
    if(haeFBO() == 0)
        return false;
    
    //Liitetään FBO 
    glBindFramebuffer(GL_FRAMEBUFFER, haeFBO() );
    
    //Asetetaan kuvakehys koko tekstuuriin
    glViewport(0, 0, haeLeveys(), haeKorkeus() );
    
    return true;
}


//Luo tekstuuri ja aseta annettu kuvatiedosto siihen
bool kuva::lataa(std::string kuvatiedosto) {
    if(kuvatiedosto == "")
        return false;
    
    if(!luoTekstuuri())
        return false;
    
    std::cout << "Ladataan kuva " << kuvatiedosto << "\n";;
    int flags = CV_LOAD_IMAGE_COLOR;
    cv::Mat ladattuKuva = cv::imread(kuvatiedosto.c_str(), flags);
    cv::flip(ladattuKuva, ladattuKuva, 0);
    
    //Laitetaan kuvan pikselit vektoriin
    cv::Mat flat = ladattuKuva.reshape(1, ladattuKuva.total() * ladattuKuva.channels() );
    std::vector<uint8_t> pikselit = ladattuKuva.isContinuous()? flat : flat.clone();
    
    if(pikselit.empty()) {
        std::cout << "Kuvan lataaminen ei onnistunut\n";
        return false;
    }
    
    asetaPikselit(  ladattuKuva.cols, 
                    ladattuKuva.rows, 
                    &pikselit[0], 
                    GL_BGR, 
                    GL_UNSIGNED_BYTE
                );
    
    luoFBO();
    return true;
}


bool kuva::lataa_16(std::string kuvatiedosto) {
    if(kuvatiedosto == "")
        return false;
    
    if(!luoTekstuuri())
        return false;
    
    std::cout << "Ladataan kuva " << kuvatiedosto << "\n";;
    int flags = CV_LOAD_IMAGE_COLOR | CV_LOAD_IMAGE_ANYDEPTH;
    cv::Mat ladattuKuva = cv::imread(kuvatiedosto.c_str(), flags);
    cv::flip(ladattuKuva, ladattuKuva, 0);
    
    //Laitetaan kuvan pikselit vektoriin
    cv::Mat flat = ladattuKuva.reshape(1, ladattuKuva.total() * ladattuKuva.channels() );
    std::vector<uint16_t> pikselit = ladattuKuva.isContinuous()? flat : flat.clone();
    
    if(pikselit.empty()) {
        std::cout << "Kuvan lataaminen ei onnistunut\n";
        return false;
    }
    
    asetaPikselit(  ladattuKuva.cols, 
                    ladattuKuva.rows, 
                    (uint8_t*)&pikselit[0], 
                    GL_BGR, 
                    GL_UNSIGNED_SHORT
                );
    
    luoFBO();
    return true;
}


bool kuva::tallenna(std::string tiedosto) {

    //Haetaan pikselit vektoriin. Muoto: {B, G, R, ...}, tyyppi: uint8
    // Voisi optimoida niin ettei tätä tarvitse aina varata uudestaan.
    std::vector<uint8_t> pikselit((long)leveys * (long)korkeus * 3);
    
    if(!kayta() )
        return false;
    
    glGetTexImage(
        GL_TEXTURE_2D,
        0,
        GL_BGR,
        GL_UNSIGNED_BYTE,
        &pikselit[0]
    );
    
    //Tehdään OpenCV-matriisi pikseleistä.
    // CV_8UC3: tyyppi on uint8, 3 kanavaa
    cv::Mat m(korkeus, leveys, CV_8UC3, &pikselit[0]);
    cv::flip(m, m, 0);
    
    //Tallennetaan tiedostoon
    return cv::imwrite(tiedosto, m);
}


bool kuva::asetaTyhjana(int w, int h) {
    if(!luoTekstuuri())
        return false;
    
    asetaPikselit(w, h, 0);
    luoFBO();
    return true;    
        
}



bool kuva::kayta(int tekstuuripaikka) const {
    if(!haeIkkuna().otaKayttoon() )
        return false;
    
    if(haeTekstuuri() == 0)
        return false;
    
    glActiveTexture(GL_TEXTURE0 + tekstuuripaikka);
    glBindTexture(GL_TEXTURE_2D, haeTekstuuri() );
    return true;
}


bool kuva::piirra(const kuva& toinen) const {
    ikkuna& I = haeIkkuna();
    //Sido kuvan tekstuuri framebufferiin, käytä annettua kuvaa.
    // Palautetaan framebuffer-sidos ikkunaan jos epäonnistuu.
    if(!asetaKohteeksi() )
        return false;
    if(!toinen.kayta(0)) {
        I.asetaKohteeksi();
        return false;
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    I.asetaKohteeksi();
    return true;
}


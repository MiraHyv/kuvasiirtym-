#include "siirtyma.h"
#include <iostream>


siirtyma::siirtyma() {
    lataaAsetukset("asetukset.conf");
    
    if(tallennetaan) {
        tavoiteFPS = fps_tulos;
    }

    else {
        tavoiteFPS = fps_esikatselu;
        T1 *= tavoiteFPS/fps_tulos;
        T2 *= tavoiteFPS/fps_tulos;
        marginaali = 0;
    }

    lataaKuvat();
    frame = 0;
    
    t.setPeriod_fps(tavoiteFPS);
    t.reset();
    t0.reset();
    
    suorita();
}


bool siirtyma::lataaKuvat() {
    kuva1.lataa(kuvatiedostot[0]);  //alkukuva
    kuva2.lataa(kuvatiedostot[1]);  //välikuva
    kuva3.lataa(kuvatiedostot[2]);  //loppukuva
    
    kartta1.lataa_16(karttatiedostot[0]);
    kartta2.lataa_16(karttatiedostot[1]);
    kartta3.lataa_16(karttatiedostot[2]);
    kartta4.lataa_16(karttatiedostot[3]);
    kartta5.lataa_16(karttatiedostot[4]);
    kartta6.lataa_16(karttatiedostot[5]);
    
    tmp.asetaTyhjana(leveys, korkeus);
    tmp2.asetaTyhjana(leveys, korkeus);
    sekoitus.asetaTyhjana(leveys, korkeus);
    tulos.asetaTyhjana(leveys, korkeus);
}


void siirtyma::kerroAjat() {
    //kulunut aika sekunteina
    float t_s = t0.getTime();
    float fps = (float)frame / t_s;
    float eta = (T2-frame) / fps;
    
    std::cout << "frame " << frame << " / " << T2 << "\n";
    std::cout << "fps: " << fps << "\n";

    std::cout << "kestää vielä noin ";
    
    if(eta < 60)
        std::cout << (int)eta << " s\n";
    else {
        int tunnit = eta/3600;
        int minuutit = (eta - tunnit*3600)/60;
        if(tunnit > 0)
            std::cout << tunnit << " h ";
        std::cout << minuutit << " min\n";
    }
}
    
    
std::string siirtyma::luoTiedostonimi() {
    static int i = 0;
    std::string tulos = "data/frames/frame" + std::to_string(i) + ".jpg";
    i++;
    return tulos;
}
    
    
void siirtyma::suorita() {
    ikkuna& I = haeIkkuna();
    I.piirra(kuva1);
    I.paivita();
    
    //tallennetaan alkukuvaa marginaalin verran
    for(int i=0; i<marginaali; i++) {
        std::cout << "tallennetaan kuva " << i << "/" << marginaali << "\n";
        kuva1.tallenna(luoTiedostonimi() );
    }
    
    do{
        if(frame < T1) {
            //0...1
            float f = ((float)frame) / T1;
            
            //sekoitusmäärä:
            float s1 = f;
            float s2 = f;
            
            //syrjäytysmäärät:
            float d1 = f;
            
            //sekoitetaan alkukuvaa välikuvaan -> sekoitus
            piirraKuvaan(sekoitus, SEKOITUS, s1, kuva1, kuva2);
            
            //syrjäytetään sekoitusta -> tmp
            piirraKuvaan(tmp, SYRJAYTYS, d1, sekoitus, kartta1);
            
            //sekoitetaan loppukuvaan -> tulos
            piirraKuvaan(tulos, SEKOITUS, s2, tmp, kuva3);

        }
        else if (frame<T2) {
            ;
        }
        else{
            piirraKuvaan(tulos, kuva3);
        }
    
        I.piirra(tulos);
        
        if(frame < T2) {
            //tallennetaan ja näytetään tulos
            if(tallennetaan)
                tulos.tallenna(luoTiedostonimi() );                
            kerroAjat();

        }
        
        else if (frame == T2) {
            //tallennetaan alkukuvaa marginaalin verran
            for(int i=0; i<marginaali; i++) {
                std::cout << "tallennetaan kuva " << i << "/" << marginaali << "\n";
                kuva3.tallenna(luoTiedostonimi() );
            }
            std::cout << "Siirtymä päättyi\n";
        }

        if(!tallennetaan)
            t.sleep();

        frame++;
    }
    //Jos tallennetaan, lopetetaan ohjelma heti kun siirtymä päättyy
    //Jos ei tallenneta, odotetaan että käyttäjä sulkee ohjelman
    while(I.paivita() && (frame <= T2 || tallennetaan == false) );
}    
    

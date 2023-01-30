#include "kuvanPiirtaja.h"

#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>

#include <opencv2/opencv.hpp>


bool kuvanPiirtaja::piirraKuvaan( kuva& kohde, const kuva& lahde) {
    return piirraKuvaan(kohde, EI_EFEKTIA, 0.5, lahde);
}

    
bool kuvanPiirtaja::piirraKuvaan(   kuva& kohde, 
                                    int efekti, 
                                    float maara,
                                    const kuva& lahde1,
                                    const kuva& lahde2,
                                    const kuva& lahde3,
                                    const kuva& lahde4,
                                    const kuva& lahde5
                                ) {
    auto it = shaderit.find(efekti);
    if (it == shaderit.end())
        efekti = EI_EFEKTIA;
    shaderit[efekti].asetaUniform("maara", maara);
    shaderit[efekti].kayta();
    lahde2.kayta(1);
    lahde3.kayta(2);
    lahde4.kayta(3);
    lahde5.kayta(4);

    return kohde.piirra(lahde1);
}


kuvanPiirtaja::kuvanPiirtaja() {

    //Luodaan ikkuna. Jos se on jo luotu, ei haittaa
    ikkuna& I = luoIkkuna(1500, 1000);

    //Ladataan shaderit
    const std::string vs = "data/shaderit/kokoruutu.vsh";
    shaderit[EI_EFEKTIA].lataa(vs, "data/shaderit/default.fsh");
    shaderit[SEKOITUS].lataa(vs, "data/shaderit/sekoitus.fsh");
    shaderit[SYRJAYTYS].lataa(vs, "data/shaderit/syrjaytys.fsh");
    
    shaderit[EI_EFEKTIA].kayta();
}

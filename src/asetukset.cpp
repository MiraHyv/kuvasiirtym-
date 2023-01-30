#include "asetukset.h"

#include <fstream>
#include <exception>
#include <iostream>


bool asetukset::lataaAsetukset(std::string tiedosto) {
    if(tiedosto == "")
        return true;
    
    else {
        //avataan tiedosto ja luodaan json-olio
        std::ifstream i(tiedosto);
        if(i.is_open() ) {
            nlohmann::json J;
            try{
                i >> J;
            }
            catch(std::exception& e) {
                std::cout   << "Asetustiedoston " << tiedosto
                            << " avaaminen ei onnistunut\n";
                i.close();
                return false;
            }
            i.close();
            
            //Luetaan arvot json-oliosta
            try{
                if(J.count("tallennetaan") && J["tallennetaan"].is_boolean() )
                    tallennetaan = J["tallennetaan"];
                if(J.count("T1") && J["T1"].is_number() )
                    T1 = J["T1"];
                if(J.count("T2") && J["T2"].is_number() )
                    T2 = J["T2"];
                if(J.count("leveys") && J["leveys"].is_number() )
                    leveys = J["leveys"];
                if(J.count("korkeus") && J["korkeus"].is_number() )
                    korkeus = J["korkeus"];
                if(J.count("fps_tulos") && J["fps_tulos"].is_number() )
                    fps_tulos = J["fps_tulos"];
                if(J.count("fps_esikatselu") && J["fps_esikatselu"].is_number() )
                    fps_esikatselu= J["fps_esikatselu"];
                if(J.count("marginaali") && J["marginaali"].is_number() )
                    marginaali= J["marginaali"];
                
                if(J.count("kuva1") && J["kuva1"].is_string() )
                    kuvatiedostot[0]= J["kuva1"];
                if(J.count("kuva2") && J["kuva2"].is_string() )
                    kuvatiedostot[1]= J["kuva2"];
                if(J.count("kuva3") && J["kuva3"].is_string() )
                    kuvatiedostot[2]= J["kuva3"];
                
                if(J.count("kartta1") && J["kartta1"].is_string() )
                    karttatiedostot[0]= J["kartta1"];
                if(J.count("kartta2") && J["kartta2"].is_string() )
                    karttatiedostot[1]= J["kartta2"];
                if(J.count("kartta3") && J["kartta3"].is_string() )
                    karttatiedostot[2]= J["kartta3"];
                if(J.count("kartta4") && J["kartta4"].is_string() )
                    karttatiedostot[3]= J["kartta4"];
                if(J.count("kartta5") && J["kartta5"].is_string() )
                    karttatiedostot[4]= J["kartta5"];
                if(J.count("kartta6") && J["kartta6"].is_string() )
                    karttatiedostot[5]= J["kartta6"];

            }
            catch(std::exception& e) {
                std::cout   << "Arvojen lukeminen asetustiedostosta "
                            << tiedosto << " ei onnistunut\n";
                return false;
            }
        }
        else {
            std::cout   << "Asetustiedoston " << tiedosto
                        << " avaaminen ei onnistunut\n";
            return false;            
        }
    }
}

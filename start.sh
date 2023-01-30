#!/usr/bin/python
#coding=UTF-8

#Generoi asetustiedosto ja käynnistä sitten ohjelma

import os
import sys
import json

#Asetukset
asetukset = {
    "T1" : 1000,
    "T2" : 2000,
    "tallennetaan" : False
}

framepolku = "data/frames/"
asetuspolku = "asetukset.conf"
kuvapolku = "data/kuvat/"
karttapolku = "data/kartat/"

for a in sys.argv:
    if a == "--render":
        asetukset["tallennetaan"] = True

if(asetukset["tallennetaan"]):
    #Jos frame-polkua ei ole, luodaan se
    if os.path.isdir(framepolku) == False:
        print("Luodaan " + framepolku)
        os.mkdir(framepolku)

#Katsotaan kuvien nimet kansiosta
kuvatiedostot = os.listdir(kuvapolku)
kuvatiedostot.sort()
i = 1
for tiedosto in kuvatiedostot:
    kuvastr = "kuva" + str(i)
    i = i + 1
    asetukset[kuvastr] = kuvapolku + tiedosto

#Katsotaan karttojen nimet kansiosta
karttatiedostot = os.listdir(karttapolku)
karttatiedostot.sort()
i = 1
for tiedosto in karttatiedostot:
    karttastr = "kartta" + str(i)
    i = i + 1
    asetukset[karttastr] = karttapolku + tiedosto

#Tuotetaan asetuksista json ulos
my_json = json.dumps(asetukset, indent=2)
f = open(asetuspolku, "w")
f.write(my_json)
print(my_json)
f.close()

#Käynnistetään ohjelma
os.system("./piirto")

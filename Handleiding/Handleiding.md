# **Handleiding: Watermanagementsysteem voor een Eco-Serre**

## Overzicht
Dit project beheert het watergebruik in een eco-serre door gebruik te maken van:

Regenwateropvang op basis van weersvoorspellingen.
Twee reservoirs met niveaucontrole.
Waterkwaliteitscontrole (pH en EC) in het grote reservoir.
Notificatiesystemen voor lage waterniveaus of afwijkende waterkwaliteit.

## Benodigdheden
Hardware

1. Reservoirs:
Klein reservoir (opvang van regenwater).
Groot reservoir (opslag en kwaliteitscontrole).

2. Vlottersensoren:
Eerste reservoir: 3 sensoren (laag, gemiddeld, voldoende water).
Tweede reservoir: 4 sensoren (laag, gemiddeld, voldoende, vol/overflow).

3. Pompen:
 pomp voor watertransport van klein naar groot reservoir.

4. pH-sensor (bijvoorbeeld SEN0161).

5. EC-sensor (bijvoorbeeld een analoge geleidbaarheidssensor).

6. ESP32 microcontroller:
Voor sensoren en draadloze communicatie.
Weerstation API (bijvoorbeeld OpenWeatherMap) voor regenvoorspellingen.

7. afvoer:
Leidingen.

8. Software
Visual studio code voor programmeren van de ESP32.
Bibliotheken:
WiFi (voor verbinding met internet).
HTTPClient (voor API-communicatie).

## Instalatie

### Opstelling van de Hardware

1. reservoirs en Vlottersensoren

    Installeer de vlottersensoren op de juiste niveaus in de reservoirs:
    Eerste reservoir: Laag, gemiddeld, voldoende water.
    Tweede reservoir: Laag, gemiddeld, voldoende water, vol/overflow.
    Verbind de sensoren met de ESP32 via de digitale pinnen.

2. Pomp
    Verbind de pomp voor watertransport met het kleine reservoir.
    Sluit de pomp aan op een relaismodule, zodat de ESP32 de pomp kan aansturen.

3. waliteitssensoren
    Installeer de pH- en EC-sensor in het grote reservoir.
    Verbind ze met de analoge pinnen van de ESP32.

4. Overflow
    Zorg voor een overflowleiding van het tweede reservoir naar de grond.
    
5. Programma instaleren


## Hoe het Werkt

### Weersvoorspellingen:
   De ESP32 haalt voorspellingen op en berekent de verwachte hoeveelheid regenwater voor de volgende dag, de volgende 3 dagen en de komende 7 dagen.
### Niveausensoren:
   De sensoren in het reservoir zijn switches die een '0' geven als er geen water is en een '1' als er wel water is.
    Deze sensoren hangen op 4 of 3 verschillende niveaus in het reservoir.
    Activeren de pomp indien er genoeg water in het 1ste reservoir is en er water nodig is in het 2de reservoir.
### Kwaliteitscontrole:
Controleert pH en EC in het grote reservoir.
Stuurt meldingen bij afwijkende waarden.
### Overflow:
Voert overtollig water uit het tweede reservoir af naar de grond.

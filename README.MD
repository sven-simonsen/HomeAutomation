# Automatische Feuchtigkeitssteuerung für Innenräume

## Hintergrund
Wir hatten Probleme mit Feuchtigkeit in Lagerraum und Keller. Im Altbau nicht überraschend, aber trotzdem nicht tragbar.  
Aktiven Entfeuchter reinstellen war uns auf Dauer zu Strom-intensiv und irgendwie un-smart, aber immer hinrennen wenn es Draußen warm und trocken wirkt auch keine verlockende Aussicht. Noch dazu wann ist Außenluft eigentlich nochmal trocken?  
Daten mussten her!  
Und idealerweise automatische Lüftung.

## Schritt 1: Messen
Drinnen messen war sowieso nötig um unsere Maßnahmen besser tracken zu können. Man will ja nich 15€ im Monat für Entfeuchter-Strom bezahlen und dann trotzdem Schimmel an den gelagerten Sachen haben.
Draußen noch dazu messen war da naheliegend um besser einzuschätzen wann man eigentlich lüften sollte.  
Und da man nicht stumpf bei niedriger relativer Luftfeuchtigkeit Draußen lüften sollte, denn wieviel Wasser in der Luft ist hängt sowohl von % Luftfeuchte ab als auch von Temperatur, auch noch gleich eine Berechnung dran die g/m^3 Wasser in der Luft ausspuckt.

### Home Assistant auf Raspberry Pi
Für Daten Speicherung und Anzeige haben wir uns [Home Assistant](https://www.home-assistant.io/) auf einen Raspberry Pi 4 geladen und den an unseren Router gehängt.  
Nachteil: Setup hat viel Do it Yourself Charakter.  
Vorteile: Open Source. Wenns erstmal läuft läuft es. Sehr erweiterbar, inzwischen steuern und tracken wir darüber auch Steckdosen und Wasserventile.  
  
Wie das genau geht haben wir uns von diesem Dude auf YouTube erklären lassen https://www.youtube.com/watch?v=KKj9V-tbLPY  
War zugegeben etwas fummeliger als dort beschrieben.

### ESP-Boards und Sensoren
Im nächsten Schritt haben wir dann ESP8266 Boards in HomeAssistant angemeldet und per ESP-Home mit Software bespielt. Diese kleinen Boards mit integriertem WLAN chip kann man überall in WLAN Reichweite hinlegen, solange man sie per micro-USB port mit Strom versorgt hat man dann einen WLAN-connecteten minicomputer mit Anschlusspins zum lesen oder schreiben.  
Wieder der gute Simon dazu https://www.youtube.com/watch?v=9YkICApObvo  
An diese Boards haben wir DHT22 Sensoren (drauf achten welche mit Pullup-Wiederstand zu besorgen sonst muss man selber einen dran microlöten) gesteckt die Drinnen und Draußen Temperatur und relative Luftfeuchte messen sollten. (Siehe AutoVenterBoard.jpg)

### ESP-Home
Wir haben erst mit Arduino IDE C code geschrieben und auf die Boards hart drauf gespielt, aber das ist nicht Weg den man gehen will.  
Als gute Lösung hat sich ESP-Home erwiesen, wo man für ESP-Boards in einer yaml Syntax "coden" kann. Das Aktionsflow ist für Programmierer etwas unintuitiv, aber dafür sind alle möglichen Treiber (zb für die verschiedenen DHT sensoren) und tool (zb eines das absolute Luftfeuchtigkeit berechnet) leicht einzubinden.  
SimpleHygrosensor.yaml ist so ein code den man auf ein Board aufspielen kann um einen einzelnen Sensor auszulesen.  
Mit 2 Sensoren kann man dann noch einen Vergleich einbauen. VentingInformer.yaml macht das zb und knippst dann ein LED an wenn man Lüften sollte.  
Alle Ergebnisse über HomeAssistant ganz gut graphisch einsehbar und langzeit-gespeichert (auch wenn die zeitliche Auflösung gesenkt wird bei älteren Daten).

### Lüfter Steuerung
So cool jetzt haben wir also Daten, was automatisieren wir jetzt damit?  
Wir haben im Keller einen und im Schuppen 2 Lüfter in ein Fenster gesetzt (abgeguckt bei Ulrich Schmerolds Doc "Das Taupunkt Lüftungssystem" liegt bei) und diese, teilweise per SSRelay, teilweise per WLAN-Steckdose, an und aus geschalten. Das Ganze auf Basis der Sensorvergleiche wie sie zb in VentingInformer.yaml schon zu sehen waren.  
Die Relay Lösung war eigentlich am Elegantesten, das Sensorboard muss dafür ein Signalkabel zum Relay führen und diese Kabel ansteuern immer dann wenn die Logik sagt es sei gut zu Lüften.  
Verkabelung sieht man in AutoVenterRelayOff.jpg und den Code dazu in AutoVenter.yaml  
Um das Relay einzubauen unterbricht man das Stromkabel des Lüfters und auf der P Ader das Relay dazwischen. Den Rest der Adern verbindet man wieder. Das Signal vom ESP-Board ans Relay steuert jetzt wann das Relay die P-Ader verbindet und damit den Lüfter aktiviert.  
  
Ulrich Schmerolds hat in seinem PDF da auch Verkabelungsdiagramme zu die das Konzept aufzeigen. Ein großer Unterschied für uns ist, dass die ESP-Boards selber 5V Strom liefern können und damit das Relay keinen separaten Strom von Netzteil braucht für die Signalseite.

## Was man braucht
Raspberry Pi (wir haben den 4er)  
Crimpwerkzeug (Crimpzange, Stecker, Kabelstücke, etc)  
[ESP8266 Boards](https://www.amazon.de/dp/B09Z6T2XS4?psc=1&ref=ppx_yo2ov_dt_b_product_details)  
[DHT22 Sensoren](https://www.amazon.de/dp/B07L4WCFPZ?ref=ppx_yo2ov_dt_b_product_details&th=1) Hier haben wir noch nix Besseres gefunden, aber die Sensoren neigen ehrlich gesagt dazu langsam zu wandern und daher müssen wir die einmal im Jahr sammeln und gegeneinander abgleichen. Nervig.  
Man sagte mir ein Breadboard wäre gut gewesen um vorher zu Testen was man bauen will...  

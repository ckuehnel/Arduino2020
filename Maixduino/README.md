## Maixduino Benchmarks

Um die Performance des Maixduino gegenüber anderen Arduinos resp. Arduino-kompatiblen Mikrocontrollern zu vergleichen, habe ich zwei Benchmarks laufen lassen:
•	Sieve of Eratosthenes
•	CoreMark
Den ersten Benchmark habe ich zu Vergleichszwecken verwendet, da ich in der Vergan-genheit damit bereits zahlreiche Tests vorgenommen habe.

Erweiterung der Arduino-Familie: ESP32 – wie gut ist es?
https://www.elektroniknet.de/design-elektronik/embedded/erweiterung-der-arduino-familie-esp32-wie-gut-ist-es-160294.html

Arduino32: Die jungen Wilden.
http://www.elektroniknet.de/embedded/arduino32-die-jungen-wilden-131502.html

CoreMark 1.0 hingegen ist ein vom EEMBC empfohlener Test und zudem an die Arduino-Umgebung angepasst (https://www.eembc.org/coremark/). Beide Benchmarks stehen hier zum Download zur Verfügung.

Die Resultate der beiden Benchmarks zeigen die Abbildungen Sieve.png und Coremark.png.. 
Verglichen wurden ein Arduino Due (AT91SAM3X8E@84 MHz), eine ESPduino-32 (ESP-Wroom-32@80 MHz) und ein Maixduino (Kendryte K210 RISC-V@400 MHz).


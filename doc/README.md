# Starter do obsługi rajdów

Jacek Dobrowolski, Filip Kulik

## Krótki opis tematu projektu

Celem projektu jest stworzenie startera do rajdów. Urządzenie ma umożliwiać odliczanie czasu do startów zawodników. Będzie miało zegar synchronizowany z satelitami GPS, będzie sterowany zdalnie. Ponad to będzie zapisywać dane dotyczące startów na karcie SD.

## Wykaz realizowanych funkcji

- Wyświetlacz dla zawodnika odliczający czas do startu
- Zielona dioda sygnalizująca start
- Czerwona dioda sygnalizująca to, że fotokomórki się nie widzą lub są przecięte
- Wyświetlacz z godzina (godzina i minuta)
- Synchronizacja czasu z GPS
- 3 tryby pracy: 
    - start co 30 sekund
    - start co 60 sekund
    - start z zewnętrznego wyzwolenia (przycisk wciśnięty, na starterze pokazuje się 3, 2, 1, zielone)
- Detekcja falstartu przy użyciu fotokomórki 
    - jeśli przecięcie miedzy 5, a 1 to  to zielona dioda miga.
    - jeśli przecięcie miedzy 10, a 5 to nie odlicza dalej (bo to znaczy, ze np. startujący zawodników wszedł przez przypadek między fotokomórki albo fotokomórki są źle ustawione się nie widza) czerwona dioda się zapala.
- Łączność po WiFi można ustawić tryb pracy oraz wysyłanie czasu przecięcia.
- Zapiswanie czasów przecięcia na karcie SD.

## 4. List modułów i komponentów potrzebnych do realizacji projektu

- NUCLEO-H755ZI-Q płytka nucleo
- X-NUCLEO-IDW04A1 rozszerzenie do płytki nucleo z Wi-Fi i kartą SD
- NEO M6 GPS module
- dioda i odbiornik IR tworzące fotokomórkę
- przycisk zastępujący pedał
- 2 wyświetlacze LED
- 1 dioda zielona
- 1 dioda czerwona

## 5. Ogólny opis algorytmu działania programu

Program sprawdza czas GPS i się z nim synchronizuje co minutę, tak aby nie wpływać na czas odliczania do startu. Zależnie od trybu pracy wchodzi w stan odliczania do startu co 30 sekund, co 60 sekund lub po otrzymaniu sygnału z przycisku. Po odliczeniu zapisuje czasy przecięcia, a jeśli jest połączony po WiFi ze smartfonem to wysyła czas przecięcia fotokomórki i zapisuje go na karcie SD. Po otrzymaniu odpowiedniego sygnału z WiFi przechodzi w stan zmiany ustawień.
Mikrokontroler będzie działał poprzez wybudzanie z użyciem przerwań generowanych przez timery, fotokomórke lub w jednym z trybów pracy przycisk, po wykonaniu pracy przechodzi do trybu niskiego poboru energii.

## 6. Harmonogram prac wraz z planowanym podziałem zadań

| Czynność                   | Kto? | Do kiedy? |
|----------------------------|:----:|----------:|
| Zakup dodatkowych podzespołów | JD | 11 listopada |
| Zaimplementowanie wyświetlania godziny. | JD | 15 grudnia |
| Zaimplementowanie odliczania i wyświetlenie tego na wyświetlaczu. | FK | 20 grudnia |
| Zaimplementowanie wyzwalania zewnętrznego startu | FK | 23 grudnia |
| Zaimplementowanie synchronizacji godziny z GPS | JD | 10 stycznia |
| Zaimplementowanie łączności po WiFi | JD | 20 stycznia |
| Zaimplementowanie zapisywania na kartę SD | FK | 25 stycznia |

	JD - Jacek Dobrowolski
	FK - Filip Kulik

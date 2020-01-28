# Opis działania

[Dokumentacja wstępna](./README.md)

#### Rozruch

Zaraz po podłączeniu do zasilania przez pierwsze 15 sekund urządzenie jest w trybie wyboru. Poprzez przycisk użytkownika można wybierać pomiędzy pracą automatyczną, start co 30 sekund lub strat co 60 sekund albo startem ręcznym. Tryby są oznaczone cyframi na dolnym wyświetlaczu kolejno 30, 60, 3. Po odczekaniu wyznaczonego czasu urządzenie rozpocznie pracę. W tym czasie będzie oczekiwać na dane z modułu GPS. Kiedy moduł wyśle pełny pakiet z czasem i datą starter zapiszę je w peryferium RTC. Moduł GPS wysyła pakiety w momencie wybicia kolejnej sekundy. Fakt ten jest wykorzystywany do synchronizacji.

#### Tryb automatyczny

Starter w trybie automatycznym odlicza od 30 lub 60 do 0. W przedziale 10, 5 urządzenie jest w trybie gotowości i przecięcie bramki oznacza że prawdopodobnie fotokomórki nie mają dobrego pola widzenia. W tej sytuacji licznik wraca na początek i zapala czerwoną diodę. Gdy licznik jest pomiędzy 5 a 0 starter jest "uzbrojony" przecięci fotokomórki oznacza falstart (zapala się czerwona dioda) i jest postrzegane jako start czas zostanie zapisany i wysłany. Gdy na liczniku pokaże się 0 zapali się zielona dioda oznacza to że zawodnik może startować. Gdy nastąpi przecięcie gaśnie zielona dioda a czas startu (czas 0 na liczniku zostaje zapisany i wysłany). 5 sekund później starter wraca w stan spoczynku i przecięcia są ignorowane gasną również diody czerwona i zielona. Jeśli nie wykryto przecięcia przez całą sekwencje startową czas nie zostaje zapisany ani wysłany.

#### Tryb ręczny

Starter w trybie ręcznym jest wyzwalany przyciskiem użytkownika (niebieski). Po wciśnięciu na wyświetlaczu pojawi się 3, 2, 1, oraz zapali się zielona dioda. W każdym momencie po wciśnięciu przycisku zawodnik może startować bez falstartu. Czas przecięcia zostaje zapisany i wysłany.

#### Fotokomórka

Poprawność pracy fotokomórki można sprawdzić poprzez przecięcie jej wiązki zmieni to stan diody pomarańczowej.

#### Praca zdalna

Urządzenie jest wyposażone w moduł WiFi który umożliwia odbieranie czasu startu przez socket tcp port 8888. Wysłana jest godzina minuta sekunda i milisekunda. Dodatkowo kopia wysłanych danych jest zapisana na karcie SD.

# Podsumowanie projektu

#### Problemy w czasie prac

W czasie prac napotkaliśmy kilka problemów

- Chińskie instrukcje napisane przez słaby translator
- CubeMX i problemy z generacją konfiguracji większość jest wykonana ręcznie
- Brak wsparcia niektórych funkcjonalności przez bibliotekę HAL
- Przerwania z GPIO przez EXTI są dziwnie obsługiwane przez HAL
- Zamienione RXTX (nawet shield z WiFi nie może być wpięty bezpośrednio w płytkę Nucleo)
- Porzucone wsparcie dla modułu WiFi przez CubeMX
- Problem z odbiorem UART z modułu WiFi

#### Czego się nauczyliśmy

- Zawsze sprawdzać piny RXTX interfejsu UART
- Biblioteki producentów często są nie pełne lub mają błędy

#### Rozbieżności z dokumentacją wstępną

- Miganie diody zamienione na przełączanie lub ciągłe świecenie
- Brak zdalnego wyboru trybu pracy.





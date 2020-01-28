/**
  ******************************************************************************
  * @file           global_variables.h
  * @authors        Filip Kulik, Jacek Dobrowolski
  * @brief          Plik nagłówkowy gromadzoncy zmienne globalne
  ******************************************************************************
  */
#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "TM1637.h"
#include "stm32h7xx.h"

/**
 * Uchwyt do peryferium zegara czasu rzeczywistego
 */
extern RTC_HandleTypeDef hrtc;

/**
 * Uchwyt do peryferium uart połączonego z modułem GPS
 */
extern UART_HandleTypeDef huart4;

/**
 * Uchwyt do peryferium uart połączonego z STLINK
 */
extern UART_HandleTypeDef huart3;

/**
 * Uchwyt do peryferium uart połączonego z modułem WiFi
 */
extern UART_HandleTypeDef huart6;

/**
 * Uchwyt do wyświetlacza LED wyświetlającego godzinę
 */
extern TM1637_TypeDef display_clock;

/**
 * Uchwyt do wyświetlacza z odliczaniem do startu
 */
extern TM1637_TypeDef display_counter;

/**
 * Struktura przechowująca czas
 */
extern volatile RTC_TimeTypeDef time;

/**
 * Struktura przechowująca czas ostatniego startu
 */
extern volatile RTC_TimeTypeDef start_time;

/**
 * Struktura przechowywująca datę
 */
extern volatile RTC_DateTypeDef date;

/**
 * Enumerator określający stan synchronizacji
 */
extern volatile enum SyncState{
    /*! Zegar zsynchronizowany z czasem UTC*/
    IN_SYNC,
    /*! Zegar czeka na synchronizację */
    WAITING_FOR_SYNC
    }
    /*! przechowuje stan synchronizacji zegara z czasem UTC */
    gps_sync;

/**
 * Enumerator określający tryb pracy startera
 */
extern volatile enum StarterMode{
    /*!  Wyzwalanie automatyczne co 30 lub 60 sekund w zależności od ::counter_reload */
    AUTO_START,
    /*!  Wyzwalanie z przycisku użytkownika */
    EXTERNAL,
    /*!  Wybór trybu pracy */
    SETUP,
    /*!  Inicjalizacja */
    INIT
    }
    /*! Przechowuje tryb pracy startera */
    starter_mode;

/**
 * Enumerator określający stan startu
 */
extern volatile enum StartState{
    /*! Bramka otwarta zawodnik może startować przecięcie zostanie uznane za start */
    GATE_OPEN,
    /*! Bramka zamknięta przecięcie w tym momencie spowoduje falstart */
    GATE_CLOSED,
    /*! Bramka jest przygotowana do startu jeśli w tym momencie nastąpi
        przecięcie, odliczanie zacznie się od początku i zapali się czerwona
        dioda */
    GATE_READY,
    /*! Bramka jest w spoczynku przecięcie zostanie zignorowane */
    NO_START,
    /*! Bramka została przecięta przed końcem odliczania */
    FALSTART
    }
    /*!  Przechowuje stan startu */
    start_state;

/**
 * Główny licznik odlicza czas do startu
 */
extern volatile uint8_t counter;

/**
 * Wartość do której przeładowywuje się główny licznik ::counter
 */
extern volatile uint8_t counter_reload;

/**
 * Bufor do którego przechwytywane są komunikaty z modułu GPS
 */
extern uint8_t rx_data[64];
		
/**
	* Ciąg przechowywujący datę
	*/
extern char date_string[10];

#endif //  GLOBAL_VARIABLES_H

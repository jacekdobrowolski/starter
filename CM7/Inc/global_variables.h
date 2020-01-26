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
RTC_HandleTypeDef hrtc;

/**
 * Uchwyt do peryferium uart połączonego z modułem GPS
 */
UART_HandleTypeDef huart4;

/**
 * Uchwyt do peryferium uart połączonego z STLINK
 */
UART_HandleTypeDef huart3;

/**
 * Uchwyt do peryferioum uart połączonego z modułem WiFi
 */
UART_HandleTypeDef huart6;

/**
 * Uchwyt do wyświetlacza LED wyświetlającego godzinę
 */
TM1637_TypeDef display_clock;

/**
 * Uchwyt do wyświetlacza z odliczaniem do startu
 */
TM1637_TypeDef display_counter;

/**
 * Struktura przechowywująca czas
 */
volatile RTC_TimeTypeDef time = {0};

/**
 * Struktura przechowywująca datę
 */
volatile RTC_DateTypeDef date = {0};

/**
 * Enumerator określający stan synchronizacji
 */
volatile enum SyncState{IN_SYNC, WAITING_FOR_SYNC}gps_sync;

/**
 * Enumerator określający tryb pracy startera
 */
volatile enum StarterMode{
    AUTO_START, /*!  Wyzwalanie automatyczne co 30 lub 60 sekund w zależności od ::counter_reload */
    EXTERNAL,   /*!  Wyzwalanie z przycisku użytkownika */
    SETUP,      /*!  Wybór trybu pracy */
    INIT        /*!  Inicjalizacja */
    }starter_mode;

/**
 * Enumerator okjreślający stan startu
 */
volatile enum StartState{
    GATE_OPEN,   /*! Bramka otwarta zawodnik może startować przecięcie zostanie uznane za start */
    GATE_CLOSED, /*! Bramka zamknięta przcięcie w tym momencie spowoduje falstart */
    GATE_READY,  /*! Bramka jest przygotowana do startu jeśli w tym momencie nastąpi
                     przecięcie odliczanie zacznie się odpoczątku i zapali się czerwona
                     dioda sygnalizująca usterke fotokomórki */
    NO_START,    /*! Bramka jest w spoczynku przecięcie zostanie zignorowane */
    FALSTART     /*! Bramka została przecięta przed końcem odliczania */
    }start_state;

/**
 * Główny licznik odlicza czas do startu
 */
volatile uint8_t counter;

/**
 * Wartość do której przeładowywuje się główny licznik ::counter
 */
volatile uint8_t counter_reload;

/**
 * Bufor do którego przechwytywane są komunikaty z modułu GPS
 */
uint8_t rx_data[64];

#endif //  GLOBAL_VARIABLES_H

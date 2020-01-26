/**
  ******************************************************************************
  * @file           : TM1637.h
  * @authors        : Filip Kulik, Jacek Dobrowolski
  * @brief          : Plik naglowkowy dla pliku TM1637.c.
  *                   Ten plik zawiera deklaracje funkcji zwiazanych z wyswietlaczem TM1637.
  ******************************************************************************
  */
#ifndef TM1637_TYPEDEF_H
#define TM1637_TYPEDEF_H

#include <stdint.h>
#include "stm32h7xx_hal.h"

/// Włączyć dwukropek pośrodku wyświetlacza
#define TM1637_SEPARATOR_ON 1
/// Wyłączyć dwukropek pośrodku wyświetlacza
#define TM1637_SEPARATOR_OFF 0
/// Maksymalna jasność wyświetlacza
#define TM1637_MAX_BRIGHTNESS 0x8f
/// Stały adres TM1637
#define TM1637_FIXED_ADDRESS 0x44
/// Odstęp pomiędzy adresami
#define TM1637_AUTO_INCREMENT_ADDRESS 0x40
/// Adres pierwszego segmentu
#define TM1637_FIRST_SEGMENT_ADDRESS 0xc0
/// Bit odpowiadający dwukropkowi pośrodku wyświetlacza
#define TM1637_SEPARATOR 0x80
/// Brak separatora 
#define TM1637_NO_SEPARATOR 0x00

	/**
	 * Struktura odpowiedzialna za przechowywanie wartosci potrzebnych do dzialania wyswietlacza TM1637
	 */
typedef struct 
{
	uint16_t clk;/**<numer pinu clk podpietego do pinu clk wyswietlacza.*/
	uint16_t dio;/**<numer pinu dio podpietego do pinu dio wyswietlacza.*/
	GPIO_TypeDef* GPIO;/**<wskaznik do portu GPIO, w ktorej znajduja sie piny dio i clk.*/
}TM1637_TypeDef;
	/**
	 * Funkcja przestawia piny wyswietlacza na czytanie.
	 * \param display wskaznik na wyswietlacz (strukture).
	 */
void TM1637_PinsToIn(TM1637_TypeDef *display);

	/**
	 * Funkcja przestawia piny wyswietlacza na pisanie.
	 * \param display wskaznik na wyswietlacz (strukture).
	 */
void TM1637_PinsToOut(TM1637_TypeDef *display);

	/**
	 * Ustawia sekwencje startowa na pinach dio i clk (oba na stan wysoki, a nastepnie dio na niski).
	 * \param display wskaznik na wyswietlacz (strukture).
	 */
void TM1637_Start (TM1637_TypeDef *display);
 	/**
	 * Czeka na odpowiedz od wyswietlacza (ustawienie dio na stan niski)
	 * \param display wskaznik na wyswietlacz (strukture).
	 */
void TM1637_Ask (TM1637_TypeDef *display);
	/**
	 * Ustawia sekwencje stopu na pinach dio i clk (oba na stan niski, a nastepnie clk na wysoki i dio na wysoki).
	 * \param display wskaznik na wyswietlacz (strukture).
	 */
void TM1637_Stop (TM1637_TypeDef *display);
	/**
	 * Wysyla jeden bajt danych do wyswietlacza.
	 * \param display wskaznik na wyswietlacz (strukture).
	 * \param oneByte jeden bajt danych.
	 */
void TM1637_WriteByte(TM1637_TypeDef *display, unsigned char oneByte);
	/**
	 * Ustawia jasnosc wyswietlacza na podana wartosc.
	 * \param display wskaznik na wyswietlacz (strukture).
	 * \param brightness wartosc jasnosci, ktora ma byc ustawiona na wyswietlaczu
	 */
void TM1637_setBrightness(TM1637_TypeDef *display, unsigned char brightness);
	/**
	 * Inicjuje strukture wyswietlacza
	 * \param display wskaznik na wyswietlacz (strukture).
	 * \param clk numer pinu, ktory jest podpiety do pinu clk w wyswietlaczu
	 * \param dio numer pinu, ktory jest podpiety do pinu dio w wyswietlaczu
	 * \param GPIO wskaznik do portu GPIO, w ktorej znajduja sie piny dio i clk.
	 */
void TM1637_Init(TM1637_TypeDef *display, uint16_t clk, uint16_t dio, GPIO_TypeDef* GPIO);
	/**
	 * Konwertuje cyfre dziesietna na zapis szesnastkowy tego jakie segmenty maja zostac zapalone w wyswietlaczu.
	 * \param digit cyfra dziesietna do przekonwertowania (podanie 10 powoduje zwrot liczby, ktora odpowiada za zapalenie wszystkich segmentow,
	 * a 11 powoduje zwrot liczby, ktora odpowiada za niezapalenie zadnego segmentu.
	 */

unsigned char TM1637_ConvertDecToSegment(uint16_t digit);
	/**
	 * Wysyla do wyswietlacza cyfry, ktore maja zostac wyswietlone.
	 * \param display wskaznik na wyswietlacz (strukture).
	 * \param digits tablica 4 liczb, ktore maja zostac wyswietlone na wyswietlaczu.
	 * \param has_separator informacja czy separator powinien zostac zapalony na wyswietlaczu.
	 */
void TM1637_WriteDigits(TM1637_TypeDef *display, uint16_t digits[], uint8_t has_separator);

	/**
	 * Konwertuje czas na odpowiednie cyfry i wysyla je do wyswietlacza.
	 * \param display wskaznik na wyswietlacz (strukture).
	 * \param hour godzina, ktora ma zostac wyswietlona na wyswietlaczu.
	 * \param minute minuta, ktora ma zostac wyswietlona na wyswietlaczu.
	 * \param has_separator informacja czy separator powinien zostac zapalony na wyswietlaczu.
	 */
void TM1637_WriteTime(TM1637_TypeDef *display, uint8_t hour, uint8_t minute, uint8_t has_separator);

#endif // TM1637_TYPEDEF_H

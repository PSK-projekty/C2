#pragma once

/*****************************************************************//**
 * \file Global.hpp
 * \brief
 * Wartosci stale wielokrotnie powtarzajace sie oraz struktory wykorzystywane wielokrotnie
 *********************************************************************/

#include <cmath>

/// <summary>
/// W tym pliku znajduja sie stale wartosci, ktore sluza jako konfiguracja dla gry. Sa to miedzy innymi wartosci dotyczace szybkosci ruchu przeciwnikow, gracza, pociskow, a takze poziomow trudnosci, liczby przeciwnikow, klatek animacji itp.
/// </summary>

constexpr unsigned char BASE_SIZE = 16;
constexpr unsigned char ENEMY_BULLET_SPEED = 2;
constexpr unsigned char ENEMY_HIT_TIMER_DURATION = 3;
//Im wiecej przeciwnikow zabijemy tym szybciej reszta bedzie sie poruszac
constexpr unsigned char ENEMY_MOVE_PAUSE_DECREASE = 1;
constexpr unsigned char ENEMY_MOVE_PAUSE_MIN = 3;
constexpr unsigned char ENEMY_MOVE_PAUSE_START = 63;
constexpr unsigned char ENEMY_MOVE_PAUSE_START_MIN = 47;
constexpr unsigned char ENEMY_MOVE_SPEED = 2;
constexpr unsigned char ENEMY_TYPES = 5;
constexpr unsigned char EXPLOSION_ANIMATION_SPEED = 2;
constexpr unsigned char FAST_RELOAD_DURATION = 8;
//Przejscie do kolejnego poziomu po uplywie 64 klatek lub 1 sekundy
constexpr unsigned char NEXT_LEVEL_TRANSITION = 64;
constexpr unsigned char PLAYER_BULLET_SPEED = 4;
constexpr unsigned char PLAYER_MOVE_SPEED = 2;
constexpr unsigned char POWERUP_ANIMATION_SPEED = 16;
constexpr unsigned char POWERUP_SPEED = 2;
constexpr unsigned char POWERUP_TYPES = 3;
constexpr unsigned char RELOAD_DURATION = 31;
constexpr unsigned char SCREEN_RESIZE = 4;
constexpr unsigned char TOTAL_LEVELS = 8;
constexpr unsigned char UFO_ANIMATION_SPEED = 8;
constexpr unsigned char UFO_MOVE_SPEED = 1;

constexpr unsigned short ENEMY_SHOOT_CHANCE = 8192;
constexpr unsigned short ENEMY_SHOOT_CHANCE_INCREASE = 64;
constexpr unsigned short ENEMY_SHOOT_CHANCE_MIN = 1024;
constexpr unsigned short POWERUP_DURATION = 512;
constexpr unsigned short SCREEN_HEIGHT = 180;
constexpr unsigned short SCREEN_WIDTH = 320;
constexpr unsigned short UFO_TIMER_MAX = 1024;
constexpr unsigned short UFO_TIMER_MIN = 768;

constexpr std::chrono::microseconds FRAME_DURATION (16667);

/// <summary>
/// Ta struktura definiuje klase Bullet, ktora reprezentuje pociski przeciwnikow.
/// </summary>

struct Bullet {
	/// <summary>
	/// dead - flaga oznaczajaca, czy pocisk jest martwy (wychodzi poza ekran) czy nie.
	/// real_x i real_y - przechowuja rzeczywista pozycje pocisku.
	/// step_x i step_y - przechowuja predkosc pocisku w osi X i Y.
	/// x i y - przechowuja pozycje pocisku zaokraglona do najblizszej calkowitej wartosci.
	/// previous_x i previous_y - przechowuja poprzednia pozycje pocisku.
	/// </summary>
	bool dead;

	float real_x;
	float real_y;
	float step_x;
	float step_y;

	short x;
	short y;

	std::array<short, 3> previous_x;
	std::array<short, 3> previous_y;

	/// <summary>
	/// Konstruktor klasy Bullet. 
	/// </summary>
	/// <param name="i_step_x">Predkosc pocisku w osi x.</param>
	/// <param name="i_step_y">Predkosc pocisku w osi y.</param>
	/// <param name="i_x">Pozycja poczatkowa pocisku w osi x.</param>
	/// <param name="i_y">Pozycja poczatkowa pocisku w osi y.</param>
	Bullet (float i_step_x, float i_step_y, short i_x, short i_y):
		dead (0),
		real_x (i_x),
		real_y (i_y),
		step_x (i_step_x),
		step_y (i_step_y),
		x (i_x),
		y (i_y) {
		previous_x.fill (x);
		previous_y.fill (y);
	}

	/// <summary>
	/// Funkcja jest odpowiedzialna za aktualizacje pozycji pocisku. Przede wszystkim, sprawdza czy pocisk jest martwy (wychodzi poza ekran) za pomoca flagi dead. Jesli nie jest martwy, to nastepujace kroki sa wykonywane:
	/// 1. real_x i real_y sa aktualizowane o wartosci step_x i step_y.
	/// 2. W petli for, dla kazdej pozycji w tablicy previous_x i previous_y, aktualna pozycja jest przypisywana do nastepnej pozycji.
	/// 3. x i y sa ustawione na wartosci zaokraglone do najblizszej calkowitej wartosci dla real_x i real_y.
	/// 4. Sprawdzenie czy pocisk wychodzi poza ekran. Jesli tak, flaga dead jest ustawiona na true.
	/// 
	/// Dzieki tej funkcji pocisk jest przesuwany zgodnie z jego predkoscia i jego pozycja jest aktualizowana. Sprawdza takze czy pocisk wychodzi poza ekran i ustawia flage dead na true, gdy tak sie dzieje.
	/// </summary>
	void update () {
		if( 0 == dead ) {
			real_x += step_x;
			real_y += step_y;

			for( unsigned char a = 0; a < previous_x.size () - 1; a++ ) {
				previous_x[a] = previous_x[1 + a];
				previous_y[a] = previous_y[1 + a];
			}

			previous_x[previous_x.size () - 1] = x;
			previous_y[previous_y.size () - 1] = y;

			x = round (real_x);
			y = round (real_y);

			if( x <= -BASE_SIZE || y <= -BASE_SIZE || SCREEN_HEIGHT <= y || SCREEN_WIDTH <= x ) {
				dead = 1;
			}
		}
	}

	/// <summary>
	/// Zwraca prostokat reprezentujacy hitbox pocisku.
	/// </summary>
	/// <returns>Hitbox</returns>
	sf::IntRect get_hitbox () const {
		return sf::IntRect (x + 0.375f * BASE_SIZE, y + 0.375f * BASE_SIZE, 0.25f * BASE_SIZE, 0.25f * BASE_SIZE);
	}
};


/// <summary>
/// Ta struktura definiuje klase Powerup, ktora reprezentuje power-upy (bonusy) w grze.
/// </summary>
struct Powerup {
	bool dead;

	short x;
	short y;

	//0 - Tarcza
	//1 - Szybkie przeladowanie
	//2 - Potrojny strzal

	unsigned char type;

	/// <summary>
	/// Konstruktor klasy Powerup
	/// </summary>
	/// <param name="i_x">Pozycja poczatkowa power-upu w osi x.</param>
	/// <param name="i_y">Pozycja poczatkowa power-upu w osi y.</param>
	/// <param name="i_type">Typ power-upu.</param>
	Powerup (short i_x, short i_y, unsigned char i_type):
		dead (0),
		x (i_x),
		y (i_y),
		type (i_type) {

	}

	/// <summary>
	/// Zwraca prostokat reprezentujacy hitbox power-upu.
	/// </summary>
	/// <returns>Hitbox bonusu</returns>
	sf::IntRect get_hitbox () const {
		return sf::IntRect (x + 0.25f * BASE_SIZE, y + 0.25f * BASE_SIZE, 0.5f * BASE_SIZE, 0.5f * BASE_SIZE);
	}
};


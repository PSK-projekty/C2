/*****************************************************************//**
 * \file Enemy.cpp
 * \brief
 * Plik z implementacja klasy Enemy
 *********************************************************************/


#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "../Includes/Global.hpp"
#include "../Includes/Enemy.hpp"

using namespace std;

/// <summary>
/// Konstruktor klasy Enemy przyjmuje parametry typu, pozycji x i y. Ustawia kierunek ruchu w zaleznosci od pozycji y, zdrowie na podstawie typu oraz ustawia licznik trafien na 0.
/// </summary>
/// <param name="i_type">Typ przeciwnika.</param>
/// <param name="i_x">Pozycja x startowa przeciwnika.</param>
/// <param name="i_y">Pozycja y startowa przeciwnika</param>

Enemy::Enemy (unsigned char i_type, unsigned short i_x, unsigned short i_y):
	direction (0 == ( i_y / BASE_SIZE ) % 2 ? -1 : 1),
	health (1 + i_type),
	hit_timer (0),
	type (i_type),
	x (i_x),
	y (i_y) {}

/// <summary>
/// Zwraca zdrowie przeciwnika
/// </summary>
/// <returns>Zdrowie przeciwnika</returns>
unsigned char Enemy::get_health () const {
	return health;
}

/// <summary>
/// Zwraca licznik trafien przeciwnika
/// </summary>
/// <returns>Licznik trafien przeciwnika</returns>
unsigned char Enemy::get_hit_timer () const {
	return hit_timer;
}

/// <summary>
/// Zwraca typ przeciwnika
/// </summary>
/// <returns>Typ przeciwnika</returns>
unsigned char Enemy::get_type () const {
	return type;
}

/// <summary>
/// Zwraca pozycje x przeciwnika
/// </summary>
/// <returns>Pozycja x przeciwnika</returns>
unsigned short Enemy::get_x () const {
	return x;
}

/// <summary>
/// Zwraca pozycje y przeciwnika
/// </summary>
/// <returns>Pozycja y przeciwnika</returns>
unsigned short Enemy::get_y () const {
	return y;
}

/// <summary>
/// Ustawia licznik trafien przeciwnika
/// </summary>
void Enemy::hit () {
	hit_timer = ENEMY_HIT_TIMER_DURATION;
}


/// <summary>
/// Metoda przesuwa przeciwnika w zaleznosci od aktualnego kierunku ruchu. Jesli kierunek ruchu jest rowny 1 lub - 1, przeciwnik przesuwa sie w lewo lub prawo, unikajac krawedzi ekranu. Jesli kierunek ruchu jest rowny 0, przeciwnik przesuwa sie w dol. Jesli przeciwnik dotrze do konca ekranu, zmienia kierunek ruchu na przeciwny.
/// </summary>
void Enemy::move () {
	if( 0 != direction ) {
		if( ( 1 == direction && x == SCREEN_WIDTH - 2 * BASE_SIZE ) || ( -1 == direction && x == BASE_SIZE ) ) {
			//Kiedy przeciwnik dotrze do krawedzie zacznie poroszac sie w dol
			direction = 0;

			y += ENEMY_MOVE_SPEED;
		}
		else {
			//Roch poziomy
			x = std::clamp<short> (x + ENEMY_MOVE_SPEED * direction, BASE_SIZE, SCREEN_WIDTH - 2 * BASE_SIZE);
		}
	}
	else {
		y = std::min<short> (y + ENEMY_MOVE_SPEED, BASE_SIZE * ceil (y / static_cast<float>( BASE_SIZE )));

		if( y == BASE_SIZE * ceil (y / static_cast<float>( BASE_SIZE )) ) {
			//Poruszanie sie wezykiem
			direction = 0 == ( y / BASE_SIZE ) % 2 ? -1 : 1;
		}
	}
}


/// <summary>
/// Metoda tworzy nowy pocisk i dodaje ja do wektora pociskow przeciwnika
/// </summary>
/// <param name="i_enemy_bullets">Wektor pociskow przeciwnika</param>
void Enemy::shoot (std::vector<Bullet>& i_enemy_bullets) {
	i_enemy_bullets.push_back (Bullet (0, ENEMY_BULLET_SPEED, x, y));
}

/// <summary>
/// Metoda aktualizuje stan przeciwnika - zmniejsza licznik trafien, a jesli jest on rowny 0, zmniejsza zdrowie przeciwnika
/// </summary>
void Enemy::update () {
	if( 0 < hit_timer ) {
		if( 1 == hit_timer ) {
			health = std::max (0, health - 1);
		}

		hit_timer--;
	}
}

/// <summary>
/// Metoda zwraca hitbox przeciwnika jako prostokat o wspolrzednych x, y, szerokosci i wysokosci okreslonych jako 0.5 * BASE_SIZE
/// </summary>
/// <returns>Hitbox przeciwnika</returns>
sf::IntRect Enemy::get_hitbox () const {
	return sf::IntRect (x + 0.25f * BASE_SIZE, y + 0.25f * BASE_SIZE, 0.5f * BASE_SIZE, 0.5f * BASE_SIZE);
}
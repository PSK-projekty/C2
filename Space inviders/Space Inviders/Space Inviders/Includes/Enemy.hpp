#pragma once

/*****************************************************************//**
 * \file Enemy.hpp
 * \brief
 * Plik naglowkowy klasy Enemy
 *********************************************************************/

/// <summary>
/// Klasa reprezentujaca pojedynczego przeciwnika w grze.
/// </summary>

class Enemy{
	//Kierunek ruchu przeciwnika
	//-1 - Left
	//0 - Down
	//1 - Right
	char direction;

	unsigned char health;
	//Przeciwnik staje sie bialy na kilka klatek po trafieniu
	unsigned char hit_timer;
	unsigned char type;

	unsigned short x;
	unsigned short y;
public:
	/// <summary>
	/// Tworzenie nowego preciwnika
	/// </summary>
	/// <param name="i_type">Typ wroga.</param>
	/// <param name="i_x">Wspolrzedna x pozycji startowej wroga.</param>
	/// <param name="i_y">Wspolrzedna y pozycji startowej wroga.</param>
	Enemy(unsigned char i_type, unsigned short i_x, unsigned short i_y);

	/// <summary>
	/// Pobiera aktualne zdrowie wroga.
	/// </summary>
	/// <returns>Aktualne zdrowie wroga.</returns>
	unsigned char get_health() const;

	/// <summary>
	/// Pobiera timer trafienia wroga.
	/// </summary>
	/// <returns>Timer trafienia wroga.</returns>
	unsigned char get_hit_timer() const;

	/// <summary>
	/// Pobiera typ wroga.
	/// </summary>
	/// <returns>Typ wroga.</returns>
	unsigned char get_type() const;

	/// <summary>
	/// Pobiera wspolrzedna x pozycji wroga.
	/// </summary>
	/// <returns>Wspolrzedna x pozycji wroga.</returns>
	unsigned short get_x() const;

	/// <summary>
	/// Pobiera wspolrzedna y pozycji wroga.
	/// </summary>
	/// <returns>Wspolrzedna y pozycji wroga.</returns>
	unsigned short get_y() const;

	/// <summary>
	/// Trafienie
	/// </summary>
	void hit();

	/// <summary>
	/// Przesuniecie preciwnika
	/// </summary>
	void move();

	/// <summary>
	/// Powoduje, ze wrog strzela.
	/// </summary>
	/// <param name="i_enemy_bullets">Wektor obiektow pociskow.</param>
	void shoot(std::vector<Bullet>& i_enemy_bullets);

	/// <summary>
	/// Aktualizuje wroga.
	/// </summary>
	void update();

	/// <summary>
	/// Pobiera hitboxa wroga.
	/// </summary>
	/// <returns>Hitboxa wroga.</returns>
	sf::IntRect get_hitbox() const;
};
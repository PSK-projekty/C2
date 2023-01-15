#pragma once

/*****************************************************************//**
 * \file Player.hpp
 * \brief
 * Plik naglowkowy klasy Player
 *********************************************************************/

/// <summary>
/// Klasa reprezentuje gracza w grze. Posiada pola i metody do zarzadzania pozycja, zyciem, pociskami itp.
/// </summary>
class Player {

	bool dead;
	bool dead_animation_over;
	bool shield_animation_over;

	unsigned char current_power;
	unsigned char reload_timer;

	unsigned short power_timer;
	unsigned short x;
	unsigned short y;

	std::vector<Bullet> bullets;

	sf::Sprite bullet_sprite;
	sf::Sprite sprite;

	sf::Texture bullet_texture;
	sf::Texture texture;

	Animation explosion;
	public:
	/// <summary>
	/// Inicjalizuje pola klasy oraz laduje tekstury i animacje
	/// </summary>
	Player ();

	/// <summary>
	/// Zwraca czy gracz jest martwy
	/// </summary>
	/// <returns>true jesli gracz jest martwy</returns>
	/// <returns>false w przeciwnym wypadku</returns>
	bool get_dead () const;

	/// <summary>
	/// Zwraca informacje czy animacja smierci gracza zakonczona
	/// </summary>
	/// <returns>true jesli animacja sie zakonczyla</returns>
	/// <returns> false jeslia animacja jeszcze sie nie zakonczyla</returns>
	bool get_dead_animation_over () const;

	/// <summary>
	/// Zwraca aktualnie uzywany power-up
	/// </summary>
	/// <returns>Numer power-upu</returns>
	unsigned char get_current_power () const;

	/// <summary>
	/// Zwraca czas trwania power-upu
	/// </summary>
	/// <returns>Czas trwania power-upu</returns>
	unsigned short get_power_timer () const;

	/// <summary>
	/// Zwraca pozycje gracza na osi y
	/// </summary>
	/// <returns>Pozycja gracza wzgledem osi y</returns>
	unsigned short get_y () const;

	/// <summary>
	/// Metoda wywolywana gdy gracz umiera. Ustawia flage dead na true oraz rozpoczyna animacje eksplozji.
	/// </summary>
	void die ();

	/// <summary>
	/// Rysuje gracza na ekranie
	/// </summary>
	/// <param name="i_window">Okno gry</param>
	void draw (sf::RenderWindow& i_window);

	/// <summary>
	/// Resetuje stan gracza
	/// </summary>
	void reset ();

	/// <summary>
	/// Aktualizuje stan gracza
	/// </summary>
	/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
	/// <param name="i_enemy_bullets">Wektor pociskow przeciwnikow</param>
	/// <param name="i_enemies">Wektor przeciwnikow</param>
	/// <param name="i_ufo">Obiekt reprezentujacy UFO</param>
	void update (std::mt19937_64& i_random_engine, std::vector<Bullet>& i_enemy_bullets, std::vector<Enemy>& i_enemies, Ufo& i_ufo);

	/// <summary>
	/// Zwraca hitbox gracza
	/// </summary>
	/// <returns>Hitbox gracza</returns>
	sf::IntRect get_hitbox () const;
};
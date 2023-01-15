#pragma once

/*****************************************************************//**
 * \file EnemyMenager.hpp
 * \brief
 * Plik naglowkowy klasy EnemyMenager
 *********************************************************************/

/// <summary>
/// Klasa zarzadzajaca przeciwnikami - tworzenie nowych przeciwnikow, ich ruch, strzelanie oraz kolizje z graczem.
/// </summary>
class EnemyManager{
	unsigned short move_pause;
	unsigned short move_timer;

	//Aby skorzystac z losowosci z biblioteki <random>, musimy zdefiniowac rozklad.
	std::uniform_int_distribution<unsigned short> shoot_distribution;

	std::vector<Animation> enemy_animations;

	std::vector<Bullet> enemy_bullets;

	std::vector<Enemy> enemies;

	sf::Sprite enemy_bullet_sprite;

	sf::Texture enemy_bullet_texture;
public:
	/// <summary>
	/// Tworzy nowy obiekt klasy EnemyManager, inicjuje zmienne i przygotowuje tekstury i animacje przeciwnikow.
	/// </summary>
	EnemyManager();

	/// <summary>
	/// Metoda sprawdza czy przeciwnicy dotarli do poziomu gracza na ekranie
	/// </summary>
	/// <param name="i_player_y">Pozycja y gracza</param>
	/// <returns>True jesli przeciwnicy dotarli do gracza, false w przeciwnym wypadku</returns>
	bool reached_player(unsigned short i_player_y) const;

	/// <summary>
	/// Metoda rysuje przeciwnikow i ich pociski na ekranie
	/// </summary>
	/// <param name="i_window">Okno na ktorym rysujemy</param>
	void draw(sf::RenderWindow& i_window);

	/// <summary>
	/// Metoda resetuje stan przeciwnikow - tworzy nowych przeciwnikow i czysci wektory pociskow przeciwnikow i przeciwnikow
	/// </summary>
	/// <param name="i_level">Poziom</param>
	void reset(unsigned short i_level);

	/// <summary>
	/// Metoda aktualizuje stan przeciwnikow - ich ruch, strzelanie oraz kolizje z graczem
	/// </summary>
	/// <param name="i_random_engine">Silnik losowosci do generowania momentow strzelania przez przeciwnikow</param>
	void update(std::mt19937_64& i_random_engine);

	/// <summary>
	/// Zwraca referencje do wektora kul przeciwnikow
	/// </summary>
	/// <returns>Referencja do wektora kul przeciwnikow</returns>
	std::vector<Bullet>& get_enemy_bullets();

	/// <summary>
	/// Zwraca referencje do wektora przeciwnikow
	/// </summary>
	/// <returns>Referencja do wektora przeciwnikow</returns>
	std::vector<Enemy>& get_enemies();
};
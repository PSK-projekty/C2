/*****************************************************************//**
 * \file Ufo.cpp
 * \brief
 * Plik z implementacja klasy Ufo
 *********************************************************************/

#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "../Includes/Animation.hpp"
#include "../Includes/Global.hpp"
#include "../Includes/Ufo.hpp"


/// <summary>
/// Konstruktor klasy Ufo. Tworzy on nowy obiekt typu "Ufo" i inicjalizuje jego pola.
/// 
/// "y (BASE_SIZE)" inicjalizuje pole "y" obiektu do wartosci "BASE_SIZE"
/// 
/// "powerup_distribution (0, POWERUP_TYPES - 1)" tworzy rozklad losowy, ktory bedzie uzywany do wybierania rodzaju powerupow, z zakresu od 0 do (POWERUP_TYPES - 1)
/// 
/// "timer_distribution (UFO_TIMER_MIN, UFO_TIMER_MAX)" tworzy rozklad losowy, ktory bedzie uzywany do okreslania czasu, po jakim UFO pojawi sie na ekranie, z zakresu od UFO_TIMER_MIN do UFO_TIMER_MAX
/// 
///"animation (UFO_ANIMATION_SPEED, 2 * BASE_SIZE, "Images / Ufo.png")" tworzy obiekt "animation" klasy "Animation" z parametrami : predkosci animacji UFO_ANIMATION_SPEED, rozmiaru 2 * BASE_SIZE, oraz pliku obrazu "Images/Ufo.png"
/// 
///"explosion (EXPLOSION_ANIMATION_SPEED, 2 * BASE_SIZE, "Images / ExplosionBig.png")" tworzy obiekt "explosion" klasy "Animation" z parametrami : predkosci animacji EXPLOSION_ANIMATION_SPEED, rozmiaru 2 * BASE_SIZE, oraz pliku obrazu "Images/ExplosionBig.png"
/// 
///"reset (1, i_random_engine);" wywoluje metode "reset" na nowo utworzonym obiekcie, z parametrem 1, i przekazujac silnik losowy "i_random_engine"
/// 
/// "for( unsigned char a = 0; a < POWERUP_TYPES; a++ )" petla, ktora iteruje tyle razy, ile jest typow powerupow
/// 
/// "powerup_animations.push_back (Animation (POWERUP_ANIMATION_SPEED, BASE_SIZE, "Images/Powerup" + std::to_string (static_cast<unsigned short>( a )) + ".png"));" tworzy obiekt "Animation" z parametrami: predkosci animacji POWERUP_ANIMATION_SPEED, rozmiaru BASE_SIZE, oraz pliku obrazu "Images/Powerup" + numer powerupu + ".png", i dodaje go do wektora "powerup_animations".
/// 
/// W skrocie kod tworzy obiekt Ufo i inicjalizuje jego pola. Tworzy rozklady losowe
/// </summary>
/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
Ufo::Ufo (std::mt19937_64& i_random_engine):
	y (BASE_SIZE),
	powerup_distribution (0, POWERUP_TYPES - 1),
	timer_distribution (UFO_TIMER_MIN, UFO_TIMER_MAX),
	animation (UFO_ANIMATION_SPEED, 2 * BASE_SIZE, "Images/Ufo.png"),
	explosion (EXPLOSION_ANIMATION_SPEED, 2 * BASE_SIZE, "Images/ExplosionBig.png") {
	reset (1, i_random_engine);

	for( unsigned char a = 0; a < POWERUP_TYPES; a++ ) {
		powerup_animations.push_back (Animation (POWERUP_ANIMATION_SPEED, BASE_SIZE, "Images/Powerup" + std::to_string (static_cast<unsigned short>( a )) + ".png"));
	}
}


/// <summary>
/// Ta funkcja jest metoda klasy "Ufo" i nazywa sie "check_bullet_collision". Sluzy ona do sprawdzania czy UFO zostaje trafione przez pocisk, i wykonuje odpowiednie dzialania w przypadku kolizji.
/// 
/// "if( 0 == dead )" sprawdza czy UFO nie jest juz martwe
/// 
/// "if( 1 == get_hitbox ().intersects (i_bullet_hitbox) )" sprawdza czy hitbox UFO przecina sie z hitbox pocisku (i_bullet_hitbox). Jesli tak, to warunek jest spelniony
/// 
/// "dead = 1;" oznacza, ze UFO zostaje zabite
/// 
/// "explosion_x = x;" zapisuje pozycje x, w ktorej ma byc wyswietlona animacja eksplozji
/// 
/// "powerups.push_back (Powerup (x + 0.5f * BASE_SIZE, y, powerup_distribution (i_random_engine)));" tworzy nowy obiekt "Powerup" i umieszcza go na pozycji (x + 0.5f * BASE_SIZE, y) oraz wybiera losowy powerup za pomoca rozkladu losowego "powerup_distribution" i przesyla silnik losowy "i_random_engine"
/// 
/// 
/// </summary>
/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
/// <param name="i_bullet_hitbox"></param>
/// <returns>1 jesli kolizja zostala wykryta</returns>
/// <returns>0 jesli kolizja nie zostala wykryta</returns>
bool Ufo::check_bullet_collision (std::mt19937_64& i_random_engine, const sf::IntRect& i_bullet_hitbox) {
	if( 0 == dead ) {
		if( 1 == get_hitbox ().intersects (i_bullet_hitbox) ) {
			dead = 1;

			explosion_x = x;

			powerups.push_back (Powerup (x + 0.5f * BASE_SIZE, y, powerup_distribution (i_random_engine)));

			return 1;
		}
	}

	return 0;
}

/// <summary>
/// Funkcja sprawdzajaca kolizje z bonusem. Funkcjha przechodzi przez wszystkie power-upy i sprawdza czy hitbox gracza i bonusu przecinaja sie. 
/// </summary>
/// <param name="i_player_hitbox">Hitbox gracza</param>
/// <returns>0 jesli kolizja nie zostanie wykryta</returns>
/// <returns>1+typ jesli kolizja zostanie wykryta</returns>
unsigned char Ufo::check_powerup_collision (const sf::IntRect& i_player_hitbox) {
	for( Powerup& powerup : powerups ) {
		if( 0 == powerup.dead && 1 == powerup.get_hitbox ().intersects (i_player_hitbox) ) {
			powerup.dead = 1;
			//Dodac jeden poniewaz 0 oznaczalobym ze nie ma zadnych bonusow
			return 1 + powerup.type;
		}
	}
	return 0;
}

/// <summary>
/// Funkcja jest odpowiedzialna za wyswietlanie ufo w oknie. Funkcja rozpoczyna sie od sprawdzenia, czy UFO nie jest martwe, a jesli tak, wywoluje funkcje „draw” na obiekcie animacji w biezacej pozycji UFO x i y. Nastepnie sprawdza, czy martwa animacja sie nie skonczyla, a jesli tak, to wywoluje funkcje „narysuj” na obiekcie eksplozji w biezacej pozycji x i y UFO, kolorem (255, 36, 0). Na koniec funkcja przechodzi przez wszystkie ulepszenia i wywoluje funkcje „dobierz” odpowiednia animacje ulepszenia w biezacej pozycji x i y ulepszenia
/// </summary>
/// <param name="i_window">Okno</param>
void Ufo::draw (sf::RenderWindow& i_window) {
	if( 0 == dead ) {
		animation.draw (x, y, i_window);
	}

	if( 0 == dead_animation_over ) {
		explosion.draw (explosion_x, y - 0.5f * BASE_SIZE, i_window, sf::Color (255, 36, 0));
	}

	for( Powerup& powerup : powerups ) {
		powerup_animations[powerup.type].draw (powerup.x, powerup.y, i_window);
	}
}

/// <summary>
/// Funkcja resetuje ufo.Funkcja najpierw ustawia zmienna skladowa „dead” na wartosc argumentu wejsciowego „i_dead”, a zmienna skladowa „dead_animation_over” na 0. Nastepnie ustawia zmienne skladowe „explosion_x” i „x” na wartosc stala „SCREEN_WIDTH”. Wykorzystuje generator liczb losowych do generowania nowej wartosci zmiennej skladowej „timer” za pomoca funkcji „timer_distribution”. Resetuje animacje oraz pozycje wybuchu ulepszenia
/// </summary>
/// <param name="i_dead">Zmienna przechowujaca informacje czy Ufo zyje</param>
/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
void Ufo::reset (bool i_dead, std::mt19937_64& i_random_engine) {
	dead = i_dead;
	dead_animation_over = 0;

	explosion_x = SCREEN_WIDTH;
	x = SCREEN_WIDTH;

	timer = timer_distribution (i_random_engine);

	powerups.clear ();

	animation.reset ();
	explosion.reset ();
}
/// <summary>
/// Funkcja najpierw sprawdza, czy UFO nie jest martwe (martwe = 0). Jesli nie jest martwy, przesuwa UFO w lewo poprzez zmniejszenie wartosci jego wspolrzednej x o "UFO_MOVE_SPEED". Jesli wspolrzedna x UFO jest mniejsza lub rowna -2 razy „BASE_SIZE”, jest uwazana za poza ekranem, a zmienna martwa jest ustawiona na 1. Aktualizuje rowniez animacje UFO.
/// 
///Jesli UFO jest martwe, funkcja sprawdza, czy martwa animacja sie skonczyla (dead_animation_over = 0).Jesli to nie koniec, aktualizuje animacje eksplozji i ustawia „dead_animation_over” na wartosc zwracana przez funkcje aktualizacji.Jesli zmienna „timer” ma wartosc 0, UFO jest resetowane do pierwotnego stanu przez wywolanie funkcji „reset” z argumentami 0 i „i_random_engine”.Jesli licznik czasu nie wynosi 0, jest zmniejszany o 1.
/// 
///Nastepnie funkcja przechodzi przez kontener „ulepszen”, przesuwajac kazde ulepszenie w dol o „POWERUP_SPEED” i oznaczajac je jako martwe, jesli zniknie z dolnej czesci ekranu.Aktualizuje rowniez animacje kazdego wzmocnienia.Na koniec usuwa wszystkie martwe ulepszenia z kontenera za pomoca funkcji remove_if.
/// </summary>
/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
void Ufo::update (std::mt19937_64& i_random_engine) {
	if( 0 == dead ) {
		x -= UFO_MOVE_SPEED;

		if( x <= -2 * BASE_SIZE ) {
			dead = 1;
		}

		animation.update ();
	}
	else {
		if( 0 == dead_animation_over ) {
			dead_animation_over = explosion.update ();
		}

		if( 0 == timer ) {
			reset (0, i_random_engine);
		}
		else {
			timer--;
		}
	}

	for( Powerup& powerup : powerups ) {
		powerup.y += POWERUP_SPEED;

		if( SCREEN_HEIGHT <= powerup.y ) {
			powerup.dead = 1;
		}
	}

	for( Animation& powerup_animation : powerup_animations ) {
		powerup_animation.update ();
	}

	powerups.erase (remove_if (powerups.begin (), powerups.end (), [] (const Powerup& i_powerup) {
		return 1 == i_powerup.dead;
							   }), powerups.end ());
}


/// <summary>
/// Funkcja wykorzystuje biezaca wartosc zmiennej skladowej x, y i BASE_SIZE do utworzenia i zwrocenia obiektu „sf::IntRect”, ktory reprezentuje pole trafienia obiektu UFO.To pole trafienia jest prostokatem, ktorego lewy gorny rog znajduje sie w miejscu, w ktorym znajduje sie UFO, a jego szerokosc i wysokosc wynosza odpowiednio 2 * BASE_SIZE i BASE_SIZE.
/// </summary>
/// <returns>Hitbox Ufo</returns>
sf::IntRect Ufo::get_hitbox () const {
	return sf::IntRect (x, y, 2 * BASE_SIZE, BASE_SIZE);
}

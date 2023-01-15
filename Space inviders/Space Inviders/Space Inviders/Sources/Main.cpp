/*****************************************************************//**
 * \file main.cpp
 * \brief
 * Glowny plik projektu
 *********************************************************************/

#include <array>
#include <chrono>
#include <random>

#include <SFML/Graphics.hpp>

#include "../Includes/Animation.hpp"
#include "../Includes/DrawText.hpp"
#include "../Includes/Global.hpp"
#include "../Includes/Enemy.hpp"
#include "../Includes/EnemyManager.hpp"
#include "../Includes/Ufo.hpp"
#include "../Includes/Player.hpp"

/// <summary>
/// Ten plik jest glownym plikiem projektu bedacego gra oparta na klasycznej grze zrecznosciowej „Space Invaders”.
/// Gra wykorzystuje biblioteke SFML do obslugi grafiki i danych wejsciowych.
/// 
/// Kod zaczyna sie od dolaczenia niezbednych naglowkow i bibliotek, takich jak <array>, <chrono>, <random>, <SFML/Graphics.hpp> oraz niektorych niestandardowych naglowkow, takich jak Animation.hpp, DrawText.hpp, Global.hpp, Enemy.hpp , EnemyManager.hpp, Ufo.hpp i Player.hpp. Deklaruje i inicjalizuje kilka zmiennych i obiektow, ktore sa uzywane w calej grze.
/// 
/// game_over i next_level to flagi, ktore sprawdzaja, czy gra sie skonczyla lub czy gracz osiagnal kolejny poziom.
/// 
/// level sledzi aktualny poziom gry
/// 
/// next_level_timer to licznik czasu do przejscia na nastepny poziom.
/// 
/// lag to zmienna uzywana do sledzenia czasu, jaki uplynal miedzy klatkami.
/// 
/// random_engine to generator liczb losowych, ktory sluzy do losowego losowania zachowania UFO.
/// 
/// event jest obiektem typu sf::Event, ktory jest uzywany do obslugi zdarzen wejsciowych.
/// 
/// window jest obiektem typu sf::RenderWindow, ktory reprezentuje okno gry.
/// 
/// background_sprite i background_texture to odpowiednio obiekty sf::Sprite i sf::Texture, ktore sluza do wyswietlania tla gry.
/// 
/// font_texture jest obiektem klasy sf::Texture uzywanym do wyswietlania czcionki gry.
/// 
/// enemy_manager to obiekt niestandardowej klasy EnemyManager, ktory sluzy do zarzadzania wrogami w grze.
/// 
/// player jest obiektem niestandardowej klasy Player, ktory reprezentuje postac gracza.
/// 
/// ufo jest obiektem niestandardowej klasy Ufo, ktora reprezentuje UFO.
/// 
/// 
/// Program stawia teksture tla na ikonke tla, zmienia rozmiar okna i ustawia zadany rozmiar widoku. Nastepnie kod wchodzi do glownej petli, ktora wielokrotnie obsluguje dane wejsciowe, aktualizuje logike gry i renderuje grafike gry w oknie.Petla wykorzystuje staly krok czasowy i zmienna „opoznienia”, aby zapewnic plynna animacje.
/// 
/// Logika gry obejmuje aktualizacje pozycji i animacji gracza, wrogow i UFO, sprawdzanie kolizji miedzy pociskami gracza a wrogami i UFO, sprawdzanie, czy gracz lub wrogowie dotarli do konca ekranu oraz obsluge przejscia miedzy poziomy.
/// 
/// Wejscie jest obslugiwane przez oodczytywanie zdarzen z obiektu okna, sprawdza typ zdarzenia i jesli jest zamkniete, zamyka okno.
/// 
/// Gra zawiera rowniez stan zakonczenia gry, kiedy zycie gracza dobiegnie konca, oraz stan nastepnego poziomu, gdy wszyscy wrogowie zostana pokonani, wyswietli sie komunikat o nastepnym poziomie.
/// 
/// Cala logika gry zawarta jest w petli while
/// </summary>


int main () {

	bool game_over = 0;
	bool next_level = 0;

	unsigned short level = 0;
	unsigned short next_level_timer = NEXT_LEVEL_TRANSITION;

	std::chrono::microseconds lag (0);

	std::chrono::steady_clock::time_point previous_time;

	//Ustawianie ziarna generotora liczb pseudolosowych
	std::mt19937_64 random_engine (std::chrono::system_clock::now ().time_since_epoch ().count ());

	sf::Event event;

	sf::RenderWindow window (sf::VideoMode (SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Space Invaders", sf::Style::Close);
	//Resizing the screen.
	window.setView (sf::View (sf::FloatRect (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));


	sf::Sprite background_sprite;

	sf::Texture background_texture;

	background_texture.loadFromFile ("Images/Background.png");

	sf::Texture font_texture;
	font_texture.loadFromFile ("Images/Font.png");

	EnemyManager enemy_manager;

	Player player;

	Ufo ufo (random_engine);

	background_sprite.setTexture (background_texture);


	previous_time = std::chrono::steady_clock::now ();

	while( 1 == window.isOpen () ) {
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::steady_clock::now () - previous_time );

		lag += delta_time;

		previous_time += delta_time;

		while( FRAME_DURATION <= lag ) {
			lag -= FRAME_DURATION;

			while( 1 == window.pollEvent (event) ) {
				switch( event.type ) {
					case sf::Event::Closed:
					{
						window.close ();
					}
				}
			}

				if( 1 == player.get_dead_animation_over () ) {
					game_over = 1;
				}

				if( 1 == enemy_manager.reached_player (player.get_y ()) ) {
					player.die ();
				}

				if( 0 == game_over ) {
					if( 0 == enemy_manager.get_enemies ().size () ) {
						if( 0 == next_level_timer ) {
							next_level = 0;

							level++;
							next_level_timer = NEXT_LEVEL_TRANSITION;

							player.reset ();

							enemy_manager.reset (level);

							ufo.reset (1, random_engine);
						}
						else {
							next_level = 1;

							next_level_timer--;
						}
					}
					else {
						player.update (random_engine, enemy_manager.get_enemy_bullets (), enemy_manager.get_enemies (), ufo);

						enemy_manager.update (random_engine);

						ufo.update (random_engine);
					}
				}
				else if( 1 == sf::Keyboard::isKeyPressed (sf::Keyboard::Enter) ) {
					game_over = 0;

					level = 0;

					player.reset ();

					enemy_manager.reset (level);

					ufo.reset (1, random_engine);
				}

				if( FRAME_DURATION > lag ) {
					window.clear ();

					window.draw (background_sprite);

					if( 0 == player.get_dead () ) {
						enemy_manager.draw (window);

						ufo.draw (window);

					}

					player.draw (window);

					draw_text (0.25f * BASE_SIZE, 0.25f * BASE_SIZE, "Level: " + std::to_string (level), window, font_texture);

					if( 1 == game_over ) {
						draw_text (0.5f * ( SCREEN_WIDTH - 5 * BASE_SIZE ), 0.5f * ( SCREEN_HEIGHT - BASE_SIZE ), "Game over!", window, font_texture);
					}


					if( level == 21 ) {

						window.clear ();

						background_texture.loadFromFile ("Images/End_bg.png");
						background_sprite.setTexture (background_texture);

						window.draw (background_sprite);

						draw_text (0.3f * ( SCREEN_WIDTH - 5 * BASE_SIZE ), 0.4f * ( SCREEN_HEIGHT - BASE_SIZE ), "Congratulations\nYou finish this game", window, font_texture);

					}

					if( 1 == next_level ) {
						draw_text (0.5f * ( SCREEN_WIDTH - 5.5f * BASE_SIZE ), 0.5f * ( SCREEN_HEIGHT - BASE_SIZE ), "Next level!", window, font_texture);
					}
					window.display ();
				}
			}
		}
	}

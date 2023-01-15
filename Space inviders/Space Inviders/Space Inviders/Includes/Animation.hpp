#pragma once

/*****************************************************************//**
 * \file Animation.hpp
 * \brief
 * Plik naglowkowy klasy Animation
 *********************************************************************/

class Animation{
	//Zmienna animation_iterator jest uzywana do okreslenia kiedy nalezy zmienic klatke animacji.Iterator jest inkrementowany przy kazdym wywolaniu metody update (), a klatka jest zmieniana dopiero wtedy, gdy iterator przekroczy wartosc animation_speed.
	unsigned short animation_iterator; 

	//Zmienna animation_speed okresla jak czesto nalezy zmieniac klatke animacji. Im wieksza wartosc animation_speed tym rzadziej nastepuje zmiana klatki.
	unsigned short animation_speed; 

	//Zmienna current_frame przechowuje informacje o aktualnie wyswietlanej klatce animacji. Wartosc current_frame jest zmieniana przez metody change_current_frame () oraz update ().
	unsigned short current_frame; 

	//Zmienna frame_width przechowuje szerokosc klatki animacji. Szerokosc klatki jest potrzebna do wyodrebnienia poszczegolnych klatek z tekstury.
	unsigned short frame_width; 

	// Zmienna total_frames przechowuje informacje o liczbie klatek animacji.Liczba klatek jest obliczana na podstawie szerokosci tekstury i szerokosci klatki.
	unsigned short total_frames; 

	// Zmienna texture przechowuje teksture animacji. Tekstura jest ladowana z pliku podczas tworzenia obiektu klasy Animation.
	sf::Sprite sprite; 

	//Zmienna texture przechowuje teksture animacji. Tekstura jest ladowana z pliku podczas tworzenia obiektu klasy Animation.
	sf::Texture texture; 
public:
	/// <summary>
	/// Klasa Animation jest odpowiedzialna za animacje. Konstruktor przyjmuje parametry predkosci animacji, szerokosci klatki oraz lokalizacji tekstury. Klasa przechowuje informacje o aktualnie wyswietlanej klatce, liczbie klatek oraz predkosci animacji.
	/// </summary>
	/// <param name="i_animation_speed">predkosc animacji</param>
	/// <param name="i_frame_width">szerokosc klatki</param>
	/// <param name="i_texture_location">lokalizacja tekstury</param>
	Animation(unsigned short i_animation_speed, unsigned short i_frame_width, const std::string& i_texture_location);
	/// <summary>
	/// Metoda change_current_frame() zmienia aktualnie wyswietlana klatke i zwraca informacje czy animacja zakonczyla sie. Jesli aktualna klatka jest ostatnia klatka animacji to zwraca 1, w przeciwnym wypadku zwraca 0.
	/// </summary>
	/// <returns>true - animacja zakonczona</returns>
	/// <returns>false - animacja jeszcze nie zakonczona</returns>
	bool change_current_frame();
	/// <summary>
	/// Metoda update() aktualizuje animacje i zwraca informacje czy animacja zakonczyla sie. Jesli aktualna klatka jest ostatnia klatka animacji to zwraca 1, w przeciwnym wypadku zwraca 0.
	/// </summary>
	/// <returns>true - animacja zakonczona</returns>
	///	<returns>false - animacja nie zakonczona</returns>
	bool update();
	/// <summary>
	/// Metoda draw() rysuje aktualnie wyswietlana klatke. Rysuje teksture na podstawie aktualnej klatki, pozycji na ekranie oraz koloru.
	/// </summary>
	/// <param name="i_x">pozycja x na ekranie</param>
	/// <param name="i_y">pozycja y na ekranie</param>
	/// <param name="i_window">okno renderowania</param>
	/// <param name="i_color">kolor klatki</param>
	void draw(short i_x, short i_y, sf::RenderWindow& i_window, const sf::Color& i_color = sf::Color(255, 255, 255));
	/// <summary>
	/// Metoda reset() resetuje animacje do pierwszej klatki.
	/// </summary>
	void reset();
};
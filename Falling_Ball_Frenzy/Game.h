#pragma once

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/System.hpp>
#include<SFML/Network.hpp>
#include<SFML/Window.hpp>

/*
*   Class that acts as the Game engine
*   Wrapper Class
*/

class Game
{
private:
	//Variables
	// Window
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode;

	//Mouse positions in window
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;

	//Text
	sf::Text uiText;

	//Game Logic
	bool endgame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int MaxEnemies;
	bool MouseHeld;

	//Game objects
	std::vector<sf::RectangleShape> enemies;

	sf::RectangleShape enemy;

	//Private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
public:
	//Constructor/Destructors
	Game();
	~Game();

	//Accesors
	const bool running()const;
	const bool getEndgame()const;

	//Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePositions();
	void updateText();
	void gameOverText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
};


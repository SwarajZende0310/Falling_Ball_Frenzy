#include "Game.h"

//Private Functions
void Game::initVariables()
{
	this->window = nullptr;

	//Game Logic
	this->endgame = false;
	this->points = 0;
	this->health = 10;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->MaxEnemies = 5;
	this->MouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Falling Ball Frenzy", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf")) {
		std::cout << "ERROR::GAME::INITFONTS:Failed to load Fonts!" << std::endl;
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(30);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("None");
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Cyan);
	//this->enemy.setOutlineColor(sf::Color::Green);
	//this->enemy.setOutlineThickness(2.f);
}

//Constructors /Destructors
Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initFonts();
	this->initText();
}

Game::~Game() {
	delete this->window;
}

//Accesors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndgame() const
{
	return this->endgame;
}

//Functions
void Game::spawnEnemy()
{
	/*
	* @returns void
	*
	* Spawns enemies and sets their types , colors and positions.
	* Sets a random type , position and random colour
	*/
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	//Randomize enemy type
	int type = rand() % 5;
	switch (type) {
	case 0:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color(255, 165, 0, 255));
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color(128, 0, 128, 255));
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(130.f, 130.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(150.f, 150.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	}

	//Spawn the enemy
	this->enemies.push_back(this->enemy);


}

void Game::pollEvents()
{
	//Updating the timer for enemy spawning
	while (this->window->pollEvent(this->ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{
	/*
	* @return void
	*
	* Updates current mouse position
	*    -mouse position relative to window(Vecotr2i)
	*
	*/
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << std::endl << "Health: " << this->health;

	this->uiText.setString(ss.str());
}

void Game::gameOverText()
{
	this->window->clear();
	std::stringstream ss1, ss;

	this->uiText.setCharacterSize(100);
	this->uiText.setFillColor(sf::Color::Red);
	this->uiText.setPosition(sf::Vector2f(170.f, 150.f));
	ss1 << "GAME OVER" << std::endl;
	this->uiText.setString(ss1.str());
	this->renderText(*this->window);

	ss.clear();
	this->uiText.setCharacterSize(40);
	this->uiText.setFillColor(sf::Color::Yellow);
	this->uiText.setPosition(sf::Vector2f(340.f, 400.f));
	ss << "Points: " << this->points << std::endl;

	this->uiText.setString(ss.str());
	this->renderText(*this->window);

	this->window->display();

	bool keyboardInterrupt = false;
	while (!keyboardInterrupt) {
		this->window->pollEvent(this->ev);
		switch (ev.type) {
		case sf::Event::Closed:
			this->window->close();
			keyboardInterrupt = true;
			break;

		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			keyboardInterrupt = true;
			break;
		}
	}

}

void Game::updateEnemies()
{
	/*
	* @returns void
	*
	* update enemies spawn timers and spawn enemies
	* when total amount of enemies is smaller than max
	* Moves the enemies downward
	* Removes the enemies at the edge pf the screen
	*/
	if (this->enemies.size() < this->MaxEnemies) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			//Spawn the enemy and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	//Moving and updating enemies
	for (int i = 0; i < enemies.size(); i++) {
		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
		}
		else {
			this->enemies[i].move(0.f, 5.f);
		}
	}
	//Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (this->MouseHeld == false) {
			this->MouseHeld = true;
			bool deleted = false;

			for (int i = 0; i < this->enemies.size() && !deleted; i++) {
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
					//Gain points
					if (this->enemies[i].getFillColor() == sf::Color::Red)
						points += 10;
					else if (this->enemies[i].getFillColor() == sf::Color(255, 165, 0, 255))
						points += 7;
					else if (this->enemies[i].getFillColor() == sf::Color(128, 0, 128, 255))
						points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
						points += 3;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						points += 1;
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
		else {
			this->MouseHeld = false;
		}
	}
}

void Game::update()
{
	this->pollEvents();
	if (this->endgame == false) {
		//Update mouse position
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	//Endgame condition
	if (health <= 0)endgame = true;
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	//Rendering all enemies
	for (auto& e : this->enemies) {
		target.draw(e);
	}
}

void Game::render()
{
	/*
	* @return void
	*
	* -clear old frames
	* -render objects
	* -display frame in window
	*
	* Renders the Game objects
	*/
	this->window->clear();

	//Draw Game objects
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();

}

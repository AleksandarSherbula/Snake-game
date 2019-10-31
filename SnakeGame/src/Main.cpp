#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include <list>
#include <thread>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include <iostream>
#include <Windows.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 608;

struct Point
{
	int x = 0;
	int y = 0;
};

std::string IntToString(int number)
{
	std::ostringstream os;
	os << number;
	return os.str();
}

int main()
{
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake Game");

	sf::Font f1;
	// Font taken from:
	// https://www.dafont.com/brady-bunch.font
	f1.loadFromFile(("font/BradBunR.ttf"));

	sf::Texture t1;
	// Block and Background image taken from:
	// https://learnopengl.com/In-Practice/2D-Game/Levels
	t1.loadFromFile("images/block.png");

	sf::Text gameOverText, commandText, scoreText, pauseText, hintText, hintText2;

	pauseText.setFont(f1);
	pauseText.setString("Game Paused!");
	pauseText.setCharacterSize(50);
	pauseText.setFillColor(sf::Color(0, 0, 100, 255));

	gameOverText.setFont(f1);
	gameOverText.setString("Game Over!");
	gameOverText.setCharacterSize(50);
	gameOverText.setFillColor(sf::Color(0, 0, 100, 255));

	commandText.setFont(f1);
	commandText.setCharacterSize(25);
	commandText.setFillColor(sf::Color(0, 0, 100, 255));

	scoreText.setFont(f1);
	scoreText.setCharacterSize(45);
	scoreText.setFillColor(sf::Color::White);

	hintText.setFont(f1);
	hintText.setString("Pause: Space");
	hintText.setCharacterSize(30);
	scoreText.setFillColor(sf::Color::White);

	hintText2.setFont(f1);
	hintText2.setString("ESC: Exit game");
	hintText2.setCharacterSize(30);
	scoreText.setFillColor(sf::Color::White);

	sf::Sprite snakePiece(t1);
	snakePiece.setColor(sf::Color::Blue);
	snakePiece.setScale(sf::Vector2f(0.25f, 0.25f));
	float sprSize = t1.getSize().x * 0.25f; // Only need 1 axis for size since sprite is a square

	sf::CircleShape food(sprSize / 2.0f);
	food.setFillColor(sf::Color::Blue);

	sf::RectangleShape SubTextBackground;
	SubTextBackground.setSize(sf::Vector2f(SCREEN_WIDTH, 32));
	SubTextBackground.setFillColor(sf::Color(0, 0, 100, 255));
	SubTextBackground.setPosition(0, SCREEN_HEIGHT - 32);

	Point move = {1, 0};

	std::list<Point> snakeSize = { {9, 8}, {8, 8}, {7, 8}, {6, 8} };

	bool isDead = false;
	bool isPaused = false;

	Point unitSize =
	{
		SCREEN_WIDTH / sprSize,
		SCREEN_HEIGHT / sprSize
	};

	Point foodPos =
	{
		rand() % unitSize.x,
		rand() % (unitSize.y - 1)
	};

	int score = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up && move.y == 0)
				{
					move.x = 0;
					move.y = -1;
				}

				else if (event.key.code == sf::Keyboard::Down && move.y == 0)
				{
					move.x = 0;
					move.y = 1;
				}

				else if (event.key.code == sf::Keyboard::Left && move.x == 0)
				{
					move.x = -1;
					move.y = 0;
				}

				else if (event.key.code == sf::Keyboard::Right && move.x == 0)
				{
					move.x = 1;
					move.y = 0;
				}

				if (event.key.code == sf::Keyboard::Escape)
					event.type = sf::Event::Closed;

				if (event.key.code == sf::Keyboard::Space)
				{
					if (isDead)
					{
						snakeSize.clear();
						snakeSize = { {9, 8}, {8, 8}, {7, 8}, {6, 8} };
						move = { 1, 0 };
						score = 0;
						foodPos = { rand() % unitSize.x, rand() % (unitSize.y - 1) };
						isDead = false;
					}
					else
					{
						isPaused = !isPaused;
					}
				}
				
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (!isPaused)
		{
			if (!isDead)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				if (1)
				{
					snakeSize.push_front({ snakeSize.front().x + move.x, snakeSize.front().y + move.y });
					snakeSize.pop_back();
				}

				//Snake vs Screen edge collision
				if (snakeSize.front().x < 0 || snakeSize.front().x >= unitSize.x ||
					snakeSize.front().y < 0 || snakeSize.front().y >= unitSize.y - 1)
					isDead = true;

				//Snake vs Snake collision
				for (std::list<Point>::iterator i = snakeSize.begin(); i != snakeSize.end(); i++)
				{
					if (i != snakeSize.begin() && snakeSize.front().x == i->x && snakeSize.front().y == i->y)
						isDead = true;
				}

				//Snake vs Food collision
				if (snakeSize.front().x == foodPos.x && snakeSize.front().y == foodPos.y)
				{
					snakeSize.push_back({ snakeSize.back().x - move.x, snakeSize.back().y - move.y });
					for (std::list<Point>::iterator i = snakeSize.begin(); i != snakeSize.end(); i++)
					{
						if (i->x == foodPos.x && i->y == foodPos.y)
							foodPos = { rand() % unitSize.x, rand() % (unitSize.y - 1) };
					}
					score += 10;
				}
			}
		}
		window.clear(sf::Color(0.0f, 200.0f, 255.0f, 255.0f));

		for (std::list<Point>::iterator i = snakeSize.begin(); i != snakeSize.end(); i++)
		{
			snakePiece.setPosition(i->x * sprSize, i->y * sprSize);
			window.draw(snakePiece);
		}

		food.setPosition(foodPos.x * sprSize, foodPos.y * sprSize);
		window.draw(food);

		if (isPaused)
		{
			pauseText.setPosition(SCREEN_WIDTH / 2.0f - pauseText.getCharacterSize() * 2.5f, SCREEN_HEIGHT / 2.0f - gameOverText.getCharacterSize() * 1.5f);
			commandText.setString("Press Space to continue");
			commandText.setPosition(SCREEN_WIDTH / 2.0f - 125, SCREEN_HEIGHT / 2.0f - gameOverText.getCharacterSize() / 2.0f);
			window.draw(pauseText);
			window.draw(commandText);
		}
		
		if (isDead)
		{
			gameOverText.setPosition(SCREEN_WIDTH / 2.0f - gameOverText.getCharacterSize() * 2, SCREEN_HEIGHT / 2.0f - gameOverText.getCharacterSize() * 1.5f);
			commandText.setString("Press Space to start again");
			commandText.setPosition(SCREEN_WIDTH / 2.0f - 125, SCREEN_HEIGHT / 2.0f - gameOverText.getCharacterSize() / 2.0f);
			window.draw(gameOverText);
			window.draw(commandText);
		}

		window.draw(SubTextBackground);

		std::string number = IntToString(score);
		scoreText.setString("Score: " + number);
		scoreText.setPosition(SCREEN_WIDTH / 2 - 96, SCREEN_HEIGHT - 48);

		hintText.setString("Space: Pause Game");
		hintText.setPosition(0, SCREEN_HEIGHT - 38);

		hintText2.setString("ESC: Exit Game");
		hintText2.setPosition(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 38);

		window.draw(scoreText);
		window.draw(hintText);
		window.draw(hintText2);
		

		window.display();
	}
	return 0;
}
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


const int NUM_CLD = 3;
const int NUM_BRANCHES = 6;
const float AXE_POSITION_LEFT = 700.f;
const float AXE_POSITION_RIGHT = 1075.f;

void updateBranches(int seed);

sf::Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE }; 
side branchPositions[NUM_BRANCHES];

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 780), "Timber", sf::Style::Fullscreen);

	sf::Texture textureBG;
	textureBG.loadFromFile("graphics/background.png");
	sf::Sprite spriteBG;
	spriteBG.setTexture(textureBG);


	sf::Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	sf::Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);


	sf::Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	sf::Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);


	// Is the bee currently moving?
	bool beeActive = false;
	// How fast can the bee fly
	float beeSpeed = 0.0f;

	// make 3 cloud sprites from 1 texture
	sf::Texture textureCloud;
	// Load 1 new texture
	textureCloud.loadFromFile("graphics/cloud.png");


	// 3 New sprites with the same texture
	sf::Sprite spriteCloud[3];
	// Are the clouds currently on screen?
	bool cloudActive[3] = { false };
	// How fast is each cloud?
	float cloudSpeed[3] = { 0.0f };

	for (int i = 0; i < NUM_CLD; i++)
	{
		// Position the clouds on the left of the screen
		// at different heights
		spriteCloud[i].setTexture(textureCloud);
		spriteCloud[i].setPosition(-400, i * 250);
	}

	sf::Clock clock;
	
	// A boolean to make the game paused
	bool paused = true;


	// the font and text render
	sf::Font myFont;
	myFont.loadFromFile("fonts/KOMIKAP_.ttf");

	// Message to say start the game, you lost HUD
	sf::Text messageText;
	messageText.setFont(myFont);
	messageText.setString("Please Enter to start");
	messageText.setCharacterSize(75);
	messageText.setFillColor(sf::Color::White);
	
	sf::FloatRect textRect = messageText.getLocalBounds();
	messageText.setPosition(1920.0f / 2.f - textRect.width / 2.0f,
		780.0f / 2.0f + textRect.height / 2.0f);
	
	// score text
	sf::Text scoreText;
	scoreText.setFont(myFont);
	scoreText.setString("Score : 0");
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(sf::Color::White);

	int score{};

	sf::Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	sf::RectangleShape timeBar;
	float timeBarWidth = 400.0f;
	float timeBarHeight = 80.0f;
	timeBar.setSize(sf:: Vector2f(timeBarWidth, timeBarHeight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarWidth / 2, 980);

	sf::Time gameTimeTotal{};
	float timeRemaining{ 6.f };
	float timeBarWidthPerSecond = timeBarWidth / timeRemaining;

	
	sf::Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	sf::Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	side playerSide = side::LEFT;

	sf::Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	sf::Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe
	sf::Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	sf::Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	sf::Texture textureLog;
	textureLog.loadFromFile("graphics/Log.png");
	sf::Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive{ false };
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	sf::SoundBuffer rip;
	rip.loadFromFile("sound/death.wav");
	sf::Sound ripSound;
	ripSound.setBuffer(rip);

	while (window.isOpen())
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			paused = false;
			messageText.setString("");
			timeRemaining = 6.f;
			timeBar.setSize(sf::Vector2f(timeBarWidth, timeBarHeight));

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			// Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);
			// Move the player into position
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		sf::Event event{};

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased && !paused)
			{
				acceptInput = true;

				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}
			



		if (!paused)
		{
			sf::Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));
		
			if (!beeActive)
			{
				// set the speed of the bee
				srand((int)time(0));
				beeSpeed = rand() % 200 + 200;

				// set the height of the bee 
				srand((int)time(0) * 2);
				float height = rand() % 500 + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				// Moving the bee to the left of the screen
				spriteBee.setPosition(spriteBee.getPosition().x - beeSpeed * dt.asSeconds(),
									spriteBee.getPosition().y);


				if (spriteBee.getPosition().x <= - static_cast<float>(textureBee.getSize().x))
				{
					beeActive = false;
				}

			}

			for (int i = 0; i < NUM_CLD; i++)
			{
				if (!cloudActive[i])
				{
					// how fast is the cloud
					srand((int)time(0) * i);
					cloudSpeed[i] = rand() % 200;

					// How height the cloud
					srand((int)time(0) * i);
					float height = rand() % 150;
					spriteCloud[i].setPosition(-250, height);
					cloudActive[i] = true;
				}
				else
				{
					spriteCloud[i].setPosition(spriteCloud[i].getPosition().x + cloudSpeed[i] * dt.asSeconds(),
						spriteCloud[i].getPosition().y);

					// checck if the cloud are off--screen on the right
					if (spriteCloud[i].getPosition().x >= 2000.0f)
					{
						cloudActive[i] = false;
					}
				}
			}

			if (acceptInput)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					playerSide = side::RIGHT;
					score++;

					timeRemaining += (2 / score) + .15;
					//std::cout << "time remaining = " << timeRemaining << std::endl;

					spritePlayer.setPosition(1200,720);
					spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

					updateBranches(score);

					spriteLog.setPosition(810, 720);
					logSpeedX = -5000;
					logActive = true;


					acceptInput = false;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					playerSide = side::LEFT;
					score++;

					timeRemaining += (2 / score) + .15;

					spriteAxe.setPosition(AXE_POSITION_LEFT,
						spriteAxe.getPosition().y);

					spritePlayer.setPosition(580, 720);

					// update the branches
					updateBranches(score);
					// set the log flying


					spriteLog.setPosition(810, 720);
					logSpeedX = 5000;
					logActive = true;

					acceptInput = false;
				}
			}

			// Update the score 
			std::stringstream ss;
			ss << "Score : " << score;
			scoreText.setString(ss.str());


			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);

				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}
			
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + dt.asSeconds() * logSpeedX,
					spriteLog.getPosition().y + dt.asSeconds() * logSpeedY);

				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			if (timeRemaining <= 0.f)
			{
				messageText.setString("Out of Time!");
				paused = true;
			}

			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;
				// Draw the gravestone
				spriteRIP.setPosition(525, 760);
				// hide the player
				spritePlayer.setPosition(2000, 660);
				// Change the text of the message
				messageText.setString("SQUISHED!!");

				ripSound.play();
			}

		}

		window.clear();

		window.draw(spriteBG);


		window.draw(spriteCloud[0]);
		window.draw(spriteCloud[1]);
		window.draw(spriteCloud[2]);

		window.draw(spriteTree);

		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		window.draw(spriteBee);

		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);

		window.draw(messageText);
		window.draw(scoreText);

		window.draw(timeBar);

		window.display();
	}

	return 0;
}

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	srand((int)(time(0)) + seed);
	int r = rand() % 5;

	switch (r)
	{

	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;

		break;
	}
}

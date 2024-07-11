#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>

// Make code easier to type with "using namespace"
using namespace sf;

// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
// Where is the player/branch?
// Left or Right
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);
    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Default);
    // Set the window view to match the resolution
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);
    // Create a texture to hold a graphic on the GPU
    Texture textureBackground;
    // Load a graphic into the texture
    textureBackground.loadFromFile("content/graphics/background.png");
    // Create a sprite
    Sprite spriteBackground;
    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);
    // Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("content/graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);
    // Enhanced version
	Texture textureTree2;
	textureTree2.loadFromFile("content/graphics/tree2.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
    spriteTree2.setScale(0.5f, 0.95f);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
    spriteTree4.setScale(0.3f, 0.85f);
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
    spriteTree5.setScale(1, 0.97f);
	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);


	spriteTree2.setPosition(20, 0);
	spriteTree3.setPosition(300, -100);
	spriteTree4.setPosition(1200, 0);
	spriteTree5.setPosition(1500, 0);
	spriteTree6.setPosition(1900, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("content/graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    // Is the bee currently moving?
    bool beeActive = false;
    // How fast can the bee fly
    float beeSpeed = 0.0f;

    // make 3 cloud sprites from 1 texture
    Texture textureCloud;
    // Load 1 new texture
    textureCloud.loadFromFile("content/graphics/cloud.png");

    // Make the clouds with arrays
	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++)
	{
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;
	}

    // Variables to control time itself
    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Track whether the game is running
    bool paused = true;

    // Draw some text
    int score = 0;
    Text messageText;
    Text scoreText;
    Text fpsText;
    // We need to choose a font
    Font font;
    font.loadFromFile("content/fonts/KOMIKAP_.ttf");
    // Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);
    fpsText.setFont(font);
	
	// Set up the fps text
	fpsText.setFillColor(Color::White);
	fpsText.setCharacterSize(100);
	fpsText.setPosition(1200, 20);
    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    // Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f,1080 / 2.0f);
    scoreText.setPosition(20, 20);
    // Backgrounds for the text
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600,105));
	rect1.setPosition(0,30);

	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(1000, 105));
	rect2.setPosition(1150, 30);

    // Prepare 5 branches
    Texture textureBranch;
    textureBranch.loadFromFile("content/graphics/branch.png");

    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);

        // Set the sprite's origin to dead center
        // We can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }

    float beeHeight = (rand() % 500) + 500;

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("content/graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("content/graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("content/graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("content/graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control the player input
    bool acceptInput = false;
    
    // Prepare the sound
    SoundBuffer chopBuffer;
    if (!chopBuffer.loadFromFile("sound/chop.wav"))
    {
        std::cout << "didn't load chop.wav";
    }

    Sound chop;
    chop.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("content/sounds/death.wav"))
    {
        std::cout << "didn't load death.wav";
    }
    Sound death;
    death.setBuffer(deathBuffer);

    // Out of time
    SoundBuffer ootBuffer;
    if (!ootBuffer.loadFromFile("content/sounds/out_of_time.wav"))
    {
        std::cout << "didn't load out_of_time.wav";
    }
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);
	// control the drawing of the score
	int lastDrawn = 0;
    while (window.isOpen())
    {
        /*
		****************************************
		Handle the players input
		****************************************
		*/

        Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::KeyReleased && !paused)
            {
                // Listen for key presses again
                acceptInput = true;
                // hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
            else if (event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Escape)
                {
                    window.close();
                }

                if(event.key.code == Keyboard::Enter)
                {
                    paused = false;
                    
                    // Reset the time and the score
                    score = 0;
                    timeRemaining = 6;
                    clock.restart();
                    // Make all the branches disappear
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

                // Wrap the player controls to
                // Make sure we are accepting input
                if (acceptInput)
                {
                    // First handle pressing the right cursor key
                    if (event.key.code == Keyboard::Right)
                    {
                        // Make sure the player is on the right
                        playerSide = side::RIGHT;
                        score ++;
                        // Add to the amount of time remaining
                        timeRemaining += (2 / score) + .15;
                        spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                        spritePlayer.setPosition(1200, 720);
                        // Update the branches
                        updateBranches(score);
                        // Set the log flying to the left
                        spriteLog.setPosition(810, 720);
                        logSpeedX = -5000;
                        logActive = true;
                        acceptInput = false;

                        // Play a chop sound
                        chop.play();
                    }
                }

                // Handle the left cursor key
                if (event.key.code == Keyboard::Left)
                {
                    // Make sure the player is on the left
                    playerSide = side::LEFT;
                    score++;
                    // Add to the amount of time remaining
                    timeRemaining += (2 / score) + .15;
                    spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(580, 720);
                    // update the branches
                    updateBranches(score);
                    // set the log flying
                    spriteLog.setPosition(810, 720);
                    logSpeedX = 5000;
                    logActive = true;
                    acceptInput = false;

                    // Play a chop sound
                    chop.play();
                }
            }
        }

        /*
        ****************************************
        Update the scene
        ****************************************
        */
        if(!paused)
        {

            // Measure time
            Time dt = clock.restart();
            // Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {

                // Pause the game
                paused = true;

                // Change the message shown to the player
                messageText.setString("Out of time!!");

                //Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                // Play the out of time sound
                outOfTime.play();
            }

            // Setup the bee
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0)*10);
                beeSpeed = (rand() % 200) + 200;
                // How high is the bee
                srand((int)time(0) * 10);
                beeHeight = (rand() % 500) + 500;
                spriteBee.setPosition(2000, beeHeight);
                beeActive = true;
            }
            else
            {
                // Move the bee
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                beeHeight + 50.0f * sin(2.0f * timeRemaining));
                // Has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    // Set it up ready to be a whole new bee next frame
                    beeActive = false;
                }
            }

			// Manage the clouds with arrays
			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!cloudsActive[i])
				{
					// How fast is the cloud
					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);

					// How high is the cloud
					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;

				}
				else
				{
					// Set the new position
					clouds[i].setPosition(
						clouds[i].getPosition().x +
						(cloudSpeeds[i] * dt.asSeconds()),
						clouds[i].getPosition().y);

					// Has the cloud reached the right hand edge of the screen?
					if (clouds[i].getPosition().x > 1920)
					{
						// Set it up ready to be a whole new cloud next frame
						cloudsActive[i] = false;
					}

				}
            }
			// Draw the score and the frame rate once every 100 frames
			lastDrawn++;
			if (lastDrawn == 100) {
				// Update the score text
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());

				// Draw the fps
				std::stringstream ss2;
				ss2 << "FPS = " << 1/dt.asSeconds();
				fpsText.setString(ss2.str());
				lastDrawn = 0;
			}

            // update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    // Move the sprite to the left side
                    branches[i].setPosition(610, height);
                    // Flip the sprite round the other way
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330, height); 
					// Set the sprite rotation to normal
					branches[i].setOrigin(220, 40);
                    branches[i].setRotation(0);
                }
                else
                {
					// Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }
            // Handle a flying log
            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), 
                                        spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
                // Has the log reached the right hand edge?
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
					// Set it up ready to be a whole new cloud next frame
					logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            // has the player been squished by a branch?
            if (branchPositions[5] == playerSide)
            {
                // death
                paused = true;
                acceptInput = false;
                // Draw the gravestone
                spriteRIP.setPosition(525, 760); 
                // hide the player
                spritePlayer.setPosition(2000, 660);
                // Change the text of the message
                messageText.setString("SQUISHED!!");
                // Center it on the screen
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                // Play the death sound
                death.play();
            }
        } // End if(!paused)
		
        /*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from the last frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);

        // Draw the clouds
		for (int i = 0; i < NUM_CLOUDS; i++)
		{
			window.draw(clouds[i]);
		}

        // Enhanced version
		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);
		window.draw(spriteTree5);
		window.draw(spriteTree6);

        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        // Draw the tree
        window.draw(spriteTree);
        // Draw the player
        window.draw(spritePlayer);
        // Draw the axe
        window.draw(spriteAxe);
        // Draw the flying log
        window.draw(spriteLog);
        // Draw the gravestone
        window.draw(spriteRIP);
		// Draw backgrounds for the text
		window.draw(rect1);
		window.draw(rect2);

		// Drawraw the bee
        window.draw(spriteBee);
        // Draw the score
        window.draw(scoreText);
        // Draw the FPS
		window.draw(fpsText);
        // Draw the timebar
        window.draw(timeBar);

        if (paused)
        {
            // Draw our message 
            window.draw(messageText); 
        }

        // Show everything we just drew
        window.display();
    }
    
    return 0;
}

// Function definition
void updateBranches(int seed)
{
    // Move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);
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
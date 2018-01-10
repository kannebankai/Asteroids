#include "World.hpp"
using namespace std;

//Constructeur unique de la classe World
World::World()
{
	//Lance la fonction d'initialisation du jeu
	init();
}

//Fonction qui va lancer toutes les fonctions d'initialisation et lancer le jeu
void World::init()
{
	//Lance toutes les fonctions d'initialisations
	initConfig();
	initIcon();
	initInput();
	initSoundBuffer();
	//Lance le jeu
	start();
}

//Fonction qui initie la configuration du jeu d'apres le fichier config
void World::initConfig()
{
	//On ouvre le fichier config en lecture
	//Les lignes du fichier de config sont de la forme "attribut:valeur"
	ifstream fichier("config", ios::in);
	string delimiter=":";
	string line;
	string attribute;
	string value;
	if(fichier)
	{
		//On configure le jeu d'apres le fichier de configuration config
		while(getline(fichier, line))
		{
			//On separe l'attribut et la valeur selon le delimiteur
			attribute = line.substr(0, line.find(delimiter));
			value = line.substr(line.find(delimiter)+1, line.length());
			//Si la ligne fait reference a l'icone on change le nom de l'icone
			if(attribute=="icon")
				iconName=value;
			//Si la ligne fait reference au nombre de joueur on change le nombre de joueur
			else if(attribute=="players")
			{
				if(value=="two")
					nbPlayers=2;
				else
					nbPlayers=1;
			}
			//Si la ligne fait reference aux controles du joueur 1
			else if(attribute=="input1")
			{
				if(value=="mouse")
					input[0]=1;
				else if(value=="gamepad")
				{
					input[0]=2;
				}
				else
					input[0]=0;

			}
			//Si la ligne fait reference aux controles du joueur 2
			else if(attribute=="input2")
			{
				if(value=="mouse" && input[0]!=1)
					input[1]=1;
				else if(value=="gamepad")
					input[1]=2;

			}
			//Si la ligne fait reference au reseau
			else if(attribute=="network")
			{
				if(value=="true")
					network=true;
				else
					network=false;

			}
			//Si la ligne fait reference a la musique
			else if(attribute=="music")
			{
				musicName=value;

			}
			//Si la ligne fait reference a la sensibilite de la souris
			else if(attribute=="mouseSensitivity")
			{
				mouseSensitivity=stoi(value);

			}

		}

		fichier.close();
	}
	//Si le fichier n'a pas pu etre ouvert alors on met des valeurs par defaut
	else
	{
		iconName="icon.png";
		nbPlayers=1;
		input[0]=0;
		input[1]=0;
		network=false;
		musicName="music.wav";
		mouseSensitivity=500;
	}
}

//Fonction qui initie l'icone du jeu
//Les icones doivent etre de taille 60x60 sous peine d'etre deformees
void World::initIcon()
{
	sf::Image icon;
	//Si on reussi a charge l'icone definie dans la fonction config
	if(icon.loadFromFile(iconName))
		window.setIcon(60, 60, icon.getPixelsPtr());
	//Sinon on charge l'icone par defaut
	else if(icon.loadFromFile("icon.png"))
		window.setIcon(60, 60, icon.getPixelsPtr());

}

//Fonction qui initie les controles
void World::initInput()
{
	initKeyboardInput();
	//Les gamepad et la souris ne sont pas initialises ici sinon cela ne marche pas
	//Ils seront initialises daans la fonction principale du jeu et de sa fenetre
	//Car ils ont besoin de cette derniere pour etre initialise
}

//Fonction qui initie les controles au clavier
void World::initKeyboardInput()
{

	//inputTab[i][0] = avancer
	//inputTab[i][1] = reculer
	//inputTab[i][2] = aller vers la droite
	//inputTab[i][3] = aller vers la gauche
	//inputTab[i][4] = tirer

	inputTab[0][0]=sf::Keyboard::Z;
	inputTab[0][1]=sf::Keyboard::S;
	inputTab[0][2]=sf::Keyboard::D;
	inputTab[0][3]=sf::Keyboard::Q;
	inputTab[0][4]=sf::Keyboard::Space;

	inputTab[1][0]=sf::Keyboard::Numpad8;
	inputTab[1][1]=sf::Keyboard::Numpad5;
	inputTab[1][2]=sf::Keyboard::Numpad6;
	inputTab[1][3]=sf::Keyboard::Numpad4;
	inputTab[1][4]=sf::Keyboard::Numpad0;
}

//Initie les controles souris en mettant la souris au centre de l'ecran
void World::initMouseInput()
{
	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
}

//Initie les controles gamepad
//Cette fonction regarde si assez de gamepad sont connectes et reconnus par rapport au
//nombre de joueur ayant demande de jouer au gamepad
//Si le nombre de gamepad disponible est insuffisant les controles des joueurs a qui il
//manque un gamepad sont remplace par les controles clavier
void World::initGamepadInput()
{
	int j = 0;
	for (; j < nbPlayers; j++)
		if (input[j] == 2)
			break;
	for (int i=0; i<8 && j<nbPlayers; i++)
		if (sf::Joystick::isConnected(i)) {
			gamepadId[j] = i;
			input[j] = 2;
		}

	for (j=0; j < nbPlayers; j++)
	{
		if (input[j]==2 && gamepadId[j] == -1)
			input[j] = 0;
	}
}

//Initialise les SoundBuffer du jeu
//Ils vont charger leur fichier associe
void World::initSoundBuffer()
{
	asteroidBuffer.loadFromFile("asteroidExplosion.wav");
	shipBuffer.loadFromFile("asteroidExplosion.wav");
}

//Fonction qui gere les controles clavier dans la boucle d'evenement
//Elle prend comme parametre l'id du joueur qui doit etre gere (ships[id])
void World::handleKeyboardInput(int playerId)
{
	//Si le joueur avance
	if(sf::Keyboard::isKeyPressed(inputTab[playerId][0]))
	{
		ships[playerId].modifyVelocity(0.1);
	}

	//Si le joueur recule
	if(sf::Keyboard::isKeyPressed(inputTab[playerId][1]))
	{
		ships[playerId].modifyVelocity(-0.1);
	}

	//Si le joueur tourne vers la droite
	if(sf::Keyboard::isKeyPressed(inputTab[playerId][2]))
	{
		ships[playerId].addRotation(5);
	}

	//Si le joueur tourne vers la gauche
	if(sf::Keyboard::isKeyPressed(inputTab[playerId][3]))
	{
		ships[playerId].addRotation(-5);
	}
}

//Fonction qui gere les controles souris dans la boucle d'evenement
//Elle prend comme parametre l'id du joueur qui doit etre gere (ships[id])
void World::handleMouseInput(int playerId)
{
	//Le vecteur actualMousePosition prend la position actuelle de la souris dans le
	//fenetre de jeu
	sf::Vector2i actualMousePosition=sf::Mouse::getPosition(window);
	//On met les coordonnees de ce vecteur dans des variables
	float x = actualMousePosition.x;
	float y = actualMousePosition.y;

	//On modifie ces coordonnees
	x -= window.getSize().x / 2;
	y -= window.getSize().y / 2;

	//On deplace le vaisseau par rapport a l'addition des valeurs absolues de ces 
	//coordonnees (le traitement d'une vitesse trop eleve est traitee dans 
	//modifyVelocityGamepad)
	float speed = abs(x) + abs(y);
	ships[playerId].modifyVelocityGamepad(speed);

	//normalisation du vecteur de position
	x /= window.getSize().x / 2;
	y /= window.getSize().y / 2;

	//On regarde l'angle de rotation
	float angle = 0;
	if (y <= 0)
	{
		angle = acos(-x);

	}
	if (y > 0)
	{
		angle = acos(-x);
		angle = M_PI + (M_PI - angle);
	}
	angle -= M_PI / 2;
	//On effectue la rotation du vaisseau
	ships[playerId].setRotationGamepad(angle);

	//Si le joueur a fait un clic gauche, synonyme de tir
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		ships[playerId].createBullet();
}

//Fonction qui gere les controles gamepad dans la boucle d'evenement
//Elle prend comme parametre l'id du joueur qui doit etre gere (ships[id])
void World::handleGamepadInput(int playerId)
{
	//On deplace le vaisseau par rapport a l'addition des valeurs absolues de ces 
	//coordonnees (le traitement d'une vitesse trop eleve est traitee dans 
	//modifyVelocityGamepad)
	float speed = abs(sf::Joystick::getAxisPosition(gamepadId[playerId], sf::Joystick::X)) + abs(sf::Joystick::getAxisPosition(gamepadId[playerId], sf::Joystick::Y));
	ships[playerId].modifyVelocityGamepad(speed);

	//On regarde l'angle de rotation
	float angle = 0;
	if (sf::Joystick::getAxisPosition(gamepadId[playerId], sf::Joystick::Y) <= 0)
	{
		angle = acos(-sf::Joystick::getAxisPosition(gamepadId[playerId], sf::Joystick::X) / 100);

	}
	if (sf::Joystick::getAxisPosition(gamepadId[playerId], sf::Joystick::Y) > 0)
	{
		angle = acos(-sf::Joystick::getAxisPosition(gamepadId[playerId], sf::Joystick::X) / 100);
		angle = M_PI + (M_PI-angle);
	}
	angle -= M_PI / 2;
	//On effectue la rotation du vaisseau
	ships[playerId].setRotationGamepad(angle);

	//Si le joueur a appuye sur la touche correspond au tir
	if (sf::Joystick::isButtonPressed(gamepadId[playerId], 5))
		ships[playerId].createBullet();
}

//Fonction qui gere les controles dans la boucle d'evenement
void World::handlePlayerInput(int playerId)
{
	if(!input[playerId])
		handleKeyboardInput(playerId);
	if(input[playerId]==1)
		handleMouseInput(playerId);
	if(input[playerId]==2)
		handleGamepadInput(playerId);
}

//Fonction qui cree un nouvel asteroide si cela est possible
void World::createAsteroid(int x, int y)
{
	//Parcours le tableau d'asteroides voir s'il reste de la place pour un nouveau
	for (int i = 0; i<nbMaxAsteroid; i++)
		if (!asteroids[i].exist)
		{
			//La direction des nouveaux asteroides est aleatoire
			float directionX = (((float)(rand() % 201) - 100) / 100);
			float directionY = (((float)(rand() % 201) - 100) / 100);
			//On cree ce nouvel asteroide
			asteroids[i] = Asteroid(window.getSize(), x, y, sf::Vector2f(directionX,directionY));
			//On sort de la boucle
			break;
		}
}

//Fonction qui indique de quel cote de l'ecran l'asteroide doit apparaitre
void World::spawnAsteroid()
{
	nbAsteroid++;
	//side indique de quel côté l'astéroide va apparaître
	//On fait apparaitre l'asteroide hors de l'ecran pour ne pas avoir un effet de
	//d'apparition instantanee
	int side = rand() % 4;
	//0=top 1=right 2=bottom 3=left
	if (!side)
	{
		int pos = rand() % window.getSize().x;
		createAsteroid(pos, -ASTEROID_RADIUS);
	}
	else if (side == 1)
	{
		int pos = rand() % window.getSize().y;
		createAsteroid(window.getSize().x + ASTEROID_RADIUS, pos);
	}
	else if (side == 2)
	{
		int pos = rand() % window.getSize().x;
		createAsteroid(pos, window.getSize().y + ASTEROID_RADIUS);
	}
	else if (side == 3)
	{
		int pos = rand() % window.getSize().y;
		createAsteroid(-ASTEROID_RADIUS, pos);
	}
}

//Fonction qui gere les collisions du jeu
void World::handleCollision()
{
	//Regarde pour chaque joueur actif si son vaisseau ou ses munitions entrent en 
	//collision avec des asteroides
	for (int i = 0; i < nbPlayers; i++)
	{
		if (ships[i].exist)
			for (int j = 0; j < nbMaxAsteroid; j++)
			{
				if(asteroids[j].exist)
					handleCollision(ships[i], asteroids[j]);
			}
	}
}

//Fonction qui gere les collisions entre le vaisseau et l'asteroide
void World::handleCollision(Ship &ship, Asteroid &asteroid)
{
	sf::Vector2f d = ship.getPosition() - asteroid.getPosition();
	float dist2 = d.x*d.x + d.y*d.y;
	float radiusSum = ship.getRadius() + asteroid.getRadius();
	bool test;
	test = dist2 <= radiusSum*radiusSum;			
	if (test)
	{
		sound.setBuffer(shipBuffer);
		sound.play();
		ship.hp--;
		if (!ship.hp)
			ship.exist = false;
		else
			ship.setPosition(window.getSize().x / 2, window.getSize().y / 2);
		asteroid.exist = false;
		nbAsteroid--;
	}

	for (int i = 0; i < ship.nbMaxBullet; i++)
	{
		if (ship.bullets[i].exist) 
		{
			test = handleCollision(ship.bullets[i], asteroid);
			if (test)
			{
				ship.bullets[i].exist = false;
				asteroid.exist = false;
				sound.setBuffer(asteroidBuffer);
				sound.play();
				ship.score += asteroid.value;
				nbAsteroid--;
				asteroid.hp--;
				if (asteroid.hp >0 && nbAsteroid < nbMaxAsteroid - 2)
				{
					for (int j = 0; j < 2;)
					{
						for (int k = 0; k<nbMaxAsteroid; k++)
							if (!asteroids[k].exist)
							{
								asteroids[k] = Asteroid(window.getSize(), asteroid.getPosition().x, asteroid.getPosition().y, asteroid.direction);
								asteroids[k].littleAsteroid(ship.bullets[i].direction, -M_PI / 4 + (j * M_PI/2));
								nbAsteroid++;
								j++;
								break;
							}
					}
				}
			}
		}
	}

	if (nbPlayers == 1 && !ships[0].exist)
	{
		middleText.setString("GAME OVER\nPress R to restart");
		gameNotOver = false;
	}
	else if (!ships[0].exist)
	{
		middleText.setString("Player 2 won !\nPress R to restart");
		gameNotOver = false;
	}
	else if (!ships[1].exist)
	{
		middleText.setString("Player 1 won !\nPress R to restart");
		gameNotOver = false;
	}
}

bool World::handleCollision(Bullet &bullet, Asteroid &asteroid)
{
	sf::Vector2f d = bullet.getPosition() - asteroid.getPosition();
	float dist2 = d.x*d.x + d.y*d.y;
	float radiusSum = bullet.getRadius() + asteroid.getRadius();
	return dist2 <= radiusSum*radiusSum;
}

void World::moveEntities()
{
	for(int i=0;i<nbPlayers;i++)
	{
		if(ships[i].exist)
		{
			ships[i].moveShip();
			for(int j=0;j<25;j++)
				if(ships[i].bullets[j].exist)
					ships[i].bullets[j].moveBullet();
		}
	}
	for(int i=0;i<nbMaxAsteroid;i++)
		if(asteroids[i].exist)
			asteroids[i].moveAsteroid();
}

void World::draw()
{
	for(int i=0;i<nbPlayers;i++)
	{
		if(ships[i].exist)
		{
			window.draw(ships[i]);
			for(int j=0;j<25;j++)
				if(ships[i].bullets[j].exist)
					window.draw(ships[i].bullets[j]);
		}
	}
	for(int i=0;i<nbMaxAsteroid;i++)
		if(asteroids[i].exist)
			window.draw(asteroids[i]);

	for (int i = 0; i < nbPlayers; i++)
		for (int j = 0; j < ships[i].hp; j++)
			window.draw(lifeShip[i][j]);

	window.draw(middleText);

	//Modifie la place du score du deuxième joueur si celui ci dépasse du cadre
	int moreSpace = 0, tmp = ships[1].score;
	while (tmp > 10)moreSpace++, tmp /= 10;
	playerText[1].setPosition(10 + window.getSize().x - 50 - (10 * moreSpace), 5);

	for (int i = 0; i < nbPlayers; i++)
	{
		playerText[i].setString(to_string(ships[i].score));
		window.draw(playerText[i]);
	}
}

void World::display()
{
	window.clear();
	draw();
	window.display();
}

void World::initShip()
{
	initShipHp();
	initShipPosition();
}

void World::initShipPosition()
{
	if (nbPlayers == 1)
		ships[0].startingPosition(0);
	else
	{
		ships[0].startingPosition(1);
		ships[1].startingPosition(2);
	}
}

void World::initShipHp()
{
	ships[0] = Ship(sf::Color::Blue, window.getSize());
	ships[1] = Ship(sf::Color::Red, window.getSize());

	for (int i = 0; i < nbPlayers; i++)
		for (int j = 0; j < ships[i].hp; j++)
		{
			lifeShip[i][j] = ships[i];
			lifeShip[i][j].setScale(0.75, 0.75);
			lifeShip[i][j].setPosition(0 + 20 * (j + 1) + (i * 720), 50);
		}
}

//Cette fonction est appelée avant la boucle de jeu et n'est pas regroupée avec les autres car on ne veut instancier la musique qu'une seule fois
void World::initMusic()
{
	isPlaying = true;
	volume = 100;


	if (!music.openFromFile(musicName))
		musicEnabled = false;
	music.setVolume(volume);
	music.setLoop(true);
}

void World::initAsteroids()
{
	for (int i = 0; i < nbMaxAsteroid; i++)
		asteroids[i].exist=false;
}

void World::initInGame()
{
	initShip();
	initGamepadInput();
	initAsteroids();

	//Cet appel de fonction est séparé de la fonction initMusic car on veut que la musique se relance à chaque fois que la partie redémarre
	music.play();
}

void World::timeLaps()
{

	//Temps entre le lancement du jeu et la prise des contrôles
	startingClock.restart();
	while (startingClock.getElapsedTime().asSeconds() < 5)
	{
		//On veut que le texte informatif reste afficher une seconde du coup il faut annulé le changement en dessous de une seconde
		if (startingClock.getElapsedTime().asSeconds() < 1);

		else if (startingClock.getElapsedTime().asSeconds() < 2)
		{
			middleText.setString("3");
		}
		else if (startingClock.getElapsedTime().asSeconds() < 3)
		{
			middleText.setString("2");
		}
		else if (startingClock.getElapsedTime().asSeconds() < 4)
		{
			middleText.setString("1");
		}
		else if (startingClock.getElapsedTime().asSeconds() < 5)
		{
			middleText.setString("GO");
		}
		display();
	}

	middleText.setString("");
}

void World::start()
{
	window.create(sf::VideoMode(800, 800), "Asteroid v0.0.5");
	sf::Image icon;
	if (icon.loadFromFile(iconName))
		window.setIcon(60, 60, icon.getPixelsPtr());
	if(input[0]==1||input[1]==1)
	{
		initMouseInput();
		//window.setMouseCursorVisible(false);
		window.setMouseCursorGrabbed(true);
	}

	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	//Initialisation de middleText (ne marche pas quand on l'initialise au même endroit mais dans une fonction)
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "font problem" << endl;
	}
	middleText = sf::Text("", font, 24);
	middleText.setFillColor(sf::Color::White);
	middleText.setOutlineColor(sf::Color::Green);
	middleText.setOutlineThickness(3);
	middleText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 - 50);

	//Initialisation des scores
	for (int i = 0; i < nbPlayers; i++)
	{
		playerText[i] = sf::Text("Score", font, 24);
		playerText[i].setFillColor(sf::Color::White);
		playerText[i].setOutlineThickness(3);
		playerText[i].setPosition(10 + i*(window.getSize().x - 50), 5);

	}
	playerText[0].setOutlineColor(sf::Color::Blue);
	playerText[1].setOutlineColor(sf::Color::Red);

	initMusic();

	gameNotOver = true;
	restart = false;
	while (window.isOpen())
	{
		initInGame();

		if(restart)middleText.setString("RESTART IN");
		else middleText.setString("START IN");

		timeLaps();

		spawnClock.restart();
		srand(time(0));
		spawnRate = 5;
		spawnAsteroid();

		while (window.isOpen())
		{
			sf::Event event;
			if (isPlaying && gameNotOver)
			{
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();

					if (event.type == sf::Event::KeyReleased)
					{
						if (ships[0].exist && input[0] == 0 && event.key.code == sf::Keyboard::Space)
						{
							ships[0].createBullet();
						}
						if (ships[1].exist && input[1] == 0 && event.key.code == inputTab[1][4])
						{
							ships[1].createBullet();
						}
						if (event.key.code == sf::Keyboard::P)
						{
							middleText.setString("PAUSE");
							isPlaying = false;
							if (musicEnabled)
								music.pause();
						}
						if (event.key.code == sf::Keyboard::M)
						{
							if (musicEnabled)
							{
								musicEnabled = false;
								music.stop();
							}
							else
							{
								musicEnabled = true;
								music.play();
							}
						}
						if (event.key.code == sf::Keyboard::O)
						{
							volume += 5;
							if (volume>100)volume = 100;
							music.setVolume(volume);
						}
						if (event.key.code == sf::Keyboard::L)
						{
							volume -= 5;
							if (volume<0)volume = 0;
							music.setVolume(volume);
						}
					}

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
				{
					window.close();
				}

				for (int i = 0; i<nbPlayers; i++)
					if (ships[i].exist)
						handlePlayerInput(i);

				moveEntities();

				handleCollision();

				if (spawnClock.getElapsedTime().asSeconds() > spawnRate)
				{
					spawnAsteroid();
					spawnClock.restart();
				}
				display();
			}
			else
			{
				restart = false;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();

					if (gameNotOver && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P) // || (input[0] == 2 && sf::Joystick::isButtonPressed(gamepadId[0], 9)) || (input[1] == 2 && sf::Joystick::isButtonPressed(gamepadId[1], 9)))
					{
						middleText.setString("PAUSE END IN");
						timeLaps();
						isPlaying = true;
						if (musicEnabled)
							music.play();
					}

					if (!gameNotOver && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
					{
						gameNotOver = true;
						restart = true;
					}

				}
				if (restart) break; //Pour revenir au début de la boucle de jeu pour relancer la partie
			}
		}
	}
	
}

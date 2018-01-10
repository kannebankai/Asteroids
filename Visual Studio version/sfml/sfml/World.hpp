#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "Ship.hpp"
#include "Asteroid.hpp"
#define ASTEROID_RADIUS 15

using namespace std;
//Description de la classe World
//C'est la classe principale du jeu, c'est elle qui contient la boucle de jeu et la gestion
//de tous les evenements
//Sans elle, pas de fenetre de jeu, et sans fenetre de jeu, pas de jeu
class World
{

	public:
		//La RenderWindow correspondant a la fenetre de jeu
		sf::RenderWindow window;

		//Le nom de l'icone du jeu (on peut changer l'icone du jeu)
		string iconName;

		//Le nombre de joueurs dans la partie
		int nbPlayers;

		//Tableau qui garde en memoire quels controles les joueurs ont choisis
		//input: 0=keyboard, 1=mouse, 2=gamepad
		int input[2];
		//La tableau qui correspond aux touches que les joueurs peuvent utiliser si ils
		//ont choisis d'utiliser le clavier
		sf::Keyboard::Key inputTab[2][5];
		//Le tableau qui stocke a quel gamepad le joueur correspond et -1 si aucun gamepad
		int gamepadId[2] = {-1,-1};

		//Indique si le jeu est en reseau
		//Le reaseau n'est pas implemente
		bool network;

		//Indique le nom de la musique du jeu
		string musicName;
		//Indique si la musique est jouee actuellement
		bool musicEnabled;
		//Indique le volume de la musique
		float volume;

		//Indique la sensibilite des controles souris
		int mouseSensitivity;

		//Indique si le jeu est en pause ou non
		bool isPlaying;

		//La tableau de vaisseaux
		Ship ships[2];
		//Le tableau d'asteroides
		Asteroid asteroids[25];
		//Indique le nombre maximal d'asteroides pouvant etre present sur la fenetre de
		//jeu en meme temps
		int nbMaxAsteroid=25;
		//Indique le nombre d'asteroides actuellement present sur la fenetre de jeu
		int nbAsteroid;

		//Indique le laps de temps necessaire entre deux apparitions d'asteroides
		float spawnRate;
		//Sert pour calculer le laps de temps entre deux apparitions d'asteroides
		sf::Clock spawnClock;

		//music sert a jouer la musique du jeu
		sf::Music music;
		//sound sert a lancer les differents SoundBuffer
		sf::Sound sound;
		//asteroidBuffer correspond a l'effet sonore produit quand un asteroide est detruit
		//par une munition
		sf::SoundBuffer asteroidBuffer;
		//shipBuffer correspond a l'effet sonore produit quand un vaisseau est detruit
		//par un asteroide
		sf::SoundBuffer shipBuffer;

		//Tableau correspondant a l'affichage des points de vie des vaisseaux sur la
		//fenetre de jeu
		Ship lifeShip[2][3];

		//La texte informatif de l'etat du jeu
		sf::Text middleText;
		//L'horloge qui est utilise pour modifie le texte informatif lorsque son etat
		//change (lorsque la partie se lance par exemple)
		sf::Clock startingClock;

		//Le tableau de texte permettant d'afficher le score des joueurs
		sf::Text playerText[2];

		//indique si le jeu n'est pas fini
		bool gameNotOver;
		//Indique si un des joueurs a demande de rejoue la partie une fois cette derniere
		//terminee
		bool restart;


	public:
		//Unique constructeur de la classe World qui va lancer les fonctions qui vont
		//lancer toutes les autres fonctions
		World();

		//Fonction qui initie le jeu
		void init();
		//Fonction qui initie la configuration du jeu
		void initConfig();
		//Fonction qui initie l'icone du jeu
		void initIcon();

		//Fonction qui initie les controles du jeu
		void initInput();
		//Fonction qui initie les controles clavier du jeu
		void initKeyboardInput();
		//Fonction qui initie les controles souris du jeu
		void initMouseInput();
		//Fonction qui initie les controles gamepad du jeu
		void initGamepadInput();

		//Fonc
		void initShip();
		void initShipHp();
		void initShipPosition();

		//Fonction qui initialise les SoundBuffer du jeu
		void initSoundBuffer();
		void initMusic();
		void initAsteroids();

		void initInGame(); //Initialise les éléments qui ne peuvent être initialisés que pendant que la fenêtre est active

		void start();

		//Fonction qui cree un nouvel asteroide si cela est possible
		void createAsteroid(int x, int y);
		//Fonction qui indique de quel cote de l'ecran l'asteroide doit apparaitre
		void spawnAsteroid();

		//Fonction qui gere les controles la boucle d'evenement
		void handlePlayerInput(int playerId);
		//Fonction qui gere les controles clavier dans la boucle d'evenement
		void handleKeyboardInput(int playerId);
		//Fonction qui gere les controles souris dans la boucle d'evenement
		void handleMouseInput(int playerId);
		//Fonction qui gere les controles gamepad dans la boucle d'evenement
		void handleGamepadInput(int playerId);

		//Fonction qui gere les collisions du jeu
		void handleCollision();
		//Fonction qui gere les collisions entre le vaisseau et l'asteroide
		void handleCollision(Ship &ship, Asteroid &asteroid);
		bool handleCollision(Bullet &bullet, Asteroid &asteroid);

		void moveEntities();

		void timeLaps();
		void draw();
		void display();


};

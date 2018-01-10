#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#define M_PI  3.14159265358979323846

//C'est la classe Ship qui va gerer les instances de la classe Bullet
#include "Bullet.hpp"

//Description de la classe Ship
//Cette classe correspond aux vaisseaux que piloteront les joueurs
//Elle etend la classe sf::ConvexShape pour donner une forme plus 
//triangulaire et fin au vaisseau
class Ship : public sf::ConvexShape
{

	public:
		//Le vecteur indiquant la position de la tourelle
		//C'est de cette position que les instances de la classe Bullet partiront
		sf::Vector2f turretPosition;
		//La rotation actuelle du vaisseau
		float rotation;

		//La velocite a laquelle le vaisseau se deplace
		float velocity;
		//La velocite maximale a laquelle le vaisseau peut se deplacer
		float maxVelocity;
		//Indique si le vaisseau a change de velocite dans cette occurence de boucle
		//Sert pour activer l'inertie avec les controles clavier
		bool velocityChanged;

		//Le tableau de Bullet correspondant aux munitions que le vaisseau peut tirer
		Bullet bullets[25];
		//Indique le nombre maximal de Bullet que le vaisseau peut tirer
		//Il ne pourra jamais depasser ce nombre
		//Le cas echeant les premieres instances de Bullet seraient remplaces par
		//de nouvelles
		int nbMaxBullet;
		//Indique la position de la prochaine instance de Bullet dans le tableau bullets
		int indexBullet;

		//Le vecteur indiquant la dimension de la fenetre de jeu
		sf::Vector2u windowSize;
		//Si le Ship existe actuellement dans le jeu
		bool exist;

		//Indique le laps de temps necessaire entre deux tirs
		float fireRate;
		//Sert pour calculer le laps de temps entre deux tirs
		sf::Clock shipClock;

		//sound et bulletBuffer servent pour lancer l'effet sonore associe
		//au tir de munition par le vaisseau (l'instanciation de Bullet par Ship)
		sf::Sound sound;
		sf::SoundBuffer bulletBuffer;

		//Indique le nombre de points de vie du vaisseau
		int hp;
		//Indique le score du vaisseau
		int score;


		//Constructeur de la classe Ship avec dans l'ordre sa couleur et la dimension de
		//la fenetre de jeu
		Ship(sf::Color color, sf::Vector2u wdSize);
		//Constructeur par recopie servant uniquement pour creer instances de Ship qui
		//seront utilises pour afficher les points de vie de l'instance Ship copiee
		Ship(Ship &ship);
		//Constructeur vide servant a l'initialisation des tableaux de Ship
		Ship();

		//Fonction qui retourne le radius du vaisseau
		//Un convexShape n'est pas sense avoir de radius
		//mais etant donne que le calcul de collision le
		//plus rapide se fait avec des spheres toutes les
		//instances des elements du jeu sont geres comme
		//des sphere pour les collisions
		float Ship::getRadius();
		//Fonction qui change l'orientation du vaisseau pour le clavier
		void addRotation(float rotate);
		//Fonction qui change l'orientation du vaisseau pour la souris et les gamepads
		void setRotationGamepad(float rotate);
		//Fonction qui retourne la direction actuelle vers laquelle se dirige le Ship
		sf::Vector2f setDirection();
		//Fonction qui change la position de la tourelle du vaisseau pour qu'elle soit
		//toujours a la bonne position
		void setTurretPosition();

		//Fonction qui va placer les vaisseaux sur la fenetre de jeu
		//Si number=0 c'est le mode 1 joueur, number=1 ou number=2 c'est le mode 2 joueurs
		void startingPosition(int number);

		//Fonction qui modifie la velocite du vaisseau avec les controles clavier
		void modifyVelocity(float speed);
		//Fonction qui modifie la velocite du vaisseau avec les controles souris et
		//gamepad
		void modifyVelocityGamepad(float speed);
		//Fonction qui gere l'inertie du vaisseau
		void fixVelocity();
		//Fonction qui va bouger le vaisseau sur la fenetre de jeu
		void moveShip();
		//Comme le monde est circulaire (si on sort de l'ecran par un des cotes on revient 
		//par le cote oppose a ce dernier) cette fonction a pour but de ramener les 
		//vaisseaux qui seraient sorti de l'ecran pour les ramener du cote oppose
		void getBackOnTheScreen();

		//Fonction qui cree une nouvelle instance de la classe Bullet si les prerequis
		//sont atteint (si le laps de temps entre deux tirs est respecte)
		void createBullet();





};

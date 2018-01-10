#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#define M_PI 3.14159265358979323846

//Description de la classe Bullet
//Cette classe correspond aux munitions que les vaisseaux tirent
//Elle etend la classe sf::CircleShape, de ce fait sa forme est circulaire
class Bullet : public sf::CircleShape
{

	public:
		//Le vecteur indiquant la direction vers laquelle se dirige la Bullet
		sf::Vector2f direction;
		//La vitesse à laquelle la Bullet se déplace
		float velocity;
		//Si la Bullet existe actuellement dans le jeu
		bool exist;
		//Variable indiquant le temps qu'il reste à la Bullet pour faire ses actions avant de disparaitre
		int lifespan;

		//Le vecteur indiquant la dimension de la fenetre de jeu
		sf::Vector2u windowSize;

		//Constructeur de la classe Bullet avec dans l'ordre: sa direction, sa position, sa couleur et la dimension de la fenetre de jeu
		Bullet(sf::Vector2f vect, sf::Vector2f position, sf::Color color, sf::Vector2u windowSize);
		//Constructeur vide servant a l'initialisation des tableaux de Bullet
		Bullet();

		//Fonction principale (et unique fonction appelee en dehors de la classe) de la classe Bullet
		//Cette fonction deplace l'instance de la classe Bullet
		void moveBullet();

		//Comme le monde est circulaire (si on sort de l'ecran par un des cotes on revient par le cote oppose a ce dernier) cette fonction a pour but de ramener
		//Les instances de Bullet qui seraient sorti de l'ecran pour les ramener du cote oppose
		void getBackOnTheScreen();





};

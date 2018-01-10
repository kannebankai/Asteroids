#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#define M_PI  3.14159265358979323846

//Description de la classe Asteroid
//Cette classe correspond aux asteroides que les vaisseaux devront esquiver et abattre
//Elle etend la classe sf::CircleShape, de ce fait sa forme est pseudo circulaire
class Asteroid : public sf::CircleShape
{

	public:
		//La rotation actuelle de l'asteroide
		float rotation;
		//La velocite a laquelle l'asteroide se deplace
		float velocity;
		//La valeur qui indique la vitesse actuelle de la rotation
		float theta;

		//Le vecteur indiquant la dimension de la fenetre de jeu
		sf::Vector2u windowSize;
		//Le vecteur indiquant la direction vers laquelle se dirige l'asteroide
		sf::Vector2f direction;
		//Si l'Asteroid existe actuellement dans le jeu
		bool exist;

		//Indique le nombre de points de vie de l'asteroide
		int hp;
		//Indique la valeur de l'asteroide
		int value;

		//Constructeur vide servant a l'initialisation du tableau d'asteroide
		Asteroid();
		//Constructeur de la classe Asteroid avec dans l'ordre sa couleur et la dimension 
		//de la fenetre de jeu, ses coordonnees de position x et y et sa direction
		Asteroid(sf::Vector2u wdSize, int x, int y, sf::Vector2f direction);

		//Fonction qui ajoute de la rotation a l'asteroide
		void addRotation();
		//Fonction qui modifie les valeurs de l'asteroide pour le faire devenir plus petit
		//Cette fonction est utilisee pour creer les petits asteroides quand un grand
		//asteroide est detruit
		void littleAsteroid(sf::Vector2f bulletDir, float theta);

		//Fonction qui va bouger l'asteroide sur la fenetre de jeu
		void moveAsteroid();
		//Comme le monde est circulaire (si on sort de l'ecran par un des cotes on revient 
		//par le cote oppose a ce dernier) cette fonction a pour but de ramener les
		//asteroides qui seraient sorti de l'ecran pour les ramener du cote oppose
		void getBackOnTheScreen();





};

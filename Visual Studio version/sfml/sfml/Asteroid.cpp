#include "Asteroid.hpp"
#include <iostream>
#define ASTEROID_RADIUS 15
using namespace std;


//Constructeur vide de la classe Ship
//Ce contructeur sert a initialiser le tableau d'Asteroid
//Vu que chaque instance d'Asteroid effective dans le jeu sera cree par le constructeur 
//principal il faut juste preciser ici que les Asteroid cree par ce constructeur n'existent 
//pas dans le jeu
Asteroid::Asteroid()
{
	//Indique que l'instance d'Asteroid n'existe pas dans le jeu
	exist=false;
}

//Constructeur principal de la classe Asteroid
//C'est ce constructeur qui est appele lorsque les Asteroid existent dans le jeu
Asteroid::Asteroid(sf::Vector2u wdSize, int x, int y, sf::Vector2f dir)
{
	//Indique que l'instance d'Asteroid existe dans le jeu
	exist=true;
	//Modifie le nombre de points servant pour definir la partie graphique de l'asteroide
	//Ce nombre permet de donner une structure moins lisse a l'asteroide
	setPointCount(7);
	//Modifie la radius de Bullet
	setRadius(ASTEROID_RADIUS);

	
	//Indique la couleur de l'Asteroid
	//Les asteroides ont une taille plus grande que celle de leur hitbox pour permettre 
	//d'etre plus permissif avec le joueur
	setFillColor(sf::Color::White);
	setOutlineThickness(3);

	//Modifie la position de l'asteroide
	setPosition(x,y);
	//Modifie l'origine de l'asteroide
	setOrigin(15,15);

	//Indique la rotation initiale de l'asteroide
	rotation=M_PI/2;
	//Indique la velocite de l'asteroide
	velocity=3;

	//Indique le facteur de rotation de l'asteroide
	theta = 5;

	//Indique les dimensions de la fenetre de jeu
	windowSize = wdSize;
	//Indique la direction de l'asteroide
	direction = dir;

	//Indique les points de vie de l'asteroide
	hp = 2;
	//Indique la valeur de l'asteroide
	value = 10;
}

//Fonction qui effectue la rotation de l'asteroide
void Asteroid::addRotation()
{
	//Modifie la rotation de l'asteroide pour les calculs
	rotation += M_PI/(180/theta);
	//Effectue la rotation de l'asteroide
	rotate(theta);
}

//Fonction qui permet de creer de petits asteroides
void Asteroid::littleAsteroid(sf::Vector2f bulletDir, float theta)
{
	//Modifie le vecteur direction pour refleter sa nouvelle direction
	direction.x = bulletDir.x*cos(theta) - bulletDir.y*sin(theta);
	direction.y = bulletDir.x*sin(theta) + bulletDir.y*cos(theta);

	//Modifie le radius de l'asteroide
	setRadius(ASTEROID_RADIUS/2);
	//Modifie l'origine de l'asteroide
	setOrigin(ASTEROID_RADIUS / 2, ASTEROID_RADIUS / 2);

	//Modifie les points de vie de l'asteroide
	hp = 1;
	//Modifie la valeur de l'asteroide
	value = 20;
	//Modifie la vitesse de l'asteroide
	velocity*=2;
}

//Fonction qui va deplacer l'instance de l'Asteroid
void Asteroid::moveAsteroid()
{
	//Deplace l'asteroide dans la fenetre de jeu dans la direction "direction" multiplie 
	//par sa velocity "velocity"
	move(velocity*direction);
	//Lance la rotation de l'asteroide
	addRotation();
	//On remet l'asteroide sur la fenetre de jeu si ce dernier n'y etait plus
	getBackOnTheScreen();
}

//Dans ce jeu, le monde est circulaire. Si l'on s'eloigne trop d'un cote de l'ecran on 
//doit reapparaitre du cote oppose
//Cette fonction a pour but de faire de la fenetre de jeu un monde circulaire
void Asteroid::getBackOnTheScreen()
{
	//Le vecteur position correspond a la position actuelle de l'asteroide sur l'ecran
	sf::Vector2f position=getPosition();
	//Pour chaque coordonnee (x et y)
	//Si la coordonnee se situe en dehors de l'ecran elle est modifie pour correspondre a son oppose
	//Cependant les test ne sont pas fait sur les extremites de la fenetre mais sur les extremites +/- 20
	//Pour ne pas avoir d'effet "je me teleporte instantanement" des que l'on touche un des bords de la fenetre de jeu
	if(position.x < -20)
		position.x=windowSize.x;
	else if(position.x > windowSize.x)
		position.x=0;
	if(position.y < -20)
		position.y=windowSize.y;
	else if(position.y > windowSize.y)
		position.y=0;

	//On modifie la position de l'asteroide
	setPosition(position);
}


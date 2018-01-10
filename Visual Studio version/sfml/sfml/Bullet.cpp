#include "Bullet.hpp"
using namespace std;

//Constructeur principal de la classe Bullet
//C'est ce constructeur qui est appele lorsque les Bullet existent dans le jeu
Bullet::Bullet(sf::Vector2f vect, sf::Vector2f position, sf::Color color, sf::Vector2u wdSize)
{
	//Modifie la radius de Bullet
	setRadius(5.f);
	//Modifie la couleur de Bullet
	setFillColor(color);
	//Modifie le vecteur position pour qu'il correspond bien a l'endroit ou se trouve le canon sur le vaisseau
	position.x+=5;
	position.y-=5;
	setPosition(position);

	//Donne la direction que va suivre Bullet à chaque mouvement
	direction=vect;
	//Donne la velocite
	velocity = 10;
	//Indique que l'instance de Bullet existe dans le jeu
	exist = true;
	//Donne le temps de vie de chaque Bullet
	lifespan=100;
	
	//Donne le vecteur representant la dimension de la fenetre de jeu
	windowSize=wdSize;
}

//Constructeur vide de la classe Bullet
//Ce contructeur sert a initialiser les tableaux de Bullet de chaque vaisseau
//Vu que chaque instance de Bullet effective dans le jeu sera cree par le constructeur principal
//il faut juste preciser ici que les Bullet cree par ce constructeur n'existent pas dans le jeu
Bullet::Bullet()
{
	//Indique que l'instance de Bullet n'existe pas dans le jeu
	exist=false;
	//Une deuxieme securite pour etre sur que les instances de Bullet cree par ce constructeur n'existeront jamais dans le jeu
	lifespan=0;
}

//Fonction principale de la classe Bullet
//Cette fonction va deplacer l'instance de Bullet dans selon la direction et la velocity de ses attributs
void Bullet::moveBullet()
{
	//Deplace l'instance de Bullet dans la fenetre de jeu dans la direction (direction) multiplie par sa velocite (velocity)
	move(velocity*direction);
	//On decremente lifespan a chaque deplacement de l'instance de Bullet
	lifespan--;
	//Teste si lifespan est egal a 0, si oui l'instance de la classe Bullet n'existe plus et son attribut exist prend donc la valeur false
	if(!lifespan) exist=false;
	//Fonction qui permet de remettre l'instance de Bullet dans la fenetre de jeu
	//Voir getBackOnScreen
	getBackOnTheScreen();
}

//Deuxieme et derniere fonction de la classe
//Dans ce jeu, le monde est circulaire. Si l'on s'eloigne trop d'un cote de l'ecran on doit reapparaitre du cote oppose
//Cette fonction a pour but de faire de la fenetre de jeu un monde circulaire
void Bullet::getBackOnTheScreen()
{
	//Le vecteur position correspond a la position actuelle de l'instance de Bullet sur l'ecran
	sf::Vector2f position=getPosition();
	//Pour chaque coordonnee (x et y)
	//Si la coordonnee se situe en dehors de l'ecran elle est modifie pour correspondre a son oppose
	//Cependant les test ne sont pas fait sur les extremites de la fenetre mais sur les extremites +/- 20
	//Pour ne pas avoir d'effet "je me teleporte instantanement" des que l'on touche un des bords de la fenetre de jeu
	if(position.x < -20)
		position.x=windowSize.x;
	else if(position.x > windowSize.x+20)
		position.x=0;
	if(position.y < -20)
		position.y=windowSize.y;
	else if(position.y > windowSize.y+20)
		position.y=0;
	
	//On modifie la position de l'instance de Bullet 
	setPosition(position);
}

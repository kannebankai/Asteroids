#include "Ship.hpp"
using namespace std;

//Constructeur principal de la classe Ship
//C'est ce constructeur qui est appele lorsque les Ship existent dans le jeu
Ship::Ship(sf::Color color, sf::Vector2u wdSize)
{
	//Indique que l'instance de Ship existe dans le jeu
	exist=true;

	//Indique que notre convexShape est constitue de trois points
	setPointCount(3);
	//Indique la position de chaque point pour creer le vaisseau (les vaisseaux sont des
	//triangles)
	setPoint(0, sf::Vector2f(10,0));
	setPoint(1, sf::Vector2f(0,25));
	setPoint(2, sf::Vector2f(20,25));
	//Modifie l'origine du vaisseau
	setOrigin(10,12.5);

	//Indique la couleur de Ship
	//Les vaisseaux n'ont qu'une couleur exterieure, ils sont noir a l'interieur (la 
	//couleur de la fenetre de jeu) et sont de la couleur "color" a l'exterieur
	//Cependant pour ne pas fausser les calculs de collisions la couleur exterieur part
	//vers l'interieur (d'ou l'absence de couleur interieure)
	setOutlineColor(color);
	setOutlineThickness(-3);

	//Indique que la rotation initiale du vaisseau
	rotation=M_PI/2;
	//Indique la velocite initiale du vaisseau
	velocity=0;
	//Indique la velocite maximale du vaisseau
	maxVelocity=5;
	//Indique la velocite maximale a laquelle le vaisseau peut aller
	velocityChanged = false;

	//Initialise l'index du tableau de Bullet
	indexBullet =0;
	//Indique le nombre maximale de Bullet qui peuvent exister en meme temps
	nbMaxBullet = 25;
	//Initialise le tableau de Bullet avec le constructeur vide Bullet()
	for(int i=0;i<nbMaxBullet;i++)bullets[i] = Bullet();

	//Indique les dimensions de la fenetre de jeu
	windowSize=wdSize;
	//Indique le temps (en seconde) necessaire entre chaque tir
	fireRate = 0.25;
	//Initialise l'horloge du laps de temps entre deux tirs
	shipClock.restart();

	//Charge le fichier audio correspondant a l'effet sonore produit lorsque le vaisseau
	//tire
	bulletBuffer.loadFromFile("bullet.wav");

	//Initialise les points de vie du vaisseau
	hp = 3;
	//Initialise le score du vaisseau
	score = 0;
}

//Constructeur par recopie de la classe Ship utilise uniquement pour instancier les 
//vaisseaux servant a l'affichage des points de vie des vaisseaux
Ship::Ship(Ship &ship)
{
	//Indique que l'instance de Ship existe dans le jeu
	exist = true;

	//Indique que notre convexShape est constitue de trois points
	setPointCount(3);
	//Indique la position de chaque point pour creer le vaisseau (les vaisseaux sont des
	//triangles)
	setPoint(0, sf::Vector2f(10, 0));
	setPoint(1, sf::Vector2f(0, 25));
	setPoint(2, sf::Vector2f(20, 25));
	setOrigin(10, 12.5);

	//Indique la couleur de Ship
	//Les vaisseaux n'ont qu'une couleur exterieure, ils sont noir a l'interieur (la 
	//couleur de la fenetre de jeu) et sont de la couleur "color" a l'exterieur
	//Cependant pour ne pas fausser les calculs de collisions la couleur exterieur part
	//vers l'interieur (d'ou l'absence de couleur interieure)
	setOutlineColor(ship.getOutlineColor());
	setOutlineThickness(ship.getOutlineThickness());
}

//Constructeur vide de la classe Ship
//Ce contructeur sert a initialiser les tableaux de Ship
//Vu que chaque instance de Ship effective dans le jeu sera cree par le constructeur 
//principal il faut juste preciser ici que les Ship cree par ce constructeur n'existent 
//pas dans le jeu

Ship::Ship()
{
	//Indique que l'instance de Ship n'existe pas dans le jeu
	exist=false;
}

//Fonction qui retourne le radius du vaisseau
//C'est une valeur arbitraire decidee apres moults tests
float Ship::getRadius()
{
	return 10;
}

//Fonction qui change l'orientation du vaisseau pour les controles claviers
void Ship::addRotation(float theta)
{
	//Modifie la rotation du vaisseau pour les calculs de direction
	rotation += M_PI/(180/theta);
	//Modifie la rotation du vaisseau sur la fenetre de jeu
	rotate(theta);
}

//Fonction qui change l'orientation du vaisseau pour les controles souris et gamepad
void Ship::setRotationGamepad(float theta)
{
	//Modifie la rotation du vaisseau pour les calculs de direction
	rotation = theta+M_PI/2;
	//Remplace la rotation du vaisseau sur la fenetre de jeu
	//Car les controles sont analogiques et ne peuvent donc pas juste modifier la rotation
	//par rapport a la rotation actuelle
	setRotation(((theta / M_PI) * 180));
}

//Fonction qui retourne la direction vers laquelle le vaisseau pointe
sf::Vector2f Ship::setDirection()
{
	//Initialise en vecteur direction
	sf::Vector2f direction;
	//La valeur des coordonnees du vecteur direction est calculee par cosinus et sinus
	direction.x = -cos(rotation);
	direction.y = -sin(rotation);

	//Retourne le vecteur direction
	return direction;
}

//Fonction qui modifie la position de la tourelle pour qu'elle soit en adequation avec sa
//representation graphique
void Ship::setTurretPosition()
{
	//Le vecteur turretPosition prend la valeur du vecteur position du vaisseau
	turretPosition = getPosition();
	//Modifie le vecteur turretPosition nouvellement affecte
	turretPosition.x-=10;
	turretPosition.y-=0;
}

//Fonction qui va placer les vaisseaux sur la fenetre de jeu
void Ship::startingPosition(int number)
{
	//Si c'est le mode 1 joueur le vaisseau est place au centre de la fentre de jeu, en
	//etant legerement plus bas pour ne pas gener l'affichage
	if (!number)
	{
		setPosition(windowSize.x / 2, windowSize.y / 2 + 50);
	}
	//Si c'est le mode deux joueur et que c'est le joueur 1
	else if (number == 1)
	{
		setPosition(200, windowSize.y / 2 + 50);
	}
	//Si c'est le mode deux joueur et que c'est le joueur 2
	else if (number == 2)
	{
		setPosition(windowSize.x-200, windowSize.y / 2 + 50);
	}
}

//Fonction qui modifie la velocite du vaisseau avec les controles clavier
void Ship::modifyVelocity(float speed)
{
	//Le joueur a appuye sur le touche pour reculer
	if(speed < 0 && velocity > -maxVelocity) velocity+=speed;
	//Le joueur a appuye sur la touche pour avancer
	if(speed > 0 && velocity < maxVelocity) velocity+=speed;
	//La velocite a change
	velocityChanged = true;

}

//Fonction qui modifie la velocite du vaisseau avec les controles souris et gamepad
void Ship::modifyVelocityGamepad(float speed)
{
	//Initie tmpVelocity
	// (100/maxVelocity) permet d'empeche que tmpVelocity ne soit trop eleve pour les 
	//tests
	float tmpVelocity = speed / (100 / maxVelocity);
	//Modifie tmpVelocity pour ne jamais depasser maxVelocity
	if (tmpVelocity > maxVelocity)
		tmpVelocity = maxVelocity;

	//Si la velocite n'est pas assez importante pour se deplacer
	if (velocity < tmpVelocity - 0.0001 && velocity > tmpVelocity + 0.0001)
		velocity = 0;
	//Le vecteur velocity prend la valeur tmpVelocity
	else
		velocity = tmpVelocity;
	//la velocite a change
	velocityChanged = true;

}

//Fonction qui applique l'inertie au vaisseau en cas de controle clavier
void Ship::fixVelocity()
{
	//Si le vaisseau avance on le fait ralentir en augmentant sa vitesse
	if(velocity > 0)velocity-=0.1;
	//Si le vaisseau recule on le fait ralentir en diminuant sa vitesse
	if(velocity < 0)velocity+=0.1;
	//Si le vaisseau a une vitesse quasi nulle elle devient nulle
	if(velocity > -0.1 && velocity < 0.1)velocity=0;
}

//Fonction qui va deplacer l'instance de Ship
void Ship::moveShip()
{
	//Deplace le vaisseau dans la fenetre de jeu dans la direction retournee par 
	//"setDirection()" multiplie par sa velocity "velocity"
	move(velocity*setDirection());
	//Si la velocite du vaisseau n'a pas change ce cycle-ci on applique l'inertie
	if(!velocityChanged)
	{
		fixVelocity();
	}
	//On remet velocityChanged a false si jamais il ete a true
	velocityChanged=false;
	//On remet la vaisseau sur la fenetre de jeu si ce dernier n'y etait plus
	getBackOnTheScreen();
}

//Dans ce jeu, le monde est circulaire. Si l'on s'eloigne trop d'un cote de l'ecran on 
//doit reapparaitre du cote oppose
//Cette fonction a pour but de faire de la fenetre de jeu un monde circulaire
void Ship::getBackOnTheScreen()
{
	//Le vecteur position correspond a la position actuelle du vaisseau sur l'ecran
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
	
	//On modifie la position du vaisseau
	setPosition(position);
}

//Fonction qui va permettre au vaisseau de tirer si les conditions sont remplis
void Ship::createBullet()
{
	//Teste si le laps de temps entre le dernier tir est superieur a la cadence de tir
	//du vaisseau
	if (shipClock.getElapsedTime().asSeconds() > fireRate)
	{
		//On appelle la fonction permettant de remettre la tourelle a sa bonne position
		setTurretPosition();
		//On initialise une instance de Bullet avec la bonne couleur, position et direction
		bullets[indexBullet] = Bullet(setDirection(), turretPosition, getOutlineColor(), windowSize);

		//On lance le son de tir
		sound.setBuffer(bulletBuffer);
		sound.play();

		//On met à jour notre tableau de Bullet
		indexBullet++;
		indexBullet == nbMaxBullet ? indexBullet = 0 : indexBullet;
		//On redemarre l'horloge qui calcule le laps de temps entre deux tirs
		shipClock.restart();
	}
}

#include<iostream>

using namespace std;
/* " " eau,  
"O" tire louper, 
"X" tire toucher, 
"A" porte Avion(5 case), 
"C" Croiseur(4 cases),
"S" sous marin x2(3 cases),
"T" torpilleur(2 cases)*/
char plateau_joueur1[10][10];
char plateau_joueur2[10][10]; 

char bateau[] = { 'T','S','C','A'};

void init_plateau_vide() {
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			plateau_joueur1[i][j] = ' ';
			plateau_joueur2[i][j] = ' ';
		}
	}
}


bool verifie_emplacement_disponible(char plateau_a_verif[][10], int x, int y, int taille, int o) {
	for (int i = 0; i < taille+3; i++)
	{
		if (o==0)
		{
			if (plateau_a_verif[y][x+i]!=' ')
			{
				return false;
			}
		}
		else {
			if (plateau_a_verif[y+i][x] != ' ')
			{
				return false;
			}
		}
	}
	return true;
}


void init_plateau() {
	
	init_plateau_vide();
	int x,y;
	for (int j = 3; j >= 0; j--)
	{
		int o = rand() % 2; // si o==0 alors le bateau sera horizontal si o==1 alors vertical
		if (o == 0) // horizontal
		{
			x = rand() % (10-(j+1));
			y = rand() % 10;
			while (verifie_emplacement_disponible(plateau_joueur1, x, y, j, o)==false)
			{
				x = rand() % (10 - (j + 1));
				y = rand() % 10;
			}
			for (int i = 0; i < j+2; i++)
			{
				plateau_joueur1[y][x + i] = bateau[j];
			}
			if (j == 1)
			{
				x = rand() % 10;
				y = rand() % (10 - (j + 1));
				while (verifie_emplacement_disponible(plateau_joueur1, x, y, j, o) == false)
				{
					x = rand() % 10;
					y = rand() % (10 - (j + 1));
				}
				for (int i = 0; i < 3; i++) {
					plateau_joueur1[y+i][x] = bateau[j];
				}
			}
		}
		else { // vertical
			x = rand() % 10;
			y = rand() % (10-(j+1));
			while (verifie_emplacement_disponible(plateau_joueur2, x, y, j, o) == false)
			{
				x = rand() % 10;
				y = rand() % (10 - (j + 1));
			}
			for (int i = 0; i < j+2; i++)
			{
				plateau_joueur1[y + i][x] = bateau[j];
			}
			if (j == 1)
			{
				x = rand() % (10 - (j + 1));
				y = rand() % 10;
				while (verifie_emplacement_disponible(plateau_joueur2, x, y, j, o) == false)
				{
					x = rand() % (10 - (j + 1));
					y = rand() % 10;
				}
				for (int i = 0; i < 3; i++) {
					plateau_joueur1[y][x + i] = bateau[j];
				}
			}
		}
	}
	for (int j = 3; j >= 0; j--)
	{
		int o = rand() % 2;
		if (o == 0) // horizontal
		{
			x = rand() % (10 - (j + 1));
			y = rand() % 10;
			for (int i = 0; i < j + 2; i++)
			{
				plateau_joueur2[y][x + i] = bateau[j];
			}
			if (j == 1)
			{
				x = rand() % 10;
				y = rand() % (10 - (j + 1));
				for (int i = 0; i < 3; i++) {
					plateau_joueur2[y + i][x] = bateau[j];
				}
			}
		}
		else { // vertical
			x = rand() % 10;
			y = rand() % (10 - (j + 1));
			for (int i = 0; i < j + 2; i++)
			{
				plateau_joueur2[y + i][x] = bateau[j];
			}
			if (j == 1)
			{
				x = rand() % (10 - (j + 1));
				y = rand() % 10;
				for (int i = 0; i < 3; i++) {
					plateau_joueur2[y][x + i] = bateau[j];
				}
			}
		}
	}
}


void affiche() {
	cout << "joueur1" << endl;
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			cout << plateau_joueur1[i][j];
		}
		cout << endl;
	}
	cout << "joueur2" << endl;
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			cout << plateau_joueur2[i][j];
		}
		cout << endl;
	}
}

int main() {
	srand(time(NULL));
	init_plateau();

	affiche();
	
}

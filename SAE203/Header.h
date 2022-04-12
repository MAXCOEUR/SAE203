#define A0 26
#define A1 25
#define A2 34
#define A3 39
#define A4 36
#define A5 4

#define D2 14
#define D3 32
#define D4 15
#define D5 14

#define RX RX
#define TX TX

#define SCL 22
#define SDA 23
#define BB 32
#define BC 14

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void rectangle_plein(int x_start, int y_start, int width, int height);
void init_plateau_vide();
void affiche_tableau(int joueur);
bool verifie_emplacement_disponible(char plateau_a_verif[][10], int x, int y, int taille, int o);
void init_plateau();
void demande_coordonne(int& x, int& y,int j);
bool tire(char tab_joueur_qui_recoit[10][10],int collone, int ligne);
void attente_joueur(int j1);
void joue_IA(char tab_joueur_adverse[10][10]);
void jeux_1v1();
void jeux_1vIA();
void dessine_plateau_vide();
void dessine_croix(int x_start, int y_start, int width, int height);
int affiche_menu_local();
int affiche_menu();
void menu_local();
void menu();
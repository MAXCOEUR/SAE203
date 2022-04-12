#include<Adafruit_GFX.h>
#include<Adafruit_SH110X.h>;
#include <ChainableLED.h>
hw_timer_t * timer = NULL;


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

ChainableLED leds(D2,D3,1); //LED

char plateau_joueur1[10][10];
char plateau_joueur2[10][10]; 
  
char bateau[] = { 'T','S','C','A'};

bool valide = false;

unsigned long last=0;
unsigned long last_deplacement=0;
unsigned long attente=0;

void IRAM_ATTR tmp(){
  if(millis()-last>200){
    valide = !valide;
    last=millis();
  }
}

Adafruit_SH1107 display = Adafruit_SH1107(64,128, &Wire);
void setup() {
  Serial.begin(115000);
  display.begin(0x3c, true);
  display.clearDisplay();
  display.setRotation(1);
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  /* " " eau,  
  "O" tire louper, 
  "X" tire toucher, 
  "A" porte Avion(5 case), 
  "C" Croiseur(4 cases),
  "S" sous marin x2(3 cases),
  "T" torpilleur(2 cases)*/
  pinMode(D4, INPUT_PULLUP); // 
  pinMode(A0, INPUT); // joystique
  pinMode(A1, INPUT); // joystique
  attachInterrupt (D4, tmp, FALLING);
  leds.init();

  randomSeed(analogRead(0));
}

void rectangle_plein(int x_start, int y_start, int width, int height){
  for (int i = 0; i<height;i++){
    display.drawLine(x_start,y_start+i,x_start+width-1,y_start+i,SH110X_WHITE);
  }
}

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

void affiche_tableau(int joueur){
  dessine_plateau_vide();
  if (joueur == 1){
    for (int i=0; i<10; i++){
      for (int j=0;j<10; j++){
        if (plateau_joueur1[i][j]!=' '){
          if (plateau_joueur1[i][j]=='X'){
          dessine_croix(i*6,j*6,6,6);
          }
          else{
            if(plateau_joueur1[i][j]=='O'){
              rectangle_plein(i*6+2,j*6+2,3,3);
            }
            else{
              rectangle_plein(i*6,j*6,6,6);
            }
          }
        }

        if (plateau_joueur2[i][j]=='X'){
          dessine_croix(i*6+67,j*6,6,6);
        }
        else{
          if(plateau_joueur2[i][j]=='O'){
              rectangle_plein(i*6+69,j*6+2,3,3);
           }
        }
        
      }
    }
  }
  else{
    for (int i=0; i<10; i++){
      for (int j=0;j<10; j++){
        if (plateau_joueur2[i][j]!=' '){
          if (plateau_joueur2[i][j]=='X'){
          dessine_croix(i*6,j*6,6,6);
          }
          else{
            if(plateau_joueur2[i][j]=='O'){
              rectangle_plein(i*6+2,j*6+2,3,3);
            }
            else{
              rectangle_plein(i*6,j*6,6,6);
            }
          }
        }
        if (plateau_joueur1[i][j]=='X'){
          dessine_croix(i*6+67,j*6,6,6);
        }
        else{
          if(plateau_joueur1[i][j]=='O'){
              rectangle_plein(i*6+69,j*6+2,3,3);
           }
        }
      }
    }
  }
}


bool verifie_emplacement_disponible(char plateau_a_verif[][10], int x, int y, int taille, int o) {
  for (int i = 0; i < taille+2; i++)
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

void affiche(int joueur=0) {
  if (joueur==1)
  {
    Serial.println("joueur1");
    Serial.println( " |0|1|2|3|4|5|6|7|8|9|");
    for (size_t i = 0; i < 10; i++)
    {
      Serial.print(i);
      Serial.print("|");
      for (size_t j = 0; j < 10; j++)
      {
        Serial.print(plateau_joueur1[i][j]);
        Serial.print("|");
      }
      Serial.println();
    }
  }
  else if(joueur==2){
    Serial.println("joueur2");
    Serial.println( " |0|1|2|3|4|5|6|7|8|9|");
    for (size_t i = 0; i < 10; i++)
    {
      Serial.print(i);
      Serial.print("|");
      for (size_t j = 0; j < 10; j++)
      {
        Serial.print(plateau_joueur2[i][j]);
        Serial.print("|");
      }
      Serial.println();
    }
  }
  else {
    Serial.println("joueur1" );
    Serial.println(" |0|1|2|3|4|5|6|7|8|9|" );
    for (size_t i = 0; i < 10; i++)
    {
      Serial.println( i + "|");
      for (size_t j = 0; j < 10; j++)
      {
        Serial.print(plateau_joueur1[i][j]);
        Serial.print("|");
      }
      Serial.println( );
    }
    Serial.println("\njoueur2" );
    Serial.println("joueur2");
    Serial.println( " |0|1|2|3|4|5|6|7|8|9|");
    for (size_t i = 0; i < 10; i++)
    {
      Serial.print(i);
      Serial.print("|");
      for (size_t j = 0; j < 10; j++)
      {
        Serial.print(plateau_joueur2[i][j]);
        Serial.print("|");
      }
      Serial.println();
    }
  }
  
}

void init_plateau() {
  
  init_plateau_vide();
  int x,y;
  for (int j = 3; j >= 0; j--) // mettre j=3 pour normal
  {
    int o = random(2);
    if (o == 0) // horizontal
    {
      x = random(10 - (j + 1));
      y = random(10);
      while (verifie_emplacement_disponible(plateau_joueur1, x, y, j, o) == false)
      {
        x = random(10 - (j + 1));
        y = random(10);
      }
      for (int i = 0; i < j + 2; i++)
      {
        plateau_joueur1[y][x + i] = bateau[j];
      }
      if (j == 1)
      {
        x = random(10);
        y = random(10 - (j + 1));
        while (verifie_emplacement_disponible(plateau_joueur1, x, y, j, 1) == false)
        {
          x = random(10);
          y = random(10 - (j + 1));
        }
        for (int i = 0; i < 3; i++) {
          plateau_joueur1[y + i][x] = bateau[j];
        }
      }
    }
    else { // vertical
      x = random(10);
      y = random(10 - (j + 1));
      while (verifie_emplacement_disponible(plateau_joueur1, x, y, j, o) == false)
      {

        x = random(10);
        y = random(10 - (j + 1));
      }
      for (int i = 0; i < j + 2; i++)
      {
        plateau_joueur1[y + i][x] = bateau[j];
      }
      if (j == 1)
      {
        x = random(10 - (j + 1));
        y = random(10) ;
        while (verifie_emplacement_disponible(plateau_joueur1, x, y, j, 0) == false)
        {
          x = random(10 - (j + 1));
          y = random(10);
        }
        for (int i = 0; i < 3; i++) {
          plateau_joueur1[y][x + i] = bateau[j];
        }
      }
    }
  }
  for (int j = 3; j >= 0; j--)
  {
    int o = random(2);
    if (o == 0) // horizontal
    {
      x = random(10 - (j + 1));
      y = random(10);
      while (verifie_emplacement_disponible(plateau_joueur2, x, y, j, o) == false)
      {
        x = random(10 - (j + 1));
        y = random(10);
      }
      for (int i = 0; i < j + 2; i++)
      {
        plateau_joueur2[y][x + i] = bateau[j];
      }
      if (j == 1)
      {
        x = random(10);
        y = random(10 - (j + 1));
        while (verifie_emplacement_disponible(plateau_joueur2, x, y, j, 1) == false)
        {
          x = random(10);
          y = random(10 - (j + 1));
        }
        for (int i = 0; i < 3; i++) {
          plateau_joueur2[y + i][x] = bateau[j];
        }
      }
    }
    else { // vertical
      x = random(10);
      y = random(10 - (j + 1));
      while (verifie_emplacement_disponible(plateau_joueur2, x, y, j, o) == false)
      {
        
        x = random(10);
        y = random(10 - (j + 1));
      }
      for (int i = 0; i < j + 2; i++)
      {
        plateau_joueur2[y + i][x] = bateau[j];
      }
      if (j == 1)
      {
        x = random(10 - (j + 1));
        y = random(10);
        while (verifie_emplacement_disponible(plateau_joueur2, x, y, j, 0) == false)
        {
          x = random(10 - (j + 1));
        y = random(10);
        }
        for (int i = 0; i < 3; i++) {
          plateau_joueur2[y][x + i] = bateau[j];
        }
      }
    }
  }
}

int absolue(int a){
  if (a>=0){
    return a;
  }
  else{
    return -a;
  }
}

void ecran_victoire(int j){
  display.clearDisplay();
  display.setCursor(0,0);
  if(j!=3){
    display.println("Le Joueur " +(String)j + " a GAGNER");
    
  }
  else{
    display.println("Le bot a GAGNER");
  }
  display.display();
  attente=millis()-1;
  while(millis()-attente<=4000){
    leds.setColorRGB(0, 255, 0, 0);
    delay(150);
    leds.setColorRGB(0, 0, 0, 0);
    delay(150);
  }
}

void demande_coordonne(int& x, int& y,int j) {
  x=0;
  y=0;
  affiche_tableau(j);
  rectangle_plein(x*6+67,y*6,6,6);
  display.display();
  while (!valide){
    int sensorValuex = analogRead(A1)-1800;
    int sensorValuey = analogRead(A0)-1800;
    if (absolue(sensorValuex)>absolue(sensorValuey) and sensorValuex!=2295){
        if(sensorValuex>500){
          if(millis()-last_deplacement>200){
            x=(x+1)%10;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);

            affiche_tableau(j);
            rectangle_plein(x*6+67,y*6,6,6);
            display.display();
            last_deplacement=millis();
          }
        }
        if(sensorValuex<-500){
          if(millis()-last_deplacement>200){
            x--;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
            if(x==-1){
              x=9;
            }

            affiche_tableau(j);
            rectangle_plein(x*6+67,y*6,6,6);
            display.display();
            last_deplacement=millis();
          }
        }
      }
      else{
        if(sensorValuey<-500){
          if(millis()-last_deplacement>200){
            y--;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
            if(y==-1){
              y=9;
            }

            affiche_tableau(j);
            rectangle_plein(x*6+67,y*6,6,6);
            display.display();
            last_deplacement=millis();
          }
        }
        if(sensorValuey>500){
          if(millis()-last_deplacement>200){
            y=(y+1)%10;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);

            affiche_tableau(j);
            rectangle_plein(x*6+67,y*6,6,6);
            display.display();
            last_deplacement=millis();
          }
        }
      }
    }
  valide=!valide;
}

bool tire(char tab_joueur_qui_recoit[10][10],int collone, int ligne){

  if (tab_joueur_qui_recoit[ligne][collone] != ' ' && tab_joueur_qui_recoit[ligne][collone] != 'O') {
    tab_joueur_qui_recoit[ligne][collone] = 'X';
    return true;
  }
  else {
    tab_joueur_qui_recoit[ligne][collone] = 'O';
    return false;
  }
}

int a_gagne() {
  bool joueur1 = false;
  bool joueur2 = false;
  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {

      if (plateau_joueur1[i][j] == bateau[0] || plateau_joueur1[i][j] == bateau[1] || plateau_joueur1[i][j] == bateau[2] || plateau_joueur1[i][j] == bateau[3]) {
        joueur1 = true;
      }
      if (plateau_joueur2[i][j] == bateau[0] || plateau_joueur2[i][j] == bateau[1] || plateau_joueur2[i][j] == bateau[2] || plateau_joueur2[i][j] == bateau[3]) {
        joueur2 = true;
      }
    }
  }
  if (joueur1 == false) {
    return 2;
  }
  if (joueur2 == false) {
    return 1;
  }
  return 0;
}

void joue_IA(char tab_joueur_adverse[10][10]) {
  int ligne_depart = random(10);
  int colone_depart = random(10);
  int colone = colone_depart;
  int ligne = ligne_depart;
  int direction = random(4);
  bool rejoue = tire(tab_joueur_adverse, ligne, colone);
  int cptr = 0;
  while (rejoue == true) {
    if (cptr == 5) {
      ligne_depart = random(10);
      colone_depart = random(10);
      colone = colone_depart;
      ligne = ligne_depart;
      direction = random(4);
      cptr = 0;
      rejoue = tire(tab_joueur_adverse, ligne, colone);
    }
    else {
      if (direction == 0 && rejoue == true) {
        if (ligne == 0) {
          ligne = ligne_depart;
          colone = colone_depart;
          direction = 2;
          ligne = ligne + 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
        else {
          ligne = ligne - 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
      }
      if (direction == 1 && rejoue == true) {
        if (colone == 9) {
          ligne = ligne_depart;
          colone = colone_depart;
          direction = 3;
          colone = colone - 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
        else {
          colone = colone + 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
      }
      if (direction == 2 && rejoue == true) {
        if (ligne == 10) {
          ligne = ligne_depart;
          colone = colone_depart;
          direction = 2;
          ligne = ligne - 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
        else {
          ligne = ligne + 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
      }
      if (direction == 3 && rejoue == true) {
        if (colone == 0) {
          ligne = ligne_depart;
          colone = colone_depart;
          direction = 1;
          colone = colone + 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
        else {
          colone = colone - 1;
          rejoue = tire(tab_joueur_adverse, ligne, colone);
          cptr++;
        }
      }
    }
  }
}


void attente_joueur(int j1){
  display.clearDisplay();
  display.println("Next\nPlayer");
  display.display();
  if (j1==2){
    for (int i=0 ;i<1;i++){
      leds.setColorRGB(0, 0, 255, 0);
    delay(500);
    leds.setColorRGB(0, 0, 0, 0);
    delay(500);
    }
    
    for (int i=0 ;i<3;i++){
      leds.setColorRGB(0, 0, 255, 0);
    delay(250);
    leds.setColorRGB(0, 0, 0, 0);
    delay(250);
    }
    for (int i=0 ;i<4;i++){
      leds.setColorRGB(0, 0, 255, 0);
    delay(100);
    leds.setColorRGB(0, 0, 0, 0);
    delay(100);
    }
    for (int i=0 ;i<3;i++){
      leds.setColorRGB(0, 0, 255, 0);
    delay(50);
    leds.setColorRGB(0, 0, 0, 0);
    delay(50);
    }
  }
  if (j1==1){
    for (int i=0 ;i<1;i++){
      leds.setColorRGB(0, 255, 0, 0);
    delay(500);
    leds.setColorRGB(0, 0, 0, 0);
    delay(500);
    }
    
    for (int i=0 ;i<3;i++){
      leds.setColorRGB(0, 255, 0, 0);
    delay(250);
    leds.setColorRGB(0, 0, 0, 0);
    delay(250);
    }
    for (int i=0 ;i<4;i++){
      leds.setColorRGB(0, 255, 0, 0);
    delay(100);
    leds.setColorRGB(0, 0, 0, 0);
    delay(100);
    }
    for (int i=0 ;i<3;i++){
      leds.setColorRGB(0, 255, 0, 0);
    delay(50);
    leds.setColorRGB(0, 0, 0, 0);
    delay(50);
    }
  }
  
  
}


void jeux_1v1() {
  init_plateau();
  bool continuer = true;
  int win=0;
  while (continuer)
  {
    leds.setColorRGB(0, 255, 0, 0);
    affiche_tableau(1);
    display.display();
    int x = 0, y = 0;
    demande_coordonne(x, y,1);
    while (tire(plateau_joueur2, y, x)) {
      win=a_gagne();
      if (win != 0)
      {
        ecran_victoire(win);
        continuer = false;
        break;
        return;
      }
      demande_coordonne(x, y,1);
    }
    


    attente_joueur(2);
    leds.setColorRGB(0, 0, 255, 0);
    affiche_tableau(2);
    display.display();
    demande_coordonne(x, y,2 );
    while (tire(plateau_joueur1, y, x)) {
      win=a_gagne();
      if (win != 0)
      {
        ecran_victoire(win);
        continuer = false;
        break;
        return;
      }
      demande_coordonne(x, y,2);
    }
    attente_joueur(1);
  }
}


void jeux_1vIA() {
  init_plateau();
  bool continuer = true;
  int win=0;
  while (continuer)
  {
    leds.setColorRGB(0, 255, 0, 0);
    affiche_tableau(1);
    display.display();
    int x = 0, y = 0;
    demande_coordonne(x, y,1);
    while (tire(plateau_joueur2, y, x)) {
      win=a_gagne();
      if (win != 0)
      {
        ecran_victoire(win);
        continuer = false;
        break;
        return;
      }
      demande_coordonne(x, y,1);
    }


    affiche(2);
    joue_IA(plateau_joueur1);

    win=a_gagne();
    if (win != 0)
    {
      ecran_victoire(3);
      continuer = false;
      break;
      return;
    }
  }
}


void dessine_plateau_vide(){
  display.clearDisplay();
  display.setCursor(0,0);
  for (int i=0; i<63; i=i+6){
    display.drawLine(i,0,i,60,SH110X_WHITE);
    display.drawLine(i+67,0,i+67,60,SH110X_WHITE);
    
  }
  for (int i = 0; i<61; i=i+6){
    display.drawLine(0,i,60,i,SH110X_WHITE);
    display.drawLine(67,i,127,i,SH110X_WHITE);
  }
}

void dessine_croix(int x_start, int y_start, int width, int height){  
  display.drawLine(x_start, y_start, x_start+width, y_start+height, SH110X_WHITE);  
  display.drawLine(x_start+width, y_start, x_start, y_start+height, SH110X_WHITE);
} 

int affiche_menu_local(){
  int y2=0;
  display.clearDisplay();
  display.setCursor(0,2);
  display.println(" 1 Player");
  display.setCursor(0,42);
  display.println(" 2 Players");
  display.drawRect(0,y2*40,120,20,SH110X_WHITE);
  display.display();
while (!valide){
  int sensorValuey = analogRead(A0)-1800;
  
  if(sensorValuey<-500){
          if(millis()-last_deplacement>200){
            y2--;
            if(y2==-1){
              y2=1;
            }

            display.clearDisplay();
            display.setCursor(0,2);
            display.println(" 1 Joueur");
            display.setCursor(0,42);
            display.println(" 2 Joueur");
            display.drawRect(0,y2*40,120,20,SH110X_WHITE);
            display.display();
            last_deplacement=millis();
          }
        }
        if(sensorValuey>500){
          if(millis()-last_deplacement>200){
            y2=(y2+1)%2;

            display.clearDisplay();
            display.setCursor(0,2);
            display.println(" 1 Joueur");
            display.setCursor(0,42);
            display.println(" 2 Joueur");
            display.drawRect(0,y2*40,120,20,SH110X_WHITE);
            display.display();
            last_deplacement=millis();
          }
        }
}
valide=!valide;
return y2;
}

void menu_local(){
  int o=affiche_menu_local();
  if (o==0){
    jeux_1vIA();
  }
  if(o==1){
    jeux_1v1();
  }
}

void loop() {
  menu_local();
}

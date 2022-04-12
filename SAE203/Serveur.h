


void jeux_online_2ESP(){
  init_plateau();
  memcpy(myData_tableau.plateau_joueur1,plateau_joueur1,sizeof(myData_tableau.plateau_joueur1));
  memcpy(myData_tableau.plateau_joueur2,plateau_joueur2,sizeof(myData_tableau.plateau_joueur2));
  esp_now_send(broadcastAddress, (uint8_t *) &myData_tableau, sizeof(myData_tableau));
  
  bool continuer = true;
  int win=0;
  bool touche=true;
  while (continuer)
  {
    leds.setColorRGB(0, 255, 0, 0);
    affiche_tableau(1);
    display.display();
    int x = 0, y = 0;
    demande_coordonne(x,y,1);
    touche=tire(plateau_joueur2, y,x);
    memcpy(myData_tableau.plateau_joueur1,plateau_joueur1,sizeof(myData_tableau.plateau_joueur1));
    memcpy(myData_tableau.plateau_joueur2,plateau_joueur2,sizeof(myData_tableau.plateau_joueur2));
    esp_now_send(broadcastAddress, (uint8_t *) &myData_tableau, sizeof(myData_tableau));
    while (touche) {
      affiche_tableau(1);
      display.display();
      demande_coordonne(x,y,1);
      touche=tire(plateau_joueur2, y,x);
      memcpy(myData_tableau.plateau_joueur1,plateau_joueur1,sizeof(myData_tableau.plateau_joueur1));
      memcpy(myData_tableau.plateau_joueur2,plateau_joueur2,sizeof(myData_tableau.plateau_joueur2));
      esp_now_send(broadcastAddress, (uint8_t *) &myData_tableau, sizeof(myData_tableau));
      win=a_gagne();
      if (win != 0)
      {
        ecran_victoire(win);
        continuer = false;
        break;
        return;
      }
    }
    if (!continuer){
        break;
      }
    
    myData_tableau.joueur2_joue=true;
    memcpy(myData_tableau.plateau_joueur1,plateau_joueur1,sizeof(myData_tableau.plateau_joueur1));
    memcpy(myData_tableau.plateau_joueur2,plateau_joueur2,sizeof(myData_tableau.plateau_joueur2));
    esp_now_send(broadcastAddress, (uint8_t *) &myData_tableau, sizeof(myData_tableau));


    leds.setColorRGB(0, 0, 255, 0);
    while (myData_tableau.joueur2_joue) {
      //Serial.println(myData_tableau.joueur2_joue);
      affiche_tableau(1);
      display.display();
      win=a_gagne();
      if (win != 0)
      {
        ecran_victoire(win);
        continuer = false;
        break;
        return;
      }
    }
  }
}

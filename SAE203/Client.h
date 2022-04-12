void jeux_online_2ESP(){
  bool continuer = true;
  int win=0;
  bool touche=true;
  init_plateau_vide();
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connexion");
  display.display();
  while (start_attente)
    {
      Serial.println("yes");
      delay(500);
    }
  while (continuer)
    {
      leds.setColorRGB(0, 255, 0, 0);
      affiche_tableau(2);
      display.display();
      
      while (myData_tableau.joueur2_joue==0) {
        //Serial.println(myData_tableau.joueur2_joue);
        affiche_tableau(2);
        display.display();
        win=a_gagne();
        if (win != 0)
        {
          ecran_victoire(win);
          continuer = false;
          start_attente =true;
          break;
          return;
        }
      }
      if (!continuer){
        break;
      }
          
      leds.setColorRGB(0, 0, 255, 0);
      affiche_tableau(2);
      display.display();
      int x = 0, y = 0;
      demande_coordonne(x,y,2);
      touche=tire(plateau_joueur1, y,x);
      memcpy(myData_tableau.plateau_joueur1,plateau_joueur1,sizeof(myData_tableau.plateau_joueur1));
      memcpy(myData_tableau.plateau_joueur2,plateau_joueur2,sizeof(myData_tableau.plateau_joueur2));
      esp_now_send(broadcastAddress, (uint8_t *) &myData_tableau, sizeof(myData_tableau));
      while (touche) {
        
        demande_coordonne(x,y,2);
        touche=tire(plateau_joueur1, y,x);
        memcpy(myData_tableau.plateau_joueur1,plateau_joueur1,sizeof(myData_tableau.plateau_joueur1));
        memcpy(myData_tableau.plateau_joueur2,plateau_joueur2,sizeof(myData_tableau.plateau_joueur2));
        esp_now_send(broadcastAddress, (uint8_t *) &myData_tableau, sizeof(myData_tableau));
        win=a_gagne();
        if (win != 0)
        {
          ecran_victoire(win);
          continuer = false;
          start_attente =true;
          break;
          return;
        }
        
      }
      myData_tableau.joueur2_joue=false;
      memcpy(myData_tableau.plateau_joueur1,plateau_joueur1,sizeof(myData_tableau.plateau_joueur1));
      memcpy(myData_tableau.plateau_joueur2,plateau_joueur2,sizeof(myData_tableau.plateau_joueur2));
      esp_now_send(broadcastAddress, (uint8_t *) &myData_tableau, sizeof(myData_tableau));
    }
}

// RIEN A MODIFIER //

#ifndef CHRONO_H
#define CHRONO_H

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/

#include "stm32f103xb.h" 

/* =====================================================================================
Les fonctions qui g�rent les IO (ajout par rapport � l'activit� 1)
=======================================================================================*/


#define PinStart LL_GPIO_PIN_8
#define PinStop LL_GPIO_PIN_6
// � compl�ter pour la pin reset du chronometre
#define PinLED LL_GPIO_PIN_10
#define GPIO_Pin GPIOC
#define GPIO_LED GPIOC






/**
	* @brief  polle les 3 entr�es et g�n�re les actions � faire
  * @note   Fct � lancer en t�che de fond (non bloquante)
	* @param  None 
  * @retval None
  */
		
void Chrono_Background(void);

//=======================================================================================//




// Type de donn�e utilis� dans le module

typedef struct {
	char Hund;
	char Sec;
	char Min;
} Time;




/**
	* @brief  Configure le chronom�tre. 
  * @note   A lancer avant toute autre fonction.
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer);






/**
	* @brief  D�marre le chronom�tre. 
  * @note   si la dur�e d�passe 59mn 59sec 99 cent, elle est remise � z�ro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void);


/**
	* @brief  Arr�te le chronom�tre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void);


/**
	* @brief  Remet le chronom�tre � 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void);


/**
	* @brief  Renvoie l'adresse de la variable Time priv�e g�r�e dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void);

/**
	* @brief  Permet de configurer les 3 IO pour recevoir les 2 BP et la LED. Priv�e et doit �tre appel�e par Chrono_Conf( )
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
static void Chrono_Conf_io(void);

/**
	* @brief  Appel�e en permanence dans le main, � l�int�rieur du while(1). C�est la t�che de fond non bloquante du module chronom�tre. 
Elle est la plus courte possible. Elle consiste juste � lire l��tat des divers boutons puis de faire les actions associ�es (start, stop, reset).
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Background(void); 

#endif

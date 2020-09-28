// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_gpio.h"

// variable privée de type Time qui mémorise la durée mesurée
static Time Chrono_Time; // rem : static rend la visibilité de la variable Chrono_Time limitée à ce fichier 

// variable privée qui mémorise pour le module le timer utilisé par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par défaut au cas où l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// déclaration callback appelé toute les 10ms
void Chrono_Task_10ms(void);

int stop = 0;
int start = 0;

/**
	* @brief  Configure le chronomètre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	// Réglage Timer pour un débordement à 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// Réglage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	// Configuration des 3 IO : 2 BP et la LED
	Chrono_Conf_io();
}


/**
	* @brief  Démarre le chronomètre. 
  * @note   si la durée dépasse 59mn 59sec 99 Hund, elle est remise à zéro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arrête le chronomètre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronomètre à 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arrêt Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time privée gérée dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}




/**
	* @brief  incrémente la variable privée Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
	// Allume la LED quand BP1 est appuyé
	if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_8) | LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)) // A Voir (BP2)
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_10);
	else LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_10);
}

/**
	* @brief  Permet de configurer les 3 IO pour recevoir les 2 BP et la LED. Privée et doit être appelée par Chrono_Conf( )
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
static void Chrono_Conf_io(void)
{
	LL_GPIO_InitTypeDef BP1 ; 
	LL_GPIO_InitTypeDef BP2 ; 
	LL_GPIO_InitTypeDef LED ; 

// StructInit
	LL_GPIO_StructInit(&BP1);
	LL_GPIO_StructInit(&BP2);
	LL_GPIO_StructInit(&LED);

// Pin
	BP1.Pin = LL_GPIO_PIN_8;
	BP2.Pin = LL_GPIO_PIN_0; // A Voir
	LED.Pin = LL_GPIO_PIN_10;

// Mode
	BP1.Mode = LL_GPIO_MODE_FLOATING;
	BP2.Mode = LL_GPIO_MODE_FLOATING; // A Voir
	LED.Mode = LL_GPIO_MODE_OUTPUT;

// Pull
	LED.Pull = LL_GPIO_PULL_UP;

// Output type
	LED.OutputType = LL_GPIO_OUTPUT_PUSHPULL ; // Push-Pull ou Open-Drain

	LL_GPIO_Init(GPIOA, &BP1) ;
	LL_GPIO_Init(GPIOA, &BP2) ;
	LL_GPIO_Init(GPIOA, &LED) ;
}

/**
	* @brief  Appelée en permanence dans le main, à l’intérieur du while(1). C’est la tâche de fond non bloquante du module chronomètre. 
Elle est la plus courte possible. Elle consiste juste à lire l’état des divers boutons puis de faire les actions associées (start, stop, reset).
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Background(void)
{
	if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_8)) {
		start=1;
	} else if (start == 1) {
		Chrono_Start();
		stop=0;
		start=0;
	}
	if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)) 
	{
		if (stop == 0) {
			Chrono_Stop(); // A Voir (BP2)
			stop=1;
		} else if (stop == 2) {
			Chrono_Reset();
			stop=0;
		}
	} else if (stop == 1) stop=2;
}

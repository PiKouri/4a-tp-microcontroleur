// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 

// Active l'horloge et règle l'ARR et le PSC du timer visé

void (* handler) (void);

void TIM1_BRK_IRQHandler (void) {handler();} ;
void TIM2_IRQHandler (void) {handler();} ;
void TIM3_IRQHandler (void) {handler();} ;
void TIM4_IRQHandler (void) {handler();} ;


void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc) {
	Timer->ARR = Arr;
	Timer->PSC = Psc;
	if (Timer == TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if (Timer == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if (Timer == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if (Timer == TIM4)	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

// Démarre le timer considéré

void MyTimer_Start(TIM_TypeDef * Timer) {
	Timer->CR1 |= TIM_CR1_CEN;
}

// Arrête le timer considéré

void MyTimer_Stop(TIM_TypeDef * Timer) {
	Timer->CR1 &= !TIM_CR1_CEN;
}

// Configure le Timer considéré en interruption sur débordement.

void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio) {
	int offset;
	if (Timer == TIM1) { offset = TIM1_BRK_IRQn; TIM1->DIER |= TIM_DIER_UIE;}
	else if (Timer == TIM2) { offset = TIM2_IRQn; TIM2->DIER |= TIM_DIER_UIE;}
	else if (Timer == TIM3) { offset = TIM3_IRQn; TIM3->DIER |= TIM_DIER_UIE;}
	else if (Timer == TIM4)	{ offset = TIM4_IRQn; TIM4->DIER |= TIM_DIER_UIE;}

	// Pour IPR : 8 bits pour la priorité mais seulement les 4 bits de poids le plus fort sont utilisés
	NVIC->IP[offset]|=(Prio << 4);
	//NVIC_SetPriority(offset, Prio);

	//TIM2_IRQHandler
	
	//NVIC_SetVector(offset, (uint32_t) IT_function); // Handler
	handler = IT_function;

	//TIM_SR_UIF; // Interrupt Flag
}

// Autorise les interruptions

void MyTimer_IT_Enable(TIM_TypeDef * Timer) {
	int offset;
	if (Timer == TIM1) offset = TIM1_BRK_IRQn;
	else if (Timer == TIM2) offset = TIM2_IRQn;
	else if (Timer == TIM3) offset = TIM3_IRQn;
	else if (Timer == TIM4)	offset = TIM4_IRQn;
	//NVIC->ISER [offset] = 1; // Set Enable Register
	NVIC_EnableIRQ(offset);
}

// Interdit les interruptions

void MyTimer_IT_Disable(TIM_TypeDef * Timer) {
	int offset;
	if (Timer == TIM1) offset = TIM1_BRK_IRQn;
	else if (Timer == TIM2) offset = TIM2_IRQn;
	else if (Timer == TIM3) offset = TIM3_IRQn;
	else if (Timer == TIM4)	offset = TIM4_IRQn;
	//NVIC->ISER [offset] = 0; // Set Enable Register
	NVIC_DisableIRQ(offset);
}

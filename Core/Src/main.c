/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fonts.h"
#include "ssd1306.h"
#include "fonts.c"
#include "ssd1306.c"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
extern I2C_HandleTypeDef hi2c1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define R1_PORT GPIOA
#define R1_PIN GPIO_PIN_7

#define R2_PORT GPIOA
#define R2_PIN GPIO_PIN_6

#define R3_PORT GPIOA
#define R3_PIN GPIO_PIN_5

#define R4_PORT GPIOA
#define R4_PIN GPIO_PIN_4

#define C1_PORT GPIOA
#define C1_PIN GPIO_PIN_3

#define C2_PORT GPIOA
#define C2_PIN GPIO_PIN_2

#define C3_PORT GPIOA
#define C3_PIN GPIO_PIN_1

#define C4_PORT GPIOA
#define C4_PIN GPIO_PIN_0


char character;
uint8_t character_position = 0;
uint8_t character_length = 0;
uint8_t p, count, time;
char default_password[] = {'3','4','5','6'};
char user_password[] = {'3','4','5','6'};
//char check_password[] = {NULL, NULL, NULL, NULL};
char new_password[] = {NULL, NULL, NULL, NULL};
char user_ans[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
char array_ans[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
char array_typed[] = {NULL, NULL, NULL, NULL};


char read_keypad(void);
void enter_character(char pass[], int j);
void enter_password(char pass[], int j);
void test_password(char pass[], char pass1[], int j);
void test_password_ques();
void question();
void luu_cau_hoi();
void enter_answer();
void answer();
void check_newpassword();
int a = 0;
int wrg = 0;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	UNUSED(GPIO_Pin);
	if(GPIO_Pin == GPIO_PIN_1){
		enter_answer();
		test_password_ques(user_ans);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	htim1.Instance->CCR1 = 1000 - 90;
	HAL_Delay(100);

	SSD1306_Init (); // initialize the display 
	SSD1306_GotoXY(0,15);
	SSD1306_Puts("Enter your new pass:", &Font_7x10, 1);
	SSD1306_UpdateScreen();
	enter_password(user_password, 4);
	luu_cau_hoi();
	wrg = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		SSD1306_GotoXY (0,15);
		SSD1306_Puts ("Enter password:", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		p = 0;
		enter_password(array_typed, 4);
		test_password(array_typed ,user_password, 4);
		if (wrg == 1 && time == 0){
			question();
		}
		if(time == 0){
			htim1.Instance->CCR1 = 1000 - 40;
			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 1);
			HAL_Delay(300);
			htim1.Instance->CCR1 = 1000 - 90;
		}
		if(time == 5){
			SSD1306_GotoXY (0,15);
			SSD1306_Puts ("Over 5 times.", &Font_7x10, 1);
			SSD1306_UpdateScreen();
			SSD1306_GotoXY (0,25);
			SSD1306_Puts ("Pls wait 10s.", &Font_7x10, 1);
			wrg = 1;
			SSD1306_UpdateScreen();
			HAL_Delay(10000);
			SSD1306_Clear();
		}
		else if(time == 9){
			SSD1306_GotoXY (0,15);
			SSD1306_Puts ("Over 10 times.", &Font_7x10, 1);
			SSD1306_UpdateScreen();
			HAL_Delay(100);
			SSD1306_GotoXY (0,25);
			SSD1306_Puts ("Pls wait 20s.", &Font_7x10, 1);
			wrg = 1;
			SSD1306_UpdateScreen();
			HAL_Delay(50000);
			SSD1306_Clear();
		}
		HAL_Delay(50);
  }
  /* USER CODE END 3 */
}
char read_keypad(void)
{
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_RESET);	// Pull R1 Low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);		// Pull R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);		// Pull R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);		// Pull R4 High

	if(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))							// if Col 1 is Low
	{
		while(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN))){
			HAL_Delay(300);
			break;
		}
		return '1';
	}

	if(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))							// if Col 2 is Low
	{
		while(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN))){
			HAL_Delay(300);
			break;
		}
		return '2';
	}
	
	if(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))							// if Col 3 is Low
	{
		while(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN))){
			HAL_Delay(300);
			break;
		}
		return '3';
	}
	
	if(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))							// if Col 4 is Low
	{
		while(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN))){
			HAL_Delay(300);
			break;
		}
		return 'A';
	}
	
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);	// Pull R1 High
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_RESET);		// Pull R2 Low
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);		// Pull R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);		// Pull R4 High

	if(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))							// if Col 1 is Low
	{
		while(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN))){
			HAL_Delay(300);
			break;
		}
		return '4';
	}
	
	if(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))							// if Col 2 is Low
	{
		while(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN))){
			HAL_Delay(300);
			break;
		}
		return '5';
	}
	
	if(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))							// if Col 3 is Low
	{
		while(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN))){
			HAL_Delay(300);
			break;
		}
		return '6';
	}
	
	if(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))							// if Col 4 is Low
	{
		while(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN))){
			HAL_Delay(300);
			break;
		}
		return 'B';
	}
	
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);	// Pull R1 High
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);		// Pull R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_RESET);		// Pull R3 Low
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);		// Pull R4 High

	if(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))							// if Col 1 is Low
	{
		while(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN))){
			HAL_Delay(300);
			break;
		}
		return '7';
	}
	
	if(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))							// if Col 2 is Low
	{
		while(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN))){
			HAL_Delay(300);
			break;
		}
		return '8';
	}
	
	if(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))							// if Col 3 is Low
	{
		while(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN))){
			HAL_Delay(300);
			break;
		}
		return '9';
	}
	
	if(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))							// if Col 4 is Low
	{
		while(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN))){
			HAL_Delay(300);
			break;
		}
		return 'C';
	}
	
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);	// Pull R1 High
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);		// Pull R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);		// Pull R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_RESET);		// Pull R4 Low

	if(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))							// if Col 1 is Low
	{
		while(!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN))){
			HAL_Delay(300);
			break;
		}
		return '*';
	}
	
	if(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))							// if Col 2 is Low
	{
		while(!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN))){
			HAL_Delay(300);
			break;
		}
		return '0';
	}
	
	if(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))							// if Col 3 is Low
	{
		while(!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN))){
			HAL_Delay(300);
			break;
		}
		return '#';
	}
	
	if(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))							// if Col 4 is Low
	{
		while(!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN))){
			HAL_Delay(300);
			break;
		}
		return 'D';
	}
	return 'k';
}
void luu_cau_hoi(){
	SSD1306_Clear();
	SSD1306_GotoXY(0,15);
	SSD1306_Puts("What's your birdthday?", &Font_7x10, 1);
	SSD1306_UpdateScreen();
	enter_answer(array_ans);
	SSD1306_Clear();
}

void answer(char pass[]){
	if(character != 'k'){
		if(character != 'A'){
			if( character_position == 8 || character_position == 32){
				SSD1306_GotoXY(character_position, 25);
				SSD1306_Putc(character, &Font_7x10, 1);
				SSD1306_UpdateScreen();
				character_position += 8;
				pass[character_length] = character;
				character_length += 1;
				SSD1306_GotoXY(character_position, 25);
				SSD1306_Puts("/", &Font_7x10, 1);
				SSD1306_UpdateScreen();
				character_position += 8;
		}else{
		    SSD1306_GotoXY(character_position, 25);
				SSD1306_Putc(character, &Font_7x10, 1);
				SSD1306_UpdateScreen();
				character_position += 8;
				pass[character_length] = character;
				character_length += 1;
		}
	}
		if(character == 'A'){
			if(character_position > 0){
				if( character_position == 24 || character_position == 48){
				character_position -= 8;
				SSD1306_GotoXY(character_position, 25);
				SSD1306_Puts(" ", &Font_7x10, 1);
				SSD1306_UpdateScreen();
				character_position -= 8;
				SSD1306_GotoXY(character_position, 25);
				SSD1306_Puts(" ", &Font_7x10, 1);
				SSD1306_UpdateScreen();
				pass[character_length] = NULL;
				character_length -= 1;
				}else{
				character_position -= 8;
				SSD1306_GotoXY(character_position, 25);
				SSD1306_Puts(" ", &Font_7x10, 1);
				SSD1306_UpdateScreen();
				pass[character_length] = NULL;
				character_length -= 1;
				}
			}
		}
	}
}

void question(){
	SSD1306_Clear();
	SSD1306_GotoXY(0,15);
	SSD1306_Puts("Your Birdthday?", &Font_7x10, 1);
	SSD1306_UpdateScreen();
	enter_answer(user_ans);
	p = 0;
	test_password(user_ans, array_ans, 8);
	wrg = 0;
	if(p == 4){
		count = 1;
	}
}

void check_newpassword(){
   	SSD1306_Clear();
		SSD1306_GotoXY(0,15);
		SSD1306_Puts("Enter new pass:", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		enter_password(array_typed, 4);
		if(p == 4){
			p = 0;
			SSD1306_Clear();
			SSD1306_GotoXY(0,15);
			SSD1306_Puts("Confirm new pass:", &Font_7x10, 1);
			SSD1306_UpdateScreen();
			enter_password(new_password, 4);
			test_password(new_password, array_typed, 4);
			if(p == 4){
				for(uint8_t i = 0; i < 4; i++){
				(user_password[i] = new_password[i]);
			}
		}
	}
}

void enter_character(char pass[], int j){
	if(character != 'k'){
		if(character != 'A'){
				SSD1306_GotoXY(character_position, 25);
				SSD1306_Puts("x", &Font_7x10, 1);
				SSD1306_UpdateScreen();
				character_position += 8;
				pass[character_length] = character;
				character_length += 1;
		}
		if(character == 'A'){
			if(character_position > 0){
				character_position -= 8;
				SSD1306_GotoXY(character_position, 25);
				SSD1306_Puts(" ", &Font_7x10, 1);
				SSD1306_UpdateScreen();
				pass[character_length] = NULL;
				character_length -= 1;
			}
		}
	}
}

void enter_password(char pass[], int j){
	character_position = 0;
	character_length = 0;
	for(uint8_t i = 0; i < j; i++){
			pass[i] = NULL;
	}
	while (1){
		character = read_keypad();
		enter_character(pass, j);
    if ( character == 'k' ){
			SSD1306_GotoXY((character_position), 25);
			SSD1306_Puts("_", &Font_7x10, 1);
			SSD1306_UpdateScreen();
			HAL_Delay(300);
			SSD1306_GotoXY(character_position , 25);
			SSD1306_Puts(" ", &Font_7x10, 1);
			SSD1306_UpdateScreen();
			HAL_Delay(300);
		}
		if(character_length == j) break;
	}
}

/*
void enter_newpassword(){
	character_position = 0;
	character_length = 0;
	for(uint8_t i = 0; i < 4; i++){
			array_typed[i] = NULL;
	}
	while (1){
		character = read_keypad();
		enter_character(new_password, 4);
    if ( character == 'k' ){
	  SSD1306_GotoXY((character_position), 25);
		SSD1306_Puts("_", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		HAL_Delay(300);
		SSD1306_GotoXY(character_position , 25);
		SSD1306_Puts(" ", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		HAL_Delay(300);
	}
		if(character_length == 4) break;
	}
}
*/

void enter_answer(char pass[]){
	character_position = 0;
	character_length = 0;
	for(uint8_t i = 0; i < 8; i++){
			pass[i] = NULL;
	}
	while (1){
		character = read_keypad();
		answer(pass);
    if ( character == 'k' ){
			SSD1306_GotoXY((character_position), 25);
			SSD1306_Puts("_", &Font_7x10, 1);
			SSD1306_UpdateScreen();
			HAL_Delay(300);
			SSD1306_GotoXY(character_position , 25);
			SSD1306_Puts(" ", &Font_7x10, 1);
			SSD1306_UpdateScreen();
			HAL_Delay(300);
		}
			if(character_length == 8) break;
	}
}

void test_password(char pass[], char pass1[], int j){
	for(uint8_t i = 0; i < j; i++){
			if(pass[i] == pass1[i]) p += 1;
	}
	if(p == j){
		time = 0;
		character_position = 0;
		character_length = 0;
		SSD1306_GotoXY(0, 35);
		SSD1306_Puts("Correct", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		HAL_Delay(2000);
		SSD1306_Clear();
		SSD1306_UpdateScreen();
	}
	else {
		time +=1;
		character_position = 0;
		character_length = 0;
		SSD1306_GotoXY(0, 35);
		SSD1306_Puts("Wrong", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		HAL_Delay(2000);
		SSD1306_Clear();
		SSD1306_UpdateScreen();
	}
}

void test_password_ques(char pass1[8]){
	for(uint8_t i = 0; i < 8; i++){
			if(array_ans[i] == pass1[i]) p += 1;
	}
	if(p == 8){
		time = 0;
		character_position = 0;
		character_length = 0;
		SSD1306_GotoXY(0, 35);
		SSD1306_Puts("Correct", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		HAL_Delay(2000);
		SSD1306_Clear();
		SSD1306_UpdateScreen();
	}
	else {
		time +=1;
		character_position = 0;
		character_length = 0;
		SSD1306_GotoXY(0, 35);
		SSD1306_Puts("Wrong", &Font_7x10, 1);
		SSD1306_UpdateScreen();
		HAL_Delay(2000);
		SSD1306_Clear();
		SSD1306_UpdateScreen();
	}
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 319;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

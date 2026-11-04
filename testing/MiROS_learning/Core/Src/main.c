
#include "main.h"
#include "usart.h"
#include "gpio.h"

#include <stdint.h>
#include <string.h>

void SystemClock_Config(void);

void sys_conf(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART2_UART_Init();
}


const char *tag = "Hello Wordl!\r\n";

uint32_t stack_main1[40];
uint32_t *sp_main1 = &stack_main1[40];
void main_1 (void)
{
	while (1)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *)tag , strlen(tag), 100);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//		HAL_Delay(90);
		for (volatile long long i=0;i<400000;i++);
	}
}

uint32_t stack_main2[40];
uint32_t *sp_main2 = &stack_main2[40];
void main_2 (void)
{
	while (1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//		HAL_Delay(350);
		for (volatile long long i=0;i<100000;i++);
	}
}

int main(void)
{

	sys_conf();

	// fake stack frame for the main_1 thread
	*(--sp_main1) = (1U << 24); //PSR
	*(--sp_main1) = (uint32_t)&main_1; //PC
	*(--sp_main1) = 0x0000000EU;
	*(--sp_main1) = 0x0000000CU;
	*(--sp_main1) = 0x00000003U;
	*(--sp_main1) = 0x00000002U;
	*(--sp_main1) = 0x00000001U;
	*(--sp_main1) = 0x00000000U;
	*(--sp_main1) = 0xFFFFFFF9U;  // EXC_RETURN, popped into PC by "pop {r7, pc}"
	*(--sp_main1) = 0x00000000U;  // dummy r7

	// fake stack frame for the main_2 thread
	*(--sp_main2) = (1U << 24); //PSR
	*(--sp_main2) = (uint32_t)&main_2; //PC
	*(--sp_main2) = 0x0000000EU;
	*(--sp_main2) = 0x0000000CU;
	*(--sp_main2) = 0x00000003U;
	*(--sp_main2) = 0x00000002U;
	*(--sp_main2) = 0x00000001U;
	*(--sp_main2) = 0x00000000U;
	*(--sp_main2) = 0xFFFFFFF9U;  // EXC_RETURN
	*(--sp_main2) = 0x00000000U;  // dummy r7


	while(1)
	{

	}

//	volatile int flag = 1;
//
//	if (flag)
//	{
//		main_1();
//	}
//	else
//	{
//		main_2();
//	}

//	main_1();
//	main_2();

}




/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

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
#ifdef USE_FULL_ASSERT
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

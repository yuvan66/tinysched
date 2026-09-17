#include "main.h"
#include "gpio.h"

   dht11_t dht;

   uint32_t last_led_ts = 0;
   uint32_t last_uart_ts = 0;
   uint32_t last_dht_ts = 0;
   //  uint8_t tempval;

   char buffer1[40];
   char buffer2[20];
   char buffer3[40];

   void task_led(void)
   {
   	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
   }

   void task_time(void)
   {
   	sprintf(buffer1, "%lu s elapsed since last reset\r\n",HAL_GetTick()/1000);
   	HAL_UART_Transmit(&huart2, (uint8_t *) buffer1, strlen(buffer1), HAL_MAX_DELAY);
   }

   void task_dht(void)
   {
   	sprintf(buffer2, "Temp: %u C  ", dht.temperature);
   	sprintf(buffer3,  "Hum : %u %%  ", dht.humidty);
   	HAL_UART_Transmit(&huart1, (uint8_t *) buffer2, strlen(buffer2), HAL_MAX_DELAY);
   	HAL_UART_Transmit(&huart1, (uint8_t *) buffer3, strlen(buffer3), HAL_MAX_DELAY);
   }

   struct task
	{
   	void (*run)(void);
   	uint32_t duration;
   	uint32_t last_occ;
	};

   struct task tasks[] =
   {
   		{task_led, 500, 0},
			{task_dht, 100, 0},
			{task_time, 1000, 0}
   };


   void scheduler_run(void)
   {
    while (1)
    {
      uint32_t now = HAL_GetTick();
      for (int i=0;i<sizeof(tasks)/sizeof(struct task);i++)
        {
          if (now - tasks[i].last_occ >= tasks[i].duration)
            {
              tasks[i].run();
              tasks[i].last_occ = now;
            }
        }
    }
   }


int main(void)
{

  // System Initialization & all other peripheral initialization
  sys_init();

  // Starts the scheduler

  /* tinysched v0.1
   doesnt actually use the RTOS, but just a simple scheduler that runs tasks based on their duration
   it doesnt slices time, but just runs the tasks in a loop and checks if they need to be run based on their duration
  */
  scheduler_run();
  
}
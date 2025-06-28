#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include <string.h>
#include <uart.h>
#define DBG_TAG "uart"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
DMA_HandleTypeDef hdma_memtomem_dma1_channel1;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
UART_HandleTypeDef huart2,huart1;
uint8_t lcd_rx_buf[64];
uint8_t rx485_rx_buf[64];
rt_mq_t uart2_rx_mq,uart1_rx_mq;
uart2msg msg,msg_uart1;
rt_err_t  s;
void MX_USART2_UART_Init(void)
{
    MX_DMA_Init();
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
    }
    //__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);//接收中断
    __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);//空闲中断
    //HAL_UART_Receive_IT(&huart2, (uint8_t *)&lcd_rx_buf, 64);
    uart2_rx_mq = rt_mq_create("uart_mq", 70, 20, RT_IPC_FLAG_FIFO);
    HAL_UART_Receive_DMA(&huart2,lcd_rx_buf,64);
    HAL_UART_Transmit_DMA(&huart2, lcd_rx_buf, 8);
}
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma1_channel1 on DMA1_Channel1 */
  hdma_memtomem_dma1_channel1.Instance = DMA1_Channel1;
  hdma_memtomem_dma1_channel1.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_channel1.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma1_channel1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma1_channel1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel1.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma1_channel1.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_channel1) != HAL_OK)
  {
    //Error_Handler( );
  }

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {


    }
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);//接收中断
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//空闲中断
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&rx485_rx_buf, 64);
    uart1_rx_mq = rt_mq_create("uart1_mq", 70, 48, RT_IPC_FLAG_FIFO);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(huart->Instance==USART2)
    {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
        /* USART2 DMA Init */
        /* USART2_RX Init */
        hdma_usart2_rx.Instance = DMA1_Channel6;
        hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart2_rx.Init.Mode = DMA_NORMAL;
        hdma_usart2_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
        if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
        {
        //Error_Handler();
        }

        __HAL_LINKDMA(huart,hdmarx,hdma_usart2_rx);
        /* USART2_TX Init */
        hdma_usart2_tx.Instance = DMA1_Channel7;
        hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart2_tx.Init.Mode = DMA_NORMAL;
        hdma_usart2_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
        if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
        {
        //Error_Handler();
        }

        __HAL_LINKDMA(huart,hdmatx,hdma_usart2_tx);
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);


        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    }
    if(huart->Instance==USART1)
    {
    /* USER CODE BEGIN USART2_MspInit 0 */

    /* USER CODE END USART2_MspInit 0 */
      /* USART2 clock enable */
      __HAL_RCC_USART1_CLK_ENABLE();

      __HAL_RCC_GPIOA_CLK_ENABLE();
      /**USART2 GPIO Configuration
      PA2     ------> USART2_TX
      PA3     ------> USART2_RX
      */
      GPIO_InitStruct.Pin = GPIO_PIN_9;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_10;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      /* USART2 interrupt Init */
      HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART1_IRQn);

    }
}


void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);

    if(RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))
    {
       __HAL_UART_CLEAR_IDLEFLAG(&huart2);

       HAL_UART_RxIdleCpltCallback(&huart2);
    }
}

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);

    if(RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
       HAL_UART_RxIdleCpltCallback(&huart1);
       __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    }
}
void DMA1_Channel6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel6_IRQn 0 */

  /* USER CODE END DMA1_Channel6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Channel6_IRQn 1 */

  /* USER CODE END DMA1_Channel6_IRQn 1 */
}
void DMA1_Channel7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel7_IRQn 0 */

  /* USER CODE END DMA1_Channel7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Channel7_IRQn 1 */

  /* USER CODE END DMA1_Channel7_IRQn 1 */
}

/**
* @brief  重映射串口DEBUG_USARTx到rt_kprintf()函数
*   Note：DEBUG_USARTx是在bsp_usart.h中定义的宏，默认使用串口1
* @param  str：要输出到串口的字符串
* @retval 无
*
* @attention
*
*/
void rt_hw_console_output(const char *str)
{
    /* 进入临界段 */
    rt_enter_critical();

    /* 直到字符串结束 */
    while (*str!='\0')
    {
        /* 换行 */
        if (*str=='\n')
        {
                HAL_UART_Transmit( &huart1,(uint8_t *)'\r',1,1000);
        }
        HAL_UART_Transmit( &huart1,(uint8_t *)(str++),1,1000);
    }

    /* 退出临界段 */
    rt_exit_critical();
}
void HAL_UART_RxIdleCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t len;
    if (huart->Instance == USART2)
    {
        HAL_UART_DMAStop(&huart2); //停止DMA接收，防止数据出错
        len = 64 - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);// 获取DMA中传输的数据个数
        HAL_UART_Receive_DMA(&huart2,lcd_rx_buf,64);   //打开DMA接收，数据存入rx_buffer数组中。
        //memcpy(msg.send_uart,lcd_rx_buf,(huart->RxXferSize)-(huart->RxXferCount));
        //msg.len = (huart->RxXferSize)-(huart->RxXferCount);
        memcpy(msg.send_uart,lcd_rx_buf,len);
        msg.len = len;
        rt_mq_send(uart2_rx_mq, &msg,sizeof(msg));
        rt_memset(&lcd_rx_buf, 0, sizeof(lcd_rx_buf));
        //huart->RxXferCount = huart->RxXferSize;
        //huart->pRxBuffPtr = &lcd_rx_buf;
    }

    if (huart->Instance == USART1)
    {
        memcpy(msg_uart1.send_uart,rx485_rx_buf,(huart->RxXferSize)-(huart->RxXferCount));
        msg_uart1.len = (huart->RxXferSize)-(huart->RxXferCount);
        //LOG_D("msg.len == %d",msg.len);
        rt_mq_send(uart1_rx_mq, &msg_uart1,sizeof(msg_uart1));
        rt_memset(&rx485_rx_buf, 0, sizeof(rx485_rx_buf));
        huart->RxXferCount = huart->RxXferSize;
        huart->pRxBuffPtr = &rx485_rx_buf;
        //LOG_D("msg.len == %d",msg.len);
    }
}

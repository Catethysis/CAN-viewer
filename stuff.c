//stuff.c

USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;

void init_timer6(int sp)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);
  /* Делитель учитывается как TIM_Prescaler + 1, поэтому отнимаем 1 */
  base_timer.TIM_Prescaler = 42*sp - 1;
  base_timer.TIM_Period = 8 - 1;
  TIM_TimeBaseInit(TIM6, &base_timer);
  /* Разрешаем прерывание по обновлению (в данном случае -
   * по переполнению) счётчика таймера TIM6.*/
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
  /* Включаем таймер */
  TIM_Cmd(TIM6, ENABLE);
  
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void init_timer7(int sp)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  
  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);
  /* Делитель учитывается как TIM_Prescaler + 1, поэтому отнимаем 1 */
  base_timer.TIM_Prescaler = 42*sp - 1;
  base_timer.TIM_Period = 8 - 1;
  TIM_TimeBaseInit(TIM7, &base_timer);
  /* Разрешаем прерывание по обновлению (в данном случае -
   * по переполнению) счётчика таймера TIM6.*/
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
  /* Включаем таймер */
  TIM_Cmd(TIM7, ENABLE);
  
  NVIC_EnableIRQ(TIM7_IRQn);
}

void USART2_Init( void ) {
    USART_ClockInitTypeDef USART_ClockInitStruct;
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            // Rx Pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Make sure you use 'GPIO_PinSource2' and NOT 'GPIO_Pin_2'.  Using the
    // latter will not work!
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource2, GPIO_AF_USART2 );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource3, GPIO_AF_USART2 );

    // Make sure syncro clock is turned off.
    USART_ClockStructInit( &USART_ClockInitStruct );
    USART_ClockInit( USART2, &USART_ClockInitStruct  );

    USART_StructInit( &USART_InitStructure );
    // Initialize USART
    USART_InitStructure.USART_BaudRate = 256000*8;    
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init( USART2, &USART_InitStructure );
    //USART2->BRR = 364;
    USART_Cmd( USART2, ENABLE );
}

DMA_InitTypeDef       DMA_InitStructure;

void DMA_USART_Init(uint32_t MemAddr)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
  
  DMA_DeInit(DMA1_Stream6);
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = MemAddr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);
  //USART2->CR3 |= USART_CR3_DMAT;
  USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
  DMA_Cmd(DMA1_Stream6, ENABLE);
}

void DMA_USART_ReInit()
{
  DMA_Cmd(DMA1_Stream6, DISABLE);
  USART_DMACmd(USART2,USART_DMAReq_Tx,DISABLE);
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);
  USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
  DMA_Cmd(DMA1_Stream6, ENABLE);
}

void init_clockout()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_5);
}

void init_gpio()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void print(const char* str)
{
    int i = 0; while(str[i]) {
        USART_SendData(USART2, (uint8_t) str[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
    }
    
}

void send(const char dat)
{
    USART_SendData(USART2, (uint8_t) dat);
    //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
}
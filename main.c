#include "main.h"
#include "stdio.h"
#define KEY_PRESSED     0x00
#define KEY_NOT_PRESSED 0x01
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage,prev;
uint8_t KeyNumber = 0x0;

void NVIC_Config(void);
void CAN_Config(void);
void Init_RxMes(CanRxMsg *RxMessage);
void Delay(void);

int total=0,id1,id2,data[8];

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
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
}

void send1(const char dat)
{
    
}

void itoa(int val)
{
  int p=0,i;
  char c[10];
  while(val>0){c[p++]='0'+val%10;val/=10;}
  //if(val==0){c[0]='0';p=1;}
  for(i=p;i>0;i--)send(c[i]);
}

char s[50];

void CAN2_RX1_IRQHandler (void)
{
  if (CAN_GetITStatus(CAN2,CAN_IT_FMP1))
    {
      CAN_ClearITPendingBit(CAN2,CAN_IT_FMP1);
    }
}

int a,b,c,d,i,p=0;

void CAN2_RX0_IRQHandler (void)
{
  //char *s;
  if (CAN_GetITStatus(CAN2,CAN_IT_FMP0))
    {			      // message pending ?
      CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
      //Delay();
      //do smth
      CanRxMsg msg_buf;
      //p++; if(p>2)p=0;
      CAN_Receive(CAN2, CAN_FIFO0, &msg_buf);
      //if((msg_buf.ExtId==458227712)||(msg_buf.ExtId==31462954))
      { 
        //if(p==0)
        //if(!((prev.ExtId==msg_buf.ExtId)&&(prev.Data[0]==msg_buf.Data[0])&&(prev.Data[1]==msg_buf.Data[1])&&(prev.Data[2]==msg_buf.Data[2])&&(prev.Data[3]==msg_buf.Data[3])&&(prev.Data[4]==msg_buf.Data[4])&&(prev.Data[5]==msg_buf.Data[5])&&(prev.Data[6]==msg_buf.Data[6])&&(prev.Data[7]==msg_buf.Data[7])))
        {
          //sprintf(s,"id: %11d %9d %3d %3d %3d %3d %3d %3d %3d %3d", msg_buf.StdId, msg_buf.ExtId, msg_buf.Data[0], msg_buf.Data[1], msg_buf.Data[2], msg_buf.Data[3], msg_buf.Data[4], msg_buf.Data[5], msg_buf.Data[6], msg_buf.Data[7]);
          //sprintf(s,"id: %11d %9d %3d %3d %3d %3d %3d %3d %3d %3d", msg_buf.StdId, msg_buf.ExtId, msg_buf.Data[0], msg_buf.Data[1], msg_buf.Data[2], msg_buf.Data[3], msg_buf.Data[4], msg_buf.Data[5], msg_buf.Data[6], msg_buf.Data[7]);
          //sprintf(s,"%11d %9d %c%c%c%c%c%c%c%c", msg_buf.StdId, msg_buf.ExtId, msg_buf.Data[0], msg_buf.Data[1], msg_buf.Data[2], msg_buf.Data[3], msg_buf.Data[4], msg_buf.Data[5], msg_buf.Data[6], msg_buf.Data[7]);
          /*if(msg_buf.Data[4]&&32==32) 
            send(3);*/
          a=msg_buf.ExtId;   
          b=a%16777216; a=a/16777216;
          c=b%65536;    b=b/65536;
          d=c%256;      c=c/256;
          
          //if(data[4]==52)
          if(msg_buf.Data[4]&32==32) 
          {
            //sprintf(s,"%c%c%c%c%c%c%c%c%c%c%c%cnew", a,b,c,d,msg_buf.Data[0], msg_buf.Data[1], msg_buf.Data[2], msg_buf.Data[3], msg_buf.Data[4], msg_buf.Data[5], msg_buf.Data[6], msg_buf.Data[7]);
            //sprintf(s,"%c%c%c%c%c%c%c%c",msg_buf.Data[0], msg_buf.Data[1], msg_buf.Data[2], msg_buf.Data[3], msg_buf.Data[4], msg_buf.Data[5], msg_buf.Data[6], msg_buf.Data[7]);
            //sprintf(s,"%c", msg_buf.Data[4]);
            //print(s);
          }
          
          //sprintf(s,"%c%c%c%cnew", a,b,c,d,msg_buf.Data[0], msg_buf.Data[1], msg_buf.Data[2], msg_buf.Data[3], msg_buf.Data[4], msg_buf.Data[5], msg_buf.Data[6], msg_buf.Data[7]); //%c%c%c%c%c%c%c%c%c%c%c
          //sprintf(s,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25); 
          send(20); send(20); //send(1); send(1);
          for(i=0;i<8;i++) {if(msg_buf.Data[i]==0) msg_buf.Data[i]=255;}
          if(a==0) a=255;
          if(b==0) b=255;
          if(c==0) c=255;
          if(d==0) d=255;
          sprintf(s,"%c%c%c%c",a,b,c,d);
          print(s);
          for(i=0;i<8;i++) {a=msg_buf.Data[i]; if(a==0) a=255; send(a);}
          //send(10); send(13);
        }
        /*prev.StdId=msg_buf.StdId;
        prev.ExtId=msg_buf.ExtId;
        prev.Data[0]=msg_buf.Data[0];
        prev.Data[1]=msg_buf.Data[1];
        prev.Data[2]=msg_buf.Data[2];
        prev.Data[3]=msg_buf.Data[3];
        prev.Data[4]=msg_buf.Data[4];
        prev.Data[5]=msg_buf.Data[5];
        prev.Data[6]=msg_buf.Data[6];
        prev.Data[7]=msg_buf.Data[7];*/
      }  
      //sprintf(s,"%9d", msg_buf.ExtId);
      /*id1=msg_buf.StdId;
      id2=msg_buf.ExtId;
      for (int i=0;i<8;i++) data[i]=msg_buf.Data[i];
      s="12345";
      //s=ltoa_(id2,s,12);
      print(s);
      send(10); send(13);
      //send('a');*/
      /*itoa(msg_buf.StdId); send(' '); itoa(msg_buf.ExtId); send(' ');
      itoa(msg_buf.Data[0]); send(' ');
      itoa(msg_buf.Data[1]); send(' ');
      itoa(msg_buf.Data[2]); send(' ');
      itoa(msg_buf.Data[3]); send(' ');
      itoa(msg_buf.Data[4]); send(' ');
      itoa(msg_buf.Data[5]); send(' ');
      itoa(msg_buf.Data[6]); send(' ');
      itoa(msg_buf.Data[7]); send(' ');
      char *s;
      sprintf(s,"%d",5);
      send(10); send(13);*/
      total++;      
    }
}

void init_gpio()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
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

int main(void)
{
  NVIC_Config();  
 
  /* Configures LED 1..4 */
  /*STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);*/
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Configure Push button key */
  //STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  /*GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/
  
  init_gpio();
   
  /* CAN configuration */
  CAN_Config();
  CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE);
  
  CAN_Config();
  CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE);
  
  USART2_Init();
  //send(0);

  //print(s);
  
/*  char s[50];
  send(' ');
  sprintf(s,"%d abc",5); 
  print(s);
  send(10); send(13);
  sprintf(s,"%d",28);
  print(s);*/
  
  /*TxMessage.StdId=134222717;
  TxMessage.ExtId=20984994;  
  TxMessage.Data[0] = 0;
  TxMessage.Data[1] = 0;
  TxMessage.Data[2] =64;
  TxMessage.Data[3] = 4;
  TxMessage.Data[4] = 7;
  TxMessage.Data[5] =34;
  TxMessage.Data[6] = 0;
  TxMessage.Data[7] = 0;*/
/*  TxMessage.StdId=134222929;
  TxMessage.ExtId=458227712;  
  TxMessage.Data[0] = 0;
  TxMessage.Data[1] = 0;
  TxMessage.Data[2] = 0;
  TxMessage.Data[3] = 0;
  TxMessage.Data[4] = 0;
  TxMessage.Data[5] = 0;
  TxMessage.Data[6] = 0;
  TxMessage.Data[7] = 0;
  CAN_Transmit(CANx, &TxMessage);*/
  
/*  TxMessage.StdId=134222929;
  TxMessage.ExtId=31462954;  
  TxMessage.Data[0] =224;
  TxMessage.Data[1] = 48;
  TxMessage.Data[2] = 0;
  TxMessage.Data[3] = 81;
  TxMessage.Data[4] = 4;
  TxMessage.Data[5] = 0;
  TxMessage.Data[6] = 25;
  TxMessage.Data[7] = 0;
  CAN_Transmit(CANx, &TxMessage);
  TxMessage.StdId=134222929;
  TxMessage.ExtId=31462954;  
  TxMessage.Data[0] = 0;
  TxMessage.Data[1] = 48;
  TxMessage.Data[2] = 0;
  TxMessage.Data[3] = 81;
  TxMessage.Data[4] = 4;
  TxMessage.Data[5] = 0;
  TxMessage.Data[6] =153;
  TxMessage.Data[7] = 0;
  CAN_Transmit(CANx, &TxMessage);
  TxMessage.StdId=134222929;
  TxMessage.ExtId=31462954;  
  TxMessage.Data[0] = 64;
  TxMessage.Data[1] = 48;
  TxMessage.Data[2] = 0;
  TxMessage.Data[3] = 81;
  TxMessage.Data[4] = 4;
  TxMessage.Data[5] = 0;
  TxMessage.Data[6] = 25;
  TxMessage.Data[7] = 0;
  CAN_Transmit(CANx, &TxMessage);*/
  //radio lock:
  //134222909 458227712   0   0   0   0   0   0   0   0
/*  TxMessage.StdId=134222909;
  TxMessage.ExtId=458227712;  
  TxMessage.Data[0] = 0;
  TxMessage.Data[1] = 0;
  TxMessage.Data[2] = 0;
  TxMessage.Data[3] = 0;
  TxMessage.Data[4] = 0;
  TxMessage.Data[5] = 0;
  TxMessage.Data[6] = 0;
  TxMessage.Data[7] = 0;
  CAN_Transmit(CAN2, &TxMessage);*/
  //134222909  31462954 224  48   0  64   7   0  25   0
/*  TxMessage.StdId=134222609;
  TxMessage.ExtId=31462954;  
  TxMessage.Data[0] =224;
  TxMessage.Data[1] =49;
  TxMessage.Data[2] = 0;
  TxMessage.Data[3] =64;
  TxMessage.Data[4] = 7;
  TxMessage.Data[5] = 0;
  TxMessage.Data[6] =25;
  TxMessage.Data[7] = 0;*/
  
  while(1)
  {
    //CAN_Transmit(CANx, &TxMessage);
    //Delay();
    /*if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)
    {
      LED_Display(++KeyNumber); if(KeyNumber==4)KeyNumber=0;
      //TxMessage.Data[0] = KeyNumber;
      //CAN_Transmit(CANx, &TxMessage);
      
      while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1);
    }*/
  }
/*  {
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)
    {
      if(KeyNumber == 0x4) 
      {
        KeyNumber = 0x00;
      }
      else
      {
        LED_Display(++KeyNumber);
        TxMessage.Data[0] = KeyNumber;
        CAN_Transmit(CANx, &TxMessage);
        Delay();
        
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1) {};
      }
    }
  }*/
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = ENABLE;//DISABLE;//один раз или бесконечно
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;//
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 125 kBps (CAN clocked at 42 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler = 42;
  CAN_Init(CANx, &CAN_InitStructure);
  
  //bit time=presc*t_apb_clk*(1+bs1+bs2)
  //baudrate=apb_freq/(presc*(1+bs1+bs2))
  //42000/x=125 => x=336=56*6=48*7=42*8=28*12

  /* CAN filter init */
#ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif  /* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
  CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE);
}

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

#ifdef  USE_CAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Initializes the Rx Message.
  * @param  RxMessage: pointer to the message to initialize
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}

/**
  * @brief  Turn ON/OFF the dedicated led
  * @param  Ledstatus: Led number from 0 to 3.
  * @retval None
  */
void LED_Display(uint8_t Ledstatus)
{
  /* Turn off all leds */
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDOff(LED5);
  STM_EVAL_LEDOff(LED6);
  
  switch(Ledstatus)
  {
    case(1): 
      STM_EVAL_LEDOn(LED4);
      break;
   
    case(2): 
      STM_EVAL_LEDOn(LED3);
      break;
 
    case(3): 
      STM_EVAL_LEDOn(LED5);
      break;

    case(4): 
      STM_EVAL_LEDOn(LED6);
      break;
    default:
      break;
  }
}

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay(void)
{
  uint16_t nTime = 0x0000;

  for(nTime = 0; nTime <0xFFF; nTime++)
  {
  }
}
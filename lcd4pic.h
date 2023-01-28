#pragma once
//********** modo 4 bits ****************************************************

/*#define LCD_DATA4_TRIS   (TRISD.RD4)   //Usando PORTB e PORTE
#define LCD_DATA4_IO     (PORTD.RD4)
#define LCD_DATA5_TRIS   (TRISD.RD5)
#define LCD_DATA5_IO     (PORTD.RD5)
#define LCD_DATA6_TRIS   (TRISD.RD6)
#define LCD_DATA6_IO     (PORTD.RD6)
#define LCD_DATA7_TRIS   (TRISD.RD7)
#define LCD_DATA7_IO     (PORTD.RD7)

#define LCD_RS_TRIS      (TRISE.RE0)
#define LCD_RS_IO        (PORTE.RE0)
#define LCD_EN_TRIS      (TRISE.RE1)
#define LCD_EN_IO        (PORTE.RE1)*/

#define LCD_Clear             (0x01)
#define LCD_Shift_LEFT        (0x18)
#define LCD_Shift_Right       (0x1C)
#define LCD_Cursor_OFF        (0X0C)
#define LCD_Cursor_Blink      (0X0D)
#define LCD_Cursor_Alt        (0X0F)
#define LCD_Cursor_Home       (0X02)
#define LCD_Shift_Left        (0X18)
#define LCD_Shift_Right        (0X1C)

#define LCD_DATA_PORT_IO      (PORTD)
#define LCD_DATA_PORT_TRIS    (TRISD)

#define LCD4bits   //escolha uma das duas opções
//#define LCD8bits

//********************************* LCD 16X2 ********************************
/*
DisplayLCD_Out(linha , Coluna, TextoLCD);
DisplayLCD_Out_Cp(TextoLCD);
DisplayLCD_Chr(Linha, Coluna, TChar);
DisplayLCD_Chr_Cp(TChar);
DisplayLCD_Cmd(CmdLCD);
DisplayLCD_Init(VOID)
*/
//****************************************************************************
//Protótipo de função
void DisplayLCD_Out(unsigned char Linha, unsigned char Coluna, unsigned char* TextoLCD);
void DisplayLCD_Out_Cp(unsigned char* TextoLCD);
void DisplayLCD_Chr(unsigned char Linha, unsigned char Coluna, unsigned char TChar);
void DisplayLCD_Chr_Cp(unsigned char TChar);
void DisplayLCD_Cmd(unsigned char Data);
void DisplayLCD_Init(); //modo 4bits ou 8bits
void LCD_Enable();
void LCD_Write_Data(unsigned char RS, unsigned char Data);
void DisplayLCD_Out_Cp(unsigned char* Ponteiro);


//****************************Funções
void LCD_Write_Data(unsigned char RS, unsigned char Data)
{
    //*****************************************************************************
    //exemplo: Data   10110010
    //              & 00010000 (ou 0x10;  observação: lembre-se que & é bit a bit)
    //                --------
    //              = 00010000
    //agora
    //                00010000
    //             == 00010000 (ou 0x10)
    //                --------
    //              = 1        (retorna 1)
    //*****************************************************************************

    LCD_RS_IO = RS;     //0
#ifdef LCD4bits
    Delay_ms(10);
    LCD_DATA4_IO = ((Data & 0x10) == 0x10);
    LCD_DATA5_IO = ((Data & 0x20) == 0x20);
    LCD_DATA6_IO = ((Data & 0x40) == 0x40);
    LCD_DATA7_IO = ((Data & 0x80) == 0x80);

    LCD_Enable(void);

    Data = Data << 4; //Rotaciona o nibble o LSB para a posição MSB
    LCD_DATA4_IO = ((Data & 0x10) == 0x10);
    LCD_DATA5_IO = ((Data & 0x20) == 0x20);
    LCD_DATA6_IO = ((Data & 0x40) == 0x40);
    LCD_DATA7_IO = ((Data & 0x80) == 0x80);

    LCD_Enable(void);
#endif
#ifdef LCD8bits
    LCD_DATA_PORT_IO = Data;
    LCD_Enable(void);
#endif
}

void LCD_Enable(void)  //INSTRUÇÃO DE CLOCK PARA O LCD
{
    Delay_us(70);
    LCD_EN_IO = 1;
    Delay_us(70);
    LCD_EN_IO = 0;
    Delay_us(70);
}

void DisplayLCD_Cmd(unsigned char CmdLCD)
{
    LCD_Write_Data(0, CmdLCD);     //RS = 0, -> COMANDO A SER PROCESSADO
}

void DisplayLCD_Chr_Cp(unsigned char Tchar)
{
    LCD_Write_Data(1, Tchar);    //RS = 1 -> DADO A SER ESCRITO
}

//funções
void DisplayLCD_Init() { //modo 4bits ou 8bits

#ifdef LCD8bits
    LCD_DATA_PORT_IO = 0;      //BARRAMENTO DO LCD EM ZERO
    LCD_DATA_PORT_TRIS = 0;    //PINOS DO MCU COMO SAIDA
    LCD_EN_TRIS = 0; LCD_RS_IO = 0;
    LCD_RS_TRIS = 0; LCD_RS_IO = 0;
    //..... COMANDOS PARA INICIALIZAÇÃO DO LCD *********
    Delay_ms(40); //aguarda Reset do Display LCD
    DisplayLCD_Cmd(0x38);
    Delay_ms(15);
    DisplayLCD_Cmd(0x38);
    Delay_ms(15);
    DisplayLCD_Cmd(0x06);
    Delay_ms(15);
    DisplayLCD_Cmd(0x0E);
    Delay_ms(15);
    DisplayLCD_Cmd(0x01);
    Delay_ms(15);
#endif
#ifdef LCD4bits

    LCD_DATA4_TRIS = 0; LCD_DATA4_IO = 0;
    LCD_DATA5_TRIS = 0; LCD_DATA5_IO = 0;
    LCD_DATA6_TRIS = 0; LCD_DATA6_IO = 0;
    LCD_DATA7_TRIS = 0; LCD_DATA7_IO = 0;
    LCD_EN_TRIS = 0; LCD_RS_IO = 0;
    LCD_RS_TRIS = 0; LCD_RS_IO = 0;

    Delay_ms(100);  //Tempo necessário para inicialização do LCD após power-on

    LCD_DATA4_IO = 1;
    LCD_DATA5_IO = 1;
    LCD_Enable();
    LCD_Enable();
    LCD_Enable();

    Delay_ms(15);

    LCD_DATA4_IO = 0;
    LCD_DATA5_IO = 1;
    LCD_Enable();
    LCD_Enable();
    LCD_Enable();

    Delay_ms(15);


    DisplayLCD_Cmd(0X28); //2X linhas 7x5 em modo 4bits
    DisplayLCD_Cmd(0X0C);
    DisplayLCD_Cmd(0X06);
    DisplayLCD_Cmd(0X01);
#endif

}

//DisplayLCD_Out(1,2, "BOM DIA");
void DisplayLCD_Out(unsigned char Linha, unsigned char Coluna,
    unsigned char* TextoLCD)
{
    LCD_RS_IO = 0; //TRATAR PRIMEIRO AS COORDENADAS X E Y
    switch (Linha)
    {
    case 1: {
        Coluna = Coluna + 0X7F;  // 2 + 0X7F = 0X81
        LCD_Write_Data(0, Coluna); //Comando, visto que estamos posicionando o cursor
        //LCD_Enable();
        break;
    }
    case 2: {
        Coluna = Coluna + 0xBF;  //2 + 0xBF = 0XC1
        LCD_Write_Data(0, Coluna);
        //LCD_Enable();
        break;
    }
    }
    DisplayLCD_Out_Cp(TextoLCD); //Escreve no LCD "BOM DIA"
}

//'A' = 65
//"A" != 65 ???
//"A" = 'A' + '\0' OU  'A' + 0


//DisplayLCD_Out_Cp("BOM DIA");
void DisplayLCD_Out_Cp(unsigned char* Ponteiro)
{
    // LCD_RS_IO = 1;  //ESCREVER UM DADO
    while (*Ponteiro) //loop até encontrar o caractere NULL (ZERO)
    {
        LCD_Write_Data(1, *Ponteiro);  //RS =1, ESCREVENDO NO LCD
        //LCD_Enable();
        ++Ponteiro;
    }

}


//DisplayLCD_Chr_Cp(1,6,'A');
void DisplayLCD_Chr(unsigned char Linha, unsigned char Coluna,
    unsigned char Tchar)
{
    LCD_RS_IO = 0; //TRATAR PRIMEIRO AS COORDENADAS X E Y
    switch (Linha)
    {
    case 1: {
        Coluna = Coluna + 0x7F;
        LCD_Write_Data(0, Coluna);
        //LCD_Enable();
        break;
    }
    case 2: {
        Coluna += 0xBF;
        LCD_Write_Data(0, Coluna);
        //LCD_Enable();
        break;
    }
    }
    DisplayLCD_Chr_Cp(Tchar); //ESCREVE NO LCD

}
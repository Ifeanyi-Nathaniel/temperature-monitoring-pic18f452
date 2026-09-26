/*
 * Temperature Monitoring Circuit — PIC18F452 + LM35 + 16x2 LCD
 * ---------------------------------------------------------------
 * Course:   EEE572, Dept. of Electrical/Electronics Engineering
 * Platform: mikroC PRO for PIC
 * Tool:     Simulated in Proteus 8 Professional
 *
 * Reads an analog voltage from an LM35 temperature sensor on AN0,
 * converts it to degrees Celsius, and displays the live reading
 * on a 16x2 character LCD.
 */

// Define LCD connections to PORTD
// This must match your Proteus circuit
sbit LCD_RS at RD1_bit;
sbit LCD_EN at RD2_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

// Define pin directions for the LCD connections
sbit LCD_RS_Direction at TRISD1_bit;
sbit LCD_EN_Direction at TRISD2_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;

// --- Main Program ---
void main() {
    unsigned int adc_value;
    float temperature;
    char temp_text[16];   // Buffer to hold the temperature string

    // Configure AN0/RA0 as an analog input
    // The rest of PORTA will be digital
    ADCON1 = 0x0E;
    TRISA  = 0x01;        // Set RA0 as an input pin

    // Initialize the LCD library
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);       // Clear the display
    Lcd_Cmd(_LCD_CURSOR_OFF);  // Turn the cursor off
    Lcd_Out(1, 1, "Temperature:"); // Write "Temperature:" on the first line

    while (1) {
        // Read the analog value from channel 0 (AN0)
        adc_value = ADC_Read(0);

        // Convert the 10-bit ADC value to temperature in Celsius
        // Formula: Temperature = (ADC_Value * Vref / 1024) / (10mV/°C)
        // Simplified: Temperature = (adc_value * 500.0) / 1023.0
        temperature = (adc_value * 500.0) / 1023.0;

        // Convert the floating-point temperature value to a string
        FloatToStr(temperature, temp_text);
        temp_text[4] = 0;   // Keep only one decimal place (e.g., "25.3")

        // Display the temperature on the second line of the LCD
        Lcd_Out(2, 1, temp_text);
        Lcd_Chr_CP(' ');      // Write a space
        Lcd_Chr_CP(223);      // Write the degree symbol '°'
        Lcd_Chr_CP('C');      // Write 'C'

        Delay_ms(500);        // Wait for half a second before the next reading
    }
}

#include "Platform_Types.h"
#include "tm4c123gh6pm.h"

#define PWM_FREQUENCY  1000  // Set default PWM frequency to 1kHz
#define SERVO_PWM_FREQUENCY  50  // Standard servo PWM frequency (50Hz)

// Function to initialize PWM on a given module and pin
void PWM_Init(uint32 pwm_module, uint32 pwm_pin, uint32 frequency) {
    SYSCTL_RCGCPWM_R |= (1 << pwm_module);  // Enable PWM module
    SYSCTL_RCGCGPIO_R |= (1 << pwm_pin);    // Enable GPIO module for PWM pin

    // Configure GPIO pin for PWM function
    GPIO_PORTB_AFSEL_R |= (1 << pwm_pin);  // Enable alternate function
    GPIO_PORTB_PCTL_R &= ~(0xF << (pwm_pin * 4));
    GPIO_PORTB_PCTL_R |= (0x4 << (pwm_pin * 4)); // Assign PWM function
    GPIO_PORTB_DEN_R |= (1 << pwm_pin);  // Enable digital function

    // Disable PWM Generator before configuring
    PWM0_0_CTL_R &= ~(1 << 0);

    // Set PWM frequency
    uint32 load_value = (16000000 / frequency) - 1;  // Assuming 16MHz system clock
    PWM0_0_LOAD_R = load_value;

    // Set PWM duty cycle (default 50%)
    PWM0_0_CMPA_R = load_value / 2;

    // Enable PWM generator and output
    PWM0_0_CTL_R |= (1 << 0);
    PWM0_ENABLE_R |= (1 << 0);
}

// Function to set PWM duty cycle
void PWM_SetDutyCycle(uint32 pwm_module, uint32 pwm_pin, float duty_cycle) {
    if (duty_cycle > 100.0) duty_cycle = 100.0;
    if (duty_cycle < 0.0) duty_cycle = 0.0;

    uint32 load_value = PWM0_0_LOAD_R;
    PWM0_0_CMPA_R = (uint32)((duty_cycle / 100.0) * load_value);
}

// Function to rotate a servo motor to a specific angle (0 to 180 degrees)
void Servo_SetAngle(uint32 pwm_module, uint32 pwm_pin, float angle) {
    if (angle > 180.0) angle = 180.0;
    if (angle < 0.0) angle = 0.0;

    // Convert angle to duty cycle (Assuming 1ms = 0° and 2ms = 180°)
    float duty_cycle = ((angle / 180.0) * 5.0) + 5.0; // Map to 5% - 10% duty cycle
    PWM_SetDutyCycle(pwm_module, pwm_pin, duty_cycle);
}

// Main function to test PWM with LED on PB4 and Servo Motor
int main() {
    SYSCTL_RCGCGPIO_R |= (1 << 1); // Enable GPIOB for LED
    GPIO_PORTB_DIR_R |= (1 << 4); // Set PB4 as output
    GPIO_PORTB_DEN_R |= (1 << 4); // Enable digital function for PB4

    PWM_Init(0, 6, PWM_FREQUENCY); // Initialize PWM on module 0, pin 6
    PWM_Init(0, 7, SERVO_PWM_FREQUENCY); // Initialize PWM for servo motor on module 0, pin 7
    volatile int i;
    while (1) {
        PWM_SetDutyCycle(0, 6, 20.0); // 50% duty cycle
        GPIO_PORTB_DATA_R = (1 << 4); // Turn on PB4 LED
        Servo_SetAngle(0, 7, 90); // Move servo to 90 degrees
        for (i = 0; i < 1000000; i++);
    }
    return 0;
}

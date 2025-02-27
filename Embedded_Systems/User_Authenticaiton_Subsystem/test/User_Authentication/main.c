// main.c - Main file using the R307 fingerprint sensor driver with enrollment and authentication flow

#include "R307_Finger_Print_Driver/R307.h"   // R307 driver header
#include "Servo_Motor_Driver/Servo_Motor.h"

// Delay function
void delay_ms(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (364 * n) );
}

int main(void)
{
    Servo_Init();     // Initialize servo motor for door control
    R307_init();      // Initialize UART and R307 fingerprint sensor

    // -------------------------------------------------
    // Enrollment Mode: Enroll a New Fingerprint
    // -------------------------------------------------
    uint8 enrollment_attempts = 0;

    while (enrollment_attempts < 3) {  // Allow max 3 attempts to enroll
        enrollment_attempts++;

        // Prompt user to place their finger
        while (!R307_captureFinger()) {
            // Wait for a valid fingerprint scan
        }

        if (!R307_image2Tz(1)) {
            continue;  // Restart if conversion fails
        }

        // Wait for user to remove finger
        delay_ms(2000);

        // Second Scan
        while (!R307_captureFinger()) {
        }

        if (!R307_image2Tz(2)) {
            continue;  // Restart if conversion fails
        }

        // Combine templates into a fingerprint model
        if (!R307_createModel()) {
            continue;
        }

        // Store fingerprint model in database (Page 1)
        if (R307_storeModel(1, 1)) {
            break;  // Exit loop if enrollment succeeds
        }
    }

    // Lock the door initially
    Servo_Rotation();

    // -------------------------------------------------
    // Authentication Mode: Wait Until the Right Finger is Authenticated
    // -------------------------------------------------
    while (1)
    {
        // Wait for a fingerprint scan
        while (!R307_captureFinger()) {
        }

        // Convert captured fingerprint to a template
        if (!R307_image2Tz(1)) {
            continue;
        }

        // Search for the fingerprint in the database
        uint16 match_page = 0;
        uint16 match_score = 0;
        if (R307_searchFingerprint(1, 1, &match_page, &match_score)) {
            // Correct fingerprint detected → Unlock the door
            Servo_Rotation();

            // Keep the door unlocked for 3 seconds
            Delay_MS(3000);

            // Then lock the door again
            Servo_Rotation();
        }

        // Small delay before accepting new scans
        Delay_MS(2000);
    }
}

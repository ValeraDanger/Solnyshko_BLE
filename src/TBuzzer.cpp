#include "TBuzzer.h"
#include <Arduino.h>

#define NOTE_C6  1047
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define BUZZER_PIN 25

TBuzzer::TBuzzer() {
    this->melody = {NOTE_G7, NOTE_C7, NOTE_E7, NOTE_C8};
}

void TBuzzer::pauseBeep() {
    for (int thisNote: melody) {
        int noteDuration = 1000 / noteDurations;
        tone(BUZZER_PIN, thisNote, noteDuration);

        int pauseBetweenNotes = noteDuration * 1.30;
        vTaskDelay(pdMS_TO_TICKS(pauseBetweenNotes));       //THIS CAN LEAD TO SOUND ARTIFACTS! CHANGE TO REGULAR DELAY IN THIS CASE

    }
    noTone(BUZZER_PIN);
}

TBuzzer Buzzer;
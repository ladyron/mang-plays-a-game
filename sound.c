#include "gba.h"
#include "sound.h"

void setupSounds() {

    REG_SOUNDCNT_X = SND_ENABLED;

	REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_TO_BOTH |
                     DSA_TIMER0 |
                     DSA_FIFO_RESET |
                     DSB_OUTPUT_RATIO_100 |
                     DSB_OUTPUT_TO_BOTH |
                     DSB_TIMER1 |
                     DSB_FIFO_RESET;

	REG_SOUNDCNT_L = 0;

}

void playSoundA(const signed char* sound, int length, int looping) {

    // Set DMA channel to 1
    dma[1].cnt = 0;
    DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    // Set up timer 0
    REG_TM0CNT = 0;
    int ticks = (PROCESSOR_CYCLES_PER_SECOND / SOUND_FREQ);
    REG_TM0D = -ticks;
    REG_TM0CNT = TIMER_ON;

    // typedef struct{
    //     const signed char* data;
    //     int length;
    //     int frequency;
    //     int isPlaying;
    //     int looping;
    //     int duration;
    //     int priority;
    //     int vBlankCount;
    // } SOUND;

    // Initialize struct members of soundA
    soundA.data = sound;
    soundA.length = length;
    soundA.frequency = SOUND_FREQ;
    soundA.isPlaying = 1; //true
    soundA.looping = looping;
    soundA.duration = ((VBLANK_FREQ * length) / SOUND_FREQ);
    soundA.vBlankCount = 0;

}

void playSoundB(const signed char* sound, int length, int looping) {

    // Set DMA channel to 2
    dma[2].cnt = 0;
    DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    // Set up timer 1
    REG_TM1CNT = 0;
    int ticks = PROCESSOR_CYCLES_PER_SECOND / SOUND_FREQ;
    REG_TM1D = -ticks;
    REG_TM1CNT = TIMER_ON;

    // Initialize struct members of soundB
    soundB.data = sound;
    soundB.length = length;
    soundB.frequency = SOUND_FREQ;
    soundB.isPlaying = 1;
    soundB.looping = looping;
    soundB.duration = ((VBLANK_FREQ * length) / SOUND_FREQ);
    soundB.vBlankCount = 0;
}

void pauseSounds() {
    soundA.isPlaying = 0;
    soundB.isPlaying = 0;

    REG_TM0CNT = TIMER_OFF;
    REG_TM1CNT = TIMER_OFF;
}

void unpauseSounds() {
    soundA.isPlaying = 1;
    soundB.isPlaying = 1;

    REG_TM0CNT = TIMER_ON;
    REG_TM1CNT = TIMER_ON;
}

void stopSounds() {
    soundA.isPlaying = 0;
    soundB.isPlaying = 0;

    dma[1].cnt = 0;
    dma[2].cnt = 0;
    
    REG_TM0CNT = 0;
    REG_TM1CNT = 0;
}

// TODO 2.1: Write this function
void setupInterrupts() {

    // Disable interrupt master enable register
    REG_IME = 0;

    // Enable VBlank interrupts
    REG_IE |= INT_VBLANK;
    REG_DISPSTAT |= INT_VBLANK_ENABLE;
    // TODO 5.3: Add bits for timer interrupts (timer 2 and timer 3)
    REG_IE |= INT_TM2 | INT_TM3;
    // Set interrupt handling function
    REG_INTERRUPT = interruptHandler;
    // Re-enable interrupt master enable register
    REG_IME = 1;

}

// TODO 2.2: Write this function
void interruptHandler() {

    // Disable interrupt master enable register
    REG_IME = 0;

    // If interrupt flag register identifies a VBlank interrupt
    if (REG_IF & INT_VBLANK) {
        
        // Handle soundA
        if (soundA.isPlaying) {
            soundA.vBlankCount++;
            if (soundA.vBlankCount >= soundA.duration) {
                if (soundA.looping) {
                    playSoundA(soundA.data, soundA.length, soundA.looping);
                } else {
                    soundA.isPlaying = 0;
                    dma[1].cnt = 0;
                    REG_TM0CNT = 0;
                }
            }
        }

        // Handle soundB
        if (soundB.isPlaying) {
            soundB.vBlankCount++;
            if (soundB.vBlankCount >= soundB.duration) {
                if (soundB.looping) {
                    playSoundA(soundB.data, soundB.length, soundB.looping);
                } else {
                    soundB.isPlaying = 0;
                    dma[2].cnt = 0;
                    REG_TM1CNT = 0;
                }
            }
        }

    // Notify GBA that the interrupt has been handled
        REG_IF = REG_IF;
    }

	// Re-enable interrupt master enable register
    REG_IME = 1;        

}
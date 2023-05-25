#include <stdlib.h>
#include "gba.h" // Mode 0 Scaffold
#include "print.h"
#include "startBg.h"
#include "startBg1.h"
#include "startBg2.h"
#include "pauseBg1.h"
#include "pauseBg2.h"
#include "gameBg.h"
#include "winBg.h"
#include "loseBg.h"
#include "game.h"
#include "instructionsBg.h"
#include "sound.h"
#include "pick.h"
#include "select.h"
#include "gameLose.h"
#include "gameWin.h"
#include "gameSong.h"
#include "menuTheme.h"
#include "spritesheet.h"
#include "parallax1.h"

void initialize();
void setupSounds();
void setupInterrupts();

void goToStart();
void goToStart2();
void goToInstructions();
void goToGame();
void goToPause();
void goToPause2();
void goToWin();
void goToLose();

void start();
void start2();
void instructions();
void game();
void pause();
void pause2();
void win();
void lose();

enum {START1, START2, INSTRUCTIONS, GAME, PAUSE1, PAUSE2, WIN, LOSE};
int state;

int screenBlock;

int pauser;

unsigned short buttons;
unsigned short oldButtons;

int seed;

OBJ_ATTR shadowOAM[128];

int main() {
    
    initialize();
    setupSounds();
    setupInterrupts();

    while(1) {
        oldButtons = buttons;
        buttons = BUTTONS;

         switch (state) {
            case START1:
                start();
                break;
            case START2:
                start2();
                break;
            case INSTRUCTIONS:
                instructions();
                break;
            case GAME:
                game();
                break;
            case PAUSE1:
                pause();
                break;
            case PAUSE2:
                pause2();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;    
        }
    }
}

void initialize() {

    // REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | SPRITE_ENABLE;
    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE;

    REG_BG1CNT = BG_8BPP | BG_SIZE_LARGE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(29);
    // REG_BG2CNT = BG_8BPP| BG_SIZE_SMALL | BG_CHARBLOCK(2) | BG_SCREENBLOCK(31);

    buttons = BUTTONS;
    oldButtons = 0;
    screenBlock = 28;

    pauser = 0;

    goToStart();
    playSoundA(menuTheme_data, menuTheme_length, 1);
}

void goToStart() {

    DMANow(3, &startBg1Pal, PALETTE, 256);
    DMANow(3, &startBg1Tiles, &CHARBLOCK[0], startBg1TilesLen / 2);
    DMANow(3, &startBg1Map, &SCREENBLOCK[29], startBg1MapLen / 2);

    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);

    state = START1;

    seed = 0;
}

void start() {
    seed++;

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);

    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(select_data, select_length, 0);
        srand(seed);
        goToGame();
        initGame();
    }
    if (BUTTON_PRESSED(BUTTON_DOWN)) {
        playSoundB(pick_data, pick_length, 0);
        goToStart2();
    }
    if(BUTTON_PRESSED(BUTTON_START)) {
        goToWin();
    }
}

void goToStart2() {


    DMANow(3, &startBg2Pal, PALETTE, 256);
    DMANow(3, &startBg2Tiles, &CHARBLOCK[0], startBg2TilesLen / 2);
    DMANow(3, &startBg2Map, &SCREENBLOCK[29], startBg2MapLen / 2);

    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);

    state = START2;

    seed = 0;

}

void start2() {

    seed++;

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);
    
    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(select_data, select_length, 0);
        goToInstructions();
    }
    if (BUTTON_PRESSED(BUTTON_UP)) {
        playSoundB(pick_data, pick_length, 0);
        goToStart();
    }  
}

void goToInstructions() {

    DMANow(3, &instructionsBgPal, PALETTE, 256);
    DMANow(3, &instructionsBgTiles, &CHARBLOCK[0], instructionsBgTilesLen / 2);
    DMANow(3, &instructionsBgMap, &SCREENBLOCK[29], instructionsBgMapLen / 2);

    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);

    state = INSTRUCTIONS;

    seed = 0;

}

void instructions() {
    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(select_data, select_length, 0);
        goToStart();
    }
}

void goToGame() {    
    // waitForVBlank();

    DMANow(3, &gameBgPal, PALETTE, 256);
    DMANow(3, &gameBgTiles, &CHARBLOCK[0], gameBgTilesLen / 2);
    DMANow(3, &gameBgMap, &SCREENBLOCK[29], gameBgMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    pauser = 0;
    
    if (pauser == 0) {
        // stopSounds();
        playSoundA(gameSong_data, gameSong_length, 1);
    } else {
        stopSounds();
    }

    DMANow(3, &spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    DMANow(3, &spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128 * 4);

    state = GAME;
}

void game() {
    updateGame();
    drawGame();
    
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);

    if (BUTTON_PRESSED(BUTTON_START)) {
        pauser = 1;
        goToPause();
    }
    if (ballsLeft == 0) {
        goToWin();
    }
    if (mang.health == 0) {
        goToLose(); // test
    }
    if (van.health <= 0 && cooky.health <= 0 && koya.health <= 0) {
        goToWin();
    }

}

void goToPause() {
    DMANow(3, &pauseBg1Pal, PALETTE, 256);
    DMANow(3, &pauseBg1Tiles, &CHARBLOCK[0], pauseBg1TilesLen / 2);
    DMANow(3, &pauseBg1Map, &SCREENBLOCK[29], pauseBg1MapLen / 2);
    
    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);
    stopSounds();
    playSoundB(pick_data, pick_length, 0);

    state = PAUSE1;
}

void pause() {
    // waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_RIGHT)) {
        pauser = 1;
        playSoundB(pick_data, pick_length, 0);
        goToPause2();
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
        pauser = 0;
        playSoundB(select_data, select_length, 0);
        goToGame();
    }
    // pauseSounds();
}

void goToPause2() {
    DMANow(3, &pauseBg2Pal, PALETTE, 256);
    DMANow(3, &pauseBg2Tiles, &CHARBLOCK[0], pauseBg2TilesLen / 2);
    DMANow(3, &pauseBg2Map, &SCREENBLOCK[29], pauseBg2MapLen / 2);
    
    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);

    state = PAUSE2;
} 

void pause2() {

    if (BUTTON_PRESSED(BUTTON_LEFT)) {
        pauser = 1;
        playSoundB(pick_data, pick_length, 0);
        goToPause();
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
        pauser = 1;
        playSoundB(select_data, select_length, 0);
        goToStart();
        playSoundA(menuTheme_data, menuTheme_length, 1);
    }
    // pauseSounds();
}
void goToWin() {

    // REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
    // REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(29) | BG_SIZE_LARGE | 1;
    // REG_BG0CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_SIZE_LARGE;
        DMANow(3, &winBgPal, PALETTE, 256);
        DMANow(3, &winBgTiles, &CHARBLOCK[0], winBgTilesLen / 2);
        DMANow(3, &winBgMap, &SCREENBLOCK[29],winBgMapLen / 2);

    // DMANow(3, &winBgPal, PALETTE, 256);
    // DMANow(3, &winBgTiles, &CHARBLOCK[0], parallax1TilesLen / 2);
    // DMANow(3, &winBgMap, &SCREENBLOCK[29], parallax1MapLen / 2);

    // hOff = 0;

    // if (BUTTON_HELD(BUTTON_LEFT)) {
    //     hOff--;
    // }
    // if (BUTTON_HELD(BUTTON_RIGHT)) {
    //     hOff++;
    // }

    // Parallax 1
    // DMANow(3, &parallax1Pal, PALETTE, 256);
    // DMANow(3, &parallax1Tiles, &CHARBLOCK[1], parallax1TilesLen / 2);
    // DMANow(3, &parallax1Map, &SCREENBLOCK[30], parallax1PalLen / 2);

    // REG_BG1HOFF = hOff;
    // REG_BG1VOFF = vOff / 4;

    // Parallax 2
    
    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);
    playSoundA(gameWin_data, gameWin_length, 0);

    state = WIN;
}

void win() {
    // waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(select_data, select_length, 0);
        goToStart();
        playSoundA(menuTheme_data, menuTheme_length, 1);
    }
}

void goToLose() {

    DMANow(3, &loseBgPal, PALETTE, 256);
    DMANow(3, &loseBgTiles, &CHARBLOCK[0], loseBgTilesLen / 2);
    DMANow(3, &loseBgMap, &SCREENBLOCK[29], loseBgMapLen / 2);
    
    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128 * 4);
    playSoundA(gameLose_data, gameLose_length, 0);

    state = LOSE;
}

void lose() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(select_data, select_length, 0);
        goToStart();
        playSoundA(menuTheme_data, menuTheme_length, 1);
    }
}
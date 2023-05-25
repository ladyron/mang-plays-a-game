#include <stdlib.h>
#include "gba.h"
#include "game.h"
#include "spritesheet.h"
#include "print.h"
#include "sound.h"
#include "throw.h"
#include "pick.h"
#include "select.h"
#include "hitPlayer.h"
#include "hitEnemy.h"
#include "allDown.h"

OBJ_ATTR shadowOAM[128];

int hit;
int ballsLeft;
int delay;
int runTimer;
int hOff;
int vOff;

MANG mang;
ANISPRITE ball[BALLCOUNT];
ENEMY van;
ENEMY cooky;
ENEMY koya;
ANISPRITE vanBall[BALLCOUNT];
ANISPRITE cookyBall[BALLCOUNT];
ANISPRITE koyaBall[BALLCOUNT];

enum {SPRITEFRONT, SPRITEBACK, SPRITERIGHT, SPRITELEFT, SPRITEIDLE};

void initGame() {
    mgba_open();
    mgba_printf("Debugging Initialized");	
    // DMANow(3, &spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    // DMANow(3, &spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    // hideSprites();

    vOff = 0;
    hOff = 0;

    initMang();
    initBall();
    initVan();
    initCooky();
    initKoya();

    initVanBall();
    initCookyBall();
    initKoyaBall();

    ballsLeft = BALLCOUNT;
    mgba_open();
    mgba_printf("Debugging Initialized");	
    mgba_printf_level(MGBA_LOG_DEBUG, "mang.health, van.health (%d, %d)", mang.health, van.health);
    // delay = 5;
}

void updateGame() {
    updateMang();
    for (int i = 0; i < BALLCOUNT; i++) {
        updateBall(&ball[i]);
    }
    updateVan();
    for (int i = 0; i < BALLCOUNT; i++) {
        updateVanBall(&vanBall[i]);
    }
    updateCooky();
    for (int i = 0; i < BALLCOUNT; i++) {
        updateCookyBall(&cookyBall[i]);
    }
    updateKoya();
    for (int i = 0; i < BALLCOUNT; i++) {
        updateKoyaBall(&koyaBall[i]);
    }
    mgba_printf_level(MGBA_LOG_DEBUG, "mang, van, cooky, koya (%d, %d, %d, %d)", mang.health, van.health, cooky.health, koya.health);
}

void drawGame() {

    drawMang();
    for (int i = 0; i < BALLCOUNT; i++) {
        drawBall(&ball[i]);
    }
    drawVan();
    for (int i = 0; i < BALLCOUNT; i++) {
        drawVanBall(&vanBall[i]);
    }
    drawCooky();
    for (int i = 0; i < BALLCOUNT; i++) {
        drawCookyBall(&cookyBall[i]);
    }
    drawKoya();
    for (int i = 0; i < BALLCOUNT; i++) {
        drawKoyaBall(&koyaBall[i]);
    }

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);



    REG_BG0HOFF = hOff;
    REG_BG1VOFF = vOff;
}

void initMang() {
    mang.width = 20;
	mang.height = 20;
	mang.cdel = 3;
	mang.rdel = 3;
	mang.worldRow = SCREENHEIGHT / 2 - (mang.width / 2);
	mang.worldCol = SCREENWIDTH / 2 - (mang.height / 2);
	mang.aniCounter = 0;
	mang.curFrame = 0;
	mang.numFrames = 3;
	mang.aniState = SPRITEFRONT;
    mang.ballTimer = 10;
	// mang.isHit = 0;
	mang.health = 3;
}

void updateMang() {
    if (BUTTON_HELD(BUTTON_UP) && !BUTTON_HELD(BUTTON_LEFT) && !BUTTON_HELD(BUTTON_RIGHT)
		&& mang.worldRow + mang.height >= SCREENHEIGHT - 150) {
		mang.aniState = SPRITEBACK;
		mang.worldRow -= mang.rdel;
		// mang.cDirection = 0;
        if (vOff > 0 && (mang.worldRow - vOff) <= SCREENHEIGHT / 2 ) {
            vOff--;
        }
	}
	if (BUTTON_HELD(BUTTON_DOWN) && !BUTTON_HELD(BUTTON_RIGHT) && !BUTTON_HELD(BUTTON_LEFT)
		&& mang.worldRow + mang.height < SCREENHEIGHT) {
		mang.aniState = SPRITEFRONT;
		mang.worldRow += mang.rdel;
		// vOff++
        if (vOff + SCREENHEIGHT < MAPHEIGHT && (mang.worldRow - vOff) >= SCREENHEIGHT / 2) {
               vOff++;
        }
	}
	if (BUTTON_HELD(BUTTON_LEFT)
		&& mang.worldCol + mang.width >= 20) {
        mang.aniState = SPRITELEFT;
		mang.worldCol -= mang.cdel;
		// mang.cDirection = -1;
        // hOff--;
        if (hOff > 0 && (mang.worldCol - hOff) <= SCREENWIDTH / 2) {
            hOff--;
        }
    }
    if (BUTTON_HELD(BUTTON_RIGHT)
		&& mang.worldCol + mang.width - 1 < SCREENWIDTH) {
        mang.aniState = SPRITERIGHT;
		mang.worldCol += mang.cdel;
		// mang.cDirection = 0;
       if (hOff + SCREENWIDTH < MAPWIDTH && (mang.worldCol - hOff) >= SCREENWIDTH / 2) {
            hOff++;
        }
    }
    if (BUTTON_PRESSED(BUTTON_B)) {
        throwBall();
        playSoundB(throw_data, throw_length, 0);
        mang.ballTimer = 0;
    }
    // add timer to this
    if (BUTTON_HELD(BUTTON_A)) {
        runTimer = 0;
        mang.cdel += 2; // 5/4/3
        mang.rdel += 2;
    } else {
        mang.cdel = 3;
        mang.rdel = 3;
    }
    if (mang.health == 2) {
        mang.cdel -= 1;
        mang.rdel -= 1;
    }
    if (mang.health == 1) {
        mang.cdel -= 2;
        mang.rdel -= 2;
    }
    // cheat
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        koya.health = 1;
        van.health = 1;
        cooky.health = 1;
        mang.rdel += 2;
        mang.cdel += 2;
        playSoundB(allDown_data, allDown_length, 0);
    }
    animateMang();
    mang.ballTimer++;
}

void animateMang() {

    mang.prevAniState = mang.aniState;
	mang.aniState = SPRITEIDLE;

    if (mang.aniCounter % 15 == 0) {
        mang.curFrame = (mang.curFrame + 1) % mang.numFrames;
    }

// enum {SPRITEFRONT, SPRITEBACK, SPRITERIGHT, SPRITELEFT, SPRITEIDLE};
    if (BUTTON_HELD(BUTTON_UP)){
        mang.aniState = 1;
    }

    if (BUTTON_HELD(BUTTON_DOWN)){
        mang.aniState = 0;
    }

    if (BUTTON_HELD(BUTTON_LEFT)){
        mang.aniState = 3;
    }

    if (BUTTON_HELD(BUTTON_RIGHT)){
        mang.aniState = 2;
    }

    if (mang.aniState == SPRITEIDLE) {
        mang.curFrame = 0;
        mang.aniCounter = 0;
        mang.aniState = mang.prevAniState;
    } else {
        mang.aniCounter++;
    }
}
void drawMang() {
    shadowOAM[0].attr0 = (ROWMASK & (mang.worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
    shadowOAM[0].attr1 = (COLMASK & (mang.worldCol - hOff)) | ATTR1_MEDIUM;
    shadowOAM[0].attr2 = ATTR2_TILEID(mang.aniState * 4, mang.curFrame * 4) | ATTR2_PALROW(0);
}

void initBall() {

    for (int i = 0; i < BALLCOUNT; i++) {
        ball[i].width = 10;
        ball[i].height = 11;
        ball[i].worldRow = mang.worldRow;
        ball[i].worldCol = mang.worldCol;
        ball[i].aniCounter = 0;
        ball[i].curFrame = 0;
        ball[i].numFrames = 0;
        ball[i].hide = 1;
        ball[i].rdel = 0;
        ball[i].cdel = 0;
        ball[i].aniState = SPRITEFRONT;
    }
}

void throwBall() {
    
    for (int i = 0; i < BALLCOUNT; i++) {
        if (ball[i].hide == 1) {

            ball[i].worldRow = mang.worldRow;
            ball[i].worldCol = mang.worldCol + (mang.height / 2);

            ball[i].hide = 0;

            break;
        }
    }
}

void updateBall(ANISPRITE* b) {
    //redo and add timer
    if (b->hide == 0) {
        if (mang.aniState == SPRITEFRONT) { // front
            b->rdel = 3;
            b->cdel = 0;
        }
        else if (mang.aniState == SPRITEBACK) { // back
            b->rdel = -3;
            b->cdel = 0;
        }
        else if (mang.aniState == SPRITERIGHT) { // right
            b->cdel = 3;
            b->rdel = 0;
        }
        else if (mang.aniState == SPRITELEFT) { // left
            b->cdel = -3;
            b->rdel = 0;
        }
        // else if (mang.aniState == SPRITEIDLE) {
        //     b->rdel = 3;
        // }
        // boundary check
        if (b->worldRow + b->height + 1 <= 0) {
            b->hide = 1;
        } 
        if (b->worldRow + b->height + 1 >= SCREENHEIGHT + b->height) {
            b->hide = 1;
        }
        if (b->worldCol + b->width + 1 <= 0) {
            b->hide = 1;
        }
        if (b->worldCol + b->width + 1 >= SCREENWIDTH + b->width) {
            b->hide = 1;
        }
        // collision ...
        if (collision(b->worldCol, b->worldRow, b->width, b->height, van.worldCol, van.worldRow, van.width, van.height)) {
            van.health--;
            b->hide = 1;
            playSoundB(hitEnemy_data, hitEnemy_length, 0);
            mgba_printf("collision");
        }
        if (collision(b->worldCol, b->worldRow, b->width, b->height, cooky.worldCol, cooky.worldRow, cooky.width, cooky.height)) {
            cooky.health--;
            playSoundB(hitEnemy_data, hitEnemy_length, 0);
            b->hide = 1;
        }   
        if (collision(b->worldCol, b->worldRow, b->width, b->height, koya.worldCol, koya.worldRow, koya.width, koya.height)) {
            koya.health--;
            playSoundB(hitEnemy_data, hitEnemy_length, 0);
            b->hide = 1;
        } 
        b->worldRow += b->rdel;
        b->worldCol += b->cdel;
    } else {
        b->hide = 1;
    }
}

void drawBall(ANISPRITE* b) {

    if (b->hide == 0) {
        for (int i = 0; i < BALLCOUNT; i++) {
            shadowOAM[i + 1].attr0 = (ROWMASK & (b->worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[i + 1].attr1 = (COLMASK & (b->worldCol - hOff)) | ATTR1_SMALL;
            shadowOAM[i + 1].attr2 = ATTR2_TILEID(1, 13) | ATTR2_PALROW(0);
        }
    } else {
        shadowOAM[1].attr0 = ATTR0_HIDE;
    }
}

void initVan() { // top

    van.worldRow = abs(rand() % 5);
    van.worldCol = abs(rand() % (256 - van.width + 1));
    van.rdel = 1;
    van.cdel = 1;
    van.width = 20;
    van.height = 20;
    van.aniCounter = SPRITEFRONT;
    van.aniState = 0;
    van.curFrame = 0;
    van.numFrames = 3;
    van.hide = 0;
    van.health = 2;
    van.ballTimer = 10;
    van.screenTimer = 100;
}

void updateVan() {

    van.screenTimer = 0;
    if (van.hide == 0) {
        while (van.screenTimer != 100) {
            if (van.screenTimer % 20 == 0) {
                throwVanBall();
            }
            van.screenTimer++;
        }
        if (van.health == 0) {
            van.hide = 1;
        }
    }
    animateEnemies();
}

void drawVan() {

    if (van.hide == 0) {
        shadowOAM[4].attr0 = (ROWMASK & (van.worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
        shadowOAM[4].attr1 = (COLMASK & (van.worldCol - hOff)) | ATTR1_MEDIUM;
        shadowOAM[4].attr2 = ATTR2_TILEID(van.curFrame * 4, van.aniState * 3 + 15) | ATTR2_PALROW(0); // 0, 15 

    } else {
        van.numFrames = 2;
        // shadowOAM[4].attr0 = ATTR0_HIDE; // instead of hide, have them collapse instead and breathe
        if (koya.health != 0 || cooky.health != 0) {
            shadowOAM[4].attr0 = (ROWMASK & (van.worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[4].attr1 = (COLMASK & (van.worldCol - hOff)) | ATTR1_MEDIUM;
            shadowOAM[4].attr2 = ATTR2_TILEID(van.curFrame * 4 + 12, van.aniState * 2 + 15) | ATTR2_PALROW(0); 
        }
    }
}

void initCooky() { // left

    // int num = (rand() % (upper - lower + 1)) + lower;
    cooky.worldRow = abs(rand() % (150 - 10 + 1)) + 10;
    cooky.worldCol = abs(rand() % (10 - 5 + 1)) + 5;
    cooky.rdel = 1;
    cooky.cdel = 1;
    cooky.width = 20;
    cooky.height = 20;
    cooky.aniCounter = SPRITEFRONT;
    cooky.aniState = 0;
    cooky.curFrame = 0;
    cooky.numFrames = 3;
    cooky.hide = 0;
    cooky.health = 2;
    cooky.ballTimer = 10;
    cooky.screenTimer = 10;
}

void updateCooky() {

    cooky.screenTimer = 0;
    if (cooky.hide == 0) {
        while (cooky.screenTimer != 100) {
            if (cooky.screenTimer % 30 == 1) {
                throwCookyBall();
            }
            cooky.screenTimer++;
        }
        if (cooky.health == 0) {
            cooky.hide = 1;
        }
    }  
    animateEnemies();

}

void drawCooky() {

    if (cooky.hide == 0) {
        shadowOAM[5].attr0 = (ROWMASK & (cooky.worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
        shadowOAM[5].attr1 = (COLMASK & (cooky.worldCol - hOff)) | ATTR1_MEDIUM;
        shadowOAM[5].attr2 = ATTR2_TILEID(cooky.curFrame * 4, cooky.aniState * 3 + 20) | ATTR2_PALROW(0); // 0, 19
        // shadowOAM[5].attr2 = ATTR2_TILEID(12, 19);
    } else {
        cooky.numFrames = 2;
        if (koya.health != 0 || van.health != 0) {
            shadowOAM[5].attr0 = (ROWMASK & (cooky.worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[5].attr1 = (COLMASK & (cooky.worldCol - hOff)) | ATTR1_MEDIUM;
            shadowOAM[5].attr2 = ATTR2_TILEID(cooky.curFrame * 4 + 12, cooky.aniState * 2 + 20) | ATTR2_PALROW(0); 
        }
        // shadowOAM[5].attr0 = ATTR0_HIDE; // instead of hide, have them collapse instead and breathe
    }   
}

void initKoya() { // right

    // int num = (rand() %(upper - lower + 1)) + lower;
    koya.worldRow = abs(rand() % (150 - 20 + 1)) + 20;
    koya.worldCol = abs(rand() % (220 - 210 + 1)) + 210;
    koya.rdel = 1;
    koya.cdel = 1;
    koya.width = 25;
    koya.height = 20;
    koya.aniCounter = SPRITEFRONT;
    koya.aniState = 0;
    koya.curFrame = 0;
    koya.numFrames = 3;
    koya.hide = 0;
    koya.health = 2;
    koya.ballTimer = 10;
    koya.screenTimer = 10;
}

void updateKoya() {

    koya.screenTimer = 0;
    if (koya.hide == 0) {
        while (koya.screenTimer != 100) {
            if (koya.screenTimer % 25 == 0) {
                throwKoyaBall();
                
            }
            koya.screenTimer++;
        }
        if (koya.health == 0) {
            koya.hide = 1;
            // if hidden then hide ball too
        }
    } 
    animateEnemies();
}

void drawKoya() {

    if (koya.hide == 0 ){
        shadowOAM[6].attr0 = (ROWMASK & (koya.worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
        shadowOAM[6].attr1 = (COLMASK & (koya.worldCol - hOff)) | ATTR1_MEDIUM;
        shadowOAM[6].attr2 = ATTR2_TILEID(koya.curFrame * 4, koya.aniState * 3 + 23) | ATTR2_PALROW(0);
    } else {
        koya.numFrames = 2;
        if (cooky.health != 0 || van.health != 0) {
            shadowOAM[6].attr0 = (ROWMASK & (koya.worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[6].attr1 = (COLMASK & (koya.worldCol - hOff)) | ATTR1_MEDIUM;
            shadowOAM[6].attr2 = ATTR2_TILEID(koya.curFrame * 4 + 12, koya.aniState * 2 + 23) | ATTR2_PALROW(0); 
        }
        // shadowOAM[6].attr0 = ATTR0_HIDE; // instead of hide, have them collapse instead and breathe
    }
}

void initVanBall() {

    for (int i = 0; i < BALLCOUNT; i++) {
        vanBall[i].width = 10;
        vanBall[i].height = 11;
        vanBall[i].worldRow = van.worldRow - (van.height / 2);
        vanBall[i].worldCol = van.worldCol - (van.width / 2);
        vanBall[i].aniCounter = 0;
        vanBall[i].curFrame = 0;
        vanBall[i].numFrames = 0;
        vanBall[i].hide = 1;
        vanBall[i].rdel = 0;
        vanBall[i].cdel = 0;
        vanBall[i].aniState = SPRITEFRONT;
    }
}

void throwVanBall() { // top

    for (int i = 0; i < BALLCOUNT; i++) {
        if (vanBall[i].hide == 1) {

            vanBall[i].worldRow = van.worldRow;
            vanBall[i].worldCol = van.worldCol + (van.height / 2);

            vanBall[i].hide = 0;

            break;
        }
    }
}

void updateVanBall(ANISPRITE* v) {

    if (v->hide == 0) {
        if (van.aniState == SPRITEFRONT) { // front
            v->rdel = 3;
            v->cdel = 0;
        }
        // boundary check
        if (v->worldRow + v->height + 1 <= 0) {
            v->hide = 1;
        } 
        if (v->worldRow + v->height + 1 >= SCREENHEIGHT + v->height) {
            v->hide = 1;
        }
        if (v->worldCol + v->width + 1 <= 0) {
            v->hide = 1;
        }
        if (v->worldCol + v->width + 1 >= SCREENWIDTH + v->width) {
            v->hide = 1;
        }
        // collision ...
        if (collision(v->worldCol, v->worldRow, v->width, v->height, mang.worldCol, mang.worldRow, mang.width, mang.height)) {
            mang.health--;
            playSoundB(hitPlayer_data, hitPlayer_length, 0);
            v->hide = 1;
        }
        v->worldRow += v->rdel;
        v->worldCol += v->cdel;
    } else {
        v->hide = 1;
    }
}

void drawVanBall(ANISPRITE* v) {
    
    if (v->hide == 0) {
        for (int i = 0; i < BALLCOUNT; i++) {
            shadowOAM[i + 7].attr0 = (ROWMASK & (v->worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[i + 7].attr1 = (COLMASK & (v->worldCol - hOff)) | ATTR1_SMALL;
            shadowOAM[i + 7].attr2 = ATTR2_TILEID(5, 13) | ATTR2_PALROW(0);
        }
    } else {
        shadowOAM[7].attr0 = ATTR0_HIDE;
    }
}

void initCookyBall() {

    for (int i = 0; i < BALLCOUNT; i++) {
        cookyBall[i].width = 10;
        cookyBall[i].height = 11;
        cookyBall[i].worldRow = cooky.worldRow - (cooky.height / 2);
        cookyBall[i].worldCol = cooky.worldCol - (cooky.width / 2);
        cookyBall[i].aniCounter = 0;
        cookyBall[i].curFrame = 0;
        cookyBall[i].numFrames = 0;
        cookyBall[i].hide = 1;
        cookyBall[i].rdel = 0;
        cookyBall[i].cdel = 0;
        cookyBall[i].aniState = SPRITEFRONT;
    }
}

void throwCookyBall() {

    for (int i = 0; i < BALLCOUNT; i++) {
        if (cookyBall[i].hide == 1) {

            cookyBall[i].worldRow = cooky.worldRow;
            cookyBall[i].worldCol = cooky.worldCol + (cooky.height / 2);

            cookyBall[i].hide = 0;

            break;
        }
    }
}

void updateCookyBall(ANISPRITE* c) {

    if (c->hide == 0) {
        if (van.aniState == SPRITEFRONT) { // front
            c->cdel = 3;
            c->rdel = 0;
        }
        // boundary check
        if (c->worldRow + c->height + 1 <= 0) {
            c->hide = 1;
        } 
        if (c->worldRow + c->height + 1 >= SCREENHEIGHT + c->height) {
            c->hide = 1;
        }
        if (c->worldCol + c->width + 1 <= 0) {
            c->hide = 1;
        }
        if (c->worldCol + c->width + 1 >= SCREENWIDTH + c->width) {
            c->hide = 1;
        }
        // collision ...
        if (collision(c->worldCol, c->worldRow, c->width, c->height, mang.worldCol, mang.worldRow, mang.width, mang.height)) {
            mang.health--;
            playSoundB(hitPlayer_data, hitPlayer_length, 0);
            c->hide = 1;
        }
        c->worldRow += c->rdel;
        c->worldCol += c->cdel;
    } else {
        c->hide = 1;
    }
}

void drawCookyBall(ANISPRITE* c) {
    
    if (c->hide == 0) {
        for (int i = 0; i < BALLCOUNT; i++) {
            shadowOAM[i + 10].attr0 = (ROWMASK & (c->worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[i + 10].attr1 = (COLMASK & (c->worldCol - hOff)) | ATTR1_SMALL;
            shadowOAM[i + 10].attr2 = ATTR2_TILEID(9, 13) | ATTR2_PALROW(0);
        }
    } else {
        shadowOAM[10].attr0 = ATTR0_HIDE;
    }
}

void initKoyaBall() {
    for (int i = 0; i < BALLCOUNT; i++) {
        koyaBall[i].width = 10;
        koyaBall[i].height = 11;
        koyaBall[i].worldRow = koya.worldRow;
        koyaBall[i].worldCol = koya.worldCol;
        koyaBall[i].aniCounter = 0;
        koyaBall[i].curFrame = 0;
        koyaBall[i].numFrames = 0;
        koyaBall[i].hide = 1;
        koyaBall[i].rdel = 0;
        koyaBall[i].cdel = 0;
        koyaBall[i].aniState = SPRITEFRONT;
    }

}

void throwKoyaBall() {

    for (int i = 0; i < BALLCOUNT; i++) {
        if (koyaBall[i].hide == 1) {

            koyaBall[i].worldRow = koya.worldRow;
            koyaBall[i].worldCol = koya.worldCol + (koya.height / 2);

            koyaBall[i].hide = 0;

            break;
        }
    }
}

void updateKoyaBall(ANISPRITE* k) {

    if (k->hide == 0) {
        if (van.aniState == SPRITEFRONT) { // front
            k->cdel = -3;
            k->rdel = 0;
        }
        // boundary check
        if (k->worldRow + k->height + 1 <= 0) {
            k->hide = 1;
        } 
        if (k->worldRow + k->height + 1 >= SCREENHEIGHT + k->height) {
            k->hide = 1;
        }
        if (k->worldCol + k->width + 1 <= 0) {
            k->hide = 1;
        }
        if (k->worldCol + k->width + 1 >= SCREENWIDTH + k->width) {
            k->hide = 1;
        }
        // collision
        if (collision(k->worldCol, k->worldRow, k->width, k->height, mang.worldCol, mang.worldRow, mang.width, mang.height)) {
            mang.health--;
            playSoundB(hitPlayer_data, hitPlayer_length, 0);
            k->hide = 1;
        }
        k->worldRow += k->rdel;
        k->worldCol += k->cdel;
    } else {
        k->hide = 1;
    }
}

void drawKoyaBall(ANISPRITE* k) {

    if (k->hide == 0) {
        for (int i = 0; i < BALLCOUNT; i++) {
            shadowOAM[i + 12].attr0 = (ROWMASK & (k->worldRow - vOff)) | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[i + 12].attr1 = (COLMASK & (k->worldCol - hOff)) | ATTR1_SMALL;
            shadowOAM[i + 12].attr2 = ATTR2_TILEID(13, 13) | ATTR2_PALROW(0);
        }
    } else {
        shadowOAM[12].attr0 = ATTR0_HIDE;
    }
}

void animateEnemies() {
        
    van.prevAniState = van.aniState;
	van.aniState = SPRITEFRONT;

    cooky.prevAniState = cooky.aniState;
	cooky.aniState = SPRITEFRONT;

    koya.prevAniState = koya.aniState;
	koya.aniState = SPRITEFRONT;

    if (van.aniCounter % 50 == 0) {
        van.curFrame = (van.curFrame + 1) % van.numFrames;
    }
    van.aniCounter++;
    if (cooky.aniCounter % 50 == 0) {
        cooky.curFrame = (cooky.curFrame + 1) % cooky.numFrames;
    }
    cooky.aniCounter++;
    if (koya.aniCounter % 50 == 0) {
        koya.curFrame = (koya.curFrame + 1) % koya.numFrames;
    }
    koya.aniCounter++;
}
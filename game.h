typedef struct {
    int worldRow;
    int worldCol;
    int rdel;
    int cdel;
    int width;
    int height;
    int aniCounter;
    int aniState;
    int prevAniState;
    int curFrame;
    int numFrames;
    int hide;
    int health;
    int ballTimer; 
} MANG;

typedef struct {
    int worldRow;
    int worldCol;
    int rdel;
    int cdel;
    int width;
    int height;
    int aniCounter;
    int aniState;
    int prevAniState;
    int curFrame;
    int numFrames;
    int hide;
    int health;
    int ballTimer;
    int screenTimer;
} ENEMY;

#define BALLCOUNT 1
#define MAPHEIGHT 256
#define MAPWIDTH 256
#define LIFECOUNT

extern int hit;
extern int ballsLeft;
extern int delay;
extern int health;
extern int vanHealth;
extern int cookyHealth;
extern int koyaHealth;

extern int hOff;
extern int vOff;
extern int screenBlock;

extern MANG mang;
extern ANISPRITE ball[BALLCOUNT];
extern ENEMY van;
extern ENEMY cooky;
extern ENEMY koya;
extern ANISPRITE vanBall[BALLCOUNT];
extern ANISPRITE cookyBall[BALLCOUNT];
extern ANISPRITE koyaBall[BALLCOUNT];

void initGame();
void updateGame();
void drawGame();

void initMang();
void updateMang();
void animateMang();
void drawMang();

void initBall();
void throwBall();
void updateBall(ANISPRITE *);
void drawBall(ANISPRITE *);

void initVan();
void updateVan();
void drawVan();

void initCooky();
void updateCooky();
void drawCooky();

void initKoya();
void updateKoya();
void drawKoya();

void initVanBall();
void throwVanBall();
void updateVanBall(ANISPRITE *);
void drawVanBall(ANISPRITE *);

void initCookyBall();
void throwCookyBall();
void updateCookyBall(ANISPRITE *);
void drawCookyBall(ANISPRITE *);

void initKoyaBall();
void throwKoyaBall();
void updateKoyaBall(ANISPRITE *);
void drawKoyaBall(ANISPRITE *);

void animateEnemies();

void updateBG();

void cheat(ANISPRITE *);
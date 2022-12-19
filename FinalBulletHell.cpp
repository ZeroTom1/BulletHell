#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<iostream>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
const int PLAYER_WIDTH = 60;
const int PLAYER_HEIGHT = 90;
const int ENEMY_WIDTH = 90;
const int ENEMY_HEIGHT = 90;
const int BULLET_WIDTH = 40;
const int BULLET_HEIGHT = 40;
const int SWORD_WIDTH = 20;
const int SWORD_HEIGHT = 40;
const int BACKGROUND_WIDTH = 2000;
const int BACKGROUND_HEIGHT = 2250;
const int PLAYER_STARTING_X = 320;
const int PLAYER_STARTING_Y = 240;
const int FPS = 60;
const int frameDelay = 1000 / FPS;
const int PlayerSpeed = 10;
const int MAX_BULLETS = 100;
const int MAX_PLAYER_BULLETS = 5;
const int ENEMY_X = 0;
const int ENEMY_Y = 640;
const double BulletSpeed = 0.3;
const int SpiralX = 500;
const int SpiralY = 10;
const int enemy3 = 100;
const int BulletCooldown = 2000; //2000 = 2 seconds
const int Boss1Health = 40;
const int Boss2Health = 20;
const int Boss3Health = 80;
const int BosssHealth = Boss1Health + Boss2Health + Boss3Health; //used mainly in score calculation
const int PlayerHealthConst = 5; //total player healt
const int PlayerShotCooldown = -500; //-500 = 0.5 second
const int immortaltime = -1000; // -1000 = 1 second
const int BonusHealth = 5; //how much player is healed after picking bonus

struct SDL_MANAGING {
        SDL_Event event;
        SDL_Surface* screen, * charset;
        SDL_Surface* geralt, * background1, * babawodna, * Bullet, * MenuPage, * background2, * background3, * golem, * dragon1, * dragon2, * dragon3, * sword, * shield, * gameover,
                * potion, * health, * shield1, * shield2, * shield3, * shield4, * rankup, * victory, * geralt1, * geralt2, * geralt3, * geralt4, * geralt5, * health1, * health2;
        SDL_Texture* scrtex;
        SDL_Window* window;
        SDL_Renderer* renderer;
};
struct Colors {
        char text[128];
        int czarny = 0;
        int zielony = 0;
        int czerwony = 0;
        int niebieski = 0;
};
struct GameNumbers {
        double t1 = SDL_GetTicks();
        double t2 = 0;
        int quit = 0;
        double frames = 0;
        int rc = 0;
        int intworldTime = 0;
        double delta = 0;
        double worldTime = 0;
        double fpsTimer = 0;
        double fps = 0;
        double distance = 0;
        double PlayerX = SCREEN_WIDTH / 2, PlayerY = SCREEN_HEIGHT / 2;
        Uint32 frameStart = SDL_GetTicks();
        int frameTime = SDL_GetTicks() - frameStart;
        int reset = 0;
        int stagenumber = 0;
        int stagelast = -99;
        int space = 0;
        int lastspace = 0;
        double PlayerLastShot = PlayerShotCooldown;
        double playerHitTime = -1;
        int PlayerHealth = PlayerHealthConst;
        int Boss1Health = Boss1Health;
        int Boss2Health = Boss2Health;
        int Boss3Health = Boss3Health;
        int Level2access = 0;
        int Level3access = 0;
        double SCORE = 0;
        int HitStreak = 0;
        int BosssHealth = 0;
        int PlayerHits = 0;
        int Victory = 0;
        int BounsPresnt = 0;
        int BonusX = 0, BonusY = 0;
        int enemymove = 0;
        int RankUpAnimation = 0;
        int lastRank = 0;
        int Rank = 0;
        double RankUpAnimationTime = 0;
        double PlayerAnimationTime = 0;
        double EnemyAnimationTime = 0;
        double HealthAnimationTime = 0;
        int healthanimationframe = 0;
        int animationframe = 0;
        int enemyanimationframe = 0;
        int PlayerAnimationXUP = 0;
        int PlayerAnimationYRight = 0;
        int PlayerAnimationXDOWN = 0;
        int PlayerAnimationYLeft = 0;
        int HealthAnimation = 0;
        int HealthTime = 0;
};

struct Bullet {
        int x;
        int y;
        int directionX;
        int directionY;
        int present;
        int id;
        double AdvancedPaternX = 0;
        double AdvancedPaternY = 0;
        double SpawnTime;
        int lastPlayerSpace = 0;
};


void StageChanger(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS], Colors* Colors); //handles stage changing
void Stage1(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //stage 1 setup
void Stage2(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //stage 2 setup
void Stage3(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //stage 3 setup
void GameOver(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //losing stage
void VictoryStage(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]);//victory stage
void Menu(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //show menu
void GameReset(GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //fully reset game
void StageReset(GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); // reset current stage
void SmallNumberChange(GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]);//value change
void StageManagment(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //functions making sure stage is displayed and working
void GameInfo(SDL_MANAGING* SDL_MANAGER, Colors* colors, GameNumbers* GameNumbers);// displays game information
void RankShield(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers); //shield displaying current rank
void RankShieldAnimation(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers); //little pop up saying rank when rank is changed
void HPManagment(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers); //counts healts and displays player health
void Bonus(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers); //spawn and manage bonus
void BulletValues(Bullet bullets[MAX_BULLETS], int x, int y, int id); //setting bullet values
void CreatingBullets(Bullet arr[MAX_BULLETS], int x, int y); // filling bullet array
void SpawnBulletsRandom(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], int BotBorder, int TopBorder, int directionX, int directionY, int AdvancedPaternX, int AdvancedPaternY); //spawn bullets randomly
void PlayerBulletSpawn(GameNumbers* GameNumbers, Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //spawn player bullets
void BulletUpdate(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]); //checking collision and moving bullets
void PlayerBulletUpdate(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]);//handling of player bullets
void SpawnEnemy(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers); //spawning enemy for each stage
void PlayerAnimation(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers); // animating the playermovement
void HealthAnimation(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers);// health animation
void PlayerInvi(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers); //makes invisible frames (immortal for a time period)
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset); //draw string
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y); //draw image
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color); //draw pixel
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color); //draw line
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor); //draw rectangle
void KeyboardInput(GameNumbers* GameNumbers); //manage keyboard input
void PlayerMovementRight(GameNumbers* GameNumbers); //move player right
void PlayerMovementLeft(GameNumbers* GameNumbers); // move player left
void PlayerMovementUp(GameNumbers* GameNumbers); //move player up
void PlayerMovementDown(GameNumbers* GameNumbers); //move player down
void FPSCount(GameNumbers* GameNumbers); //count fps
void ColorsManager(SDL_MANAGING* SDL_MANAGING, Colors* colors); //setting colors values
void GamePrep(SDL_MANAGING* SDL_MANAGER); //prepare game data
void Rendering(SDL_MANAGING* SDL_MANAGER); //render screen
void BMPLoading(SDL_MANAGING* SDL_MANAGER); //load bmp images
void ENDING(SDL_MANAGING* SDL_MANAGER); //fully end program




#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char** argv) {
        SDL_MANAGING SDL_MANAGER;
        Colors Colors;
        GameNumbers GameNumbers;
        Bullet Bullets[MAX_BULLETS];
        Bullet PlayerBullets[MAX_BULLETS];
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                printf("SDL_Init error: %s\n", SDL_GetError());
                return 1;
        }
        GameNumbers.rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &SDL_MANAGER.window, &SDL_MANAGER.renderer);
        if (GameNumbers.rc != 0) {
                SDL_Quit();
                printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
                return 1;
        };
        GamePrep(&SDL_MANAGER);
        CreatingBullets(Bullets, ENEMY_X, ENEMY_Y);
        CreatingBullets(PlayerBullets, PLAYER_STARTING_X, PLAYER_STARTING_Y);
        while (!GameNumbers.quit) {
                FPSCount(&GameNumbers);
                SDL_ShowCursor(SDL_DISABLE);
                if (frameDelay > GameNumbers.frameTime) {
                        SDL_Delay(frameDelay - GameNumbers.frameTime);
                }
                SDL_FillRect(SDL_MANAGER.screen, NULL, Colors.czarny);
                if (GameNumbers.stagenumber == 0) { Menu(&SDL_MANAGER, &GameNumbers, Bullets, PlayerBullets); }
                if (GameNumbers.stagenumber == 1) { Stage1(&SDL_MANAGER, &GameNumbers, &Colors, Bullets, PlayerBullets); }
                if (GameNumbers.stagenumber == 2 && GameNumbers.Level2access == 1) { Stage2(&SDL_MANAGER, &GameNumbers, &Colors, Bullets, PlayerBullets); }
                if (GameNumbers.stagenumber == 3 && GameNumbers.Level3access == 1) { Stage3(&SDL_MANAGER, &GameNumbers, &Colors, Bullets, PlayerBullets); }
                if (GameNumbers.stagenumber == -1) { GameOver(&SDL_MANAGER, &GameNumbers, Bullets, PlayerBullets); }
                if (GameNumbers.stagenumber == -2) { VictoryStage(&SDL_MANAGER, &GameNumbers, Bullets, PlayerBullets); }
                if (GameNumbers.stagenumber != 0) { KeyboardInput(&GameNumbers); }
                while (SDL_PollEvent(&SDL_MANAGER.event)) {
                        switch (SDL_MANAGER.event.type) {
                        case SDL_KEYDOWN:
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_ESCAPE) GameNumbers.quit = 1;
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_m) {
                                        GameNumbers.stagenumber = 0;
                                        StageReset(&GameNumbers, Bullets, PlayerBullets);
                                }
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_1) {
                                        GameNumbers.stagenumber = 1;
                                        StageReset(&GameNumbers, Bullets, PlayerBullets);
                                }
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_2 && GameNumbers.Level2access == 1) {
                                        GameNumbers.stagenumber = 2;
                                        StageReset(&GameNumbers, Bullets, PlayerBullets);
                                }
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_3 && GameNumbers.Level3access == 1) {
                                        GameNumbers.stagenumber = 3;
                                        StageReset(&GameNumbers, Bullets, PlayerBullets);
                                }
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_SPACE) {
                                        if (GameNumbers.stagenumber != 0 && GameNumbers.PlayerLastShot - GameNumbers.t2 < PlayerShotCooldown) {
                                                GameNumbers.PlayerLastShot = GameNumbers.t2;
                                                GameNumbers.space++;
                                                GameNumbers.lastspace = GameNumbers.space;
                                        }
                                }
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_n) { GameReset(&GameNumbers, Bullets, PlayerBullets); }
                                if (SDL_MANAGER.event.key.keysym.sym == SDLK_r) { StageReset(&GameNumbers, Bullets, PlayerBullets); }
                                break;
                        case SDL_QUIT:
                                GameNumbers.quit = 1;
                                break;
                        };
                };
                GameNumbers.frames++;
        };
        ENDING(&SDL_MANAGER);
        return 0;
};

void StageChanger(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS], Colors* Colors) {
        if (GameNumbers->stagenumber == 1 && GameNumbers->Boss1Health <= 0) {
                GameNumbers->Level2access = 1;
                GameNumbers->stagelast = GameNumbers->stagenumber;
                GameNumbers->stagenumber = -2;
        }
        if (GameNumbers->stagenumber == 2 && GameNumbers->Boss2Health <= 0) {
                GameNumbers->Level3access = 1;
                GameNumbers->stagelast = GameNumbers->stagenumber;
                GameNumbers->stagenumber = -2;
        }
        if (GameNumbers->stagenumber == 3 && GameNumbers->Boss3Health <= 0) {
                GameNumbers->stagelast = GameNumbers->stagenumber;
                GameNumbers->stagenumber = -2;
        }
}
void Stage1(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        int patern = 0;
        if ((int(GameNumbers->worldTime) + 1) % 2 == 0) {
                patern = 0;
        }
        if ((int(GameNumbers->worldTime) + 1) % 5 == 0) {
                patern = 1;
        }
        if ((int(GameNumbers->worldTime) + 1) % 6 == 0) {
                patern = 2;
        }
        if (patern == 0) {
                SpawnBulletsRandom(SDL_MANAGER, GameNumbers, Bullets, 0, 5, 0, -1, 0, 0);
        }
        if (patern == 1) {
                SpawnBulletsRandom(SDL_MANAGER, GameNumbers, Bullets, 6, 10, 1, -1, 0, 0);
                SpawnBulletsRandom(SDL_MANAGER, GameNumbers, Bullets, 16, 20, -1, -1, 0, 0);
        }
        if (patern == 0) {
                SpawnBulletsRandom(SDL_MANAGER, GameNumbers, Bullets, 11, 15, 1, -2, 0, 0);
                SpawnBulletsRandom(SDL_MANAGER, GameNumbers, Bullets, 21, 25, -1, -2, 0, 0);
        }
        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->background1, GameNumbers->PlayerX, GameNumbers->PlayerY);
        PlayerAnimation(SDL_MANAGER, GameNumbers);
        StageManagment(SDL_MANAGER, GameNumbers, Colors, Bullets, PlayerBullets);
}
void Stage2(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->background2, GameNumbers->PlayerX, GameNumbers->PlayerY);
        PlayerAnimation(SDL_MANAGER, GameNumbers);
        SpawnBulletsRandom(SDL_MANAGER, GameNumbers, Bullets, 0, 50, 0, -1, 1, 1);
        StageManagment(SDL_MANAGER, GameNumbers, Colors, Bullets, PlayerBullets);
}
void Stage3(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->background3, GameNumbers->PlayerX, GameNumbers->PlayerY);
        PlayerAnimation(SDL_MANAGER, GameNumbers);
        SpawnBulletsRandom(SDL_MANAGER, GameNumbers, Bullets, 0, 5, 0, -1, 0, 0);
        StageManagment(SDL_MANAGER, GameNumbers, Colors, Bullets, PlayerBullets);
}
void GameOver(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->gameover, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        if (SDL_MANAGER->event.key.keysym.sym == SDLK_m) {
                GameReset(GameNumbers, Bullets, PlayerBullets);
        }
        if (SDL_MANAGER->event.key.keysym.sym == SDLK_r) {
                GameNumbers->stagenumber = GameNumbers->stagelast;
        }
        Rendering(SDL_MANAGER);
}
void VictoryStage(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->victory, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        Rendering(SDL_MANAGER);
        if (SDL_MANAGER->event.key.keysym.sym == SDLK_m) {
                GameReset(GameNumbers, Bullets, PlayerBullets);
        }
        if (SDL_MANAGER->event.key.keysym.sym == SDLK_b) {
                GameNumbers->stagenumber = GameNumbers->stagelast + 1;
                SmallNumberChange(GameNumbers, Bullets, PlayerBullets);
        }
}
void Menu(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        GameReset(GameNumbers, Bullets, PlayerBullets);
        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->MenuPage, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        Rendering(SDL_MANAGER);
}
void GameReset(GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        GameNumbers->reset = 0;
        GameNumbers->stagenumber = 0;
        GameNumbers->lastRank = 0;
        GameNumbers->Boss1Health = Boss1Health;
        GameNumbers->Boss2Health = Boss2Health;
        GameNumbers->Boss3Health = Boss3Health;
        GameNumbers->BosssHealth = Boss1Health + Boss2Health + Boss3Health;
        GameNumbers->Level2access = 0;
        GameNumbers->Level3access = 0;
        GameNumbers->SCORE = 0;
        GameNumbers->PlayerHits = 0;
        GameNumbers->Victory = 0;
        GameNumbers->RankUpAnimation = 0;
        GameNumbers->BounsPresnt = 0;
        GameNumbers->BonusX = 0, GameNumbers->BonusY = 0;
        SmallNumberChange(GameNumbers, Bullets, PlayerBullets);
}
void StageReset(GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        GameNumbers->reset = 0;
        GameNumbers->SCORE = 0;
        GameNumbers->HitStreak = 0;
        GameNumbers->Boss1Health = Boss1Health;
        GameNumbers->Boss2Health = Boss2Health;
        GameNumbers->Boss3Health = Boss3Health;
        GameNumbers->BosssHealth = Boss1Health + Boss2Health + Boss3Health;
        SmallNumberChange(GameNumbers, Bullets, PlayerBullets);
}
void SmallNumberChange(GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        GameNumbers->PlayerX = SCREEN_WIDTH / 2;
        GameNumbers->PlayerY = SCREEN_HEIGHT / 2;
        GameNumbers->PlayerHealth = PlayerHealthConst;
        GameNumbers->t1 = SDL_GetTicks();
        GameNumbers->t2 = 0;
        GameNumbers->quit = 0;
        GameNumbers->frames = 0;
        GameNumbers->rc = 0;
        GameNumbers->worldTime = 0;
        GameNumbers->delta = 0;
        GameNumbers->worldTime = 0;
        GameNumbers->fpsTimer = 0;
        GameNumbers->fps = 0;
        GameNumbers->frameStart = SDL_GetTicks();
        GameNumbers->frameTime = SDL_GetTicks() - GameNumbers->frameStart;
        GameNumbers->space = 0;
        CreatingBullets(Bullets, ENEMY_X, ENEMY_Y);
        CreatingBullets(PlayerBullets, PLAYER_STARTING_X, PLAYER_STARTING_Y);
        GameNumbers->playerHitTime = -1;
        GameNumbers->RankUpAnimation = 0;
        GameNumbers->PlayerAnimationYLeft = 0;
        GameNumbers->PlayerAnimationXUP = 0;
        GameNumbers->PlayerAnimationYRight = 0;
        GameNumbers->PlayerAnimationXDOWN = 0;
        GameNumbers->animationframe = 0;
        GameNumbers->enemyanimationframe = 0;
        GameNumbers->PlayerAnimationTime = 0;
        GameNumbers->EnemyAnimationTime = 0;
        GameNumbers->HealthAnimation = 0;
        GameNumbers->healthanimationframe = 0;
        GameNumbers->HealthAnimationTime = 0;
}
void StageManagment(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Colors* Colors, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        PlayerInvi(SDL_MANAGER, GameNumbers);
        Bonus(SDL_MANAGER, GameNumbers);
        BulletUpdate(SDL_MANAGER, GameNumbers, Bullets, PlayerBullets);
        SpawnEnemy(SDL_MANAGER, GameNumbers);
        HPManagment(SDL_MANAGER, GameNumbers);
        StageChanger(SDL_MANAGER, GameNumbers, Bullets, PlayerBullets, Colors);
        GameInfo(SDL_MANAGER, Colors, GameNumbers);
        RankShield(SDL_MANAGER, GameNumbers);
        PlayerBulletSpawn(GameNumbers, PlayerBullets);
        Rendering(SDL_MANAGER);
}
void GameInfo(SDL_MANAGING* SDL_MANAGER, Colors* colors, GameNumbers* GameNumbers) {
        DrawRectangle(SDL_MANAGER->screen, 4, 4, SCREEN_WIDTH - 8, 52, colors->czerwony, colors->niebieski);
        sprintf(colors->text, "Czas trwania = %.1lf s  %.0lf klatek / s", GameNumbers->worldTime, GameNumbers->fps);
        DrawString(SDL_MANAGER->screen, SDL_MANAGER->screen->w / 2 - strlen(colors->text) * 8 / 2, 10, colors->text, SDL_MANAGER->charset);
        sprintf(colors->text, "Score: %.0lf points", GameNumbers->SCORE);
        DrawString(SDL_MANAGER->screen, SDL_MANAGER->screen->w / 2 - strlen(colors->text) * 8 / 2, 26, colors->text, SDL_MANAGER->charset);
        sprintf(colors->text, "Esc - wyjscie");
        DrawString(SDL_MANAGER->screen, SDL_MANAGER->screen->w / 2 - strlen(colors->text) * 8 / 2, 40, colors->text, SDL_MANAGER->charset);
}
void RankShield(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if (GameNumbers->SCORE >= 1 && (GameNumbers->SCORE <= (GameNumbers->BosssHealth / 4))) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->shield1, SCREEN_WIDTH - 60, 30);
                GameNumbers->Rank = 1;
        }
        if ((GameNumbers->SCORE > GameNumbers->BosssHealth / 4) && (GameNumbers->SCORE <= GameNumbers->BosssHealth / 2)) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->shield2, SCREEN_WIDTH - 60, 30);
                GameNumbers->Rank = 2;
        }
        if ((GameNumbers->SCORE > GameNumbers->BosssHealth / 2) && (GameNumbers->SCORE <= GameNumbers->BosssHealth * 3 / 4)) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->shield3, SCREEN_WIDTH - 60, 30);
                GameNumbers->Rank = 3;
        }
        if (GameNumbers->SCORE > GameNumbers->BosssHealth * 3 / 4) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->shield4, SCREEN_WIDTH - 60, 30);
                GameNumbers->Rank = 4;
        }
        RankShieldAnimation(SDL_MANAGER, GameNumbers);
}
void RankShieldAnimation(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if (GameNumbers->lastRank != GameNumbers->Rank) {
                GameNumbers->RankUpAnimationTime = GameNumbers->t2;
                GameNumbers->lastRank = GameNumbers->Rank;
        }
        if ((GameNumbers->RankUpAnimationTime - GameNumbers->t2 > -1000) && (GameNumbers->lastRank == GameNumbers->Rank) && (GameNumbers->lastRank != 0)) {
                if (int((GameNumbers->t2 - GameNumbers->RankUpAnimationTime) / 10) % 2 == 1) {
                        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->rankup, SCREEN_WIDTH - 60, 60);
                }
        }

}
void HPManagment(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if (GameNumbers->PlayerHealth > 0) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->health, 300, 120);
        }
        if (GameNumbers->PlayerHealth > (PlayerHealthConst / 5)) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->health, 310, 120);
        }
        if (GameNumbers->PlayerHealth > (2 * PlayerHealthConst / 5)) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->health, 320, 120);
        }
        if (GameNumbers->PlayerHealth > (3 * PlayerHealthConst / 5)) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->health, 330, 120);
        }
        if (GameNumbers->PlayerHealth > (4 * PlayerHealthConst / 5)) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->health, 340, 120);
        }
        if (GameNumbers->PlayerHealth > 5) {
                GameNumbers->PlayerHealth = PlayerHealthConst;
        }
        if (GameNumbers->PlayerHealth <= 0) {
                GameNumbers->stagelast = GameNumbers->stagenumber;
                GameNumbers->stagenumber = -1;
        }
}
void Bonus(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if ((int(GameNumbers->worldTime) + 1) % 5 == 0) {
                if (GameNumbers->BounsPresnt == 0) {
                        GameNumbers->BounsPresnt = 1;
                        GameNumbers->BonusX = PLAYER_STARTING_X + -rand() % 10 * 100;
                        GameNumbers->BonusY = PLAYER_STARTING_Y + -rand() % 10 * 100;
                }
        }
        if (GameNumbers->BounsPresnt == 1) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->potion, GameNumbers->BonusX + GameNumbers->PlayerX, GameNumbers->BonusY + GameNumbers->PlayerY);
        }
        if ((PLAYER_STARTING_X + PLAYER_WIDTH / 2 > GameNumbers->PlayerX + GameNumbers->BonusX - PLAYER_WIDTH / 2) &&
                (PLAYER_STARTING_X - PLAYER_WIDTH / 2 < GameNumbers->PlayerX + GameNumbers->BonusX + PLAYER_WIDTH / 2) &&
                (PLAYER_STARTING_Y + PLAYER_HEIGHT / 2 > GameNumbers->PlayerY + GameNumbers->BonusY - PLAYER_HEIGHT / 2) &&
                (PLAYER_STARTING_Y - PLAYER_HEIGHT / 2 < GameNumbers->PlayerY + GameNumbers->BonusY + PLAYER_HEIGHT / 2) && GameNumbers->BounsPresnt == 1) {
                GameNumbers->BounsPresnt = 0;
                GameNumbers->PlayerHealth += BonusHealth;
                GameNumbers->HealthAnimation = 1;
        }
}
void BulletValues(Bullet bullets[MAX_BULLETS], int x, int y, int id) {
        bullets[id].x = x;
        bullets[id].y = y;
        bullets[id].directionX = 0;
        bullets[id].directionY = 0;
        bullets[id].present = 0;
        bullets[id].id = id;
        bullets[id].AdvancedPaternX = 0;
        bullets[id].AdvancedPaternY = 0;
        bullets[id].SpawnTime = 0;
}
void CreatingBullets(Bullet arr[MAX_BULLETS], int x, int y) {
        for (int i = 0; i < MAX_BULLETS; i++) {
                BulletValues(arr, x, y, i);
        }
}
void SpawnBulletsRandom(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], int BotBorder, int TopBorder, int directionX, int directionY, int AdvancedPaternX, int AdvancedPaternY) {
        if (BotBorder < 0 || TopBorder > MAX_BULLETS) {
                ENDING(SDL_MANAGER);
        }
        for (BotBorder; BotBorder < TopBorder; BotBorder++) {
                if ((int(GameNumbers->worldTime + 1) % (rand() % 50 + 1)) == 0 && Bullets[BotBorder].present == 0) {
                        Bullets[BotBorder].directionX = directionX;
                        Bullets[BotBorder].directionY = directionY;
                        Bullets[BotBorder].present = 1;
                        Bullets[BotBorder].SpawnTime = GameNumbers->t2;
                        Bullets[BotBorder].AdvancedPaternX = AdvancedPaternX;
                        Bullets[BotBorder].AdvancedPaternY = AdvancedPaternY;
                }
        }
}
void PlayerBulletSpawn(GameNumbers* GameNumbers, Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        int i = GameNumbers->space;
        if (GameNumbers->space > 0 && GameNumbers->space < 6 && GameNumbers->space == GameNumbers->lastspace) {
                i = i - 1;
                if (PlayerBullets[i].present == 0 && PlayerBullets[i].lastPlayerSpace != 1) {
                        PlayerBullets[i].x = SCREEN_WIDTH - GameNumbers->PlayerX;
                        PlayerBullets[i].y = SCREEN_HEIGHT - GameNumbers->PlayerY;
                        PlayerBullets[i].SpawnTime = GameNumbers->t2;
                        PlayerBullets[i].present = 1;
                        PlayerBullets[i].lastPlayerSpace = 1;
                }
        }
        for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
                if (GameNumbers->t2 - PlayerBullets[i].SpawnTime > BulletCooldown) {
                        GameNumbers->lastspace++;
                        PlayerBullets[i].lastPlayerSpace = 0;
                }
        }
        if (GameNumbers->space >= 5) {
                GameNumbers->space = 0;
        }
}
void BulletUpdate(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        if (GameNumbers->stagenumber == 3) {
                GameNumbers->enemymove = 1;
        }
        else {
                GameNumbers->enemymove = 0;
        }
        for (int i = 0; i < MAX_BULLETS; i++) {
                double PaternX = 0;
                double PaternY = 0;
                if (Bullets[i].AdvancedPaternX == 1) {
                        PaternX = cos(GameNumbers->worldTime);
                }
                if (Bullets[i].AdvancedPaternX == 2) {
                        PaternX = sin(GameNumbers->worldTime);
                }
                if (Bullets[i].AdvancedPaternX == 3) {
                        PaternX = tan(GameNumbers->worldTime);
                }
                if (Bullets[i].AdvancedPaternY == 1) {
                        PaternY = sin(GameNumbers->worldTime);
                }
                if (Bullets[i].AdvancedPaternY == 2) {
                        PaternY = cos(GameNumbers->worldTime);
                }
                if (Bullets[i].AdvancedPaternY == 3) {
                        PaternY = tan(GameNumbers->worldTime);
                }
                int distance = (GameNumbers->t2 - Bullets[i].SpawnTime);
                int x = (Bullets[i].directionX) * (distance * BulletSpeed) + Bullets[i].x + PaternX * SpiralX + GameNumbers->PlayerX + GameNumbers->enemymove * cos(GameNumbers->worldTime) * enemy3;
                int y = (Bullets[i].directionY) * (distance * BulletSpeed) + Bullets[i].y + PaternY * SpiralY + GameNumbers->PlayerY;
                if (Bullets[i].present == 1) {
                        if (x < -(BACKGROUND_WIDTH / 2) + GameNumbers->PlayerX || x >(BACKGROUND_WIDTH / 2) + GameNumbers->PlayerX) {
                                Bullets[i].present = 0;
                                distance = 0;
                        }
                        if (y > (BACKGROUND_HEIGHT / 2) + GameNumbers->PlayerY || y < -(BACKGROUND_HEIGHT / 4) + GameNumbers->PlayerY) {
                                Bullets[i].present = 0;
                                distance = 0;
                        }
                }
                if (
                        (PLAYER_STARTING_X + PLAYER_WIDTH / 2 > x - BULLET_WIDTH / 2) &&
                        (PLAYER_STARTING_X - PLAYER_WIDTH / 2 < x + BULLET_WIDTH / 2) &&
                        (PLAYER_STARTING_Y + PLAYER_HEIGHT / 2 > y - BULLET_HEIGHT / 2) &&
                        (PLAYER_STARTING_Y - PLAYER_HEIGHT / 2 < y + BULLET_HEIGHT / 2)
                        && GameNumbers->playerHitTime == -1) {
                        Bullets[i].present = 0;
                        distance = 0;
                        GameNumbers->playerHitTime = GameNumbers->t2;
                        GameNumbers->PlayerHealth--;
                        GameNumbers->SCORE--;
                }
                if (Bullets[i].present == 1) {
                        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->Bullet, x, y);
                }
        }
        PlayerBulletUpdate(SDL_MANAGER, GameNumbers, Bullets, PlayerBullets);
}
void PlayerBulletUpdate(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers, Bullet Bullets[MAX_BULLETS], Bullet PlayerBullets[MAX_PLAYER_BULLETS]) {
        for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
                                int distance2 = (GameNumbers->t2 - PlayerBullets[i].SpawnTime);
                                int x = -PLAYER_STARTING_X + PlayerBullets[i].x + GameNumbers->PlayerX;
                                int y = (distance2 * BulletSpeed) - PLAYER_STARTING_Y + PlayerBullets[i].y + GameNumbers->PlayerY;
                                if (y > (BACKGROUND_HEIGHT / 3) + GameNumbers->PlayerY && PlayerBullets[i].present == 1) {
                                                PlayerBullets[i].present = 0;
                                                distance2 = 0;
                                                GameNumbers->HitStreak = 0;
                                }
                                if (PlayerBullets[i].present == 1) {
                                                if (
                                                                (GameNumbers->PlayerX + ENEMY_X + ENEMY_WIDTH / 2 + GameNumbers->Level3access * cos(GameNumbers->worldTime) * enemy3 > x - SWORD_WIDTH / 2) &&
                                                                (GameNumbers->PlayerX + ENEMY_X - ENEMY_WIDTH / 2 + GameNumbers->Level3access * cos(GameNumbers->worldTime) * enemy3 < x + SWORD_WIDTH / 2) &&
                                                                (GameNumbers->PlayerY + ENEMY_Y + ENEMY_HEIGHT / 2 > y - SWORD_HEIGHT / 2) &&
                                                                (GameNumbers->PlayerY + ENEMY_Y - ENEMY_HEIGHT / 2 < y + SWORD_HEIGHT / 2)
                                                                ) {
                                                                PlayerBullets[i].present = 0;
                                                                distance2 = 0;
                                                                GameNumbers->HitStreak++;
                                                                if (GameNumbers->stagenumber == 1) {
                                                                                GameNumbers->Boss1Health--;
                                                                }
                                                                if (GameNumbers->stagenumber == 2) {
                                                                                GameNumbers->Boss2Health--;
                                                                }
                                                                if (GameNumbers->stagenumber == 3) {
                                                                                GameNumbers->Boss3Health--;
                                                                }
                                                                if (GameNumbers->HitStreak > 5 && GameNumbers->HitStreak <= 10) {
                                                                        GameNumbers->SCORE = GameNumbers->SCORE + 2;
                                                                }
                                                                if (GameNumbers->HitStreak > 10) {
                                                                        GameNumbers->SCORE = GameNumbers->SCORE + 3;
                                                                }
                                                                if (GameNumbers->HitStreak <= 5) {
                                                                        GameNumbers->SCORE++;
                                                                }
                                                }
                                }
                                if (PlayerBullets[i].present == 1) {
                                                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->sword, x, y);
                                }
                }
}
void SpawnEnemy(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if (GameNumbers->stagenumber == 1) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->babawodna, ENEMY_X + GameNumbers->PlayerX, ENEMY_Y + GameNumbers->PlayerY);
        }
        if (GameNumbers->stagenumber == 2) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->golem, ENEMY_X + GameNumbers->PlayerX, ENEMY_Y + GameNumbers->PlayerY);
        }
        if (GameNumbers->stagenumber == 3) {
                if (GameNumbers->t2 - GameNumbers->EnemyAnimationTime > 200) {
                        GameNumbers->enemyanimationframe++;
                        GameNumbers->EnemyAnimationTime = GameNumbers->t2;
                }
                if (GameNumbers->enemyanimationframe > 2) {
                        GameNumbers->enemyanimationframe = 0;
                }
                switch (GameNumbers->enemyanimationframe)
                {
                case 0: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->dragon1, ENEMY_X + GameNumbers->PlayerX + cos(GameNumbers->worldTime) * enemy3, ENEMY_Y + GameNumbers->PlayerY); break;
                case 1: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->dragon2, ENEMY_X + GameNumbers->PlayerX + cos(GameNumbers->worldTime) * enemy3, ENEMY_Y + GameNumbers->PlayerY); break;
                case 2: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->dragon3, ENEMY_X + GameNumbers->PlayerX + cos(GameNumbers->worldTime) * enemy3, ENEMY_Y + GameNumbers->PlayerY); break;
                }
        }
}
void PlayerAnimation(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if (GameNumbers->PlayerAnimationYRight == 0 && GameNumbers->PlayerAnimationYLeft == 0 && GameNumbers->PlayerAnimationXDOWN == 0 && GameNumbers->PlayerAnimationXUP == 0) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->geralt, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
        else {
                if ((GameNumbers->PlayerAnimationYRight == 1 && GameNumbers->PlayerAnimationYLeft == 1) || (GameNumbers->PlayerAnimationXDOWN == 1 && GameNumbers->PlayerAnimationXUP == 1)) {
                        DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->geralt, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                }
                else if (GameNumbers->PlayerAnimationYRight == 1 || GameNumbers->PlayerAnimationYLeft == 1 || GameNumbers->PlayerAnimationXDOWN == 1 || GameNumbers->PlayerAnimationXUP == 1) {
                        if (GameNumbers->t2 - GameNumbers->PlayerAnimationTime > 100) {
                                GameNumbers->animationframe++;
                                GameNumbers->PlayerAnimationTime = GameNumbers->t2;
                        }
                        if (GameNumbers->animationframe > 4) {
                                GameNumbers->animationframe = 0;
                        }
                        switch (GameNumbers->animationframe)
                        {
                        case 0: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->geralt1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); break;
                        case 1: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->geralt2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); break;
                        case 2: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->geralt3, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); break;
                        case 3: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->geralt4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); break;
                        case 4: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->geralt5, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); break;
                        }
                }
        }
        HealthAnimation(SDL_MANAGER, GameNumbers);
}
void HealthAnimation(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if (GameNumbers->HealthAnimation == 1) {
                if (GameNumbers->t2 - GameNumbers->HealthAnimationTime > 100) {
                        GameNumbers->healthanimationframe++;
                        GameNumbers->HealthAnimationTime = GameNumbers->t2;
                        GameNumbers->HealthTime++;
                }
                if (GameNumbers->healthanimationframe > 1) {
                        GameNumbers->healthanimationframe = 0;
                }
                switch (GameNumbers->healthanimationframe)
                {
                case 0: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->health1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); break;
                case 1: DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->health2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); break;
                }
        }
        if (GameNumbers->HealthTime > 5) {
                GameNumbers->HealthAnimation = 0;
        }
}
void PlayerInvi(SDL_MANAGING* SDL_MANAGER, GameNumbers* GameNumbers) {
        if ((GameNumbers->playerHitTime != -1) && ((GameNumbers->playerHitTime - GameNumbers->t2) < immortaltime)) {
                GameNumbers->playerHitTime = -1;
        }
        if (GameNumbers->playerHitTime != -1) {
                DrawSurface(SDL_MANAGER->screen, SDL_MANAGER->shield, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
}
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
        int px, py, c;
        SDL_Rect s, d;
        s.w = 8;
        s.h = 8;
        d.w = 8;
        d.h = 8;
        while (*text) {
                c = *text & 255;
                px = (c % 16) * 8;
                py = (c / 16) * 8;
                s.x = px;
                s.y = py;
                d.x = x;
                d.y = y;
                SDL_BlitSurface(charset, &s, screen, &d);
                x += 8;
                text++;
        };
};
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
        SDL_Rect dest;
        dest.x = x - sprite->w / 2;
        dest.y = y - sprite->h / 2;
        dest.w = sprite->w;
        dest.h = sprite->h;
        SDL_BlitSurface(sprite, NULL, screen, &dest);
};
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
        int bpp = surface->format->BytesPerPixel;
        Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
        *(Uint32*)p = color;
};
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
        for (int i = 0; i < l; i++) {
                DrawPixel(screen, x, y, color);
                x += dx;
                y += dy;
        };
};
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
        int i;
        DrawLine(screen, x, y, k, 0, 1, outlineColor);
        DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
        DrawLine(screen, x, y, l, 1, 0, outlineColor);
        DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
        for (i = y + 1; i < y + k - 1; i++)
                DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};
void KeyboardInput(GameNumbers* GameNumbers) {
        const Uint8* currentPressed = SDL_GetKeyboardState(NULL);
        if (currentPressed[SDL_SCANCODE_UP]) {
                PlayerMovementUp(GameNumbers);
                GameNumbers->PlayerAnimationXUP = 1;
        }
        else {
                GameNumbers->PlayerAnimationXUP = 0;
        }
        if (currentPressed[SDL_SCANCODE_DOWN]) {
                PlayerMovementDown(GameNumbers);
                GameNumbers->PlayerAnimationXDOWN = 1;
        }
        else {
                GameNumbers->PlayerAnimationXDOWN = 0;
        }
        if (currentPressed[SDL_SCANCODE_LEFT]) {
                PlayerMovementLeft(GameNumbers);
                GameNumbers->PlayerAnimationYLeft = 1;
        }
        else {
                GameNumbers->PlayerAnimationYLeft = 0;
        }
        if (currentPressed[SDL_SCANCODE_RIGHT]) {
                PlayerMovementRight(GameNumbers);
                GameNumbers->PlayerAnimationYRight = 1;
        }
        else {
                GameNumbers->PlayerAnimationYRight = 0;
        }
}
void PlayerMovementRight(GameNumbers* GameNumbers) {
        if (GameNumbers->PlayerX > -(BACKGROUND_WIDTH / 2 - SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2)) {
                GameNumbers->PlayerX -= PlayerSpeed;
        }
}
void PlayerMovementLeft(GameNumbers* GameNumbers) {
        if (GameNumbers->PlayerX < (BACKGROUND_WIDTH / 2 + SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2)) {
                GameNumbers->PlayerX += PlayerSpeed;
        }
}
void PlayerMovementUp(GameNumbers* GameNumbers) {
        if (GameNumbers->PlayerY < (BACKGROUND_HEIGHT / 4 + SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2)) {
                GameNumbers->PlayerY += PlayerSpeed;
        }
}
void PlayerMovementDown(GameNumbers* GameNumbers) {
        if (GameNumbers->PlayerY > -(BACKGROUND_HEIGHT / 4 - SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2)) {
                GameNumbers->PlayerY -= PlayerSpeed;
        }
}
void FPSCount(GameNumbers* GameNumbers) {
        GameNumbers->t2 = SDL_GetTicks();
        GameNumbers->delta = (GameNumbers->t2 - GameNumbers->t1) * 0.001;
        GameNumbers->t1 = GameNumbers->t2;
        GameNumbers->worldTime += GameNumbers->delta;
        GameNumbers->distance = GameNumbers->distance + 0.1;
        GameNumbers->fpsTimer += GameNumbers->delta;
        if (GameNumbers->fpsTimer > 0.5) {
                GameNumbers->fps = (GameNumbers->frames * 2);
                GameNumbers->frames = 0;
                GameNumbers->fpsTimer -= 0.5;
        };
}
void ColorsManager(SDL_MANAGING* SDL_MANAGING, Colors* colors) {
        colors->czarny = SDL_MapRGB(SDL_MANAGING->screen->format, 0x00, 0x00, 0x00);
        colors->zielony = SDL_MapRGB(SDL_MANAGING->screen->format, 0x00, 0xFF, 0x00);
        colors->czerwony = SDL_MapRGB(SDL_MANAGING->screen->format, 0xFF, 0x00, 0x00);
        colors->niebieski = SDL_MapRGB(SDL_MANAGING->screen->format, 0x11, 0x11, 0xCC);
}
void GamePrep(SDL_MANAGING* SDL_MANAGER) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(SDL_MANAGER->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_SetRenderDrawColor(SDL_MANAGER->renderer, 0, 0, 0, 255);
        SDL_SetWindowTitle(SDL_MANAGER->window, "Wticher Bullet Hell");
        BMPLoading(SDL_MANAGER);
        SDL_MANAGER->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        SDL_MANAGER->scrtex = SDL_CreateTexture(SDL_MANAGER->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void Rendering(SDL_MANAGING* SDL_MANAGER) {
        SDL_UpdateTexture(SDL_MANAGER->scrtex, NULL, SDL_MANAGER->screen->pixels, SDL_MANAGER->screen->pitch);
        SDL_RenderCopy(SDL_MANAGER->renderer, SDL_MANAGER->scrtex, NULL, NULL);
        SDL_RenderPresent(SDL_MANAGER->renderer);
}
void BMPLoading(SDL_MANAGING* SDL_MANAGER) {
        SDL_MANAGER->charset = SDL_LoadBMP("./cs8x8.bmp");
        SDL_MANAGER->geralt = SDL_LoadBMP("./geralt.bmp");
        SDL_MANAGER->geralt1 = SDL_LoadBMP("./geralt1.bmp");
        SDL_MANAGER->geralt2 = SDL_LoadBMP("./geralt2.bmp");
        SDL_MANAGER->geralt3 = SDL_LoadBMP("./geralt3.bmp");
        SDL_MANAGER->geralt4 = SDL_LoadBMP("./geralt4.bmp");
        SDL_MANAGER->geralt5 = SDL_LoadBMP("./geralt5.bmp");
        SDL_MANAGER->background1 = SDL_LoadBMP("./background1.bmp");
        SDL_MANAGER->background2 = SDL_LoadBMP("./background2.bmp");
        SDL_MANAGER->background3 = SDL_LoadBMP("./background3.bmp");
        SDL_MANAGER->babawodna = SDL_LoadBMP("./baba_wodna.bmp");
        SDL_MANAGER->Bullet = SDL_LoadBMP("./Bullet.bmp");
        SDL_MANAGER->MenuPage = SDL_LoadBMP("./MenuPage.bmp");
        SDL_MANAGER->golem = SDL_LoadBMP("./golem.bmp");
        SDL_MANAGER->dragon1 = SDL_LoadBMP("./dragon1.bmp");
        SDL_MANAGER->dragon2 = SDL_LoadBMP("./dragon2.bmp");
        SDL_MANAGER->dragon3 = SDL_LoadBMP("./dragon3.bmp");
        SDL_MANAGER->sword = SDL_LoadBMP("./sword.bmp");
        SDL_MANAGER->shield = SDL_LoadBMP("./shield.bmp");
        SDL_MANAGER->gameover = SDL_LoadBMP("./gameover.bmp");
        SDL_MANAGER->victory = SDL_LoadBMP("./victoryscreen.bmp");
        SDL_MANAGER->potion = SDL_LoadBMP("./potion.bmp");
        SDL_MANAGER->health = SDL_LoadBMP("./health.bmp");
        SDL_MANAGER->health1 = SDL_LoadBMP("./health1.bmp");
        SDL_MANAGER->health2 = SDL_LoadBMP("./health2.bmp");
        SDL_MANAGER->shield1 = SDL_LoadBMP("./shield1.bmp");
        SDL_MANAGER->shield2 = SDL_LoadBMP("./shield2.bmp");
        SDL_MANAGER->shield3 = SDL_LoadBMP("./shield3.bmp");
        SDL_MANAGER->shield4 = SDL_LoadBMP("./shield4.bmp");
        SDL_MANAGER->rankup = SDL_LoadBMP("./rankup.bmp");
        SDL_SetColorKey(SDL_MANAGER->charset, true, 0x000000);
}
void ENDING(SDL_MANAGING* SDL_MANAGER) {
        SDL_FreeSurface(SDL_MANAGER->charset);
        SDL_FreeSurface(SDL_MANAGER->screen);
        SDL_DestroyTexture(SDL_MANAGER->scrtex);
        SDL_DestroyWindow(SDL_MANAGER->window);
        SDL_DestroyRenderer(SDL_MANAGER->renderer);
        SDL_Quit();
}

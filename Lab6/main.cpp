#include "mbed.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "MMA8452.h"
#include "Speaker.h"
#include "Alien.h"
#define _ 0x000000
#define X 0xFFFFFF
#define ALIEN_HEIGHT 8
#define ALIEN_WIDTH 13
#define NULL 0

PinDetect pb1(p11);
PinDetect pb2(p12);
PinDetect pb3(p13);

Speaker mySpeaker(p21);

uLCD_4DGL uLCD(p28, p27, p29);

Serial pc(USBTX,USBRX);

class Ship {
    public:
        // Draw white for moving, black for erasing
        void draw(int x1, int x2, int color) {
            uLCD.filled_rectangle(x1, 124, x2, 128,color);
        }

        void move(int x1, int x2, bool left) {
            draw(x1, x2, WHITE);
            draw(x1 + (left ? 16 : -16), x2 + (left? 16 : -16), BLACK);
        }

        void shoot(int x1, int y1, int x2, int y2) {
            uLCD.line(x1 + 8, y1, x2 - 9, y2, GREEN);
        }
};

int alienAlice_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
    _,_,_,_,_,X,X,X,_,_,_,_,_,
    _,_,X,X,X,X,X,X,X,X,X,_,_,
    _,X,X,X,X,X,X,X,X,X,X,X,_,
    _,X,X,X,_,_,X,_,_,X,X,X,_,
    _,X,X,X,X,X,X,X,X,X,X,X,_,
    _,_,_,_,X,X,_,X,X,_,_,_,_,
    _,_,_,X,X,_,_,_,X,X,_,_,_,
    _,X,X,_,_,_,X,_,_,_,X,X,_,
};

int alienBobDown_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
     _,_,_,X,_,_,_,_,_,X,_,_,_,
     _,_,_,_,X,_,_,_,X,_,_,_,_,
     _,_,_,X,X,X,X,X,X,X,_,_,_,
     _,_,X,X,_,X,X,X,_,X,X,_,_,
     _,X,X,X,X,X,X,X,X,X,X,X,_,
     _,X,_,X,X,X,X,X,X,X,_,X,_,
     _,X,_,X,_,_,_,_,_,X,_,X,_,
     _,_,_,_,X,X,_,X,X,_,_,_,_,
};

int alienBobUp_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
     _,_,_,X,_,_,_,_,_,X,_,_,_,
     _,X,_,_,X,_,_,_,X,_,_,X,_,
     _,X,_,X,X,X,X,X,X,X,_,X,_,
     _,X,X,X,_,X,X,X,_,X,X,X,_,
     _,X,X,X,X,X,X,X,X,X,X,X,_,
     _,_,_,X,X,X,X,X,X,X,_,_,_,
     _,_,_,X,_,_,_,_,_,X,_,_,_,
     _,_,X,_,_,_,_,_,_,_,X,_,_,
};

class AlienBob : public Alien {
    public:
        AlienBob(int xin, int yin, bool left, int up) : Alien(xin,yin,left), upNum(up) {
            draw();
        }

        void draw() {
            if(upNum % 2 == 0) {
                uLCD.BLIT(x, y, ALIEN_WIDTH, ALIEN_HEIGHT, alienBobDown_sprite);
            } else {
                uLCD.BLIT(x, y, ALIEN_WIDTH, ALIEN_HEIGHT, alienBobUp_sprite);
            }
            upNum++;
        }

        void shift() {
            if (moveleft) {
                x--;
                draw();
                if(x <= 0) {
                    moveleft = false;
                }
            } else {
                x++;
                draw();
                if(x >= 115) {
                    moveleft = true;
                }
            }
        }

    private:
        int upNum;
};

class AlienAlice : public Alien {
    public:
        AlienAlice(int xin, int yin, bool left) : Alien(xin,yin,left) {
            draw();
        }

        void draw() {
            uLCD.locate(0,0);
            uLCD.BLIT(x, y, ALIEN_WIDTH, ALIEN_HEIGHT, alienAlice_sprite);
        }

        void shift() {
            if (moveleft) {
                x--;
                draw();
                if(x <= 0) {
                moveleft = false;
                }
            } else {
                x++;
                draw();
                if (x >= 115) {
                    moveleft = true;
                }
            }
        }
};

class CircleAlienB: public Alien {
    public:
        CircleAlienB(int xin, int yin, bool left) : Alien(xin, yin, left) {
            draw();
        }

        void draw() {
            uLCD.locate(0, 0);
            uLCD.filled_circle(x, y, 4, BLUE);
        }

        void shift() {
            if (moveleft) {
                x--;
                draw();
                uLCD.circle(x + 1, y, 4, BLACK);
                if(x <= 0) {
                    moveleft = false;
                }
            } else {
                x++;
                draw();
                uLCD.circle(x - 1, y, 4, BLACK);
                if(x >= 115) {
                    moveleft = true;
                }
            }
        }
};

class CircleAlienR: public Alien {
    public:
        CircleAlienR(int xin, int yin, bool left) : Alien(xin, yin, left) {
            draw();
        }

        void draw() {
            uLCD.locate(0, 0);
            uLCD.circle(x, y, 4, RED);
        }

        void shift() {
            if (moveleft) {
                x -= 2;
                draw();
                uLCD.circle(x + 2, y, 4, BLACK);
                if(x <= 0) {
                    moveleft = false;
                }
            } else {
                x += 2;
                draw();
                uLCD.circle(x - 2, y, 4, BLACK);
                if(x >= 115) {
                    moveleft = true;
                }
            }
        }
};

bool goLeft = false;
bool goRight = false;
bool shoot = false;

void pb1_hit_callback (void) {
    goLeft = true;
}

void pb2_hit_callback (void) {
    goRight = true;
}

void pb3_hit_callback (void) {
    shoot = true;
}

int main() {
    mySpeaker.PlayNote(500.0,0.25,0.1);
    mySpeaker.PlayNote(450.0,0.25,0.1);
    mySpeaker.PlayNote(400.0,0.25,0.1);
    mySpeaker.PlayNote(350.0,0.25,0.1);
    mySpeaker.PlayNote(300.0,0.25,0.1);

    uLCD.baudrate(3000000);
    wait(0.2);

    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    wait(.01);

    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb3.attach_deasserted(&pb3_hit_callback);

    pb1.setSampleFrequency();
    pb2.setSampleFrequency();
    pb3.setSampleFrequency();

    Ship ship;
    int left = 55;
    int right = 70;
    ship.draw(left, right, WHITE);

    // For placement of bullet
    int bottom = 124;
    int top = 117;

    srand(time(0));
    Alien * aliens[8];
    int startLocations[8] = {10, 22, 34, 46, 58, 70, 82, 94};
    for(int i = 0; i < 8; i++) {
        int x = 10 + (rand() % 105); // Randomize starting x position
        int alienType = rand() % 4; // Randomize type of alien
        if(alienType == 0) {
            aliens[i] = new AlienBob(x, startLocations[i], true, 0);
        } else if(alienType == 1) {
            aliens[i] = new AlienAlice(x, startLocations[i], true);
        } else if(alienType == 2) {
            aliens[i] = new CircleAlienB(x, startLocations[i], true);
        } else {
            aliens[i] = new CircleAlienR(x, startLocations[i], true);
        }
    }


    int radius = 10;
    int offsetx = 63;
    int offsety = 63;
    double factor = 50;
    double x = 0, y = 0, z = 0;

    MMA8452 acc(p9, p10, 40000);
    acc.setBitDepth(MMA8452::BIT_DEPTH_12);
    acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
    acc.setDataRate(MMA8452::RATE_100);

    Timer t;
    t.start();

    bool gameOver = false;
    bool launched = false; // If bullet has been shot
    bool hit = false; // If bullet hit something

    int dead = 0;

    // As long as aliens remain
    while(!gameOver) {
        uLCD.filled_circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, BLACK);

        if(!acc.isXYZReady()) {
            wait(0.01);
        } else {
            acc.readXYZGravity(&x,&y,&z);
            uLCD.filled_circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, WHITE);
        }

        for(int i = 0; i < 8; i++) {
            // If this alien is not dead
            if(aliens[i] != NULL) {
                aliens[i]->shift();
                hit = aliens[i]->checkCollision(left, aliens[i]->getX(), top, aliens[i]->getY());
                if(hit) {
                    // Remove alien
                    uLCD.filled_rectangle(aliens[i]->getX(), aliens[i]->getY(), 128, (aliens[i]->getY() + 8), BLACK);
                    uLCD.filled_circle(aliens[i]->getX(), aliens[i]->getY(), 7, BLACK);
                    delete aliens[i];
                    aliens[i] = NULL;

                    // Remove & reset bullet
                    hit = false;
                    launched = false;
                    uLCD.line(left + 6, bottom - 2, right - 7, top + 2, BLACK); // Previously 8, 0, -9 and 0
                    bottom = 124;
                    top = 117;

                    dead++;

                    // If all aliens dead, game over
                    if(dead == 8) {
                        gameOver = true;
                        uLCD.cls();
                        t.stop();
                        uLCD.text_height(2);
                        uLCD.color(BLUE);
                        uLCD.printf("You saved\n");
                        uLCD.printf("Earth in\n");
                        uLCD.printf("%1f seconds", t.read());
                        uLCD.printf("Well done, player!");
                        break;
                    }
                }
            }
        }

        // Move bullet unless it has hit an alien or gone off screen
        if(launched && !hit) {
            ship.shoot(left, --bottom, right, --top);
            uLCD.filled_rectangle(left - 15, bottom + 4, right + 15, top + 7, BLACK);
            wait(.05);
            if (bottom < -3) {
               launched = false;
               bottom = 124;
               top = 127;
            }
        }

        if(goLeft && left > 0) {
           ship.move(--left, --right, true);
           goLeft = false;
        }

        if(goRight && right < 128) {
            ship.move(++left, ++right, false);
            goRight = false;
        }

        if(shoot) {
            mySpeaker.PlayNote(200.0,0.25,0.05);
            ship.shoot(left, bottom, right, top);
            launched = true;
            shoot = false;
        }
    }
}

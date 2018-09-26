class Alien {
    public:
        Alien(int X, int Y, bool left) {
            x = X;
            y = Y;
            moveleft = left;
        }
        
        bool checkCollision(int bulletX, int x, int bulletY, int y) {
            return bulletX > (x - 5) && bulletX < (x + 5) && bulletY == y;
        }

        int getX() {
            return x;
        }

        int getY() {
            return y;
        }
        
        virtual void draw() = 0;
        virtual void shift() = 0;

    protected:
        int x;
        int y;  
        bool moveleft;
};
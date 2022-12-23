#ifndef MOUSE_H
#define MOUSE_H
class Mouse {
    private:
        double x;
        double y;
        bool firstMouse;
        double dx;
        double dy;
    public:
        Mouse();
        double getX();
        double getY();
        double getDx();
        double getDy();
        void handleInput(double xpos, double ypos);
};
#endif
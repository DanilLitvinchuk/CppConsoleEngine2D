#include <iostream>
#include <cstdlib> 
#include <thread>
#include <chrono>
#include <cmath>


using namespace std;

const int renderSizeX = 80;
const int renderSizeY = 24;

class Canvas{
private:
    static const int xSize = renderSizeX;
    static const int ySize = renderSizeY;
    char window[ySize][xSize];
public:
    void init(){
        for(int i = 0; i < ySize; i++){
            for(int j = 0; j < xSize; j++){
                window[i][j] = '.';
            }
        }
    }

    int GetX(){
        return xSize;
    }
    int GetY(){
        return ySize;
    }

    void setChar(int x, int y, char brush) {
        if (x >= 0 && x < xSize && y >= 0 && y < ySize) {
            window[y][x] = brush;
        }
    }

    // void render(){
    //     //system("clear");
    //     cout << "\033[H\033[2J"; 
    //     for (int i = 0; i < ySize; i++) {
    //         for (int j = 0; j < xSize; j++) {
    //             cout << window[i][j];
    //         }
    //         cout << "\n";
    //     }
    // }

    void render() {
    string frame = "\033[H"; 
    for (int i = 0; i < ySize; i++) {
        for (int j = 0; j < xSize; j++) {
            frame += window[i][j]; 
        }
        frame += '\n';
    }
    cout << frame << flush;
}
};

class Point{
private:
    float posX;
    float posY;
    char brush;
    float speedX = 0;
    float speedY = 0;
public: 
    Point(float x, float y, char b){
        posX = x;
        posY = y;
        brush = b;
    }

    void SetSpeedX(float x){
        speedX = x;
    }

    void SetSpeedY(float y){
        speedY = y;
    }

    void colis(){
        if(posX > renderSizeX){
            speedX = -abs(speedX);
        }
        if(posX < 0){
            speedX = abs(speedX);
        }

        if(posY > renderSizeY){
            speedY = -abs(speedY);
        }
        if(posY < 0){
            speedY = abs(speedY);
        }
    }

    void update_pos(){
        posX+=speedX;
        posY+=speedY;
    }

    void draw(Canvas &canv){
        canv.setChar(round(posX), round(posY), brush);
    }
}; 

class Object{
private:
    float Ax;
    float Bx;
    float Cx;
    float Ay;
    float By;
    float Cy;
    char brush;
    float speedX = 0;
    float speedY = 0;
public:
    Object(float ax, float bx, float cx, float ay, float by, float cy, char b){
        Ax = ax;
        Bx = bx;
        Cx = cx;
        Ay = ay;
        By = by;
        Cy = cy;
        brush = b;
    }

    // Set
    void SetAx(float coordinat){
        Ax = coordinat;
    }
    void SetAy(float coordinat){
        Ay = coordinat;
    }
    void SetBx(float coordinat){
        Bx = coordinat;
    }
    void SetBy(float coordinat){
        By = coordinat;
    }
    void SetCx(float coordinat){
        Cx = coordinat;
    }
    void SetCy(float coordinat){
        Cy = coordinat;
    }

    void SetSpeedX(float speed){
        speedX = speed;
    }
    void SetSpeedY(float speed){
        speedY = speed;
    }

    // Get
    float GetAx(){
        return Ax;
    }
    float GetAy(){
        return Ay;
    }
    float GetBx(){
        return Bx;
    }
    float GetBy(){
        return By;
    }
    float GetCx(){
        return Cx;
    }
    float GetCy(){
        return Cy;
    }

    float GetSpeedX(){
        return speedX;
    }
    float GetSpeedY(){
        return speedY;
    }

    // logic
    void update_pos(){
        Ax+=speedX;
        Ay+=speedY;
        Bx+=speedX;
        By+=speedY;
        Cx+=speedX;
        Cy+=speedY;
    }

    void colis(){
        float minX = min(Ax, min(Bx, Cx));
        float maxX = max(Ax, max(Bx, Cx));
        float minY = min(Ay, min(By, Cy));
        float maxY = max(Ay, max(By, Cy));

         if (maxX > renderSizeX) {
            float offset = maxX - 80;
            Ax -= offset; Bx -= offset; Cx -= offset;
            speedX = -abs(speedX);
        } 
        else if (minX < 0) {
            float offset = 1 - minX;
            Ax += offset; Bx += offset; Cx += offset; 
            speedX = abs(speedX);
        }

        if (maxY > renderSizeY) {
            float offset = maxY - 24;
            Ay -= offset; By -= offset; Cy -= offset;
            speedY = -abs(speedY);
        } 
        else if (minY < 0) {
            float offset = 1 - minY;
            Ay += offset; By += offset; Cy += offset;
            speedY = abs(speedY);
        }
    }


    void drawLine(Canvas &canv, int onex, int oney, int twox, int twoy, char brush){
        float dx = twox - onex;
        float dy = twoy - oney;

        float steps = max(abs(dx), abs(dy));

        float x = onex;
        float y = oney;

        float xStep = dx / steps;
        float yStep = dy /steps;

        for(int i = 0; i <= steps; i++){
            canv.setChar(round(x), round(y), brush);
            x+=xStep;
            y+=yStep;
        }
    }

    void draw(Canvas &canv){
        drawLine(canv, Ax, Ay, Bx, By, brush);
        drawLine(canv, Bx, By, Cx, Cy, brush);
        drawLine(canv, Cx, Cy, Ax, Ay, brush);
    }
};

class Square{
private:
    Object t1;
    Object t2;
public:
    Square(float x, float y, float size, char brush):
        t1(x, x + size*2, x + size*2, y, y, y + size, brush),
        t2(x, x, x + size*2, y, y + size, y + size, brush)
        {

        }

    void SetSpeed(float x, float y){
        t1.SetSpeedX(x);
        t1.SetSpeedY(y);
        t2.SetSpeedX(x);
        t2.SetSpeedY(y);
    }

    void update_pos(){
        t1.update_pos();
        t2.update_pos();
    }
     
    void colis() {
        t1.colis();
        t2.colis();
    }

    void draw(Canvas &canv) {
        t1.draw(canv);
        t2.draw(canv);
    }
};

int main(){
    Canvas window;
    Square obj(20, 5, 5, '@');
    Object obj2(10, 5, 15, 
                15, 11, 8, '@');
    Point obj3(40, 5, '@');

    obj.SetSpeed(0, 0.1);
    obj2.SetSpeedY(0.1);
    obj3.SetSpeedY(0.1);
    while(true){
        this_thread::sleep_for(chrono::milliseconds(10));
        obj.update_pos();
        obj2.update_pos();
        obj3.update_pos();
        obj.colis();
        obj2.colis();
        obj3.colis();

        window.init();
        obj.draw(window);
        obj2.draw(window);
        obj3.draw(window);
        window.render();
    }
    return 0;
}
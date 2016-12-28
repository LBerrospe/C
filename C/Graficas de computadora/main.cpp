#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glext.h>
#include <stack>

#define WIDTH 300
#define HEIGHT 300
#define RADIAN M_PI/180
#define C_PE (2 * M_PI)/ 360
#define TWOPI 2*M_PI
#define BRES_LINE 1
#define CIRCLE 2
#define ELIPSE 3
#define POLYGON 4
#define PENCIL 5
#define ERASER 6
#define SPRAY 7
#define CURVED_LINE 8
#define FILL 9
#define GL_BGR 0x80E0

using namespace std;

struct Point {
    int x;
    int y;
};//Point
struct Point Point[8];

float colorArray[WIDTH][HEIGHT],angle;
int xStart, yStart, xFinish, yFinish, radius, radiusX, radiusY,numberSides=3,
    tool=BRES_LINE,eraserSize=10, e = eraserSize/2,spraySize=20, s = spraySize/2
    ,pressButton=0 ,click=0,xOne, yOne,xTwo,yTwo;
bool polar = true;
FILE *file;

void init(void) {
   glClearColor(1.0, 1.0, 1.0, 0.0); // set color of window
   glPointSize(1.0);
   glMatrixMode(GL_PROJECTION); // projection in a bidimensional rectangular area
   gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT); // coordinate system
}

void drawPixel(int x, int y) {
    switch(tool){
    case BRES_LINE:case CIRCLE:case ELIPSE:case POLYGON:
    case PENCIL:case SPRAY:case CURVED_LINE:case FILL:
        glBegin(GL_POINTS);
            glVertex2i(x,y);
        glEnd();
        break;

    case ERASER:
        glBegin(GL_QUAD_STRIP);
            glVertex2i(x - e, y + e);
            glVertex2i(x - e, y - e);
            glVertex2i(x + e, y + e);
            glVertex2i(x + e, y - e);
        glEnd();
        break;
    }//switch
}//putPixel

void bresenhamLine( int xStart, int yStart, int xFinish, int yFinish ) {
    int dx=fabs(xFinish-xStart);
    int dy=fabs(yFinish-yStart);
    int x=xStart, y=yStart, p, incradiusX, incradiusY, stop;
    drawPixel(xStart, yStart);
    if ( xStart == xFinish ) {
        incradiusX=0;
    } else if ( xStart < xFinish ) {
        incradiusX=1;
    } else {
        incradiusX=-1;
    }//if{x}elseif{x}else{x}
    if ( yStart == yFinish ) {
        incradiusY=0;
    } else if ( yStart < yFinish ) {
        incradiusY=1;
    } else {
        incradiusY=-1;
    }//if{y}elseif{y}else{y}
    if ( dy>dx ) {
        p=2*dx - dy;
        stop=dy;
        while ( stop > 0) {
            y=y+incradiusY;
            if ( p < 0) {
                p=p+2*dx;
            } else {
                x=x+incradiusX;
                p=p+2*(dx-dy);
            }//if{p<0}else{}
            drawPixel(x,y);
            stop--;
        }//while
    } else {
        p=2*dy - dx;
        stop=dx;
        while ( stop > 0) {
            x=x+incradiusX;
            if ( p < 0) {
                p=p+2*dy;
            } else {
                y=y+incradiusY;
                p=p+2*(dy-dx);
            }//if{p<0}else{}
            drawPixel(x,y);
            stop--;
        }//while
    }//if {dy>dx} else {}
}//bresenhamLine()

void polarCircle(int xStart, int yStart, int radius) {
    float radian=0;
    int angle;
    for ( angle = 0; angle < 360; angle++) {
        drawPixel(xStart+ ceil( radius*cos(angle*RADIAN)) , yStart+ ceil( radius*sin(angle*RADIAN)) );
    }//for
}//polarcircle

void drawOctant(int xStart, int yStart, int x, int y) {
    drawPixel(xStart+x, yStart+y);
    drawPixel(xStart-x, yStart+y);
    drawPixel(xStart+x, yStart-y);
    drawPixel(xStart-x, yStart-y);
    drawPixel(xStart+y, yStart+x);
    drawPixel(xStart-y, yStart+x);
    drawPixel(xStart+y, yStart-x);
    drawPixel(xStart-y, yStart-x);
}//drawOctan

void drawCircle( int xStart, int yStart, int radius) {
    int x = 0;
    int y = radius;
    int p = 1 - radius;
    drawOctant(xStart, yStart, x, y);
    while (x < y) {
        x++;
        if ( p < 0) {
            p+=2*x+1;
        } else {
            y--;
            p+=2*(x-y)+1;
        }//if{}else{}
        drawOctant(xStart, yStart, x, y);
    }//while
}//drawCircle

void polarElipse(int xStart, int yStart,int radiusX,int radiusY) {
    for (float i = 0; i < TWOPI; i+=C_PE) {
        drawPixel(xStart + (radiusX * cos(i)), yStart + (radiusY * sin(i)));
        drawPixel(xStart + (-radiusX * cos(i)), yStart + (radiusY * sin(i)));
        drawPixel(xStart + (radiusX * cos(i)), yStart + (-radiusY * sin(i)));
        drawPixel(xStart + (-radiusX * cos(i)), yStart +(-radiusY * sin(i)));
    }//for
}//polarElipse

void drawQuadrant(int xStart, int yStart, int x, int y){
    drawPixel(xStart+x, yStart+y);
    drawPixel(xStart-x, yStart+y);
    drawPixel(xStart+x, yStart-y);
    drawPixel(xStart-x, yStart-y);
}//drawQuadrant

void drawElipse(int xStart, int yStart, int radiusX, int radiusY) {
    float x, y,radiusXSq, radiusYSq, radiusXSq2, radiusYSq2, p, px, py;
    radiusXSq = radiusX * radiusX;
    radiusYSq = radiusY * radiusY;
    radiusXSq2 = radiusXSq * 2;
    radiusYSq2 = radiusYSq * 2;
    x = 0;
    y = radiusY;
    px = 0;
    py = 2 * radiusXSq * y;

    drawQuadrant(xStart, yStart, x, y);
    //Region 1
    p = ceil(radiusYSq - radiusXSq * radiusY + (0.25 * radiusXSq));
    while (px < py) {
        x++;
        px += radiusYSq2;
        if (p < 0){
            p +=radiusYSq + px;
        } else {
            y--;
            py -= radiusXSq2;
            p += radiusYSq + px - py;
        }//if{}else{}
        drawQuadrant(xStart, yStart, x, y);
    }//while
    //Region 2
    p = ceil(radiusYSq*(x+0.5)*(x+0.5) + radiusXSq*(y-1)*(y-1) - radiusXSq*radiusYSq);
    while (y > 0) {
        y--;
        py -= radiusXSq2;
        if (p > 0) {
            p += radiusXSq - py;
        } else {
            x++;
            px += radiusYSq2;
            p += radiusXSq2 - py + px;
        }//if{}else{}
        drawQuadrant(xStart, yStart, x, y);
    }//while
}//drawElipse

struct Point polarPoint( int xStart, int yStart, int radius, float angle) {
    struct Point polarPoint {
            xStart+ ceil(radius*cos(angle/(180/M_PI))),
            yStart+ ceil(radius*sin(angle/(180/M_PI)))
        };
        return polarPoint;
}//polarPoint

void drawPolygon ( int xStart, int yStart, int radius, float angle, int numberSides) {
    int angleIncrement= 360 / numberSides, i = 0;
    for (i; i<numberSides; i++) {
        Point[i]=polarPoint(xStart, yStart, radius, angle);
        angle+= angleIncrement;
    }//for
    for (i = 0; i < numberSides-1; i++) {
        bresenhamLine(Point[i].x, Point[i].y, Point[i+1].x, Point[i+1].y);
    }//for{}
    bresenhamLine(Point[i].x, Point[i].y, Point[0].x, Point[0].y);
}//drawPolygon

void curvedLine(int x0, int y0, int x1, int y1, int xTwo, int yTwo,
                int x3, int y3){
    double time;
    int x, y;

    for(time = 0; time <= 1.0; time += 0.001) {
        x = pow( 1-time,3 )* x0 + 3*time * pow( 1-time ,2 )* x1 + 3*pow( time, 2 )*( 1-time )* xTwo + pow( time, 3 )* x3;
        y = pow( 1-time,3 )* y0 + 3*time * pow( 1-time ,2 )* y1 + 3*pow( time, 2 )*( 1-time )* yTwo + pow( time, 3 )* y3;

        drawPixel(x,y);
    }//for
}//curvedLine

void sprayFunct(int TIME) {
    if (pressButton == 1 ) {
        int xRand = rand()%(radius+1), yRand = rand()%(radius+1);
        int radiusRadius = radius * radius;
        if (xRand*xRand + yRand*yRand - radiusRadius <= 0){
            glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                drawPixel(xStart+xRand, yStart+yRand);
                drawPixel(xStart-yRand, yStart-xRand);
                drawPixel(xStart-yRand, yStart+xRand);
                drawPixel(xStart+xRand, yStart-yRand);
            glFlush();
                glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
        }//if{}
        glutTimerFunc(1,sprayFunct,1);
    }//if{}
}//sprayFunct

void lineSegment(void) {
    glClear(GL_COLOR_BUFFER_BIT); // get display color assigned to the window
    glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
}//lineSegment

void onMotion(int x, int y) {
    switch(tool) {
    case BRES_LINE:
        xFinish=x;
        yFinish=abs(HEIGHT-y);

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
            bresenhamLine(xStart,yStart,xFinish,yFinish);
        glFlush();
        break;

    case CIRCLE:
        radius = sqrt( pow(xStart-x,2)+
                       pow(yStart-(HEIGHT-y),2) );

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
            polarCircle(xStart,yStart,radius);
        glFlush();
        break;

    case ELIPSE:
        radiusX=abs(xStart-x);
        radiusY=abs(yStart - abs(HEIGHT-y));

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
            polarElipse(xStart,yStart,radiusX, radiusY);
        glFlush();
        break;

    case POLYGON:
        radius = sqrt( pow(xStart-x,2)+
                       pow(yStart-(HEIGHT-y),2) );
        angle=(180/M_PI) * (atan2(yStart - abs(HEIGHT-y), xStart-x));

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
            drawPolygon(xStart, yStart, radius, angle, numberSides);
        glFlush();
        break;

    case PENCIL:case ERASER:
        xFinish=x;
        yFinish=abs(HEIGHT-y);
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
            bresenhamLine(xStart, yStart, xFinish, yFinish);
        glFlush();
            glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
        xStart=xFinish;
        yStart=yFinish;
        break;

    case SPRAY:
        xStart = x;
        yStart = abs(HEIGHT - y);
        break;

    case CURVED_LINE:
        switch (click) {
        case 0:
            xFinish=x;
            yFinish=abs(HEIGHT-y);

            glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                curvedLine(xStart, yStart, xStart, yStart, xStart, yStart, xFinish, yFinish);
            glFlush();
            break;

        case 1:
            xOne=x;
            yOne=abs(HEIGHT-y);

            glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                curvedLine(xStart, yStart, xOne, yOne, xOne, yOne, xFinish, yFinish);
            glFlush();
            break;

        case 2:
            xTwo=x;
            yTwo=abs(HEIGHT-y);

            glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                curvedLine(xStart, yStart, xOne, yOne, xTwo, yTwo, xFinish, yFinish);
            glFlush();
            break;
        }//switch
        break;
    }//switch
}//onMotion

void onPassive(int x, int y) {
}//onPassive

void fillUtil(int x, int y, unsigned char prevR, unsigned char prevG, unsigned char prevB) {
    unsigned char fillColor[3];
    unsigned char pickColor[3];
    stack<int> xS;
    stack<int> yS;
    fillColor[0]=0;
    fillColor[1]=0;
    fillColor[2]=0;
    int i=1;
    xS.push(x);
    yS.push(y);
    while (!xS.empty() && !yS.empty()){
        x=xS.top();
        y=yS.top();
        xS.pop();
        yS.pop();
        if (x > 0 || x < WIDTH || y > 0 || y < HEIGHT) {
            glReadPixels(x , y , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , pickColor);
            if ( (pickColor[0] == prevR && pickColor[1] == prevG && pickColor[2] == prevR) &&
                 (pickColor[0] != fillColor[0] && pickColor[1] != fillColor[1] && pickColor[2] != fillColor[2])) {
                drawPixel(x,y);
                glFlush();

                glReadPixels(x+1 , y , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , pickColor);
                if ( (pickColor[0] == prevR && pickColor[1] == prevG && pickColor[2] == prevR) &&
                     (pickColor[0] != fillColor[0] && pickColor[1] != fillColor[1] && pickColor[2] != fillColor[2])) {
                    xS.push(x+1);
                    yS.push(y);

                }//if

                glReadPixels(x-1 , y , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , pickColor);
                if ( (pickColor[0] == prevR && pickColor[1] == prevG && pickColor[2] == prevR) &&
                     (pickColor[0] != fillColor[0] && pickColor[1] != fillColor[1] && pickColor[2] != fillColor[2])) {
                    xS.push(x-1);
                    yS.push(y);

                }//if

                glReadPixels(x , y+1 , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , pickColor);
                if ( (pickColor[0] == prevR && pickColor[1] == prevG && pickColor[2] == prevR) &&
                            (pickColor[0] != fillColor[0] && pickColor[1] != fillColor[1] && pickColor[2] != fillColor[2])) {
                    xS.push(x);
                    yS.push(y+1);

                }//if


                glReadPixels(x , y-1 , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , pickColor);
                if ( (pickColor[0] == prevR && pickColor[1] == prevG && pickColor[2] == prevR) &&
                     (pickColor[0] != fillColor[0] && pickColor[1] != fillColor[1] && pickColor[2] != fillColor[2])) {
                    xS.push(x);
                    yS.push(y-1);

                } //if
            }//if{}

        }//if{}
    }//while
    printf("Filling complete!\n");
}//floodFillUtil

void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
       if (state==GLUT_DOWN) {
            switch (tool) {
            case CURVED_LINE:
                switch (click) {
                case 0:
                    xStart = x;
                    yStart = abs(HEIGHT - y);
                    xOne = xTwo = x;
                    yOne = yTwo = y;
                    break;

                case 1:
                    xOne = x;
                    yOne = abs(HEIGHT - y);
                    xTwo = xOne;
                    yTwo = yOne;
                    break;

                case 2:
                    xTwo = x;
                    yTwo = abs(HEIGHT - y);
                    break;
                }//switch
                break;

            case PENCIL:case ERASER:
                xStart = xFinish = x;
                yStart = yFinish = abs(HEIGHT - y);
                glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                    bresenhamLine(xStart, yStart, xFinish, yFinish);
                glFlush();
                    glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                break;

            case SPRAY:
                pressButton=1;
                xStart = x;
                yStart = abs(HEIGHT - y);
                radius = s;

                glutTimerFunc(1,sprayFunct,1);
                break;

            default:
                xStart = x;
                yStart = abs(HEIGHT-y);
                break;
            }//switch
        } else if (state == GLUT_UP){
            switch (tool) {
            case BRES_LINE:case POLYGON:
                glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                glFlush();
                break;

            case CIRCLE:
                glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                    drawCircle(xStart,yStart,radius);
                glFlush();
                    glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                break;

            case ELIPSE:
                glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                    drawElipse(xStart,yStart,radiusX, radiusY);
                glFlush();
                    glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                break;

            case SPRAY:
                pressButton=0;
                break;

            case CURVED_LINE:
                switch (click){
                case 0:
                    click++;//1
                    break;

                case 1:
                    click++;//2
                    break;

                case 2:
                        glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                    xStart = xOne = xTwo = xFinish = x;
                    yStart = yOne = yTwo = yFinish = y;
                    click=0;
                    break;
                }//switch
                break;

                case FILL:
                    unsigned char prevColor[3];
                    glReadPixels(xStart , yStart , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , prevColor);
                    glDrawPixels(WIDTH, HEIGHT, GL_RGB_SCALE_EXT, GL_UNSIGNED_BYTE, colorArray);
                        fillUtil(xStart, yStart, prevColor[0],prevColor[1],prevColor[2]);
                    glFlush();
                        glReadPixels(0.0, 0.0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, colorArray);
                    break;
            }//switch{}
        }//if{}else{}
    }//if
}//onMouse

void processKey(unsigned char key, int x, int y) {
    switch(key) {
    case '3':
        printf("\tCURRENT POLYGON SIDE: 3\n");
        numberSides=3;
        break;

    case '4':
        printf("\tCURRENT POLYGON SIDE: 4\n");
        numberSides=4;
        break;

    case '5':
        printf("\tCURRENT POLYGON SIDE: 5\n");
        numberSides=5;
        break;

    case '6':
        printf("\tCURRENT POLYGON SIDE: 6\n");
        numberSides=6;
        break;

    case '7':
        printf("\tCURRENT POLYGON SIDE: 7\n");
        numberSides=7;
        break;

    case '8':
        printf("\tCURRENT POLYGON SIDE: 8\n");
        numberSides=8;
        break;

    case 'l':
        tool=BRES_LINE;
        printf("CURRENT TOOL: Bresenham line\n");
        break;

    case 'c':
        tool=CIRCLE;
        printf("CURRENT TOOL: Draw circle\n");
        break;

    case 'e':
        printf("CURRENT TOOL: Draw elipse\n");
        tool=ELIPSE;
        break;
    case 'p':
        printf("CURRENT TOOL: Draw polygon\n");
        tool=POLYGON;
        break;

    case 'n':
        printf("CURRENT TOOL: Pencil\n");
        tool=PENCIL;
        break;

    case 'r':
        printf("CURRENT TOOL: Eraser\n");
        tool=ERASER;
        break;

    case 's':
        printf("CURRENT TOOL: Spray\n");
        tool=SPRAY;
        break;

    case 'u':
        printf("CURRENT TOOL: Curved line\n");
        tool=CURVED_LINE;
        break;

    case '+':
        switch (tool) {
            case ERASER:
                eraserSize++;
                e = eraserSize/2;
                printf("\tEraser size: %d\n",eraserSize);
                break;

            case SPRAY:
                spraySize++;
                s = spraySize/2;
                printf("\tSpray size: %d\n",spraySize);
                break;
        }//switch
        break;

    case '-':
        switch (tool) {
            case ERASER:
                eraserSize--;
                e = eraserSize/2;
                printf("\tEraser size: %d\n",eraserSize);
                break;

            case SPRAY:
                spraySize--;
                s = spraySize/2;
                printf("\tSpray size: %d\n",spraySize);
                break;
        }//switch
        break;

    case 'f':
        printf("CURRENT TOOL: Fill\n");
        tool=FILL;
        break;
    }//switch
}//processKey

void open(char *fileName) {
    printf("Read file\n");
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;
    file = fopen(fileName, "rw");

    if( file ) {
        fread(&bitmapFileHeader, 1, sizeof(bitmapFileHeader), file);
        fread(&bitmapInfoHeader, 1, sizeof(bitmapInfoHeader), file);
        fread(colorArray, 1, sizeof(colorArray), file);
        glDrawPixels(WIDTH, HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, colorArray);
        fclose(file);
    }//if{}
    printf("Close file\n");
}//open

void save( char *fileName) {
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;

    unsigned char *image = (unsigned char*)malloc(sizeof(unsigned char)*(WIDTH*HEIGHT*3));
    file = fopen(fileName, "wb");

        if( image!=NULL ) {
            if( file!=NULL ) {
                glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, colorArray);
                memset(&bitmapFileHeader, 0, sizeof(bitmapFileHeader));
                memset(&bitmapInfoHeader, 0, sizeof(bitmapInfoHeader));

                bitmapFileHeader.bfType = 'MB';
                bitmapFileHeader.bfSize = sizeof(bitmapFileHeader)+sizeof(bitmapInfoHeader)+ (WIDTH*HEIGHT*3);
                bitmapFileHeader.bfOffBits = sizeof(bitmapFileHeader)+sizeof(bitmapInfoHeader);
                bitmapInfoHeader.biSize = sizeof(bitmapInfoHeader);
                bitmapInfoHeader.biWidth = WIDTH;
                bitmapInfoHeader.biHeight = HEIGHT;
                bitmapInfoHeader.biPlanes = 1;
                bitmapInfoHeader.biBitCount = 24;
                bitmapInfoHeader.biSizeImage = (WIDTH*HEIGHT*3);
                fwrite(&bitmapFileHeader, sizeof(bitmapFileHeader), 1, file);
                fwrite(&bitmapInfoHeader, sizeof(bitmapInfoHeader), 1, file);
                fwrite(&colorArray, sizeof(unsigned char), (WIDTH*HEIGHT*3), file);

                fclose( file );
            }//if{}
            free( image );
        }//if{}
}//save

void openSaveAndColors ( GLint selectedOption ) {
    switch (selectedOption) {
    case 1:
        open("C:\\Users\\Lalo\\Desktop\\Taller Graficas por computadora\\BerrospeSave.bmp");
        glDrawPixels(WIDTH, HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, colorArray);
    glFlush();
        break;

    case 2:
        save("C:\\Users\\Lalo\\Desktop\\Taller Graficas por computadora\\BerrospeSave.bmp");
        glDrawPixels(WIDTH, HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, colorArray);
    glFlush();
        break;

    case 3:
        glColor3f(255, 0, 0);//Line color
        break;

    case 4:
        glColor3f(0, 255, 0);//Line color
        break;

    case 5:
        glColor3f(0, 0, 255);//Line color
        break;

    case 6:
        glColor3f(0, 255, 255);//Line color
        break;

    case 7:
        glColor3f(255, 255, 0);//Line color
        break;

    case 8:
        glColor3f(255, 0, 255);//Line color
        break;

    case 9:
        glColor3f(0, 0, 0);//Line color
        break;

    case 10:
        glColor3f(255, 255, 255);//Line color
        break;
    }//switch

}//openSave

int main(int argc, char** argv) {
    printf("\t\tInstructions:\n");
    printf("ON WINDOW:\n");
    printf("PRESS 'l' to draw a line\n");
    printf("PRESS 'c' to draw a circle\n");
    printf("PRESS 'e' to draw an elipse\n");
    printf("PRESS 'p' to draw a polygon\n");
    printf("\tON POLYGON TOOL:\n");
    printf("\tPRESS '3' to draw a polygon (3 sides)\n");
    printf("\tPRESS '4' to draw a polygon (4 sides)\n");
    printf("\tPRESS '5' to draw a polygon (5 sides)\n");
    printf("\tPRESS '6' to draw a polygon (6 sides)\n");
    printf("\tPRESS '7' to draw a polygon (7 sides)\n");
    printf("\tPRESS '8' to draw a polygon (8 sides)\n\n");
    printf("PRESS 'n' to draw with a pencil\n");
    printf("PRESS 'r' to erase\n");
    printf("\tON ERASER TOOL:\n");
    printf("\tPRESS '+' to increase size\n");
    printf("\tPRESS '-' to decrease size\n");
    printf("PRESS 's' to draw with a spray\n");
    printf("\tON SPRAY TOOL:\n");
    printf("\tPRESS '+' to increase size\n");
    printf("\tPRESS '-' to decrease size\n");
    printf("PRESS 'u' to draw a curved line\n");
    printf("PRESS 'f' to fill\n");
    printf("PRESS 'RIGHT MOUSE BUTTON' to open/save file and choose colors\n");
    printf("CURRENT TOOL: Bresenham line\n");
    srand(time(NULL));
    glColor3f(0, 0, 0);//Line color
     glutInit(&argc, argv); // Initialize GLUT
     glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); // unique refresh buffer and mode color RGB
     glutInitWindowPosition(50, 100); // initialize window position
     glutInitWindowSize(WIDTH, HEIGHT); //WIDTH and HEIGHT
     glutCreateWindow("Taller de graficas"); // create window and title assignment
     init();
     glutDisplayFunc(lineSegment); // show line in the display
     glutMotionFunc(onMotion);
     glutPassiveMotionFunc(onPassive);
     glutMouseFunc(onMouse);
     glutKeyboardFunc(processKey);

     glutCreateMenu(openSaveAndColors);//Menu
        glutAddMenuEntry("Open", 1);
        glutAddMenuEntry("Save", 2);
        glutAddMenuEntry("Red", 3);
        glutAddMenuEntry("Green", 4);
        glutAddMenuEntry("Blue", 5);
        glutAddMenuEntry("Cyan", 6);
        glutAddMenuEntry("Magenta", 7);
        glutAddMenuEntry("Yellow", 8);
        glutAddMenuEntry("Negro", 9);
        glutAddMenuEntry("Blanco", 10);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
     glutMainLoop(); // loop that checks input devices
     return EXIT_SUCCESS;
}//main

//181209 (CS 51)
//181228 (CS 51)


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <string.h>

#define PI 3.14159
#define GAME_SCREEN 0 //Constant to identify background color
#define MENU_SCREEN 4			
#define MAX_STONES  100   // max number of stones in one game 
#define MAX_STONE_TYPES 10 // max types or shapes of stones(objects)
#define stoneRotationSpeed 50 // rotation speed of each stone
#define SPACESHIP_SPEED 80 // speed of spaceship on moving using a,d,w,s
int stoneTranslationSpeed=1; //linear speed stones

GLint m_viewport[4]; //a polygon viewing region
int x,y;
int i;
int randomStoneIndices[100]; // to index every stone
int index;
int Score=0; // increses by 1 on every hit of stone by laser
int alienLife=100; // decreases by 10 on every collision with stone
int GameLvl= 1; //changes according to the score and stones alive
float mouseX ,mouseY ;	//Cursor coordinates;
float LaserAngle=0 ,stoneAngle =0,lineWidth = 1; //characterstics of laser
float xOne=0,yOne=0; //Spaceship coordinates
float xStone[MAX_STONES] ,yStone[MAX_STONES]; //coordinates of stones
float xStart = 1200; //Health bar starting coodinate
GLint stoneAlive[MAX_STONES]; //check to see if stone is killed
int spaceshipX = -54, spaceshipY = -30; //spaceship coordinates for design

//boolean values to check state of the game
bool mButtonPressed= false,startGame=false,gameOver=false;		
bool startScreen = true ,nextScreen=false,previousScreen=false;
bool gameQuit = false,instructionsGame = false, optionsGame = false;

char highScore[100],ch;
void display();
void StoneGenerate();

//display Text : Raster graphics/ bitmap graphics uses pixels arranged in a grid formation to represent an image
void displayRasterText(float x ,float y ,float z ,char *stringToDisplay) {
	int length;
	glRasterPos3f(x, y, z);
		length = strlen(stringToDisplay);

	for(int i=0 ;i<length ;i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]); 
	}
}

// Display Screen
void SetDisplayMode(int modeToDisplay) {
		switch(modeToDisplay){
		case GAME_SCREEN: glClearColor(0, 0, 0, 0.5);break; // specifies the red, green, blue, and alpha values used by glClear to clear the color buffers
		case MENU_SCREEN : glClearColor(0.4, 0.1 , 0, 1);break;
	}
}

// Initialize Stone Array
void initializeStoneArray() {
	//random stones index
	
	for(int i = 0;i < MAX_STONES ;i++) {
		randomStoneIndices[i]=rand()%MAX_STONE_TYPES;
		stoneAlive[i]=true; // stone appears on screen
	}

	xStone[0] = -(200*MAX_STONES)-600; //START LINE for stone appearance
												
	for(int i = 0;i<MAX_STONES ;i++) {	//random appearance yIndex for each stone
		yStone[i]=rand()%600;
		if(int(yStone[i])%2) 
			yStone[i]*=-1; //even values for yStone apeear on negative side
		xStone[i+1] = xStone[i] + 200;	//xIndex of stone aligned with 200 units gap
	}
}

//spaceship design
void DrawSpaceshipBody()
{
glPointSize(10.0); // specifies the rasterized diameter of points

    glBegin(GL_QUADS);
    glColor3f(0.137255,0.419608,0.556863); //middle body
    glVertex2f(spaceshipX - 25, spaceshipY + 25);
    glVertex2f(spaceshipX - 25, spaceshipY - 25);
    glVertex2f(spaceshipX + 25, spaceshipY - 25);
    glVertex2f(spaceshipX + 25, spaceshipY + 25);
    glEnd();

    glBegin(GL_POINTS); //design on middle
    glColor3f(0.90,0.91,0.98);
    glVertex2f(spaceshipX-10,spaceshipY-5);
    glVertex2f(spaceshipX+10,spaceshipY-5);
    glEnd();

    glBegin(GL_POINTS); //thrusters
    glColor3f(1,1,1);
    glVertex2f(spaceshipX-10,spaceshipY-30);
    glVertex2f(spaceshipX+10,spaceshipY-30);
    glEnd();

    glBegin(GL_TRIANGLES); //right wing
    glColor3f(0.196078,0.8,0.196078);
    glVertex2f(spaceshipX+25,spaceshipY+25);
    glVertex2f(spaceshipX+25,spaceshipY-25);
    glVertex2f(spaceshipX+45,spaceshipY-35);
    glEnd();

    glBegin(GL_TRIANGLES); //left wing
    glColor3f(0.196078,0.8,0.196078);
    glVertex2f(spaceshipX-25,spaceshipY+25);
    glVertex2f(spaceshipX-25,spaceshipY-25);
    glVertex2f(spaceshipX-45,spaceshipY-35);
    glEnd();

   
    glBegin(GL_TRIANGLES); //upper body
    glColor3f(0.99609, 0.83984, 0);
    glVertex2f(spaceshipX,spaceshipY+60);
    glVertex2f(spaceshipX-25,spaceshipY+25);
    glVertex2f(spaceshipX+25,spaceshipY+25);
    glEnd();
}

void DrawSpaceShipLazer() {
	
	glColor3f(1, 1, 1);
	glPushMatrix();// copies the top matrix and pushes it onto the stack


	float xMid =0,yMid =0;
	//Mid point of the lazer horizontal 	
	xMid = (55+50)/2.0;
	yMid = (25+35)/2.0;
	
	//Rotating about the point ,20
	glTranslated(xMid, yMid, 0);
	glRotated(LaserAngle, 0, 0 ,1);
	
	glPopMatrix(); //pops the top matrix off the stack
}

void DrawLazerBeam() {

	float xMid = -(55+50)/2.0;
	float yMid = (25+35)/2.0;
	
	float mouseXEnd = -((- mouseX) + xOne);
	float mouseYEnd = -((- mouseY) + yOne);
	glLineWidth(5); //Laser beam width

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(xMid ,yMid);
		glVertex2f(mouseXEnd ,mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch(StoneIndex) //CHANGE INDEX VALUE FOR DIFFERENT STONE VARIETY;
	{
	case 0:

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glColor3f(0.4f, 0.0f, 0.0f);
		glScalef(35,35,1);
		glutSolidSphere(1,9,50);
		
		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,10,1);
		glutSolidSphere(1,5,50);
		
		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,60,1);
		glutSolidSphere(1,5,50);
		break;

	case 1:
		glColor3f(1.0f, 0.8f, 0.8f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(15,20,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(40,5,1);
		glutSolidSphere(1,5,50);
		break;

	case 2:
		glColor3f(0.2f, 0.2f, 0.0f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,25,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,60,1);
		glutSolidSphere(1,9,50);
		
		break;

	case 3:
		glColor3f(0.8f, 0.8f, 0.1f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(35,10,1);
		glutSolidSphere(1,10,7);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(50,20,1);
		glutSolidSphere(1,5,50);
		break;
	case 4:
		glColor3f(0.26f, 0.26f, 0.26f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,55,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(20,10,1);
		glutSolidSphere(1,9,50);
		glLoadIdentity();

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle+45 ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,10,1);
		glutSolidSphere(1,9,50);
		break;
	}
	glPopMatrix();
}

bool checkIfSpaceShipIsSafe() {
		for(int i =0 ;i<MAX_STONES ;i++) {
		if(stoneAlive[i]&((xOne >= (xStone[i]/2 -70) && xOne <= (xStone[i]/2 + 70) && yOne >= (yStone[i]/2 -18 ) && yOne <= (yStone[i]/2 + 53)) || (yOne <= (yStone[i]/2 - 20) && yOne >= (yStone[i]/2 - 90) && xOne >= (xStone[i]/2 - 40) && xOne <= (xStone[i]/2 + 40))))
		{	
			stoneAlive[i]=0;
			return false;
		}
	}
	return true;
}
void SpaceshipCreate(){	
	glPushMatrix();
	glTranslated(xOne,yOne,0);
	if(!checkIfSpaceShipIsSafe() && alienLife ){
		alienLife-=10;
		xStart -= 23;//health bar
	}

	glPushMatrix();
	glTranslated(4,19,0);
	
	glPopMatrix();
	
	DrawSpaceshipBody();
	DrawSpaceShipLazer();
	if(mButtonPressed) {
		DrawLazerBeam();
	}
	glEnd(); 
	glPopMatrix();
}
void DisplayHealthBar() {
	
	glColor3f(0 ,1 ,0);
	glBegin(GL_POLYGON);
		glVertex2f(-xStart ,700);
		glVertex2f(1200 ,700);
		glVertex2f(1200 ,670);
		glVertex2f(-xStart, 670);
	glEnd();
	char temp[40];
	glColor3f(0 ,0 ,1);
	sprintf(temp,"SCORE = %d",Score);
	displayRasterText(-1100 ,600 ,0.4 ,temp); //display variable score
	sprintf(temp,"  LIFE = %d",alienLife);
	displayRasterText(800 ,600 ,0.4 ,temp);
	sprintf(temp,"  LEVEL : %d",GameLvl);
	displayRasterText(-100 ,600 ,0.4 ,temp);
	glColor3f(1 ,0 ,0);
}
void startScreenDisplay()
{	
	glLineWidth(50);
	SetDisplayMode(MENU_SCREEN);
	
	glColor3f(1,0.6,0);
	glBegin(GL_LINE_LOOP); //Border
		glVertex3f(-750 ,-500 ,0.5);
		glVertex3f(-750 ,550 ,0.5);
		glVertex3f(750 ,550 ,0.5);
		glVertex3f(750 ,-500, 0.5);
	glEnd();
	
	glLineWidth(1);

	glColor3f(1, 0.6, 0);
	glBegin(GL_POLYGON); //START GAME PLOYGON
		glVertex3f(-200 ,300 ,0.5);
		glVertex3f(-200 ,400 ,0.5);
		glVertex3f(200 ,400 ,0.5);
		glVertex3f(200 ,300, 0.5);
	glEnd();
	
	glBegin(GL_POLYGON); //INSTRUCTIONS POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON); //QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();
	
// check if the click has been made on tab 
	if(mouseX>=-100 && mouseX<=100 && mouseY>=150 && mouseY<=200){
		glColor3f(0 ,0 ,1) ;
		if(mButtonPressed){
			startGame = true ;
			gameOver = false;
			mButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);

	displayRasterText(-100 ,340 ,0.4 ,"Start Game");
	
	if(mouseX>=-100 && mouseX<=100 && mouseY>=30 && mouseY<=80) {
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			instructionsGame = true ;
			mButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);
	
	displayRasterText(-120 ,80 ,0.4 ,"Instructions");
	
	if(mouseX>=-100 && mouseX<=100 && mouseY>=-90 && mouseY<=-40){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			gameQuit = true ;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayRasterText(-100 ,-170 ,0.4 ,"    Quit");
	
}
void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);
	DisplayHealthBar();
	glScalef(2, 2 ,0);
	if(alienLife){
		SpaceshipCreate();
	}
	else {
		gameOver=true;
		instructionsGame = false;
		startScreen = false;
	} //gameover screen
	StoneGenerate();
	
	
}
void readFromFile() {

	FILE *fp = fopen("HighScoreFile.txt" ,"r");
	int i=0;
	if(fp!= NULL){
		while(fread(&ch,sizeof(char),1 ,fp)){
			highScore[i++] = ch;
		}
		highScore[i] = '\0';
	} 
	fclose(fp);
}
void writeIntoFile() {	//To write high score on to file
	FILE *fp = fopen("HighScoreFile.txt" ,"w");
	int i=0;
	char temp[40];
	if(fp!= NULL){
		int n= Score;
		while(n){
			ch = (n%10)+ '0';
			n/=10;
			temp[i++] = ch;
		}
		temp[i] = '\0';
		strrev(temp);
		puts(temp);
		if(temp[0] == '\0')
			temp[i++] = '0' ,temp[i++] = '\0';
		fwrite(temp ,sizeof(char)*i ,i ,fp);
	}
 fclose(fp);
}
void GameOverScreen()
{
	SetDisplayMode(MENU_SCREEN);
	glColor3f(1,0.6,0); 

	glLineWidth(50);
	glBegin(GL_LINE_LOOP); //Border
		glVertex3f(-650 ,-500 ,0.5);
		glVertex3f(-650 ,520 ,0.5);
		glVertex3f(650 ,520 ,0.5);
		glVertex3f(650 ,-500, 0.5);
	glEnd();
	
	glLineWidth(1);
	stoneTranslationSpeed=1;
	glColor3f(0, 0, 0);

	glBegin(GL_POLYGON); //GAME OVER
		glVertex3f(-550 ,810,0.5);
		glVertex3f(-550 ,610 ,0.5);
		glVertex3f(550 ,610 ,0.5);
		glVertex3f(550 ,810, 0.5);
	glEnd();
	
	glColor3f(1, 0.6, 0);
	glBegin(GL_POLYGON); //RESTART POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON); //QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();
		

	displayRasterText(-300 ,640 ,0.4 ,"G A M E    O V E R ! ! !");
	glColor3f(1 , 0, 0);
	char temp[40];
	
	sprintf(temp,"Score : %d",Score);
	displayRasterText(-100 ,340 ,0.4 ,temp);
	readFromFile();
	char temp2[40];
	if(atoi(highScore) < Score){
		writeIntoFile();
		sprintf(temp2 ,"Highest Score :%d" ,Score);
	} else 
		sprintf(temp2 ,"Highest Score :%s" ,highScore);

	displayRasterText(-250 ,400 ,0.4 ,temp2);
		
	if(mouseX>=-100 && mouseX<=100 && mouseY>=25 && mouseY<=75){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){  //Reset game default values
			startGame = true ;
			gameOver=false;
			mButtonPressed = false;
			initializeStoneArray();
			alienLife=100;	
			xStart=1200;
			Score=0;
			GameLvl=1;
			GameScreenDisplay();
		}
	} else
		glColor3f(0 , 0, 0);
	displayRasterText(-70 ,80 ,0.4 ,"Restart");
		
	if(mouseX>=-100 && mouseX<=100 && mouseY>=-100 && mouseY<=-50){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			exit(0);
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayRasterText(-100 ,-170 ,0.4 ,"    Quit");
	
}
void StoneGenerate(){

		if(xStone[0]>=1200){  //If the last screen hits the end of screen then go to Nxt lvl
			GameLvl++;
			stoneTranslationSpeed+=3;
			Score+=50;
			initializeStoneArray();
			GameScreenDisplay();
		}

	for(int i=0; i<MAX_STONES ;i++){
		index = i;
		
		if(mouseX <= (xStone[i]/2+20) && mouseX >=(xStone[i]/2-20) && mouseY >= (yStone[i]/2-20) && mouseY <= (yStone[i]/2+20) && mButtonPressed){
			if(stoneAlive[i]){   // IF ALIVE KILL STONE
				stoneAlive[i]=0;
				Score++;
				if(Score%3==0) {
					stoneTranslationSpeed+=1; //Rate of increase of game speed
				}							
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if(stoneAlive[i] )  //stone alive
			DrawStone(randomStoneIndices[i]);
	}
	stoneAngle+=stoneRotationSpeed;
	if(stoneAngle > 360) stoneAngle = 0;
}
void backButton() {
	if(mouseX <= -450 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250){
			glColor3f(0, 0, 1);
			if(mButtonPressed){
				mButtonPressed = false;
				instructionsGame = false;
				startScreenDisplay();
			}
	}
	else glColor3f(1, 0.6, 0);
	displayRasterText(-1000 ,-550 ,0, "Back");
}
void InstructionsScreenDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDisplayMode(MENU_SCREEN);
	//colorBackground();
	glColor3f(1, 0.6, 0);
	displayRasterText(-900 ,400 ,0.4 ,"Key 'w' to move up.");
	displayRasterText(-900 ,300 ,0.4 ,"Key 's' to move down.");
	displayRasterText(-900 ,200 ,0.4 ,"Key 'd' to move right.");
	displayRasterText(-900 ,100 ,0.4 ,"Key 'a' to move left.");
	displayRasterText(-900 ,0.0 ,0.4 ,"Left mouse click to shoot laser");
	//displayRasterText(-900 ,-100 ,0.4 ,"The packet can be placed only when 's' is pressed before.");
	displayRasterText(-900 ,-200 ,0.4 ,"You Get 1 point for shooting each object and 50 points for completing each level ");
	displayRasterText(-900, -270,0.4,"The Objective is to score maximum points");
	backButton();
	if(previousScreen)
		nextScreen = false ,previousScreen = false; //as set by backButton()


}
void display() {

	glClear(GL_COLOR_BUFFER_BIT);   
	glViewport(0,0,1200,700);
	if(startGame && !gameOver)
		GameScreenDisplay();

	else if(instructionsGame)
		InstructionsScreenDisplay();

	else if(gameOver)
		GameOverScreen();

	//Make spaceship bigger
	else if(startScreen){

			startScreenDisplay();
			if(gameQuit || startGame || optionsGame || instructionsGame){
				//startScreen = false;
				
				if(startGame){
					SetDisplayMode(GAME_SCREEN);
					startScreen = false;
					
				} else if(gameQuit) 
					exit(0);

				} else if(instructionsGame) {
					SetDisplayMode(GAME_SCREEN);
					InstructionsScreenDisplay();
				} 
		}

	//Reset Scaling values
	glScalef(1/2 ,1/2 ,0);
	glFlush();  
	glLoadIdentity(); // replaces the current matrix with the identity matrix. It is semantically equivalent to calling glLoadMatrix with the identity matrix
	glutSwapBuffers(); //exchanges the front and back buffers if the current pixel format for the window referenced by the specified device context includes a back buffer
}
void somethingMovedRecalculateLaserAngle() {

	float mouseXForTan = (-50 - mouseX) + xOne;
	float mouseYForTan = (35 - mouseY) + yOne;
	float LaserAngleInRadian = atan(mouseYForTan/mouseXForTan);
	LaserAngle = (180/PI) * LaserAngleInRadian;
}
void keys(unsigned char key, int x, int y)
{
	//if(key=='w' && key=='d' ){xOne+=0.5;yOne+=0.5;}
	if(key == 'd') xOne+=SPACESHIP_SPEED; 
	if(key == 'a') xOne-=SPACESHIP_SPEED; 
	if(key == 'w') {yOne+=SPACESHIP_SPEED;}
	if(key == 's') {yOne-=SPACESHIP_SPEED;}
	if(key == 'd' || key == 'a' || key == 'w' || key == 's')
		somethingMovedRecalculateLaserAngle();
			
	display();
	
}
void myinit()
{
	glClearColor(0.5,0.5,0.5,0);
	glColor3f(1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
    gluOrtho2D(-1200,1200,-700,700); //CHANGE THIS TO GET EXTRA SPACE
//  gluOrtho2D(-200,200,-200,200);
	glMatrixMode(GL_MODELVIEW);
}
//laser angle according to mouse pointer
void passiveMotionFunc(int x,int y) {

	//when mouse not clicked
	mouseX = float(x)/(m_viewport[2]/1200.0)-600.0; //converting screen resolution to ortho 2d spec
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);

	//Do calculations to find value of LaserAngle
	somethingMovedRecalculateLaserAngle();
	display();
}
 void mouseClick(int buttonPressed ,int state ,int x, int y) {
	
	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else 
		mButtonPressed = false;
	display();
}
 void idleCallBack() { //when no mouse or keybord pressed
	 display();
 }
 int main(int argc, char** argv) {
	
	 FILE *fp = fopen("HighScoreFile.txt" ,"r") ;  //check if HighScoreFile.txt exist if not create             
	 if(fp!=NULL)
		fclose(fp);
	 else
		 writeIntoFile(); 
		 
	glutInit(&argc, argv);    
	glutInitWindowSize(1200,700);
	glutInitWindowPosition(90 ,0);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow("THE LASER GUN FIRE");  
	glutDisplayFunc(display); 
	glutKeyboardFunc(keys);  
	glutPassiveMotionFunc(passiveMotionFunc); //set the passive motion callback for the current window
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA); 
	glutIdleFunc(idleCallBack); // GLUT program can perform background processing tasks or continuous animation when window system events are not being received
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT ,m_viewport); 
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();//calls display callback over and over, calling idle between so that it can maintain a specific framerate
	return 0;
 }

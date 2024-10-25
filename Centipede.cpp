 #include <iostream>
#include<ctime>
#include<cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window/Keyboard.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
void moveplayer(sf::Sprite& playerSprite,float player[],float bullet[]);      //used for moving player in player area helps it to kill centipede
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);    //for drawing player on screen everytime it is moved
void moveBullet(float bullet[], sf::Clock& bulletClock,float player[]);           //this moves bullet and it kills centipede and mushrooms 
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite,float player[]);   //this draws bullet after it is moved
void drawcentipede(sf::RenderWindow& window,sf::Sprite centipedeSprite[],bool centipedesegmentexists[],const int segments);     //draws centipede it is main game element
void drawcentipedehead(sf::RenderWindow& window,sf::Sprite centipedeheadSprite[]);     //drawing the centipede and showing on screen
void movecentipede(sf::Sprite centipedeSprite[],float centipedexcoordinates[],int centipedeycoordinated[],bool centipedesegmentexists[],int mushroomx[],int mushroomy[],bool  mushroomexists[],int numberofmushroom,const int segments);          //moving the centipede on screen
void movecentipedehead(sf::Sprite centipedeheadSprite[],float centipedeheadcoordinates[],int mushroomx[],int mushroomy[],bool mushroomexists[],int numberofmushroom);  //handles the head of centipede
void drawmushroom(sf::RenderWindow& window,sf::Sprite mushroomSprite,int numberofmushroom,sf::Texture& mushroomtexture,int mushroomx[],int mushroomy[],bool mushroomexists[],int mushroomlives[],bool replacementmushroomarray[],bool mushroomthere);   //draws mushroom randomnly on screen
void checkcollisions(float centipedexcoordinates[],int centipedeycoordinates[],float bullet[],bool centipedesegmentexists[],float player[],int originalmushroom,bool mushroomexists[],int mushroomx[],int mushroomy[],sf::Sprite centipedeSprite[],sf::Texture& centipedeheadtexture,int mushroomlives[],int& score);     //checks collisions between bullet and centipede giving person points
void mushroombulletcollision(int mushroomx[],int mushroomy[],float bullet[],int numberofmushroom,sf::Sprite mushroomSprite,int mushroomlives[],bool mushroomexists[],float player[],bool replacementmushroomarray[],sf::RenderWindow& window,bool mushroomthere,int& score);      //checks bullet and mushroom collisions giving user points 
void replacementmushroom(sf::RenderWindow& window,bool replacementmushroomarray[],int mushroomx[],int mushroomy[],int& i);      
void playercentipedecollision(float centipedexcoordinates[],int centipedeycoordinates[],float player[],bool centipedesegmentexists[]);    //checks collision between player and centipede then eliminating player
void playeroriginalheadcollision(float centipedeheadcoordinates[],float player[]);    //if head of centipede collides with player it eliminates player
void movementinplayerarea(float centipedexcoordinates[],int centipedeycoordinates[],sf::Sprite centipedeSprite[],bool centipedesegmentexists[]);
void scoring(sf::Text& myscoreText23,int& score,sf::Font& myfont);    //scores and gives points to  player
void poisonplayercollision(int mushroomx[],int mushroomy[],bool mushroomexists[],float player[],int numberofmushroom);  //if poisonous one forms it kills player if it touches it
void gameoverfunction(sf::RenderWindow& window,sf::Sprite gameoversprite,float player[]);  //the function to display game over
int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	//window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	window.setSize(sf::Vector2u(1780, 1000)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/nights.ogg");    //changing music
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/new (1).jpg");      //new background
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 )); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[3] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns * 3 / 4) * boxPixelsY+80;
	player[exists]=true;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] ;
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	//initializing centipede and texture
	sf::Texture centipedeTexture;
	centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	const int segments=12;       //declaring segments of centipede
	sf::Sprite centipedeSprite[segments];             //centipede srite made
	int centiwidth=boxPixelsX;
	int centiheight=32;
	int totalwidth=segments*centiwidth;              //total width of centipede
	//int checkx=rand()%30;
	//float xagain=32*checkx; 
	float xagain=rand()%576;                        //generating random position for centipede
	int checky=0;
	//int checky=rand()%25; if(checky%2!=0) {checky+=1;}
	checky*=32;                             //y random position 
	//if(checky%64!=0)
	//{checky+32;}                                              //resolutionX-totalwidth;                                        //rand()%960;                                                                   
	int yagain=checky;                        //rand()%960;
	//if(yagain%2!=0) {yagain+=1;}
	float centipedexcoordinates[segments];            //xcoordinate array centipede
	int centipedeycoordinates[segments];               //ycoordinate array  centipede
	for(int i=0;i<segments;i++)
	{
	centipedeSprite[i].setTexture(centipedeTexture);                              //loading centipede texture
	centipedeSprite[i].setTextureRect(sf::IntRect(0,0,centiwidth,centiheight));
	float x2=xagain+i*centiwidth;              //in order so segments come in line
	centipedexcoordinates[i]=x2;
	centipedeycoordinates[i]=yagain;
	centipedeSprite[i].setPosition(x2,yagain);
	}
	//initializing centipede head
	sf::Texture centipedeheadtexture;
	centipedeheadtexture.loadFromFile("Textures/c_head_left_walk.png");                   //loading head image
	const int size=1;
	sf::Sprite centipedeheadSprite[size];
	int headwidth=boxPixelsX;                    //setting head size 
	int headheight=boxPixelsY;
	float centipedeheadcoordinates[exists]; //a 2 size array to store x and y of centipede head
	for(int i=0;i<size;i++)
	{
	centipedeheadSprite[i].setTexture(centipedeheadtexture);
	centipedeheadSprite[i].setTextureRect(sf::IntRect(0,0,headwidth,headheight));               //loadig centipede image
	centipedeheadSprite[i].setPosition(xagain,yagain);              //setting centipede position
	centipedeheadcoordinates[x]=xagain;
	centipedeheadcoordinates[y]=yagain;
	}
	//initializing the mushrooms
	sf::Texture mushroomtexture;
	mushroomtexture.loadFromFile("Textures/mushroom.png");                   //loading mushroom image
	sf::Sprite mushroomSprite;          //making a mushroom sprite
	int originalmushroom=25;
	const int numberofmushroom= 25+12 ;           //rand()%9;                  //setting mushroom size
	//numberofmushroom+=20;
	 int mushroomx[numberofmushroom]; int mushroomy[numberofmushroom];            //creating arrays for mushroom coordinates
	for(int i=0;i<originalmushroom;i++)
	{int d=rand()%30;                             //placing in random columns
	mushroomx[i]=d*32;                   //placing in random rows
	int e=rand()%24+1;                  //random columns
	mushroomy[i]=e*32;}
	for(int i=originalmushroom;i<numberofmushroom;i++)
	{ mushroomx[i]=0;
	mushroomy[i]=0;} 
	
	//working on collisions
	bool centipedesegmentexists[segments]={true,true,true,true,true,true,true,true,true,true,true,true};            //array to ensure all segments true iniitiaaly
	//mushroom bullet condition
	int mushroomlives[numberofmushroom];
	//an array true for all mushrooms initially
	bool mushroomexists[numberofmushroom];
	for(int i=0;i<originalmushroom;i++)
	{mushroomexists[i]=true;}                     //all mushrooms set to true initially
	for(int i=originalmushroom;i<numberofmushroom;i++)
	{mushroomexists[i]=false;}            //extra mushroom that appear at collisio are false
	for(int i=0;i<originalmushroom;i++)
	{ mushroomlives[i]=2;}
	//initializing replacement mushroom
	bool replacementmushroomarray[numberofmushroom];
	for(int i=0;i<numberofmushroom;i++)
	{ replacementmushroomarray[i]=false;}
	bool  mushroomthere=true;
	int score=0;        //score set to zerp
     sf::Font myfont;              //initiLIaing font
     sf::Text myscoreText23;
     sf::Texture gameovertexture;
     gameovertexture.loadFromFile("Textures/gameover (1).jpeg");
     sf::Sprite gameoversprite;  //making texture to display at end
     gameoversprite.setTexture(gameovertexture);
     gameoversprite.setPosition(0,200);
   
   

	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////

		window.draw(backgroundSprite);               //the background drawn
		
		moveplayer(playerSprite,player,bullet);         //moving player function called
		  drawPlayer(window, player, playerSprite);       //draws player
		  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))         //if key board pressed only then bullet fired
		 {bullet[exists]=true;
		 bullet[y]=player[y];}
		 
		  
		if (bullet[exists]) {
			moveBullet(bullet, bulletClock,player);               //when bullet exists these two bulet handling functions called
			drawBullet(window, bullet, bulletSprite,player);
		}   
		 
		
		 
		 movecentipede(centipedeSprite,centipedexcoordinates,centipedeycoordinates,centipedesegmentexists,mushroomx,mushroomy,mushroomexists,numberofmushroom,segments);  //centipede move and draw
		drawcentipede(window,centipedeSprite,centipedesegmentexists,segments);
		movecentipedehead(centipedeheadSprite,centipedeheadcoordinates,mushroomx,mushroomy,mushroomexists,numberofmushroom);
		drawcentipedehead(window,centipedeheadSprite);       //drawing for head
		drawmushroom(window,mushroomSprite,numberofmushroom,mushroomtexture,mushroomx,mushroomy,mushroomexists,mushroomlives,replacementmushroomarray,mushroomthere);
		checkcollisions(centipedexcoordinates,centipedeycoordinates,bullet,centipedesegmentexists,player,originalmushroom,mushroomexists,mushroomx,mushroomy,centipedeSprite,centipedeheadtexture,mushroomlives,score);
            mushroombulletcollision(mushroomx,mushroomy,bullet,numberofmushroom,mushroomSprite,mushroomlives,mushroomexists,player,replacementmushroomarray,window,mushroomthere,score);
            playercentipedecollision(centipedexcoordinates,centipedeycoordinates,player,centipedesegmentexists);
            playeroriginalheadcollision(centipedeheadcoordinates,player);
           scoring( myscoreText23, score, myfont);
           poisonplayercollision(mushroomx,mushroomy,mushroomexists, player,numberofmushroom);
           gameoverfunction( window,gameoversprite, player);
            //movementinplayerarea(centipedexcoordinates,centipedeycoordinates,centipedeSprite,centipedesegmentexists);
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				return 0;
			}  
		}	 
		 window.draw(myscoreText23);	//displays score
		window.display();
		window.clear();
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {   if(player[exists]) {
	playerSprite.setPosition(player[x], player[y]);          //if player exists then it is drawm
	window.draw(playerSprite); //if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)); else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
     } }
void moveBullet(float bullet[], sf::Clock& bulletClock,float player[]) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 5)
		return;

	bulletClock.restart();
	bullet[y] -= 32;	//chaning cordinate of bullet
	if (bullet[y] < -boxPixelsY)
	{
		bullet[exists] = false;              //false when reaches top
    }}
  void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite,float player[]) {
 //bullet[x]=player[x]; bullet[y]=player[y];
	bulletSprite.setPosition(player[x], bullet[y]);      //bullet is given x coordinate of player
	window.draw(bulletSprite);
  }
void moveplayer(sf::Sprite& playerSprite,float player[],float bullet[])
{          if(player[exists]){
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))                //if left key pressed and player is in screenn it moves right
           { if(player[x]>0)
           { playerSprite.move(player[x]-1, 0); 
           player[x]-=1;   bullet[x]=player[x];}}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))           //if right pressed and player is inside left edge then it moves
            {if (player[x]<resolutionX-boxPixelsX)
            {playerSprite.move(player[x]+1, 0);
            player[x]+=1;  bullet[x]=player[x];}}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))                 //if up key pressed it is restructed to 5 rows only
        {if(player[y]>resolutionY-(5*boxPixelsY))
           { playerSprite.move(0, player[y]-1);
           player[y]-=1;}}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))                   //if down arrow pressed and it is above base it moves
        {if(player[y]<resolutionY-boxPixelsY)
           { playerSprite.move(0, player[y]+1);
                  player[y]+=1;}}                                                                          }  }
      void drawcentipede(sf::RenderWindow& window,sf::Sprite centipedeSprite[],bool centipedesegmentexists[],const int segments)
   { 
    for(int i=0;i<segments;i++)
   { if(centipedesegmentexists[i]==true)             //true segments drawn
    window.draw(centipedeSprite[i]);}
    }
    void drawcentipedehead(sf::RenderWindow& window,sf::Sprite centipedeheadSprite[])
    {
    for(int i=0;i<1;i++)
    {window.draw(centipedeheadSprite[i]);    //head always drawn but not worked on head condition
   }
  }
      void movecentipede(sf::Sprite centipedeSprite[],float centipedexcoordinates[],int centipedeycoordinates[],bool centipedesegmentexists[],int mushroomx[],int mushroomy[],bool mushroomexists[],int numberofmushroom,const int segments)
     {
        float a=-1;                    //main function dealing with centipede movement and centipede and mushroom collision
        float b=0; float c=1;     //a deals with left movement and c with right
//static int rows=0;
       for (int i=0;i<segments;i++)
     {
        if(centipedesegmentexists[i]==true){
         if(centipedeycoordinates[i]%64==0)             //this condition checks even off rows
      { 
           if((centipedexcoordinates[i]<0)  || (centipedexcoordinates[i]>resolutionX-boxPixelsX))
         {centipedeSprite[i].move(0,boxPixelsY);               //moves down when goes out of range
         centipedeycoordinates[i]+=boxPixelsY;
         centipedeSprite[i].move(c,b);              //reverses direction
        centipedexcoordinates[i]+=c;}
       else {centipedeSprite[i].move(a,b);     //normal left movement if even rows
       centipedexcoordinates[i]+=a;}
     }
    if(centipedeycoordinates[i]%64!=0)      //for odd rows
   {
   if((centipedexcoordinates[i]<0) || (centipedexcoordinates[i]>resolutionX-boxPixelsX))
    {centipedeSprite[i].move(0,boxPixelsY);
  centipedeycoordinates[i]+=boxPixelsY;   
  centipedeSprite[i].move(a,b);
   centipedexcoordinates[i]+=a;}            //reveresed movement
  else 
   {
   centipedeSprite[i].move(c,b);             //normal right movement 
  centipedexcoordinates[i]+=c;  }
  }
  }
    for (int j = 0; j < numberofmushroom; j++)        //checking collision with mushrooms
            {
                if (mushroomexists[j])
                {
                    float centipedeLeft = centipedexcoordinates[i];              //defining ranges for right and left
                    float centipedeRight = centipedexcoordinates[i] + boxPixelsX;
                    float centipedeTop = centipedeycoordinates[i];
                    float centipedeBottom = centipedeycoordinates[i] + boxPixelsY;

                    float mushroomLeft = mushroomx[j];        //ranges for mushroom
                    float mushroomRight = mushroomx[j] + boxPixelsX;
                    float mushroomTop = mushroomy[j];
                    float mushroomBottom = mushroomy[j] + boxPixelsY;

                    if (centipedeRight > mushroomLeft && centipedeLeft < mushroomRight && centipedeBottom > mushroomTop && centipedeTop < mushroomBottom)
                    {
                        // Handle collision between centipede segment i and mushroom j
                        
                        //mushroomexists[j] = false;

                        // Descend and change direction logic 
                        centipedeSprite[i].move(0, boxPixelsY);
                        centipedeycoordinates[i] += boxPixelsY;
                        centipedeSprite[i].move(c, b);
                        centipedexcoordinates[i] += c;
                    }
                }
            }

}





}
     void movecentipedehead(sf::Sprite centipedeheadSprite[],float centipedeheadcoordinates[],int mushroomx[],int mushroomy[],bool mushroomexists[],int numberofmushroom)
        {
              float a=-1;        //the function is same like centipede head
            float b=0; float c=1;
          for(int i=0;i<1;i++)
       {
//centipedeheadSprite[i].move(-0.1,0);
     if((static_cast<int>(centipedeheadcoordinates[y]))%64==0)           //moving left in even rows
      {
     if(centipedeheadcoordinates[x]<0 ||  centipedeheadcoordinates[x]>resolutionX-boxPixelsX)
       {centipedeheadSprite[i].move(0,boxPixelsY);
        centipedeheadcoordinates[y]+=boxPixelsY;
          centipedeheadSprite[i].move(c,b);           //reversed movement
          centipedeheadcoordinates[x]+=c;}
          else {centipedeheadSprite[i].move(a,b);     //normal movement
        centipedeheadcoordinates[x]+=a;}
           }
       if((static_cast<int>(centipedeheadcoordinates[y]))%64!=0)
          {if((centipedeheadcoordinates[x]<0) || (centipedeheadcoordinates[x]>resolutionX-boxPixelsX))
         {centipedeheadSprite[i].move(0,boxPixelsY);
         centipedeheadcoordinates[y]+=boxPixelsY;
         centipedeheadSprite[i].move(a,b);      //again reversed odd rows
         centipedeheadcoordinates[x]+=a;}
         else {centipedeheadSprite[i].move(c,b);    //normal
     centipedeheadcoordinates[x]+=c;}
     }
if((centipedeheadcoordinates[x]<=resolutionX-boxPixelsX) && (centipedeheadcoordinates[y]==resolutionY-boxPixelsY))  {centipedeheadSprite[0].move(0,-boxPixelsY); centipedeheadcoordinates[y]+=(-boxPixelsY);}
for (int j = 0; j < numberofmushroom; j++)
            {
                if (mushroomexists[j])
                {
                    float centipedeLeft = centipedeheadcoordinates[x];        //same as above checks centipede head collsison with musroom
                    float centipedeRight = centipedeheadcoordinates[x] + boxPixelsX;
                    float centipedeTop = centipedeheadcoordinates[y];
                    float centipedeBottom = centipedeheadcoordinates[y] + boxPixelsY;

                    float mushroomLeft = mushroomx[j];
                    float mushroomRight = mushroomx[j] + boxPixelsX;
                    float mushroomTop = mushroomy[j];
                    float mushroomBottom = mushroomy[j] + boxPixelsY;

                    if (centipedeRight > mushroomLeft && centipedeLeft < mushroomRight && centipedeBottom > mushroomTop && centipedeTop < mushroomBottom)
                   {
                        //Handle collision between centipede segment i and mushroom j
                       
                        //mushroomexists[j] = false;

                        //Descend and change direction logic (similar to moveCentipede)
                       centipedeheadSprite[i].move(0, boxPixelsY);
                      centipedeheadcoordinates[y] += boxPixelsY;
                       centipedeheadSprite[i].move(c, b);
                       centipedeheadcoordinates[x] += c;
                    

}  }
                    }
                }
            }



      void drawmushroom(sf::RenderWindow& window,sf::Sprite mushroomSprite,int numberofmushroom,sf::Texture& mushroomtexture,int mushroomx[],int mushroomy[],bool mushroomexists[],int mushroomlives[],bool replacementmushroomarray[],bool mushroomthere)
        {



        for( int i=0;i<numberofmushroom;i++)         //checks existence of each mushroom initially
      {  if(mushroomexists[i]==true) {
    mushroomSprite.setTexture(mushroomtexture);
        if( mushroomlives[i]==2  && mushroomy[i]>=0 && mushroomy[i]<800){             //if mushroom not hit and in game area not playr area

    mushroomSprite.setTextureRect(sf::IntRect(0,0,32,32));       //full red drawn
	
	
	
}



        else if ( mushroomlives[i]==1 && mushroomy[i]>-0 && mushroomy[i]<800 ) {            //if mushroom hit once and in game area

        mushroomSprite.setTextureRect(sf::IntRect(32,0,32,32));}               //half red drawn
             else if ( mushroomlives[i]==2  && mushroomy[i]>=800 && mushroomy[i]<928) {          //if hit in player area and once

         mushroomSprite.setTextureRect(sf::IntRect(0,32,32,32));}               //full white drawn
                else if ( mushroomlives[i]==1  && mushroomy[i]>=800 && mushroomy[i]<928) {             //if hit in player area second time

           mushroomSprite.setTextureRect(sf::IntRect(64,32,32,32));}            // half white drawn
	
	
	      mushroomSprite.setPosition(mushroomx[i],mushroomy[i]);
             window.draw(mushroomSprite);    //finally drawing

    }   }    

}
          void checkcollisions(float centipedexcoordinates[],int centipedeycoordinates[],float bullet[],bool centipedesegmentexists[],float player[],int originalmushroom,bool mushroomexists[],int mushroomx[],int mushroomy[],sf::Sprite centipedeSprite[],sf::Texture& centipedeheadtexture,int mushroomlives[],int& score)
       {
          for(int i=0;i<11;i++){
  
        if(player[x]<=centipedexcoordinates[i]+boxPixelsX && player[x]>=centipedexcoordinates[i] && bullet[y]<=centipedeycoordinates[i]+boxPixelsY && bullet[y]>=centipedeycoordinates[i] && bullet[exists]==true)
       {centipedesegmentexists[i]=false;           //condition set for collision check if bullet is in range of a segment of centipede it is turned off
    score+=10;        //score added
     bullet[exists]=false;        //bullet false
//if(i<11) 
      centipedeSprite[i+1].setTexture(centipedeheadtexture);             //changing next xegment to a head
        mushroomexists[i+originalmushroom]=true;       //existence of the mushroom comes true
        mushroomlives[i+originalmushroom]=2;                  //giving two lives to original mushroom
        mushroomx[i+originalmushroom]=centipedexcoordinates[i];       //coordinates set for new mushroom
       mushroomy[i+originalmushroom]=centipedeycoordinates[i];
   }


   }}   //}

     void mushroombulletcollision(int mushroomx[],int mushroomy[],float bullet[],int numberofmushroom,sf::Sprite mushroomSprite,int mushroomlives[],bool mushroomexists[],float player[],bool   replacementmushroomarray[],sf::RenderWindow& window,bool mushroomthere,int& score)
      {     
         for( int i=0;i<numberofmushroom;i++) {


            
            if(player[x]<=mushroomx[i]+boxPixelsX && player[x]>=mushroomx[i]  && bullet[y]<=mushroomy[i]+boxPixelsY && bullet[y]>=mushroomy[i] && bullet[exists]==true && mushroomexists[i]==true)
      {
          cout<<"it ih hit "<<endl;     //same centipede condition used here to match coordinates if bullet lies in 32 pixel of mushroom live decrement
         mushroomlives[i]-=1;
         bullet[exists]=false;        //bullet gets false


        if(mushroomlives[i]==0) { mushroomexists[i]=false;  score+=1;}}             //when mushroom lives are zero it gets non existant



     } }    


            void replacementmushroom(sf::RenderWindow& window,bool replacementmushroomarray[],int mushroomx[],int mushroomy[],int& i)
             { 
                cout<<"called"; if(replacementmushroomarray[i]==true){ cout<<"checked";
                sf::Texture ReplacementmushroomTexture;
               ReplacementmushroomTexture.loadFromFile("Textures/mushroom.png");
               sf::Sprite replacementmushroomSprite;
               replacementmushroomSprite.setTexture(ReplacementmushroomTexture);
              replacementmushroomSprite.setTextureRect(sf::IntRect(64,32,boxPixelsX,boxPixelsY));
              replacementmushroomSprite.setPosition(mushroomx[i],mushroomy[i]);
              window.draw(replacementmushroomSprite);}  else {cout<<"problem";}}

          void playercentipedecollision(float centipedexcoordinates[],int centipedeycoordinates[],float player[],bool centipedesegmentexists[])
          {
          for (int i = 0; i < 12; i++)
            {
                if (centipedesegmentexists[i])     //if any segment of centipede collides wuth player it dies
                {
                    float centipedeLeft = centipedexcoordinates[i];          //sets right and left boundaries of player
                    float centipedeRight = centipedexcoordinates[i] + boxPixelsX;
                    float centipedeTop = centipedeycoordinates[i];
                    float centipedeBottom = centipedeycoordinates[i] + boxPixelsY;         //alse sets  up and down

                    float playerLeft = player[x];
                    float playerRight = player[x] + boxPixelsX;
                    float playerTop = player[y];
                    float playerBottom = player[y] + boxPixelsY;

                    if (centipedeRight > playerLeft && centipedeLeft < playerRight && centipedeBottom > playerTop && centipedeTop < playerBottom)
                    { player[exists]=false;}  }  }  }              //same conditions as used for mushrooms above

     void playeroriginalheadcollision(float centipedeheadcoordinates[],float player[])
     {
         for (int j= 0; j < 1; j++)                  //if head of centipede collides with player player dies
            {
                
                
                    float centipedeLeft = centipedeheadcoordinates[x];
                    float centipedeRight = centipedeheadcoordinates[x] + boxPixelsX;         //right and left boundaries set again
                    float centipedeTop = centipedeheadcoordinates[y];
                    float centipedeBottom = centipedeheadcoordinates[y] + boxPixelsY;

                    float playerLeft = player[x];
                    float playerRight = player[x] + boxPixelsX;
                    float playerTop = player[y];
                    float playerBottom = player[y] + boxPixelsY;
                      if (centipedeRight > playerLeft && centipedeLeft < playerRight && centipedeBottom > playerTop && centipedeTop < playerBottom)
                      {player[exists]=false; } }  }          //same condition as above
                    




void movementinplayerarea(float centipedexcoordinates[], int centipedeycoordinates[], sf::Sprite centipedeSprite[], bool centipedesegmentexists[])
{
    float a = -0.1;          //made this function for back and forth movement but it does not work
    float b = 0;
    float c = 0.1;

    for (int i = 0; i < 12; i++)
    {
        if (centipedesegmentexists[i])
        {
            int rows = centipedeycoordinates[i] / boxPixelsY;         //finds rows

            for (int currentRow = 29; currentRow >= 25; currentRow--)
            {
                if (rows == currentRow)
                {
                    if ((centipedexcoordinates[i] <= 0 || centipedexcoordinates[i] >= resolutionX - boxPixelsX))      //checks bounds of centipede
                    {
                        centipedeSprite[i].move(b, -boxPixelsY);
                        centipedeycoordinates[i] += (-boxPixelsY);
                        rows -= 1;
                        if (rows >= 25 && rows <= 29)
            {
                if (rows % 2 == 0)
                {
                    centipedeSprite[i].move(a, b);
                    centipedexcoordinates[i] += a;
                }
                else if (rows % 2 != 0)
                {
                    centipedeSprite[i].move(c, b);
                    centipedexcoordinates[i] += c;
                }
                        
                        
                    }
                }
            }

            
            }
        }
    }
}

     void scoring(sf::Text& myscoreText23,int& score,sf::Font& myfont)       //scoring function
   {
    myfont.loadFromFile("OpenSans-ExtraBold.ttf");
       myscoreText23.setFont(myfont);      //sets text
        myscoreText23.setCharacterSize(2*boxPixelsX);      //size of text
          myscoreText23.setFillColor(sf::Color::Red);
             myscoreText23.setPosition(0, 0); // Adjust position as needed
                   myscoreText23.setString("Your Score: " + std::to_string(score));
                       myscoreText23.setString("Your Score: " + std::to_string(score));}
 
         void poisonplayercollision(int mushroomx[],int mushroomy[],bool mushroomexists[],float player[],int numberofmushroom)     //poisomous mushroom made if it collides with player 

     {
          for (int i = 0; i < numberofmushroom; i++)
            {
                if (mushroomexists[i] && player[exists]  && mushroomy[i]>768 && mushroomy[i]<928)
                {
                    float mushroomLeft = mushroomx[i];                  //checking right and left bounds
                    float mushroomRight = mushroomy[i] + boxPixelsX;
                    float mushroomTop = mushroomy[i];
                    float mushroomBottom = mushroomy[i] + boxPixelsY;

                    float playerLeft = player[x];
                    float playerRight = player[x] + boxPixelsX;
                    float playerTop = player[y];
                    float playerBottom = player[y] + boxPixelsY;
                     if (playerRight > mushroomLeft && playerLeft < mushroomRight && playerBottom > mushroomTop && playerTop < mushroomBottom)        //same condition like used above
                      {player[exists]=false;   }    }    }    }            //player existence becomes false
                      
                      
               void gameoverfunction(sf::RenderWindow& window,sf::Sprite gameoversprite,float player[])    //function to display game over sprite
           {
           if(player[exists]==false)
           { window.draw(gameoversprite);}
           }                   








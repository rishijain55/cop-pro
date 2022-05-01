/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 2500;
const int LEVEL_HEIGHT = 1405;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();
        
        void set(int w, int h);
		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		// LTexture(const LTexture &p1) {mTexture = p1.mTexture; mWidth = p1.mWidth;mHeight = p1.mHeight; }

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

class LWindow
{
	public:
		//Intializes internals
		LWindow();

		//Creates window
		bool init();

		//Creates renderer from internal window
		SDL_Renderer* createRenderer();

		//Handles window events
		void handleEvent( SDL_Event& e );

		//Deallocates internals
		void free();

		//Window dimensions
		int getWidth();
		int getHeight();

		//Window focii
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();

	private:
		//Window data
		SDL_Window* mWindow;

		//Window dimensions
		int mWidth;
		int mHeight;

		//Window focus
		bool mMouseFocus;
		bool mKeyboardFocus;
		bool mFullScreen;
		bool mMinimized;
};

class Player
{
    public:
        //The dimensions of the dot
        int PLAYER_WIDTH = 50;
        int PLAYER_HEIGHT = 100;

        //Maximum axis velocity of the dot
        static const int PLAYER_VEL = 50;

        //Initializes the variables
        Player();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

		void set(int w, int h);

        //Moves the dot
        void move(int camX, int camY);
        //Position accessors
        int getPosX();
        int getPosY();
        //Shows the dot on the screen
        void render(int camX,int camY,SDL_Rect* clip = NULL);
		void reset();

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;
        //The velocity of the dot
        int mVelX, mVelY;

};



//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Our custom window
//initializing textures and windows
LWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gBackgroundStartScreenTexture;
LTexture gBackgroundPlayTexture;
LTexture gSpriteSheetTexture;
Player player;
LTexture gPlayBefore;
LTexture gPlayHover;
LTexture gPlayDisplay;
LTexture quitText;
int playerHoldMoveSpeed=4;
bool play = false;
bool wasMoving = false;
int frame =4;
int countFrame =0;

const int WALKING_ANIMATION_FRAMES = 8;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

TTF_Font *gFont = NULL;

Player::Player()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}
void Player:: reset()
{

    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

}

void changeFrame( int dir ){

	if(dir ==0){
		if(frame ==0){
			frame =1;
		}
		else{
			frame = 0;
		}
	}
	else if(dir ==1){
		if(frame ==2){
			frame =3;
		}
		else{
			frame = 2;
		}	
	}
	else if(dir ==2){
		if(frame ==4){
			frame =5;
		}
		else{
			frame = 4;
		}	
	}
	else if(dir ==3){
		if(frame ==6){
			frame =7;
		}
		else{
			frame = 6;
		}

	}
	else{}

}

void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: 
				
				if(!wasMoving || countFrame==0){
					changeFrame(0);
					wasMoving=true;
					mVelY -= PLAYER_VEL;
				}

				countFrame= (countFrame+1)%playerHoldMoveSpeed;



			break;
            case SDLK_DOWN:
				 
				if(!wasMoving || countFrame==0){
					changeFrame(2);
					wasMoving=true;
					mVelY += PLAYER_VEL;
				}
				countFrame= (countFrame+1)%playerHoldMoveSpeed;

			break;
            case SDLK_LEFT:
				 
				if(!wasMoving || countFrame==0){
					changeFrame(3);
					wasMoving=true;
					mVelX -= PLAYER_VEL;
				}
				countFrame= (countFrame+1)%playerHoldMoveSpeed;

			break;
            case SDLK_RIGHT: 
				 
				if(!wasMoving || countFrame==0){
					changeFrame(1);
					wasMoving=true;
					mVelX += PLAYER_VEL;
				}
				countFrame= (countFrame+1)%playerHoldMoveSpeed;

			break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP  )
    {
		countFrame=0;
		// && e.key.repeat == 0
        //Adjust the velocity
        // switch( e.key.keysym.sym )
        // {
            // case SDLK_UP: mVelY += PLAYER_VEL; break;
            // case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
            // case SDLK_LEFT: mVelX += PLAYER_VEL; break;
            // case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
			wasMoving = false;
        // }
    }
}
void Player :: set(int w, int h)
{
	PLAYER_WIDTH=w;
	PLAYER_HEIGHT=h;
}
void Player::move(int camX, int camY)
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + PLAYER_WIDTH-camX > gWindow.getWidth()) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + PLAYER_HEIGHT -camY > gWindow.getHeight()) )
    {
        //Move back
        mPosY -= mVelY;
    }
	mVelX=0;
	mVelY=0;

}
void Player::render( int camX, int camY, SDL_Rect* clip )
{
    //Show the dot relative to the camera
    gSpriteSheetTexture.render( mPosX-camX, mPosY-camY,clip );
}

int Player :: getPosX()
{
	return mPosX;
}
int Player :: getPosY()
{
	return mPosY;
}


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture:: set(int w, int h)
{
    mWidth = w;
    mHeight = h;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	// if( clip != NULL )
	// {
	// 	renderQuad.w = clip->w;
	// 	renderQuad.h = clip->h;
	// }

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LWindow::LWindow()
{
	//Initialize non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
}

bool LWindow::init()
{
	//Create window
	mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if( mWindow != NULL )
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;
	}

	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer()
{
	return SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
}

void LWindow::handleEvent( SDL_Event& e )
{
	//Window event occured
	if( e.type == SDL_WINDOWEVENT )
	{
		//Caption update flag
		bool updateCaption = false;

		switch( e.window.event )
		{
			//Get new dimensions and repaint on window size change
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent( gRenderer );
			break;

			//Repaint on exposure
			case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent( gRenderer );
			break;

			//Mouse entered window
			case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;
			
			//Mouse left window
			case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
			case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
			case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

			//Window maxized
			case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
            break;
			
			//Window restored
			case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
            break;
		}

		//Update window caption with new data
		if( updateCaption )
		{
			std::stringstream caption;
			caption << "SDL Tutorial - MouseFocus:" << ( ( mMouseFocus ) ? "On" : "Off" ) << " KeyboardFocus:" << ( ( mKeyboardFocus ) ? "On" : "Off" );
			SDL_SetWindowTitle( mWindow, caption.str().c_str() );
		}
	}
	//Enter exit full screen on return key
	else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11)
	{
		if( mFullScreen )
		{
			SDL_SetWindowFullscreen( mWindow, SDL_FALSE );
			mFullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen( mWindow, SDL_TRUE );
			mFullScreen = true;
			mMinimized = false;
		}
	}
}

void LWindow::free()
{
	if( mWindow != NULL )
	{
		SDL_DestroyWindow( mWindow );
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int LWindow::getWidth()
{
	return mWidth;
}

int LWindow::getHeight()
{
	return mHeight;
}

bool LWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
	return mMinimized;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		if( !gWindow.init() )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = gWindow.createRenderer();
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                 //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
			}
		}
	}

	return success;
}

class Button
{
    private:
    //The attributes of the button
    SDL_Rect box;
    
    //The part of the button sprite sheet that will be shown
    string mButton;
	string mBefore;
	string mHover;
    
    public:
    //Initialize the variables
	Button( int x, int y, int w, int h, string beforeButton, string afterButton );
	void set(int x, int y, int w, int h);
    //Handles events and set the button's sprite region
    void handle_events(SDL_Event &event, bool playOrNot);
    
    //Shows the button on the screen
    void show();
};

Button::Button( int x, int y, int w, int h, string beforeButton, string afterButton )
{
    //Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    
    //Set the default button textures
	mBefore = beforeButton;
	mHover = afterButton;
    
}
void Button::set( int x, int y, int w, int h )
{
    //Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    
    
}

void Button::handle_events(SDL_Event &event, bool playOrNot)
{
    //The mouse offsets
    int x = 0, y = 0;
    
    //If the mouse moved

        //Get the mouse offsets
        // x = event.motion.x;
        // y = event.motion.y;
		// SDL_PumpEvents();
		SDL_GetMouseState(&x,&y);
        
        //If the mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {

            //Set the button sprite
            mButton = mHover;
        }
        //If not
        else
        {
            //Set the button sprite
			mButton = mBefore;
        }    
    // //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;
        
            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                play = playOrNot;
            }
        }
    }
}
void Button::show()
{
    //Show the button
	LTexture ButtonNew;
	ButtonNew.loadFromFile(mButton);
	ButtonNew.set(box.w,box.h);
	ButtonNew.render(box.x,box.y);
}



bool loadMedia()
{
	//Loading success flag
	bool success = true;

	// Load scene texture
	if( !gBackgroundStartScreenTexture.loadFromFile("technology-background.jpg"))
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !gPlayBefore.loadFromFile("playBefore.png"))
	{
		printf( "Failed to load play button!\n" );
		success = false;
	}
	if( !gPlayHover.loadFromFile("playHover.png"))
	{
		printf( "Failed to load play button on hover!\n" );
		success = false;
	}

    gFont = TTF_OpenFont( "Bevan.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    if( !gSpriteSheetTexture.loadFromFile( "movingman.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w =  114;
        gSpriteClips[ 0 ].h = 184.5;

        gSpriteClips[ 1 ].x =  0;
        gSpriteClips[ 1 ].y =  232.5;
        gSpriteClips[ 1 ].w =  114;
        gSpriteClips[ 1 ].h = 184.5;
        
        gSpriteClips[ 2 ].x = 390;
        gSpriteClips[ 2 ].y =   0;
        gSpriteClips[ 2 ].w = 126;
        gSpriteClips[ 2 ].h = 184.5;

        gSpriteClips[ 3 ].x = 390;
        gSpriteClips[ 3 ].y =   232.5;
        gSpriteClips[ 3 ].w =  126;
        gSpriteClips[ 3 ].h = 184.5;

        gSpriteClips[ 4 ].x = 265;
        gSpriteClips[ 4 ].y =   0;
        gSpriteClips[ 4 ].w =  114;
        gSpriteClips[ 4 ].h = 184.5;

        gSpriteClips[ 5 ].x = 265;
        gSpriteClips[ 5 ].y =   232.5;
        gSpriteClips[ 5 ].w =  114;
        gSpriteClips[ 5 ].h = 184.5;

        gSpriteClips[ 6 ].x = 118;
        gSpriteClips[ 6 ].y =   0;
        gSpriteClips[ 6 ].w =  135;
        gSpriteClips[ 6 ].h = 184.5;

        gSpriteClips[ 7 ].x = 118;
        gSpriteClips[ 7 ].y =   237;
        gSpriteClips[ 7 ].w =  135;
        gSpriteClips[ 7 ].h = 184.5;

    }


	return success;
}

void drawTexture( int x, int y, int w, int h, int r, int g, int b, int opak )
{
	SDL_Rect fillRect = { x, y , w, h};
	SDL_SetRenderDrawColor(gRenderer, r, g, b, opak );        
	SDL_RenderFillRect(gRenderer, &fillRect );   
}

void close()
{
	//Free loaded images
	gBackgroundStartScreenTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	gWindow.free();

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	



			//Main loop flag
			bool quit = false;

			//Make the button
			string beforePlay = "playBefore.png";
			string beforeQuit = "quitBefore.png";
			string hoverPlay = "playHover.png";
			string hoverQuit = "quitHover.png";
			Button playButton( gWindow.getWidth()/2-gWindow.getWidth()/10, gWindow.getHeight()/2-gWindow.getHeight()/10, gWindow.getWidth()/5, gWindow.getHeight()/5, beforePlay,hoverPlay );
			Button quitButton( gWindow.getWidth()-gWindow.getWidth()/5,0, gWindow.getWidth()/5, gWindow.getHeight()/10, beforeQuit,hoverQuit );
			
			//Event handler
			SDL_Event e;
			SDL_Rect camera = { 0, 0, gWindow.getWidth(),gWindow.getHeight()};

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					

					player.handleEvent(e);
					player.move(camera.x,camera.y);
					playButton.handle_events(e,true);
					quitButton.handle_events(e,false);
					//User , bool playOrNotrequests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle window events
					gWindow.handleEvent( e );
				}

				//Only draw when not minimized
				if( !gWindow.isMinimized() )
				{
					if(play)
					{
						camera.x = ( player.getPosX() + player.PLAYER_WIDTH/ 2 ) - gWindow.getWidth() / 2;
						camera.y = ( player.getPosY() + player.PLAYER_HEIGHT / 2 ) - gWindow.getHeight()  / 2;
						camera.w = gWindow.getWidth();
						camera.h = gWindow.getHeight();

						//Keep the camera in bounds
						if( camera.x < 0 )
						{ 
							camera.x = 0;
						}
						if( camera.y < 0 )
						{
							camera.y = 0;
						}
						if( camera.x > LEVEL_WIDTH - camera.w )
						{
							camera.x = LEVEL_WIDTH - camera.w;
						}
						if( camera.y > LEVEL_HEIGHT - camera.h )
						{
							camera.y = LEVEL_HEIGHT - camera.h;
						}
						quitButton.set( gWindow.getWidth()-gWindow.getWidth()/10,0, gWindow.getWidth()/10, gWindow.getHeight()/10 );

						// SDL_SetRenderDrawColor( gRenderer, 34, 139, 34, 0xFF );

						// SDL_RenderClear( gRenderer );
						// //vertical road
						// drawTexture(gWindow.getWidth()/2,0,gWindow.getWidth()/20,gWindow.getHeight(), 71, 72, 78, 255);
						// //horizontal roads
						// drawTexture(0,0,gWindow.getWidth(),gWindow.getHeight()/10, 71, 72, 78, 255);
						// drawTexture(0,gWindow.getHeight()/2,gWindow.getWidth(),gWindow.getHeight()/10, 71, 72, 78, 255);
						// drawTexture(0,gWindow.getHeight()*9/10,gWindow.getWidth(),gWindow.getHeight()/10, 71, 72, 78, 255);
						gBackgroundPlayTexture.loadFromFile("background.jpg");
						gBackgroundPlayTexture.set(gWindow.getWidth(),gWindow.getHeight());
						gBackgroundPlayTexture.render(0,0,&camera);
						SDL_Rect* currentClip = &gSpriteClips[frame];
						gSpriteSheetTexture.set(100,150);
						player.set(100,300);
						// gSpriteSheetTexture.render( turtle_specs.x, turtle_specs.y,currentClip );
						
						player.render(camera.x,camera.y,currentClip);
						quitButton.show();

						SDL_RenderPresent(gRenderer);

					}
					else{

					frame =4;
					camera.x =0;
					camera.y =0;
					player.reset();

					playButton.set( gWindow.getWidth()/2-gWindow.getWidth()/10, gWindow.getHeight()/2-gWindow.getHeight()/10, gWindow.getWidth()/5, gWindow.getHeight()/5 );
					SDL_Color textColor = { 255, 255, 255, 255 };
					string newGame = "New Game";
					string quit = "Quit";
					quitText.loadFromRenderedText(quit, textColor);
					gBackgroundStartScreenTexture.set(gWindow.getWidth(),gWindow.getHeight());
					gBackgroundStartScreenTexture.render(0,0);
					
					playButton.show();
					//Update screen
					SDL_RenderPresent( gRenderer );
					}
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
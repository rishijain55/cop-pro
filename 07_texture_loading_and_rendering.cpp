/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

//Screen dimension constants
 int SCREEN_WIDTH = 1800;
 int SCREEN_HEIGHT = 1000;

enum KeyPressTextures
{
    TURTLE_ICON,
	GREEN_Textures,
	END_IMAGE,
	END_POINT_IMAGE,
    KEY_PRESS_TEXTURE_TOTAL
};

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
		void drawTexture( int x, int y, int w, int h, int r, int g, int b, int opak);
		bool loadFromFileWithoutTransparent( std::string path );
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		//Deallocates texture
		void free();

		//Renders texture at given point

		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		bool set(int w , int h);

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// class for image loading
//Scene textures
LTexture gFooTexture;
LTexture Texttex;
LTexture gTextTexture;
LTexture gTimeTextTexture;
LTexture gBackgroundTexture;
LTexture gTurtle;
TTF_Font *gFont = NULL;

const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
LTexture gSpriteSheetTexture;



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
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

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

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
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
    
    //Return success
    return mTexture != NULL;
}

bool LTexture::loadFromFileWithoutTransparent( std::string path )
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

// void LTexture::render( int x, int y, SDL_Rect* clip )
// {
//     //Set rendering space and render to screen
//     SDL_Rect renderQuad = { x, y, mWidth, mHeight };

//     //Set clip rendering dimensions
//     // if( clip != NULL )
//     // {
//     //     renderQuad.w = clip->w;
//     //     renderQuad.h = clip->h;
//     // }

//     //Render to screen
//     SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
// }

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    // if( clip != NULL )
    // {
    //     renderQuad.w = clip->w;
    //     renderQuad.h = clip->h;
    // }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void LTexture::drawTexture( int x, int y, int w, int h, int r, int g, int b, int opak )
{
	SDL_Rect fillRect = { x, y , w, h};
	SDL_SetRenderDrawColor(gRenderer, r, g, b, opak );        
	SDL_RenderFillRect(gRenderer, &fillRect );   
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
// end class
bool LTexture::set(int w, int h){
	mWidth= w;
	mHeight= h;
	return true;
}


//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//Current displayed texture
// SDL_Texture* gKeyPressTextures[GREEN_Textures] = NULL;
SDL_Texture* gKeyPressTextures[ KEY_PRESS_TEXTURE_TOTAL ];


//The application time based timer
class LTimer
{
    public:
        //Initializes variables
        LTimer();

        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

        //Gets the timer's time
        Uint32 getTicks();

        //Checks the status of the timer
        bool isStarted();
        bool isPaused();

    private:
        //The clock time when the timer started
        Uint32 mStartTicks;

        //The ticks stored when the timer was paused
        Uint32 mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
};

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

LTimer timer;

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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC  );
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture

    if( !gTurtle.loadFromFile( "turtle.jpeg" ) )
    {
        printf( "Failed to load turtle\n" );
        success = false;
    }

    gFont = TTF_OpenFont( "lazy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };
        if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }


	gKeyPressTextures[GREEN_Textures] = loadTexture( "texture.png" );
	if( gKeyPressTextures[GREEN_Textures] == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gKeyPressTextures[END_POINT_IMAGE] = loadTexture( "endpoint.png" );
	if( gKeyPressTextures[END_POINT_IMAGE] == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	//Load PNG texture
	gKeyPressTextures[END_IMAGE] = loadTexture( "the-end.jpg" );
	if( gKeyPressTextures[END_IMAGE] == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gKeyPressTextures[TURTLE_ICON] = loadTexture( "turtle.png" );
	if( gKeyPressTextures[TURTLE_ICON] == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

    if( !gSpriteSheetTexture.loadFromFileWithoutTransparent( "movingman.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w =  184;
        gSpriteClips[ 0 ].h = 500;

        gSpriteClips[ 1 ].x =  184;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w =  184;
        gSpriteClips[ 1 ].h = 500;
        
        gSpriteClips[ 2 ].x = 368;
        gSpriteClips[ 2 ].y =   0;
        gSpriteClips[ 2 ].w =  184;
        gSpriteClips[ 2 ].h = 500;

        gSpriteClips[ 3 ].x = 752;
        gSpriteClips[ 3 ].y =   0;
        gSpriteClips[ 3 ].w =  184;
        gSpriteClips[ 3 ].h = 500;
    }

	return success;
}

void close()
{
	//Free loaded image
    gTextTexture.free();

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window    
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
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
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
    SDL_Rect turtle_specs;
    turtle_specs.x = 0;
    turtle_specs.y = 0;
    turtle_specs.w = SCREEN_WIDTH/25;
    turtle_specs.h = SCREEN_HEIGHT/13;

    SDL_Rect turtle_dis;
    turtle_dis.x = 0;
    turtle_dis.y = 0;
    turtle_dis.w = 150;
    turtle_dis.h = 500;

    SDL_Rect endpoint_specs;
    endpoint_specs.x = SCREEN_WIDTH*19/20;
    endpoint_specs.y = SCREEN_HEIGHT*9/10;
    endpoint_specs.w = SCREEN_WIDTH/20;
    endpoint_specs.h = SCREEN_HEIGHT/10;

    SDL_Rect green_bg_specs;
    green_bg_specs.x = 0;
    green_bg_specs.y = 0;
    green_bg_specs.w = SCREEN_WIDTH;
    green_bg_specs.h = SCREEN_HEIGHT;
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

			//Event handler
			SDL_Event e;

			//While application is running
			int frame =0;
			bool move = false;

            SDL_Color textColor = { 0, 0, 0, 255 };

            //Current time start time
            Uint32 startTime = 0;

            //In memory text stream
            std::stringstream timeText;
	
            double degrees = 0;

            //Flip type
            SDL_RendererFlip flipType = SDL_FLIP_NONE;

			bool dir = true;
			bool ud = true;
			timer.start();
			Uint32 t;
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;

					}
                    else if( e.type == SDL_KEYDOWN )
                    {
						frame++;

							//Cycle animation
						if( frame >= WALKING_ANIMATION_FRAMES )
						{
							frame = 0;
						}
                // Update screen
				 		move = true;

						

                        //Select surfaces based on key press
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
							if(turtle_specs.y>0 && turtle_specs.x==SCREEN_WIDTH/2){
                            turtle_specs.y-=10;
							degrees = 270.0;
							flipType = SDL_FLIP_NONE;
							}
                            break;

                            case SDLK_DOWN:
							if(turtle_specs.y < SCREEN_HEIGHT*9/10 && turtle_specs.x==SCREEN_WIDTH/2  ){
                            turtle_specs.y+=10;
							degrees = 90.0;			
							flipType = SDL_FLIP_NONE;
							timer.pause();
							}
							break;

                            case SDLK_LEFT:
							if(turtle_specs.x >0 && (turtle_specs.y== 0 || turtle_specs.y == SCREEN_HEIGHT/2|| turtle_specs.y == SCREEN_HEIGHT*9/10)){
                            turtle_specs.x-= 10;

							flipType = SDL_FLIP_HORIZONTAL;

							degrees=0.0;
							}
                            break;

                            case SDLK_RIGHT:
							if(turtle_specs.x < SCREEN_WIDTH-turtle_specs.w && (turtle_specs.y== 0 || turtle_specs.y == SCREEN_HEIGHT/2|| turtle_specs.y == SCREEN_HEIGHT*9/10) ){
                            turtle_specs.x+=10;
							flipType = SDL_FLIP_NONE;
							degrees = 0.0;
							break;
					
							}
							else{
							SDL_Rect* currentClip = &gSpriteClips[0];
							gSpriteSheetTexture.set(SCREEN_WIDTH/25,SCREEN_HEIGHT/10);
							gSpriteSheetTexture.render( turtle_specs.x, turtle_specs.y, currentClip );
							
							SDL_RenderPresent( gRenderer );	
							}
                            break;

                            default:

                            break;
                        }
			

                    }

				}


				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 34, 139, 34, 0xFF );

				SDL_RenderClear( gRenderer );
				//vertical road
				gFooTexture.drawTexture(SCREEN_WIDTH/2,0,SCREEN_WIDTH/20,SCREEN_HEIGHT, 71, 72, 78, 255);
				//horizontal roads
				gFooTexture.drawTexture(0,0,SCREEN_WIDTH,SCREEN_HEIGHT/10, 71, 72, 78, 255);
				gFooTexture.drawTexture(0,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT/10, 71, 72, 78, 255);
				gFooTexture.drawTexture(0,SCREEN_HEIGHT*9/10,SCREEN_WIDTH,SCREEN_HEIGHT/10, 71, 72, 78, 255);
                timeText.str( "" );
                timeText << "Milliseconds since start time " << SDL_GetTicks() - startTime;

                //Render text
				t = timer.getTicks();
                if( !gTimeTextTexture.loadFromRenderedText( to_string(t), textColor ) )
                {
                    printf( "Unable to render time texture!\n" );
                }
				
				string s = "hello world";

                // SDL_Rect fillRect = { SCREEN_WIDTH / 2, 0 , SCREEN_WIDTH / 20, SCREEN_HEIGHT};
                // SDL_SetRenderDrawColor( gRenderer, 0x47, 0x48, 0x4e, 0xFF );        
                // SDL_RenderFillRect( gRenderer, &fillRect );
				

                // SDL_Rect fillRector1 = { 0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT/10};
                // SDL_SetRenderDrawColor( gRenderer, 0x47, 0x48, 0x4e, 0xFF );        
                // SDL_RenderFillRect( gRenderer, &fillRector1 );
	
                // SDL_Rect fillRector2 = { 0, SCREEN_HEIGHT/2 , SCREEN_WIDTH, SCREEN_HEIGHT/10};
                // SDL_SetRenderDrawColor( gRenderer, 0x47, 0x48, 0x4e, 0xFF );        
                // SDL_RenderFillRect( gRenderer, &fillRector2 );

                // SDL_Rect fillRector3 = { 0, SCREEN_HEIGHT*9/10 , SCREEN_WIDTH, SCREEN_HEIGHT/10};
                // SDL_SetRenderDrawColor( gRenderer, 0x47, 0x48, 0x4e, 0xFF );        
                // SDL_RenderFillRect( gRenderer, &fillRector3 );

				SDL_RenderCopy( gRenderer, gKeyPressTextures[END_POINT_IMAGE], NULL, &endpoint_specs);
				// gTurtle.loadFromFileWithoutTransparent("movingman.png");
				// gTurtle.set(SCREEN_WIDTH/25,SCREEN_HEIGHT/10);
				// gTurtle.render(turtle_specs.x,turtle_specs.y,&turtle_dis);

                //Render current frame
				// if(move){
                SDL_Rect* currentClip = &gSpriteClips[frame];
				gSpriteSheetTexture.set(SCREEN_WIDTH/25,SCREEN_HEIGHT/10);
                gSpriteSheetTexture.render( turtle_specs.x, turtle_specs.y, currentClip,degrees,NULL,flipType );
				
                // SDL_RenderPresent( gRenderer );

				// SDL_Delay(100);
				// currentClip = &gSpriteClips[1];
				// gSpriteSheetTexture.set(SCREEN_WIDTH/25,SCREEN_HEIGHT/10);
                // gSpriteSheetTexture.render( turtle_specs.x-5, turtle_specs.y, currentClip );

                // SDL_RenderPresent( gRenderer );

				// SDL_Delay(100);
                // currentClip = &gSpriteClips[2];
				// gSpriteSheetTexture.set(SCREEN_WIDTH/25,SCREEN_HEIGHT/10);
                // gSpriteSheetTexture.render( turtle_specs.x-2.5, turtle_specs.y, currentClip );

                // SDL_RenderPresent( gRenderer );

				// SDL_Delay(100);
                // currentClip = &gSpriteClips[3];
				// gSpriteSheetTexture.set(SCREEN_WIDTH/25,SCREEN_HEIGHT/10);
                // gSpriteSheetTexture.render( turtle_specs.x, turtle_specs.y, currentClip );

                // SDL_RenderPresent( gRenderer );
				// move = false;
				// }
				// else{
                // SDL_Rect* currentClip = &gSpriteClips[0];
				// gSpriteSheetTexture.set(SCREEN_WIDTH/25,SCREEN_HEIGHT/10);
                // gSpriteSheetTexture.render( turtle_specs.x, turtle_specs.y, currentClip );
				
                // // SDL_RenderPresent( gRenderer );					
				// }



				gBackgroundTexture.loadFromFileWithoutTransparent("the-end.jpg");
				gBackgroundTexture.set(SCREEN_WIDTH,SCREEN_HEIGHT);
				
				// SDL_RenderCopy( gRenderer, gKeyPressTextures[TURTLE_ICON], NULL, &turtle_specs );

				if(turtle_specs.y == SCREEN_HEIGHT*9/10&& turtle_specs.x == SCREEN_WIDTH*19/20){
					gBackgroundTexture.render(0,0,&green_bg_specs);
				}
				gTimeTextTexture.render( gTextTexture.getWidth()*1.5, gTextTexture.getHeight());
                SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
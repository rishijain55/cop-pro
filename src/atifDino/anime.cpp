/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include<deque>
#include <string>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;
class LTimer
{
public:
	// Initializes variables
	LTimer();

	// The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	// Gets the timer's time
	Uint32 getTicks();

	// Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	// The clock time when the timer started
	Uint32 mStartTicks;

	// The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	// The timer status
	bool mPaused;
	bool mStarted;
};

LTimer frameTime;
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
		void render( int x, int y, SDL_Rect* clip = NULL );
		void renderback( int x, int y, SDL_Rect* clip = NULL);
		void renderbox( int x, int y, SDL_Rect* clip = NULL);

		//Gets image dimensions
		int getWidth();
		int getHeight();

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

//Walking animation
const int BOUNCING_FRAMES = 4;
SDL_Rect gBouncingManClips[ BOUNCING_FRAMES ];
LTexture gManTexture;
LTexture gBoxDinoTexture;
LTexture gTreeTexture;


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

void LTexture:: set(int w, int h)
{
	mWidth=w;
	mHeight=h;
}
bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	//free();

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

// bool LTexture::loadbackground(std::string path){

// 	//Get rid of preexisting texture
// 	//free();

// 	//The final texture
// 	SDL_Texture* newTextures = NULL;

// 	//Load image at specified path
// 	SDL_Surface* loadedSurfaces = IMG_Load( path.c_str() );
// 	if( loadedSurfaces == NULL )
// 	{
// 		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
// 	}
// 	else
// 	{
// 		//Color key image
// 		//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

// 		//Create texture from surface pixels
//         newTextures = SDL_CreateTextureFromSurface( gRenderer, loadedSurfaces );
// 		if( newTextures == NULL )
// 		{
// 			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
// 		}
		

// 		//Get rid of old loaded surface
// 		SDL_FreeSurface( loadedSurfaces );
// 	}

// 	//Return success
// 	backGtext = newTextures;
	
// 	return backGtext!=NULL;

// }
// bool LTexture::loadbox(std::string path){

// 	//Get rid of preexisting texture
// 	//free();

// 	//The final texture
// 	SDL_Texture* newTextures = NULL;

// 	//Load image at specified path
// 	SDL_Surface* loadedSurfaces = IMG_Load( path.c_str() );
// 	if( loadedSurfaces == NULL )
// 	{
// 		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
// 	}
// 	else
// 	{
// 		//Color key image
// 		//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

// 		//Create texture from surface pixels
//         newTextures = SDL_CreateTextureFromSurface( gRenderer, loadedSurfaces );
// 		if( newTextures == NULL )
// 		{
// 			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
// 		}
		

// 		//Get rid of old loaded surface
// 		SDL_FreeSurface( loadedSurfaces );
// 	}

// 	//Return success
// 	boxtext = newTextures;
	
// 	return backGtext!=NULL;

// }

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

void LTexture::render( int x, int y, SDL_Rect* clip)
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
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}
// void LTexture::renderback( int x, int y, SDL_Rect* clip)
// {
// 	//Set rendering space and render to screen
// 	SDL_Rect renderQuad = { x, y, 1800, 900 };

// 	//Set clip rendering dimensions
// 	if( clip != NULL )
// 	{
// 		renderQuad.w = clip->w;
// 		renderQuad.h = clip->h;
// 	}

// 	//Render to screen
// 	SDL_RenderCopy( gRenderer, backGtext, clip, &renderQuad );
// }
// void LTexture::renderbox( int x, int y, SDL_Rect* clip)
// {
// 	//Set rendering space and render to screen
// 	SDL_Rect renderQuad = { x, y, 50, 50 };

// 	//Set clip rendering dimensions
	
// 	//Render to screen
// 	SDL_RenderCopy( gRenderer, boxtext, clip, &renderQuad );
// }


int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LTimer::LTimer()
{
	// Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
{
	// Start the timer
	mStarted = true;

	// Unpause the timer
	mPaused = false;

	// Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	// Stop the timer
	mStarted = false;

	// Unpause the timer
	mPaused = false;

	// Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	// If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		// Pause the timer
		mPaused = true;

		// Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	// If the timer is running and paused
	if (mStarted && mPaused)
	{
		// Unpause the timer
		mPaused = false;

		// Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		// Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	// The actual timer time
	Uint32 time = 0;

	// If the timer is running
	if (mStarted)
	{
		// If the timer is paused
		if (mPaused)
		{
			// Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			// Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	// Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	// Timer is running and paused
	return mPaused && mStarted;
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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gManTexture.loadFromFile( "foo.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else
	{
		//Set sprite clips
		gBouncingManClips[ 0 ].x =   0;
		gBouncingManClips[ 0 ].y =   0;
		gBouncingManClips[ 0 ].w =  64;
		gBouncingManClips[ 0 ].h = 205;

		gBouncingManClips[ 1 ].x =  64;
		gBouncingManClips[ 1 ].y =   0;
		gBouncingManClips[ 1 ].w =  64;
		gBouncingManClips[ 1 ].h = 205;
		
		gBouncingManClips[ 2 ].x = 128;
		gBouncingManClips[ 2 ].y =   0;
		gBouncingManClips[ 2 ].w =  64;
		gBouncingManClips[ 2 ].h = 205;

		gBouncingManClips[ 3 ].x = 192;
		gBouncingManClips[ 3 ].y =   0;
		gBouncingManClips[ 3 ].w =  64;
		gBouncingManClips[ 3 ].h = 205;
	}
	if( !gBoxDinoTexture.loadFromFile( "crate.png" ) )
	{
		printf( "Failed to load crate!\n" );
		success = false;
	}
	gBoxDinoTexture.set(50,50);
	if( !gTreeTexture.loadFromFile( "tree.jpg" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	gManTexture.set(100,205);
	return success;
}

void close()
{
	//Free loaded images
	gManTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	int verticalloc = 500;

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
			SDL_Rect ground;
				ground.x = 0;
				ground.y = 300+205;
				ground.h = SCREEN_HEIGHT-300;
				ground.w = SCREEN_WIDTH;
			
			
			//Main loop flag
			bool quit = false,pause = false;

			//Event handler
			SDL_Event e;

			//Current animation frame
			int dinoFrame = 0;
			start : 
			double backg1loc = 0,backg2loc = 1800;
			deque<SDL_Rect> obstacles;
			deque<double> backGloc;
			double L = 0;
			for(int i = 0;i<3;++i ){
				backGloc.push_back(L);
				L += 1800.0;
			}
			SDL_Rect temp;
			double j = 500.0;
			int jumps = 1;
			double speed = 2.0;
			for(int i = 0;i<5;++i ){
				j += (400+rand()%1200);
				temp.x = j;
				temp.y = verticalloc+205-50;
				temp.h = 50;
				temp.w = 50;
				obstacles.push_back(temp);

			}
			double vert =  0.0,vert_velocity = 0.0,g = 0.0;	
			//While application is running
			SDL_Rect* backG1 = new SDL_Rect;
			SDL_Rect* Box = new SDL_Rect;
			
			backG1->x = 0;
			backG1->y = 0;
			backG1->w = 1800;
			backG1->h = 900;
			Box->x = 0;
			Box->y = 0;
			Box->w = 250;
			Box->h = 250;
			
			while( !quit && !pause)
			{
				frameTime.start();
				
				
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

                        //Select surfaces based on key press
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_SPACE:
                            
                            if(vert == 0){
								vert_velocity = 10;
								jumps++;
								g = 0.5;
								j += (200+200*speed+rand()%1200);
								temp.x = j;
								temp.y = verticalloc+205-50;
								temp.h = 50;
								temp.w = 50;
								obstacles.push_back(temp);
							}
							break;
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current dinoFrame
				
				vert = vert + vert_velocity;
				vert_velocity  = vert_velocity-g ;
				
				if(vert<=0){
					
					vert = 0;
					vert_velocity = 0;
					g = 0;
					if(!(jumps%5)){
						speed+=0.4;
						jumps++;
					}
					
				}

				bool flag = 0;    
				bool backflag = 0;
				for(auto &it : backGloc){
					it -= 8*speed;
					gTreeTexture.render(  it,0, backG1 );
					
					if(it<=-1800)
						backflag = 1;
				}
				SDL_Rect* currentClip = &gBouncingManClips[3- dinoFrame / 4 ];
				gManTexture.render(  10, verticalloc-vert, currentClip );
				for(auto &it : obstacles){
					
					it.x -= 8*speed;
					gBoxDinoTexture.render(  it.x,it.y, Box );
					if((it.x<=10+64 && it.x+50>=10+64 && it.y<=verticalloc+205-vert)||(it.x+50>=10 && it.x<=10 && it.y<=verticalloc+205-vert))
						pause = true;
					
					if(it.x<=-50)
						flag = 1;
				}
				
				
				j-=8.0*speed;
				
				
				if(backg1loc<=-1800){
					backg1loc = 0;
					
				}
				if(backflag){
					backGloc.pop_front();
					backGloc.push_back(backGloc[1]+1800.0);
				}
				
				if(flag)
					obstacles.pop_front();

				//Update screen
				SDL_RenderPresent( gRenderer );

				
				//Go to next dinoFrame
				++dinoFrame;

				//Cycle animation
				if( dinoFrame / 4 >= BOUNCING_FRAMES )
				{
					dinoFrame = 0;
				}
				if(frameTime.getTicks()<=33){
					SDL_Delay(33-frameTime.getTicks());
				}
			}
			if(pause){
				while(!quit)
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
                    {
						if(e.key.keysym.sym == SDLK_RETURN)
						pause = false;
						dinoFrame = 0;
						goto start;
                        
					}
				}
			}

		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
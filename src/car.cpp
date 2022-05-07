/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include<deque>
#include <string>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 540;
const int SCREEN_HEIGHT = 958;

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

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture(int w = 0, int h = 0);

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		//Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );


		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL );
		void rendertext( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		
		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		/*SDL_Texture* backGtext;
		SDL_Texture* boxtext;
		*/
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


LTexture::LTexture(int w, int h)
{
	//Initialize
	mTexture = NULL;
	mWidth = w;
	mHeight = h;
}


//Walking animation
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
//Globally used font
TTF_Font *gFont = NULL;
//Rendered texture
LTexture gTextTexture;
LTexture gTextTexturetime;
LTexture gSpriteSheetTexture(68,150);
LTexture backGText(540,958);
LTexture carText(75,150);

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    //free();

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
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

void LTexture::rendertext( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

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
				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
	gFont = TTF_OpenFont( "16_true_type_fonts/lazy.ttf", 28 );
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

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "racecar.png" ) || !backGText.loadFromFile("road_texture(1).png") || !carText.loadFromFile("car.png"))
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else
	{
		//Set sprite clips
		gSpriteClips[ 0 ].x =   0;
		gSpriteClips[ 0 ].y =   0;
		gSpriteClips[ 0 ].w =  68;
		gSpriteClips[ 0 ].h = 150;

	}
	
	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();
	gTextTexturetime.free();
	carText.free();
	backGText.free();
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

int main( int argc, char* args[] )
{
	int verticalloc = 700;
	int car_loc = SCREEN_WIDTH/2-35;
	int police_loc[4] = {70,50+135,20+270,3*135-10};
	int police_speed[4] = {9,2,9,2};
	int travelled = 0;
	int remainingtime = 10000;
	int extratime = 3000;

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
			int carframe = 0;
			LTimer countingtime;
			start : 
			
			travelled = 0;
			double backg1loc = 0;
			deque<SDL_Rect> obstacles[4];
			deque<double> backGloc;
			double L = 0;
			for(int i = 0;i<3;++i ){
				backGloc.push_back(L);
				L -= 958.0;
			}
			SDL_Rect temp;
			double j[4] = {0.0,0.0,0.0,0.0};
			int jumps = 1;
			double speed = 0.0;
			for(int k=0;k<4;++k){
				
				for(int i = 0;i<50;++i ){
					j[k] -= (600+rand()%1800);
					temp.x = police_loc[k];
					temp.y = j[k];
					temp.h = 71;
					temp.w = 150;
					obstacles[k].push_back(temp);

				}
				
			}
			
			
			//While application is running
			SDL_Rect* backG1 = new SDL_Rect;
			SDL_Rect* Box = new SDL_Rect;
			
			backG1->x = 0;
			backG1->y = 0;
			backG1->w = 540;
			backG1->h = 958;
			Box->x = 0;
			Box->y = 0;
			Box->w = 71;
			Box->h = 150;
			
			countingtime.start();

			while( !quit && !pause)
			{
				
				if(countingtime.getTicks()>=remainingtime){
					if(travelled<5000){
						pause = true;
						countingtime.pause();
					}
					else{
						travelled = 0;
						countingtime.start();
					}	
				}

				else if(travelled>=5000){
					travelled = 0;
					remainingtime+=2000+extratime;
					extratime/=2;
					//countingtime.start();
				}
				
				const Uint8 *state = SDL_GetKeyboardState(NULL);

						if(state[SDL_SCANCODE_W]){
							speed+=0.04;
						}
						if(state[SDL_SCANCODE_A]){
							car_loc-=3*speed;
						}
						if(state[SDL_SCANCODE_D]){
							car_loc+=3*speed;
						}
						if(state[SDL_SCANCODE_S]){
							speed-=0.1;
						}
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{						
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				speed-=0.01;
				if(speed<0) speed = 0;
				if(speed>4) speed = 4;
				if(car_loc<60) car_loc = 60;
				if(car_loc>3*135+10) car_loc = 3*135+10;

				//Render current frame
				
				bool frontflag = 0,backflag[4] = {0};    
				bool backGflag = 0;
				for(auto &it : backGloc){
					it += 8*speed;
					backGText.render(  0,it, backG1 );
					
					if(it>=958)
						backGflag = 1;
				}
				SDL_Rect* currentClip = &gSpriteClips[0];
				gSpriteSheetTexture.render(  car_loc, verticalloc, currentClip );
				for(int k = 0;k<4;++k){
					//frontflag = 0;
					backflag[k] = 0;
					for(auto &it : obstacles[k]){
						
						it.y -= (police_speed[k]-8*speed);
						carText.render(  it.x,it.y, Box );
						if(it.x<car_loc+57 && it.x>car_loc-62 && it.y<700+130 && it.y>700-140)
							pause = true;
						
						if(it.y>=958+958)
							backflag[k] = 1;
						
					}
					
					
				}
				
				
				for(int k=0;k<4;++k){
					j[k]+=(police_speed[k]-8*speed);
					if(backflag[k]){
						obstacles[k].pop_front();
						j[k] -= (600+rand()%1800);
						temp.x = police_loc[k];
						temp.y = j[k];
						temp.h = 71;
						temp.w = 150;
						obstacles[k].push_back(temp);
					}
				}
				if(backg1loc<=+958){
					backg1loc = 0;
					
				}
				if(backGflag){
					backGloc.pop_front();
					backGloc.push_back(backGloc[1]-958.0);
				}
				travelled+=8*speed;

				SDL_Color textColor = {0,0,255};
				gTextTexture.loadFromRenderedText( ("Remaining : "+to_string(5000-travelled)), textColor );
				textColor = {255,0,0};
				gTextTexturetime.loadFromRenderedText(("Remaining Time : "+to_string(int(remainingtime-countingtime.getTicks())/1000)), textColor );
				gTextTexture.rendertext( ( SCREEN_WIDTH - gTextTexture.getWidth() ) , 0 );
				gTextTexturetime.rendertext(0,0);


				//Update screen
				SDL_RenderPresent( gRenderer );

				
				//Go to next frame
				++carframe;

				//Cycle animation
				if( carframe / 4 >= WALKING_ANIMATION_FRAMES )
				{
					carframe = 0;
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
						if(e.key.keysym.sym == SDLK_RETURN){
							pause = false;
							carframe = 0;
							travelled = 0;
							//countingtime.stop();
							remainingtime = 10000;
							goto start;
							
						}	
                        
					}
				}
			}

		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
# COP-290 - TASK-2 - GAME PROJECT BY RISHI JAIN AND VATSAL JINGAR

## ABOUT THE GAME : 
1. This game is 2 player maze game which can be played from 2 different PCs. 
2. Folder assets contains all the image and music files.  
3. Folder src contains all the source code 
4. Check rules.pdf for playing the game. 
5. For Ubuntu:(install the necessary dependent libraries) 

## HOW TO RUN THE GAME :
Install SDL2 using
```
cd
git clone https://github.com/libsdl-org/SDL.git -b SDL2
cd SDL
mkdir build
cd build
../configure
make
sudo make install 
```

Clone the repository using 
```
git clone https://github.com/rishijain55/cop-pro 
```

To run the game 
```
cd src
make clean 
make 
./game 
```

To run the game change to ip address of server pc in line 3700 of game.cpp

> Note: For more information read RulesMazeGame.pdf and cop290slides.odp

## DEPENDENCIES : 
LIBRARIES :- SDL2, SDL2_image, SDL2_ttf, SDL2_mixer 

## SOME CHEAT CODE FOR TESTING 
1. PRESS Q FOR ZERO HAPPINESS. 
2. PRESS H FOR ZERO HEALTH. 
3. PRESS Z FOR GAMEOVER. 
4. PRESS F FOR MORE COINS.
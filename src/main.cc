#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <algorithm>

#include "game/game.h"
#include "utils/resource_manager.h"
#include "utils/input_manager.h"
#include "scene/main_scene.h"

int main(int argc, char **argv) {

    // Init everything
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        //handle error
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n",
                SDL_GetError());
        return 1;
    }

    //get screen size
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    printf("screen_width: %d, screen_height: %d\n", dm.w, dm.h);
    int width = dm.w;
    //-60 for the taskbar
    int height = dm.h-60;

    SDL_Window *window = SDL_CreateWindow("SDL test",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          width, height,
                                          SDL_WINDOW_RESIZABLE);

    if (!window) {
        printf("Error: Failed to open window\nSDL Error: '%s'\n",
                SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("Error: SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }
    
    // Game loop start
    bool running = true;
    float target_fps = 60;
    float frame_limit = 1000.f/target_fps;
    float delta = frame_limit;
    int countedFrames = 0;
    
    ResourceManager::startWorkerThread();
    Game::init(renderer, window, width, height);
    Game::setScene(new MainScene);
    while (running) {
        // input
        SDL_Event event;

        auto begin = SDL_GetTicks();

        float avgFPS = countedFrames / (begin/1000.0f);
        //printf("fps: %f\n",avgFPS);
        if( avgFPS > 2000000 ){
            avgFPS = 0;
        }
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    width = event.window.data1;
                    height = event.window.data2;
                    SDL_SetWindowSize(window, width, height);
                    break;
                case SDL_MOUSEWHEEL:
                    InputManager::updateMouseWheelScroll(event.wheel.y);
                    break;
                default:
                    break;
            }
        }
       
        
        //update the key state
        InputManager::update();
        Camera &camera = Game::getCamera();
        if (InputManager::getMouseWheelScroll() != 0) {
            float zoom = camera.getZoom() + InputManager::getMouseWheelScroll() * 0.15;  
            camera.updateZoom(zoom);
            InputManager::resetMouseWheelScroll();
        }
        // update
        Game::update(delta/1000.f);

        // render
        Game::render();
        countedFrames++;
        auto end = SDL_GetTicks();
        delta = end - begin;
        if(delta < frame_limit){
            SDL_Delay(frame_limit-delta);
            delta = frame_limit;
        }
    }

    Game::destroy();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

/*
-++++++++++++++**:.........:#.........=#..... #*:............*-.....  -+========.
+%%%%%%%%%%%%%@%+++******=%@==++++++=#@*=++++%#=++++++++++++#@*=++*#=#:%%#######=
+%%%%%%%%%%#%@#+++****##=#@==++++++=-@@-++++*++++++++=+*#+*+@@+=+=##-%@.%%######=
+%%%%%%%%%%%@%++*******=+@#=+++++#+-@@+=++++#=+++===+%%**%+-%@==#=%%-*@=+%######=
+%%%%%%%%%%%@*+****+*#==@@-+++++*#-#@@:++++#**#*=+@@@#%%+=:=%@:+@=@#-*@@ @%#####=
+%%%%%%%%%%@#=+*****#+=#@+=++++=#+-@@%-*++*@@#=+@@@@#-:---:=@@.@*=@+-*@@.=%%####=
+%%%%%%%%#%%*+**+++**==@@=+++++=%=%@@@=%++%:  @@%=:.:-====:+@@:@:#@+++@@% @%####=
+%%%%%%%%%@@**#++++%+=*@@=++++=##=@*:@=%=*# +@@..:========:%@#*@:@@=#*@@@ @%####=
+%%%%%%%%@%+**+++***==@@#=++++=#*@@ -@*%-+++@= .-=========:%@*@*%@%=%*%@@ @%####=
+%%##%%%%+****+++*#+=+@@#=++++=%%@:  @@#-+@%@@@+.:-======-:%@#@:%@@*@*@@% @%####=
+%%#%%#-#@%+#+++#*%==#@@#-#*++*@@-%@@@@%-++@@@@@@%+=--====:*@@@ #@%#%*@@* %%####=
+%%###@%@#%=#=+=%=%-=@@@%-@++=+@*#@-  @@=+:*   #*-=+*#=-=--*@@@ *@@#%#@@.-######=
+%%%%##%%*%+%=+*%-@=*@@@@:%*==%@-.+#: +@++@@@@-.-=---===++-*%@@ %@@%%@@% +@#%###=
+%%%%%##@*#+*++%*=@-%@%#@*##-=@*.--=@.:@++*+%@%-=========-+@@@@--@@+#@@ *.@*####=
+%%%%%#%#@+#+++#++%=@@**%@=@:=@*:--==-#@=*=-:::-==========+: @@@@%#.#@* @.@#####=
+%%%%%#%+@=#=*##+-%@@####@%@--@*+--=-:@#+*##@*===========::=:-@.@@#+@@ @+#%*####=
=####%%%*@=%+##+*@@@#*##*#@@@=@@#*--:=@++-::--============%%@#@*#@*@@ +@ @######=
=#######@%++++=%*=@%*+****%@@@@@%@#*-%+*==============----+++  -@@@% :@*+@#*####=
=#######@+*++@%==@@@@%####%@@@@@@@%#+##:-==============@*-**-:. @@% .@@.%%*#####-
=######@@-+%#=-+@@%%@@@@%*+=*%%%##%@@#++--..  ..:-====--+--+@*+-@@.:@@+#%#######-
=###%@#==%@*=-*@@%%%%%%@@@@@@@@@#++@##%%= =@@@#=:. .:======---.#@..@@@#%########-
=%%%*-+@#*==-#@@%%%%%%%%%@%*+++*@@##@@@=.%@@@@@@@@@%= :-=====:-@:.@@@@+#########-
+#++#@@@+==+@@@@@@%%%%%%%%@#*##*#@*---*:@@@@@@@@@@@@@@@:====-:@+ @@%@*-%#######*-
-#%@@@@=-:#@@@%##%@%%%%%%%@%*####+==-#@+@%+-:-=*#@@@@@#.===-.@@ @@%%@=+%########-
+#%@%+-:+@@@*:::+*%%%%%%%%@%#*=-::-*@@*+::=*%#++=--@@@.:==:.=@ @@%%%@.%%%#######-
+@@+::=@@@@@%**@@@@%%%%%%%%@@@@@@@@%- .--=-::=*###+@@*.-:.:%@@ @#%%@@.%@########-
=-..*@@@+#%@@#@@@%%%%%%%%%@#######**= :-=====-----::--. .%@@@@ @@%%%*-%#########-
.%@@@@@#@@@@@@@@@%%%%%%%%@@*******##@@+.:=======--.  .%@@@@%%@@.-@@@-+%#*#******:
+*---:.:: @@==*@@@@@%%%%@@**##########@@=..---.. .=@@@@@%%%%%%@@%*#@+#@%%#@@@@@@*
 :   .::-.@+.:.   *@@%%%@%**###########%@@#: .*@@@@@@@@%%%%%%%%%%@@*=%%-=@#***##:
=@@@%-.:-:=:----*@@%@@@@@**##############*@@@@@@@@%%%%@%%%%%%%%%%%@++@:+@%%##*#%-
-###%@@-.:----=**+: .#@%***#############**@@%%%%%%%%%%@%%%%%%%%%%%@+=@=%*%@%**#%-
-%####%@@..----:.:---=#@%##############*+#%#%##%###%%%%#%%%%%%%%%%@@=-*%*%@%**%@*
 @%#####@@+::::=*=---:..%@%############*#@@@@@@@@@@@@@%%%#%%@@%%%%%@#.@%#%##@@%@.
 -@######%@* +@+:------::-#@@@%######*#**@@@#===++   @@@@@%*-#%%%%%%@++@@@%##%%@ 
 :%%######%%*#@%:-=-----=--. -@@@@%###*#*+=++**##%=:+@%@@@@@@@%%%%%%@## @%=#%%#@=
 :=@###########%%:--------:-@::..+@@#%@@%%%%#%%@@%*+-.  =@@@@@@@@%%%%%=@.=@@@%%%+
 :-@############@+:--------+#:===:.=%@@@@@%%%%#=.=-:---=:-@@@@@@@@@@%@@==@+-*%##*
 -:#%############@+=#*===+:%%%@%#@@@**+%#+=*%*=*@@*+*#*+**@@@@@@@@@@@%@@---=#%**-
 -:*%############%@  =@@=@*@*  .%    @   @@:  @@  #@ - %@   @ @@@@@@@@*@%-=%@#**-
 -:+@#############@             @    @   -    #    @    +   @ @@@@@@@@@%+*@#***#-
 -:+@#############@ -  @@@*@ . .@   -@   *@ @ @@  %@   -@ . @ @@@@@@@%@@@#####*#-
 -:+%##########%%%@@@@@=.::+*@@%#@@%**#@%*%@%*++@@*+#@%@@%@*=@@@@@@@@@@@@#####*#-
   :=---------:     :-+++=        =                           @          .------.
*/
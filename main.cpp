// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

int omni_button = 0;
int quest = 0;


GameInputs inputs;
int action;
int update;

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
void init_second_map();
int main ();


int go_up(int x, int y);
int go_down(int x, int y);
int go_right(int x, int y);
int go_left(int x, int y);

int act(int x, int y);
void game_over();
void start_screen();
int pause_menu(int x, int y);



/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int life;
} Player;


/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define OMNI_BUTTON 7

int get_action(GameInputs inputs)
{
    if(!inputs.b1) return ACTION_BUTTON;
    if(!inputs.b2) return MENU_BUTTON;
    if(!inputs.b3) omni_button = !omni_button;

    if (inputs.ay > 0.2) return GO_UP;
    if (inputs.ay < -0.2) return GO_DOWN;
    if (inputs.ax < -0.2)  return GO_LEFT;
    if (inputs.ax > 0.2) return GO_RIGHT;

    else return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;

    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
        if(go_up(Player.x, Player.y) == 1) return FULL_DRAW; else break;
        case GO_LEFT:
        if(go_left(Player.x, Player.y) == 1) return FULL_DRAW; else break;
        case GO_DOWN:
        if(go_down(Player.x, Player.y) == 1) return FULL_DRAW; else break;
        case GO_RIGHT:
        if(go_right(Player.x, Player.y) == 1) return FULL_DRAW; else break;
        case ACTION_BUTTON:
        if(act(Player.x, Player.y)) return FULL_DRAW; else break;
        case MENU_BUTTON: 
        if(pause_menu(Player.x,Player.y)) return FULL_DRAW; else break;
        default:        break;
    }
    return NO_RESULT;
}

int act(int x, int y){
    MapItem* north = get_north(x,y);
    MapItem* south = get_south(x,y);
    MapItem* east = get_east(x,y);
    MapItem* west = get_west(x,y);

    // OLDMAN
    if(north -> type == OLDMAN || south -> type == OLDMAN || east -> type == OLDMAN || west -> type == OLDMAN){
        if(quest == 0){
            speech("Young soilder,","can you");
            speech("get me the","elixer from key.");
        }else if (quest == 1){
            speech("Thank you for","getting the elixer.");
            speech("Give my deepest", "gratitude to key.");
            quest=2;
            map_erase(30,30);
            add_oldman1(30,30);
        } else {
            speech("I heard key", "needed some help.");
        }
    return 1;
}
    // KEY
    if(north -> type == KEY || south -> type == KEY || east -> type == KEY || west -> type == KEY){
        if(quest == 0){
            speech("Please take this","elixer and give");
            speech("it to the","oldman.");
            quest = 1;
        } else if(quest == 2){
            speech("Help me make","more life elixers.");
            speech("Take this seal",",it will allow");
            speech("safe passage thro","ugh the portal.");
            speech("Now,", "take the portal");
            speech("and reterive nova", "herbs.");
            speech("They're hidden in", "plain sight.");
            speech("Now go","...");
            quest = 3;
        } else if(quest == 6){
            // edit the speech a bit more
            speech("You are finally","here. I have");
            speech("some bad news.", "My life elixer");
            speech("didn't work."," ");
            speech("The oldman is", "dead.");
            speech("This is the", "end my friend.");
            speech("Goodbye!"," :( ");
            game_over();
        }
        return 1;
    }
    // portal
    if(north -> type == PORTAL || south -> type == PORTAL || east -> type == PORTAL || west -> type == PORTAL){
        if(quest == 3){

           // cls_map();
            set_active_map(1);
            pc.printf(" portal check 1\n");
            init_second_map();
            Player.x=Player.y=5;
            quest = 4;
        } else if (quest == 5){
            pc.printf(" portal check 2\n");
            pc.printf(" portal check 3\n");
            set_active_map(0);
            pc.printf(" portal check 4\n");
            init_main_map();
            pc.printf(" portal check 5\n");
            map_erase(30,30);
            quest = 6;
        } else {
            speech("Quest incomplete"," or need a quest");
            speech("to enter","..");
        }
        return 1;
    }

    // NOVA herbs
    if(north -> type == NOVA || south -> type == NOVA || east -> type == NOVA || west -> type == NOVA){
         if(quest == 4){
             map_erase(6,4);
             add_dragon1(3,9);
             quest = 5;
         }
        return 1;
    }
    // DRAGON
    if(north -> type == DRAGON || south -> type == DRAGON || east -> type == DRAGON || west -> type == DRAGON){
        speech("You fool!", "Now you shall");
        speech("meet your maker,", " now dieeee");
        Player.life--;
        game_over();
    }
   // Dog
    if(north -> type == DOG || south -> type == DOG || east -> type == DOG || west -> type == DOG){
        speech("woof woof","");
        Player.x = Player.y = 33;
    }

return 0;
}
int go_up(int x, int y){
    MapItem* north = get_north(x,y);
    if (north -> walkable || omni_button){
        Player.y--;
        return 1;
    }else return 0;
}
int go_down(int x, int y){
    MapItem* south = get_south(x,y);
    if (south -> walkable || omni_button){
        Player.y++;
        return 1;
    }else return 0;
}
int go_left(int x, int y){
    MapItem* west = get_west(x,y);
    if (west -> walkable || omni_button){
        Player.x++;
        return 1;
    }else return 0;
}
int go_right(int x, int y){
    MapItem* east = get_east(x,y);
    if (east -> walkable || omni_button){
        Player.x--;
        return 1;
    }else return 0;
}
// starting screen
void start_screen()
{
    uLCD.filled_rectangle(0, 0, 128, 128, 0x000000);
    uLCD.locate(0, 0);
    uLCD.printf("\n\n\n    IMMORTALITY  \n");
    uLCD.printf("\n       QUESTS     \n");
    uLCD.printf("\n Press A to start the game\n" );
    uLCD.printf("\n By: Shyam Patel \n");
    while(1){
        inputs = read_inputs();
        if(inputs.b1 != 1){
            break;
        }
    }
    uLCD.filled_rectangle(0, 0, 128, 128, 0x000000);
}

// game over screen
void game_over()
{   uLCD.filled_rectangle(0, 0, 128, 128, 0x000000);
    uLCD.locate(0, 0);
    uLCD.printf("\n\n\n  Game Over! :( \n");
    while(1){
        wait_ms(55);
    }
}

// Pause menu
int pause_menu(int x, int y)
{
    //clears the Screen
    uLCD.filled_rectangle(0,0,128,128, 0x0000000);
    uLCD.locate(0,0);
    uLCD.printf("\n\n\n    PAUSED   \n");
    uLCD.printf(" \n    Life = %d\n", Player.life);
    uLCD.printf(" \n\n Press O to continue... ");
    while(1){
        inputs = read_inputs();
        if(inputs.b2 != 1){
            break;
        }
    }
    return MENU_BUTTON;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
        {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
            {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
                {
                draw_player(u, v, Player.has_key);
                continue;
                }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
                {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                    {
                    if (curr_item) // There's something here! Draw it
                        {
                        draw = curr_item->draw;
                        }
                    else // There used to be something, but now there isn't
                        {
                        draw = draw_nothing;
                        }
                    }
                }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
                {
                draw = draw_wall;
                }

            // Actually draw the tile
            if (draw) draw(u, v);
            }
        }

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);

    for(int i = map_width() + 3; i < map_area(); i += 39)
        {
        add_plant(i % map_width(), i / map_width());
        }
    pc.printf("plants\r\n");

    //NPC
    add_dog(10,10);
    add_oldman(30,30);
    add_portal(40,45);
    add_key(25,40);
    add_dragon(5,50);
    

    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");

    print_map();
}

void init_second_map()
{
    // "Random" fake novas
    Map* map = set_active_map(1);
    for(int i = map_width() + 3; i < map_area(); i += 39)
        {
        add_nova(i % map_width(), i / map_width());
        }
    pc.printf(" fake novas 2\r\n");

    //NPC

    add_portal(9,10);
    // nova herbs
    add_nova(6,4);


    pc.printf("Adding walls 2 !\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls 2 done!\r\n");

    print_map();
}



/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    pc.printf("Hello World!\n");
    start_screen();
    // Initialize the maps
    maps_init();
    init_main_map();
    Player.life = 1;
    pc.printf(" check 1\n ");
  //  init_second_map();
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 25;

    pc.printf(" check 2\n ");
    // Initial drawing
    draw_game(true);

    pc.printf(" check 3\n ");
    // Main game loop
    while(1)
        {
        uLCD.locate(0,0);
        uLCD.printf(" X: %d , Y: %d \n", Player.x, Player.y);
      
       
        // Timer to measure game update speed
        Timer t; t.start();

        // Actuall do the game update:
        // 1. Read inputs
        inputs = read_inputs();
        // 2. Determine action (get_action)
        action = get_action(inputs);
        // 3. Update game (update_game)
        update = update_game(action);
        // 3b. Check for game over
        // 4. Draw frame (draw_game)
        draw_game(update);


        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
        }
}

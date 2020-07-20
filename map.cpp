#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static Map map1;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
   return X*map.w + Y; // TODO: Fix me!
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
  return key%100 ; // TODO: Fix me!
}
unsigned map_hash2(unsigned key)
{
  return key%50 ; // TODO: Fix me!
}


void maps_init()
{
  map.items = createHashTable(map_hash, 100);    // TODO: Implement!
  map.w = 53;
  map.h = 53; // Initialize hash table // Set width & height

  map1.items = createHashTable(map_hash2,50);
  map1.w = 15;
  map1.h = 15;
}

Map* get_active_map()
{
    if (active_map == 0){
    return &map;
    } else if(active_map == 1){
        return &map1;
        }
    return NULL;
}

Map* set_active_map(int m)
{
    active_map = m;
    if(m == 0){
    return &map;
    } else if(m == 1){
           return &map1;
        }
     return NULL;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

void cls_map()
{
    destroyHashTable(get_active_map() -> items);
}

int map_width()
{
    return get_active_map() -> w;
}

int map_height()
{
    return get_active_map() -> h;
}

int map_area()
{
    return (get_active_map() -> w) * (get_active_map() -> h) ;
}

MapItem* get_north(int x, int y)
{
     Map* map = get_active_map();
     int key = XY_KEY(x,y-1);
     return (MapItem*)getItem(map->items,key);
}

MapItem* get_south(int x, int y)
{
     Map* map = get_active_map();
     int key = XY_KEY(x,y+1);
     return (MapItem*)getItem(map->items,key);
}

MapItem* get_east(int x, int y)
{
     Map* map = get_active_map();
     int key = XY_KEY(x-1,y);
     return (MapItem*)getItem(map->items,key);
}

MapItem* get_west(int x, int y)
{
     Map* map = get_active_map();
     int key = XY_KEY(x+1,y);
     return (MapItem*)getItem(map->items,key);
}

MapItem* get_here(int x, int y)
{
     Map* map = get_active_map();
     int key = XY_KEY(x,y);
     return (MapItem*)getItem(map->items,key);
}


void map_erase(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x,y);
    // finds item if found then deletes the item
    MapItem* eraseObj = (MapItem*) getItem(map -> items, key);
   if (eraseObj -> type == OLDMAN || eraseObj -> type == KEY || eraseObj -> type == NOVA)
        deleteItem(map -> items, key);

}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    if(w1 == NULL) {
        pc.printf("Memory out\n");
    }
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_oldman(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = OLDMAN;
    w1->draw = draw_oldman;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_oldman1(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = OLDMAN;
    w1->draw = draw_oldman1;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_key(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


void add_portal(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PORTAL;
    w1->draw = draw_portal;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_nova(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NOVA;
    w1->draw = draw_nova;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_dragon(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DRAGON;
    w1->draw = draw_dragon;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_dragon1(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DRAGON;
    w1->draw = draw_dragon1;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_dog(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOG;
    w1->draw = draw_dog;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

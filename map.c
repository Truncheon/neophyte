#include "map.h"

int TilemapAssert(Tilemap* tm);

Tilemap* TilemapNew(SDL_Renderer* rend, int t_w, int t_h, const char* tileset_path)
{
	Tilemap* tm;
	SDL_Surface* tmp;
	
	tm = (Tilemap*) calloc(1, sizeof(Tilemap));
	if(!tm){
		DEBUGMSG("Could not allocate tilemap!");
		return NULL;
	}

	/* Load surface and transform it into a texture */	
	tmp = IMG_Load(tileset_path);
	if(!tmp){
		DEBUGMSG("Could not load surface!");
		free(tm);
		return NULL;
	}

	tm->tileset = SDL_CreateTextureFromSurface(rend, tmp);
	if(!tm->tileset){
		DEBUGMSG("Could not create texture!");
		free(tm);
		SDL_FreeSurface(tmp);
		return NULL;
	}

	tm->w = tmp->w / t_w;
	tm->h = tmp->h / t_h;
	tm->tile_w = t_w;
	tm->tile_h = t_h;
	tm->num = tm->w * tm->h;

	printf("Tilemap data:\nWidth = %d\nHeight = %d\nTile size = (%d, %d)\nSource image size = (%d,%d)\n",
				tm->w, tm->h, t_w, t_h, tmp->w, tmp->h);

	SDL_FreeSurface(tmp);

	tm->mapping = (SDL_Rect*) calloc(tm->num, sizeof(SDL_Rect));
	if(tm->mapping == NULL){
		DEBUGMSG("Could not allocate mapping!");
		TilemapFree(tm);
		return NULL;
	}

	DEBUGMSG("Tilemap created!\n");

	return tm;
}

int TilemapLoad(SDL_Renderer* rend, Map* map, const char* map_path, const char* tileset_path)
{

	/* TODO: Add layer functionality */
	/* TODO: Refine scaling functionality (maybe make it a parameter? */

	int i, j, c, x;
	int tile_w, tile_h;
	FILE* fp;
	char* buffer,* pos;
	const int bufsize = 2048 * 4;
	const int scale = 2;

	//const char keys[5][16] = {"width=", "height=", "tilewidth=", "tileheight=", "data="};
	int* 	   chests[4] = {&map->w, &map->h, &tile_w, &tile_h};

	if(!rend || !map || !map_path || !tileset_path) return 1;

	/* Open file and read map */
	fp = fopen(map_path, "r");
	if(!fp){
		DEBUGMSG("Could not open file!");
		return 1;
	}

	/* Allocate memory */
	buffer = (char*) malloc(sizeof(char) * bufsize);
	if(!buffer){
		DEBUGMSG("Could not allocate/populate buffer!");
		fclose(fp);
		return 1;
	}

	memset(buffer, 0, bufsize);

	/* Load file into buffer */
	for(c = i = 0; c != EOF || i < bufsize - 1; i++){
		c = fgetc(fp);
		buffer[i] = c;
	}

	fclose(fp);

	/* Grab basic tilemap data */
	for(i = 0, pos = buffer; i < 4; i++){
		while(pos != buffer + bufsize){
			if(isdigit(pos[0])) break;
			pos++;
		}
		*chests[i] = strtol(pos, &pos, 10);
		//printf("%d %p\n", *chests[i], pos);
	}

	/* Allocate map structures */
	map->tilemap = TilemapNew(rend, tile_w, tile_h, tileset_path);
	if(!map->tilemap){
		DEBUGMSG("Could not create tilemap!");
		return 1;
	}

	map->tiles = (Tile*) calloc(map->w * map->h, sizeof(Tile));
	if(!map->tiles){
		DEBUGMSG("Could not allocate tiles!");
		TilemapFree(map->tilemap);
		return 1;
	}

	/* Establish codes for the tiles */
	for(i = 0; i < map->tilemap->w; i++)
		for(j = 0; j < map->tilemap->h; j++){
			FILLRECT(map->tilemap->mapping[i + j*map->tilemap->w],
					 map->tilemap->tile_w * i,
					 map->tilemap->tile_h * j,
					 map->tilemap->tile_w,
					 map->tilemap->tile_h);
	}
	printf("%d codes loaded!\n", map->tilemap->num);

	/* Setup pointer to read from specific point in the buffer */
	pos = strstr(buffer, "data=");
	if(!pos){
		DEBUGMSG("strstr returned nothing!");
		return 1;
	}
	pos+=5;

	/* Fill the map with the previous codes */
	x = 0;
	int bytes = 0, read = 0;
	for(j = 0; j < map->h; j++){
		for(i = 0; i < map->w; i++){
			/* TODO: Handle sscanf return value */
			sscanf(pos + read, "%d%n", &x, &bytes);
			read += bytes;
		
			map->tiles[i + map->w * j].code = x-1;
			FILLRECT(map->tiles[i + map->w * j].dest, 
					 map->tilemap->tile_w * scale * i,
					 map->tilemap->tile_h * scale * j,
					 map->tilemap->tile_w * scale,
					 map->tilemap->tile_h * scale);
		}
	}
	/* TODO: Support for more layers */

	if(!TilemapAssert(map->tilemap)){
		
	}
	free(buffer);
	return 0;
}

int TilemapAssert(Tilemap* tm)
{
	if(tm == NULL){
		DEBUGMSG("No tilemap!");
		return 0;
	}

	if(tm->tileset == NULL){
		DEBUGMSG("No tileset!");
		return 0;
	}
	if(tm->mapping == NULL){
		DEBUGMSG("No mapping!");
		return 0;
	}
	
	return 1;
}

void TilemapFree(Tilemap* tm)
{
	if(tm == NULL) return;

	if(tm->tileset) SDL_DestroyTexture(tm->tileset);
	if(tm->mapping) free(tm->mapping);
	free(tm);
}

Map* MapLoad(SDL_Renderer* rend, const char* mapfile, const char* tilesetpath)
{
	Map* m;

	if(rend == NULL || mapfile == NULL || tilesetpath == NULL){
		DEBUGMSG("Map failed to load!");
		return NULL;
	}

	m = (Map*) malloc(sizeof(Map));
	if(m == NULL){
		DEBUGMSG("Map failed to load!");
		return NULL;
	}
	
	m->renderer= rend;
	
	TilemapLoad(rend, m, mapfile, tilesetpath);

	MapPrint(m);

	return m;
}

void MapFree(Map* m)
{
	if(!m) return;

	if(m->tiles) free(m->tiles);
	TilemapFree(m->tilemap);
	free(m);
}

void MapPrint(Map* m)
{
	int x, y;

	if(!m) return;

	printf("Map: (%d by %d)\n\t", m->w, m->h);
	for(x = 0; x < m->w; x++){
		printf("%d\t", m->tiles[x].dest.x);
	}
	printf("\n");
	for(y = 0; y < m->h; y++){
		printf("%d\t", m->tiles[y * m->w].dest.y);
		for(x = 0; x < m->w; x++){
			printf("%d\t", m->tiles[y * m->w + x].code);
		}
		printf("\n");
	}
	printf("\n");
}

void MapRender(Map* m)
{
	int i, code;

	if(m == NULL || m->tilemap == NULL || m->tilemap->mapping == NULL || m->tiles == NULL)
		return;

	for(i = 0; i < m->w * m->h; i++){
		code = m->tiles[i].code;
		if(code > m->tilemap->num || code < 0)
			code = 0;

		SDL_RenderCopy( m->renderer,
						m->tilemap->tileset,
						&m->tilemap->mapping[code],
						&m->tiles[i].dest);
	}
}



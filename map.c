#include "map.h"

Tilemap* TilemapNew(SDL_Renderer* rend, int tile_w, int tile_h, const char* tileset_path)
{
	Tilemap* tm;
	int i, j;

	/* Alocate tilemap */
	tm = (Tilemap*) calloc(1, sizeof(Tilemap));
	if(!tm){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	tm->tileset = TextureLoad(rend, tileset_path);
	if(!tm->tileset){
		free(tm);
		return NULL;
	}

	/* Set basic tilemap data */
	if(tile_w == 0){
		tm->w = 1;
		tm->tile_w = tm->tileset->w;
	}
	else{
		tm->w = tm->tileset->w / tile_w;
		tm->tile_w = tile_w;
	}
	
	if(tile_h == 0){
		tm->h = 1;
		tm->tile_h = tm->tileset->h;
	}
	else{
		tm->h = tm->tileset->h / tile_h;
		tm->tile_h = tile_h;
	}
	
	tm->num = tm->w * tm->h;

	tm->mapping = (SDL_Rect*) calloc(tm->num, sizeof(SDL_Rect));
	if(!tm->mapping){
		DEBUGMSG(ERRNOMEM);
		TilemapFree(tm);
		return NULL;
	}
	
	/* Establish codes for the tiles */
	for(j = 0; j < tm->h; j++){
		for(i = 0; i < tm->w; i++){
			FILLRECT(tm->mapping[i + j * tm->w],
					 tm->tile_w * i,
					 tm->tile_h * j,
					 tm->tile_w,
					 tm->tile_h);
		}
	}

	DEBUGMSG("Tilemap created!\n");

	return tm;
}

void TilemapFree(Tilemap* tm)
{
	if(!tm) return;

	TextureFree(tm->tileset);
	if(tm->mapping) free(tm->mapping);
	free(tm);
}

Map* MapLoad(SDL_Renderer* rend, const char* mapfile, const char* tilesetpath)
{
	Map* m;

	if(!rend || !mapfile || !tilesetpath){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	m = (Map*) malloc(sizeof(Map));
	if(!m){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	MapFromFile(m, rend, mapfile, tilesetpath);

	//MapPrint(m);

	return m;
}

int MapFromFile(Map* map, SDL_Renderer* rend, const char* map_path, const char* tileset_path)
{

	/* TODO: Add layer functionality */
	/* TODO: Refine scaling functionality (maybe make it a parameter? */

	int i, c;
	int tile_w, tile_h;
	FILE* fp;
	char* buffer,* pos;
	const int bufsize = 2048 * 4;
	const int scale = 2;

	int* chests[4] = {&map->w, &map->h, &tile_w, &tile_h};

	if(!map || !map_path || !tileset_path){
		DEBUGMSG(ERRPARAM);
		return 1;
	}

	/* Open file and read map */
	fp = fopen(map_path, "r");
	if(!fp){
		DEBUGMSG(ERRFILE);
		return 1;
	}

	/* Allocate memory */
	buffer = (char*) malloc(bufsize * sizeof(char));
	if(!buffer){
		DEBUGMSG(ERRNOMEM);
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
	}

	map->tilemap = TilemapNew(rend, tile_w, tile_h, tileset_path);
	if(!map->tilemap){
		DEBUGMSG(ERRNOMEM);
		free(buffer);
		MapFree(map);
		return 1;
	}

	map->tiles = (Tile*) calloc(map->w * map->h, sizeof(Tile));
	if(!map->tiles){
		DEBUGMSG(ERRNOMEM);
		free(buffer);
		MapFree(map);
		return 1;
	}

	/* Setup pointer to read from specific point in the buffer */
	pos = strstr(buffer, "data=");
	if(!pos){
		DEBUGMSG(ERRPOINTER);
		free(buffer);
		MapFree(map);
		return 1;
	}
	pos+=5;

	MapFill(map, pos, scale);

	free(buffer);

	return 0;
}

int MapFill(Map* map, char* buffer, int scale)
{
	int i, j, bytes, read, x;

	if(!map || !buffer){
		DEBUGMSG(ERRPARAM);
		return 1;
	 }

	/* Fill the map with the codes */
	bytes = read = x = 0;
	for(j = 0; j < map->h; j++){
		for(i = 0; i < map->w; i++){

			int index = i + map->w * j;
			sscanf(buffer + read, "%d%n", &x, &bytes);
			read += bytes;
			map->tiles[index].code = x-1;
			map->tiles[index].coll = NORMAL;
			
			FILLRECT(map->tiles[index].dest, map->tilemap->tile_w * scale * i,
					 map->tilemap->tile_h * scale * j, map->tilemap->tile_w * scale,
					 map->tilemap->tile_h * scale);
		}
	}

	return 0;
}


void MapFree(Map* m)
{
	if(!m){
		DEBUGMSG(ERRPARAM);
		return;
	}

	if(m->tiles) free(m->tiles);
	TilemapFree(m->tilemap);
	free(m);
}

void MapPrint(Map* m)
{
	int x, y;

	if(!m){
		DEBUGMSG(ERRPARAM);
		return;
	}

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

	if(!m || !m->tilemap || !m->tilemap->mapping || !m->tiles){
		DEBUGMSG(ERRPARAM);
		return;
	}

	for(i = 0; i < m->w * m->h; i++){
		code = m->tiles[i].code;
		if(code > m->tilemap->num || code < 0)
			code = 0;
		TextureRender(m->tilemap->tileset, &m->tilemap->mapping[code],	&m->tiles[i].dest);
	}
}



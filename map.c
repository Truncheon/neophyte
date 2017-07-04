#include "map.h"

/*int TilemapAssert(Tilemap* tm);
*/

Tilemap* TilemapNew(SDL_Renderer* rend, int t_w, int t_h, const char* tileset_path)
{
	Tilemap* tm;
	int tmp_w, tmp_h;
	int i, j;

	/* Alocate tilemap */
	tm = (Tilemap*) calloc(1, sizeof(Tilemap));
	if(!tm){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	tm->tileset = UtilsLoadTexture(rend, tileset_path, &tmp_w, &tmp_h);
	if(!tm->tileset){
		free(tm);
		return NULL;
	}

	/* Set basic tilemap data */
	if(t_w == 0){
		tm->w = 1;
		tm->tile_w = tmp_w;
	}
	else{
		tm->w = tmp_w / t_w;
		tm->tile_w = t_w;
	}
	
	if(t_h == 0){
		tm->h = 1;
		tm->tile_h = tmp_h;
	}
	else{
		tm->h = tmp_h / t_h;
		tm->tile_h = t_h;
	}
	
	tm->num = tm->w * tm->h;

	tm->mapping = (SDL_Rect*) calloc(tm->num, sizeof(SDL_Rect));
	if(tm->mapping == NULL){
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

/*
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
}*/

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
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	m = (Map*) malloc(sizeof(Map));
	if(m == NULL){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}
	
	m->renderer = rend;

	MapFromFile(m, mapfile, tilesetpath);

	MapPrint(m);

	return m;
}

int MapFromFile(Map* map, const char* map_path, const char* tileset_path)
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

	if(!map || !map_path || !tileset_path) return 1;

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

	map->tilemap = TilemapNew(map->renderer, tile_w, tile_h, tileset_path);
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
			/* TODO: Handle sscanf return value */
			/* int w = */sscanf(buffer + read, "%d%n", &x, &bytes);
			read += bytes;
			//printf("%d\n", w);
			map->tiles[i + map->w * j].code = x-1;
			FILLRECT(map->tiles[i + map->w * j].dest, 
					 map->tilemap->tile_w * scale * i,
					 map->tilemap->tile_h * scale * j,
					 map->tilemap->tile_w * scale,
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

	if(m == NULL || m->tilemap == NULL || m->tilemap->mapping == NULL || m->tiles == NULL){
		DEBUGMSG(ERRPARAM);
		return;
	}

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



#include "map.h"

int TilemapAssert(Tilemap* tm);

Tilemap* TilemapNew(SDL_Renderer* rend, int t_w, int t_h, const char* tileset_path)
{
	Tilemap* tm;
	SDL_Surface* tmp;
	
	tm = (Tilemap*) calloc(1, sizeof(Tilemap));
	if(!tm) return NULL;

	/* Load surface and transform it into a texture */	
	tmp = IMG_Load(tileset_path);
	if(!tmp){
		printf("Could not load surface!\n");
		free(tm);
		return NULL;
	}

	tm->tileset = SDL_CreateTextureFromSurface(rend, tmp);
	if(!tm->tileset){
		printf("Could not create texture!\n");
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
		printf("Could not allocate mapping!\n");
		TilemapFree(tm);
		return NULL;
	}

	printf("Tilemap created!\n");

	return tm;
}

int TilemapLoad(SDL_Renderer* rend, Map* map, const char* map_path, const char* tileset_path)
{

	/* TODO: Add layer functionality */
	/* TODO: Refine scaling functionality (maybe make it a parameter? */

	int i, j, c, x;
	int tile_w, tile_h;
	FILE* fp;
	char* buffer,* pos,* tmp;
	const int bufsize = 2048 * 4;
	const int scale = 1;

	const char keys[5][16] = {"width=", "height=", "tilewidth=", "tileheight=", "data="};
	int* 	   chests[4] = {&map->w, &map->h, &tile_w, &tile_h};

	if(!rend || !map || !map_path || !tileset_path) return 1;

	/* Open file and read map */
	fp = fopen(map_path, "r");
	if(!fp){
		printf("Could not open file!\n");
		return 1;
	}

	/* Allocate memory */
	buffer = (char*) malloc(sizeof(char) * bufsize);
	if(!buffer){
		printf("Could not allocate/populate buffer!\n");
		fclose(fp);
		return 1;
	}

	memset(buffer, 0, bufsize);

	/* Load file into buffer */
	for(c = i = 0; c != EOF && i < bufsize - 1; i++){
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
		printf("%d %p\n", *chests[i], pos);
	}

	/* Allocate map structures */
	map->tilemap = TilemapNew(rend, tile_w, tile_h, tileset_path);
	if(!map->tilemap){
		printf("Could not create tilemap!\n");
		return 1;
	}

	map->tiles = (Tile*) calloc(map->w * map->h, sizeof(Tile));
	if(!map->tiles){
		printf("Could not allocate tiles!\n");
		TilemapFree(map->tilemap);
		return 1;
	}

	/* Establish codes for the tiles */
	for(i = 0; i < map->tilemap->w; i++)
		for(j = 0; j < map->tilemap->h; j++){
			map->tilemap->mapping[i + j*map->tilemap->w].x = map->tilemap->tile_w * i;
			map->tilemap->mapping[i + j*map->tilemap->w].y = map->tilemap->tile_h * j;
			map->tilemap->mapping[i + j*map->tilemap->w].w = map->tilemap->tile_w;
			map->tilemap->mapping[i + j*map->tilemap->w].h = map->tilemap->tile_h;
	}
	printf("%d codes loaded!\n", map->tilemap->num);

	/* Setup pointer to read from specific point in the buffer */
	pos = strstr(buffer, "data=");
	if(!pos){
		printf("strstr returned nothing!\n");
		return 1;
	}
	pos+=5;

	/* Fill the map with the previous codes */
	x = 0;
	int bytes = 0, read = 0;
	for(j = 0; j < map->h; j++){
		for(i = 0; i < map->w; i++){
			
			int w = sscanf(pos + read, "%d%n", &x, &bytes);
			read += bytes;
			//printf("bytes: %d, read: %d\n", bytes, read);

			/*if(w != 1){
				printf("something went wrong! w: %d, x: %d\n", w, x);
			}*/
			
			/*map->tiles[i + map->w * j].code   = x++;
			if(x >= map->tilemap->num){
				x = 0;
			}*/
			map->tiles[i + map->w * j].code = x-1;

			map->tiles[i + map->w * j].dest.x = map->tilemap->tile_w * scale * i;
			map->tiles[i + map->w * j].dest.y = map->tilemap->tile_h * scale * j;
			map->tiles[i + map->w * j].dest.w = map->tilemap->tile_w * scale;
			map->tiles[i + map->w * j].dest.h = map->tilemap->tile_h * scale;
		}
	}
	/* TODO: Support for more layers */

	TilemapAssert(map->tilemap);

	return 0;
}

int TilemapAssert(Tilemap* tm)
{
	if(tm == NULL){
		printf("No tilemap!\n");
		return 0;
	}

	printf("Asserting tilemap...\n\nw:%d h:%d num:%d t_w: %d, t_h: %d\n",
			tm->w, tm->h, tm->num, tm->tile_w, tm->tile_h);
	if(tm->tileset == NULL) printf("No tileset!\n");
	if(tm->mapping == NULL){
		printf("No mapping!\n");
		return 0;
	}
	return 0;
	for(int i = 0; i < tm->num; i++)
		printf("mapping(code:%d): (%d, %d, %d, %d)\n",
				i,
				tm->mapping[i].x,
				tm->mapping[i].y,
				tm->mapping[i].w,
				tm->mapping[i].h
			);
	return 0;
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

		printf("Map failed to load!\n");
		return NULL;
	}

	m = (Map*) malloc(sizeof(Map));
	if(m == NULL){
		printf("Map failed to load!\n");
		return NULL;
	}
	
	m->renderer= rend;
	
	TilemapLoad(rend, m, mapfile, tilesetpath);

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
	int i;

	if(!m) return;

	for(i = 0; i < m->w * m->h; i++)
		printf("(%d, %d) = %d\t", m->tiles[i].dest.x, m->tiles[i].dest.y, m->tiles[i].code);

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
		//m->tiles[i].code++;
	}
}



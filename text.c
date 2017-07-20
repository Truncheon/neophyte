#include "text.h"

#define NUMGLYPHS 255

Typewriter* TypewriterNew(SDL_Renderer* rend, const char* ft_path, const char* dialog_path, int ptsize)
{
	if(!ft_path || ptsize < 1){
		DEBUGMSG(ERRPARAM);
	}

	Typewriter* tw = (Typewriter*) malloc(sizeof(Typewriter));
	if(!tw){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	/*TODO: Dynamic color markup? */
	tw->color.r = 0xFF;
	tw->color.g = 0xFF;
	tw->color.b = 0xFF;
	tw->color.a = 0;

	tw->ptsize = ptsize;

	tw->font = TTF_OpenFont(ft_path, ptsize);
	if(!tw->font){
		DEBUGMSG(TTF_GetError());
		free(tw);
		return NULL;
	}

	tw->backdrop = TextureLoad(rend, dialog_path);

	tw->metrics = (GlyphMetrics*) calloc(255, sizeof(GlyphMetrics));
	if(!tw->metrics){
		DEBUGMSG(ERRNOMEM);
		TTF_CloseFont(tw->font);
		free(tw);
		return NULL;
	}

	for(int i = 0; i < NUMGLYPHS; i++){
		TTF_GlyphMetrics(tw->font, i,
				&tw->metrics[i].minx,
				&tw->metrics[i].maxx,
				&tw->metrics[i].miny,
				&tw->metrics[i].maxy,
				&tw->metrics[i].advance);
	}

	return tw;
}

void TypewriterFree(Typewriter* tw)
{
	if(!tw){
		DEBUGMSG(ERRPARAM);
		return;
	}

	TTF_CloseFont(tw->font);
	TextureFree(tw->backdrop);
	free(tw->metrics);
	free(tw);
}

void TypewriterWrite(Typewriter* tw, Dialog* dg, int x, int y)
{
	if(!tw || !dg){
		DEBUGMSG(ERRPARAM);
		return;
	}

	if(dg->over)
		return;

	SDL_Rect dest, src;
	int a = tw->backdrop->w;
	int b = tw->backdrop->h;
	int h = y + TYPEWRITERPADDINGV;

	TextureRenderAt(tw->backdrop, x, y);

	/* Vertical scrolling */
	h += dg->offset;
	int target_h = h;
	for(int i = 0; i <= dg->curr_msg && i < dg->num_msgs; i++){
		target_h += MSGVPADDING + dg->msgs[i].tex->h;
	}

	#define UPPERLIMIT (y + TYPEWRITERPADDINGV)
	#define LOWERLIMIT (y + b - TYPEWRITERPADDINGV)

	if(target_h > LOWERLIMIT){
		dg->offset--;
	}

	/*SDL_SetRenderDrawColor(tw->backdrop->rend, 0xFF, 0, 0, 0xFF);
	SDL_RenderDrawLine(tw->backdrop->rend, 0, UPPERLIMIT, 1920, UPPERLIMIT);
	SDL_RenderDrawLine(tw->backdrop->rend, 0, LOWERLIMIT, 1920, LOWERLIMIT);
	SDL_SetRenderDrawColor(tw->backdrop->rend, 0, 0xFF, 0, 0xFF);
	SDL_RenderDrawLine(tw->backdrop->rend, 0, h, 1920, h);
	*/
	for(int i = 0; i <= dg->curr_msg; i++){
		int w = 0;
		
		/* Horizontal scrolling */
		if(i == dg->curr_msg){
			if(dg->over == 1)
				break;
			if(dg->dt * dg->speed >= MSGSPEEDCONST){
				dg->dt -= MSGSPEEDCONST/dg->speed;

				if(h + dg->msgs[i].tex->h <= LOWERLIMIT)
				while(1){
					if(dg->curr_point == dg->msgs[i].len)
						break;
					char curr_glyph = dg->msgs[i].text[++dg->curr_point];
					dg->curr_w += tw->metrics[(int) curr_glyph].advance;
					if(curr_glyph != ' '){
						break;
					}
				}
				
				if(dg->curr_point == dg->msgs[i].len){
					dg->curr_point = 0;
					dg->curr_msg++;
					if(dg->curr_msg == dg->num_msgs){
						//Done with the dialog. Await key press to exit
						dg->over = 1;
					}
					dg->curr_w = 0;
					dg->dt = -MSGTIMEBETWEENLINES;
					w = dg->msgs[i].tex->w;
				}
				else
					w = dg->curr_w;
			}
			else{
				dg->dt += MSGSPEEDSTEP;
				w = dg->curr_w;
			}
		}
		else
			w = dg->msgs[i].tex->w;
		
		//Renderable
		if(h + dg->msgs[i].tex->h >= UPPERLIMIT){
			//Partially rendered
			if(h < UPPERLIMIT){
				int upperlimit = UPPERLIMIT - h;
				
				/* It seems like these checks are useless */
				/* TODO: Investigate if it is even possible for these values */
				/* to be registered ever */
				if(upperlimit < 0){
					printf("Exception 1\n");
					upperlimit = 0;
				}
				if(upperlimit > dg->msgs[i].tex->h){
					printf("Exception 2\n");
					upperlimit = dg->msgs[i].tex->h;
				}
				
				if(w > a - 2*TYPEWRITERPADDINGH){
					printf("'%s', %d\n", dg->msgs[i].text, w);
				}
				FILLRECT(dest, x + TYPEWRITERPADDINGH, UPPERLIMIT, w, dg->msgs[i].tex->h - upperlimit);
				FILLRECT(src, 0, upperlimit, w, dg->msgs[i].tex->h - upperlimit);
			}
			else{
				if(w > a - 2*TYPEWRITERPADDINGH){
					printf("'%s', %d\n", dg->msgs[i].text, w);
				}

				FILLRECT(dest, x + TYPEWRITERPADDINGH, h, w, dg->msgs[i].tex->h);
				FILLRECT(src, 0, 0, w, dg->msgs[i].tex->h);
			}
		
			SDL_RenderCopy(tw->backdrop->rend, dg->msgs[i].tex->tex, &src, &dest);
			//TextureRender(dg->msgs[i].tex, &src, &dest);
		}
		h += MSGVPADDING + dg->msgs[i].tex->h;
	}
}


char** TypewriterSplitText(Typewriter* tw, const char* input, int width, int* n)
{
	if(!tw || !n || !input || (width < 0)){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	char* msg = strdup(input);

	char** messages = (char**) malloc(sizeof(char*));
	messages[0] = msg;
	int num = 1;

	/* Splitting */
	for(int i = 0, c = 0, n = strlen(msg); i < n; i++){
		c += tw->metrics[(int) msg[i]].advance;
		if(c >= width){
			while(i >= 0){
				if(msg[i] == ' '){
					if(c <= width){
						//printf("%d vs %d\n", c, width);
						msg[i] = 0;
						break;
					}
				}
				c -= tw->metrics[(int) msg[i]].advance;
				//printf("%c, %d\n", msg[i], c);
				i--;
			}

			if(i < 0){
				DEBUGMSG("Split text failed!");
				return NULL;
			}
			i++;

			char** ptr = realloc(messages, sizeof(char*) * ++num);
			if(!ptr){
				DEBUGMSG(ERRNOMEM);
				free(messages);
				free(msg);
				return NULL;
			}
			messages = ptr;

			messages[num - 1] = msg + i;
			c = 0;
		}
	}
	*n = num;
	
	/* Duplicating */
	char** ret = (char**) malloc(sizeof(char*) * num);
	for(int i = 0; i < num; i++){
		ret[i] = strdup(messages[i]);
		
		/* TODO: Check if there is no way of strings to be ending with a space or something */
		/* Probably not */
		/*for(int j = strlen(ret[i]) - 1; j >= 0; j--){
			if(ret[i][j] != ' ' || ret[i][j] != 0){
				break;
			}
			ret[i][j] = 0;
		}*/
	}
	
	/* Not entirely sure how this works but valgrind told me this is how i */
	/* should do it */
	free(messages[0]);
	free(messages);
	
	return ret;
}

int DialogFill(Dialog* dg, Typewriter* tw, const char* msg)
{
	if(!dg || !tw || !msg){
		DEBUGMSG(ERRPARAM);
		return 1;
	}
	
	SDL_Surface* tmp;
	int width = tw->backdrop->w;
	int num_msgs;
	char* text = strdup(msg);

	//printf("%d\n", width - 2*TYPEWRITERPADDINGH);
	char** results = TypewriterSplitText(tw, msg, width - 2*TYPEWRITERPADDINGH, &num_msgs);

	dg->curr_msg = 0;
	dg->msgs = (Message*) malloc(sizeof(Message) * num_msgs);
	if(!dg->msgs){
		DEBUGMSG(ERRNOMEM);
		free(text);
		return 1;
	}

	for(int i = 0; i < num_msgs; i++){
		Message* m = &dg->msgs[i];

		m->len = strlen(results[i]);
		m->text = results[i];

		tmp = TTF_RenderText_Solid(tw->font, m->text, tw->color);
		if(!tmp){
			DEBUGMSG(TTF_GetError());
			return 1;
		}

		m->tex = TextureFromSurface(tw->backdrop->rend, tmp);
		if(!m->tex){
			SDL_FreeSurface(tmp);
			return 1;
		}

		SDL_FreeSurface(tmp);
		//printf("'%s' (%d)\n", m->text, m->tex->w);
	}
	dg->num_msgs = num_msgs;

	free(results);
	free(text);

	return 0;
}

Dialog* DialogNew(Typewriter* tw, char* msg, int spd)
{
	if(!tw || !msg){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	Dialog* dg = (Dialog*) calloc(1, sizeof(Dialog));
	if(!dg){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	dg->speed = spd;

	DialogFill(dg, tw, msg);
	
	return dg;
}

void DialogFree(Dialog* dg)
{
	if(!dg){
		DEBUGMSG(ERRPARAM);
		return;
	}
	for(int i = 0; i < dg->num_msgs; i++){
		MessageFree(&dg->msgs[i]);
	}
	free(dg->msgs);
	free(dg);
}

void MessageFree(Message* m)
{
	if(!m){
		DEBUGMSG(ERRPARAM);
		return;
	}

	if(m->tex) TextureFree(m->tex);
	if(m->text) free(m->text);
}

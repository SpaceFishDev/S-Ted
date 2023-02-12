#pragma once
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include<iostream>
#include <cctype>


class Renderer{
public:
	int Width;
	int Height;
	TTF_Font* ttfFont;
	int FontSize = 21;
	bool Quit = false;
	SDL_Rect* rect;
	char* Buffer;
	int BufferIndex = 0;
	int CursorX = 0;
	int CursorY = 0;
	bool Shift = false;

	bool Ctrl = false;
	Renderer(int Width, int Height, const char* Font){
		SDL_Init(SDL_INIT_VIDEO);
		if(TTF_Init()==-1) {
		    printf("TTF_Init: %s\n", TTF_GetError());
		    exit(2);
		}
		ttfFont = TTF_OpenFont(Font, FontSize);
		SDL_CreateWindowAndRenderer(Width, Height, 0, &sdl_Window, &sdl_Renderer);
		SDL_SetWindowTitle(sdl_Window, "S-Ted");
		Buffer = (char*)malloc(Width * Height);
		int i = 0;
		while(i != Width*Height){
			Buffer[i] = 0;
			++i;
		}
		this->Width = Width;
		this->Height = Height;
		CursorX = Width / 6;
		// BufferY = Height;
	}

	void DrawCursor(int X, int Y){
		int y = Y + FontSize;
		int x = X + 4;
		if(x <= Width / 6){
			x = Width / 6;
			return;
		}
		while(y != Y + 1){
			--y;
			SDL_RenderDrawPoint(sdl_Renderer,x,y);
		}
	}

	void DrawChar(char c){
		if(BufferIndex < 0){
			BufferIndex = 0;
		}
		char str[2] = {c, 0};
		SDL_Rect s = {0};
		SDL_Texture* t = get_text_and_rect(sdl_Renderer, BufferX, BufferY, str, ttfFont, &s);
		SDL_RenderCopy(sdl_Renderer, t, NULL, &s);
		BufferX += (FontSize / 2) + 3;
		if(BufferX > Width){
			BufferX = 32;
		    BufferY += FontSize;
		}
		SDL_DestroyTexture(t);
	}
	void DrawText(const char* c){
		while(*c){
			if(*c == '\n'){
				BufferY += FontSize;
				BufferX = (Width / 6 );
				++c;
				continue;
			}
			if(*c == '\t'){
				int i = 0;
				while(i != 5 && BufferX + 1 < Width){
					DrawChar(' ');
					++i;
				}
				++c;
				continue;
			}
			DrawChar(*c);
			++c;
		}
		
	}
	void PutChar(char c){
		Buffer[BufferIndex] = c;
		++BufferIndex;
		CursorX += (FontSize / 2) + 3;
	}

	void Update(){
		SDL_Event event;
		while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
           		Quit = true;
            }
            if(event.type == SDL_KEYDOWN){
            	switch(event.key.keysym.sym){
	            	case SDLK_LEFT:{
	            		--BufferIndex;
		            	CursorX -= 3 + (FontSize/2);
	            		continue;
	            	} break;
		            case SDLK_RIGHT:{
	            		++BufferIndex;
		            	CursorX += 3 + (FontSize/2);
	            		continue;
		            } break;
		             case SDLK_DOWN:{
	            		BufferIndex += Width;
	            		BufferY += FontSize;	
		            } break;
		             case SDLK_UP:{
	            		BufferIndex -= Width;
	            		BufferY -= FontSize;	
		            } break;
            	}
            	if(event.key.keysym.sym == SDLK_BACKSPACE){
            		int i = BufferIndex + 1;
            		if(CursorX < 5 + (Width/6)){
            			CursorX = (Width/6) + (3 + (FontSize / 2));
            			BufferIndex = 0;
            		}
            		while(i != Width*Height){
						Buffer[i] = Buffer[i + 1];
						if(Buffer[i] != 0){
						}
						++i;
					}
            		--BufferIndex;
            		CursorX -= 3 + (FontSize / 2);
					PutChar(' ');
            		CursorX -= 3 + (FontSize / 2);
					--BufferIndex;
            		continue;
            	}
            	if(event.key.keysym.sym == SDLK_LSHIFT){
            		Shift = true;
            		continue;
            	
            	}
            	if(event.key.keysym.sym == SDLK_LCTRL){
            		Ctrl = true;
            		continue;
            	
            	}
            	if(event.key.keysym.sym == SDLK_RETURN){
            		PutChar('\n');
            		CursorX = Width / 6;
            		continue;
            	}
            	if(Buffer[BufferIndex + 1] != 0){
            		int i = BufferIndex;
            		while(i < Width * Height){
	            		Buffer[i + 1] = Buffer[i];
		            	++i;
	            	}
	            	if(!Shift){
	            			PutChar(event.key.keysym.sym);
	            			if(event.key.keysym.sym == 's'){
	            				if(Ctrl){
	            					printf("save\n");
	            					event.key.keysym.sym = ' ';
	            				}
	            			}
		            	}
		            	else{

		            		    char symbols[10] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
			            		if(event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9'){
			            			PutChar(symbols[event.key.keysym.sym - '0']);
			            			continue;
			            		}
			            		if(event.key.keysym.sym == '[' || event.key.keysym.sym == ']'){
			            			event.key.keysym.sym += '{' - '['; 
			            		}
		            			PutChar(toupper(event.key.keysym.sym));
		            	}
            	}else{
	            	if(!Shift){
	            		PutChar(event.key.keysym.sym);
	            		if(event.key.keysym.sym == 's'){
	            				if(Ctrl){
	            					printf("save\n");
	            					event.key.keysym.sym = ' ';
	            				}
	            			}
	            	}
	            	else{
	            			
	            		    char symbols[10] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
		            		if(event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9'){
		            			PutChar(symbols[event.key.keysym.sym - '0']);
		            			continue;
		            		}
		            		if(event.key.keysym.sym == '[' || event.key.keysym.sym == ']'){
		            			event.key.keysym.sym += '{' - '['; 
		            		}
	            			PutChar(toupper(event.key.keysym.sym));
	            	}
            	}
            }

            if(event.type == SDL_KEYUP){
            	if(event.key.keysym.sym == SDLK_LSHIFT){
            		Shift = false;
            	}if(event.key.keysym.sym == SDLK_LCTRL){
            		Ctrl = false;
            	}
            }
        }
        DrawText(Buffer);
        SDL_SetRenderDrawColor(sdl_Renderer, 255,255,255,255);
		DrawCursor(CursorX, BufferY);
        SDL_SetRenderDrawColor(sdl_Renderer, 0,0,0,255);
		SDL_RenderPresent(sdl_Renderer);
		BufferX = Width / 6;
		BufferY = 0;
        SDL_RenderClear(sdl_Renderer);
	}

private:
	SDL_Texture* get_text_and_rect(SDL_Renderer* renderer, int x, int y, const char *text,
        TTF_Font *font, SDL_Rect *rect) {
	    int text_width;
	    int text_height;
	    SDL_Surface *surface;
	    SDL_Color textColor = {255, 255, 255, 0};

	    surface = TTF_RenderText_Solid(font, text, textColor);
	    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	    text_width = surface->w;
	    text_height = surface->h;
	    SDL_FreeSurface(surface);
	    rect->x = x;
	    rect->y = y;
	    rect->w = text_width;
	    rect->h = text_height;
	    return texture;
	}
	SDL_Window * sdl_Window;
	SDL_Renderer* sdl_Renderer;
	int BufferX = 32;
	int BufferY = 0;
};
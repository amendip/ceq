#include <SDL2/SDL.h>
#include <stdio.h>
#include <pulse/simple.h> 

#define sqrt2 1.41421356237

float fny(float rad, float x){
float y;
y=rad*rad-x*x;
for(int i=0;i<y;i++){ if(i*i==y) return i; else if(i*i>y) return i-1; }
}

void prl(float x1, float y1, float* x2, float* y2, float perc){
perc/=255;
*x2=x1+(*x2-x1)*perc;
*y2=y1-(y1-*y2)*perc;
}

int main(int argc, char* argv[])
{
float si=100, ss=500, p=20; char ceqlogo=0;
if( argc > 1 ) {
for(int i=1; i<argc; i++){

	if(argv[i][0] == '-'){
		switch(argv[i][1]){
		case 'h': printf("default value example: ceq -d100 -s500\n\t-d\tdiameter of circle\n\t-s\twindow size\n\t-h\tdisplay this help and exit\n\t-c\tdraw \"ceq\" in the middle\n"); return 0;
		case 'd': sscanf(&argv[i][2], "%f", &si); break;
		case 's': sscanf(&argv[i][2], "%f", &ss); break;
		case 'c': ceqlogo=1; break;
		}
	}
}
}
float r=si/2;
float bs=ss, st=(r/sqrt2)/p, bst=bs/(2*p*sqrt2);
int resx, resy;

pa_simple *s;
pa_sample_spec ssp;
ssp.format = PA_SAMPLE_U8;
ssp.channels = 1;
ssp.rate = 16000;

pa_buffer_attr bset;
bset.maxlength = (uint32_t) -1;
bset.tlength = (uint32_t) -1;
bset.prebuf = (uint32_t) -1;
bset.minreq = (uint32_t) -1;
bset.fragsize = 400;

s = pa_simple_new(NULL,               // Use the default server.
		"ceq",              // Application name.
		PA_STREAM_RECORD,
		NULL,               // Use the default device.
		"ceq", // Stream description.
		&ssp,               // Sample format.
		NULL,               // Use default channel map
		&bset,              // Buffering attributes.
		NULL                // Ignore error code.
		);

unsigned char data[(int)p*8];
 float x1, x2, y1, y2, col, ancol, xt, yt, bru=0;


    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(ss, ss, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;
	    /*SDL_SetWindowResizable(window, SDL_TRUE);*/
	    SDL_SetWindowTitle(window, "CEQ");

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

		
		// LOGO
		if(ceqlogo){
			SDL_SetRenderDrawColor(renderer, 167, 123, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLine(renderer, ss/2-r*0.5, ss/2-r*0.5, ss/2-r*0.5, ss/2+r*0.5);
			SDL_RenderDrawLine(renderer, ss/2-r*0.1, ss/2-r*0.5, ss/2-r*0.1, ss/2+r*0.5);
			SDL_RenderDrawLine(renderer, ss/2+r*0.3, ss/2-r*0.5, ss/2+r*0.3, ss/2+r*0.5);
			SDL_RenderDrawLine(renderer, ss/2+r*0.5, ss/2-r*0.5, ss/2+r*0.5, ss/2+r*0.5);

			SDL_RenderDrawLine(renderer, ss/2-r*0.5, ss/2-r*0.5, ss/2-r*0.3, ss/2-r*0.5);
			SDL_RenderDrawLine(renderer, ss/2-r*0.5, ss/2+r*0.5, ss/2-r*0.3, ss/2+r*0.5);

			SDL_RenderDrawLine(renderer, ss/2-r*0.1, ss/2-r*0.5, ss/2+r*0.1, ss/2-r*0.5);
			SDL_RenderDrawLine(renderer, ss/2-r*0.1, ss/2, ss/2+r*0.1, ss/2);
			SDL_RenderDrawLine(renderer, ss/2-r*0.1, ss/2+r*0.5, ss/2+r*0.1, ss/2+r*0.5);

			SDL_RenderDrawLine(renderer, ss/2+r*0.3, ss/2-r*0.5, ss/2+r*0.5, ss/2-r*0.5);
			SDL_RenderDrawLine(renderer, ss/2+r*0.3, ss/2+r*0.5, ss/2+r*0.5, ss/2+r*0.5);

			SDL_RenderDrawLine(renderer, ss/2+r*0.45, ss/2+r*0.25, ss/2+r*0.57, ss/2+r*0.5);
		}

		pa_simple_read(s, data, p*8, NULL);
		x1=ss/2,y1=ss/2-r,x2=ss/2,y2=0;
		for(int i=0; i<=p; i++){
			col=(255/p)*i; ancol=255-col;
			y1=ss/2-fny(r, ss/2-x1);
			y2=ss/2-fny(ss/2, ss/2-x2);

			if(i==p) x2=ss/2+(bs/2)/sqrt2, y2=ss/2-(bs/2)/sqrt2, x1=ss/2+r/sqrt2, y1=ss/2-r/sqrt2;
			
			xt=x2; yt=y2;
			//SDL_SetRenderDrawColor(renderer, (i*2)%256, (i*6)%256, (i*4)%256, SDL_ALPHA_OPAQUE);
			SDL_SetRenderDrawColor(renderer, 255, 255, 155-(155/p)*i, SDL_ALPHA_OPAQUE);
			prl(x1, y1, &x2, &y2, data[i]);
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

			SDL_SetRenderDrawColor(renderer, 255, col, 0, SDL_ALPHA_OPAQUE);
			x2=xt; y2=yt; prl(x1, y1, &x2, &y2, data[(int)(p+p-i)]);
			SDL_RenderDrawLine(renderer, bs-y1, bs-x1, bs-y2, bs-x2);
			
			SDL_SetRenderDrawColor(renderer, 255, 0, col, SDL_ALPHA_OPAQUE);
			x2=xt; y2=yt; prl(x1, y1, &x2, &y2, data[(int)(2*p)+i]);
			SDL_RenderDrawLine(renderer, bs-y1, x1, bs-y2, x2);
			
			SDL_SetRenderDrawColor(renderer, col, 0, 255, SDL_ALPHA_OPAQUE);
			x2=xt; y2=yt; prl(x1, y1, &x2, &y2, data[(int)((3*p)+p-i)]);
			SDL_RenderDrawLine(renderer, x1, bs-y1, x2, bs-y2);
			
			SDL_SetRenderDrawColor(renderer, 0, col, 255, SDL_ALPHA_OPAQUE);
			x2=xt; y2=yt; prl(x1, y1, &x2, &y2, data[(int)(4*p)+i]);
			SDL_RenderDrawLine(renderer, bs-x1, bs-y1, bs-x2, bs-y2);
			
			SDL_SetRenderDrawColor(renderer, 0, 255, col, SDL_ALPHA_OPAQUE);
			x2=xt; y2=yt; prl(x1, y1, &x2, &y2, data[(int)((5*p)+p-i)]);
			SDL_RenderDrawLine(renderer, y1, x1, y2, x2);
			
			SDL_SetRenderDrawColor(renderer, col/2, 255, col/2, SDL_ALPHA_OPAQUE);
			x2=xt; y2=yt; prl(x1, y1, &x2, &y2, data[(int)(6*p)+i]);
			SDL_RenderDrawLine(renderer, y1, bs-x1, y2, bs-x2);
			
			SDL_SetRenderDrawColor(renderer, 128+ancol/2, 255, 128+(17/p)*i, SDL_ALPHA_OPAQUE);
			x2=xt; y2=yt; prl(x1, y1, &x2, &y2, data[(int)((7*p)+p-i)-1]);
			SDL_RenderDrawLine(renderer, bs-x1, y1, bs-x2, y2);
			x2=xt; y2=yt;
			x2+=bst;x1+=st;
		}
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }/*else if (event.type == SDL_WINDOWEVENT) { 
			if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
		    		ss=bs=((event.window.data1<event.window.data2)? event.window.data1 : event.window.data2);
				printf("nigga\n");
		    	}
		    }*/
                }
		
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    
    pa_simple_free(s);
    SDL_Quit();
    return 0;
}

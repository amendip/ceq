#include <SDL2/SDL.h>
#include <stdio.h>
#include <pulse/simple.h> 

#define sqrt2 1.41421356237

char mod(signed char c){
if(c<0) return c*(-1); else return c;
}

int main(int argc, char* argv[])
{
float ss=500, p=320; int sampler8=16000; unsigned char col[2][3]={0}; col[1][1]=255;
if( argc > 1 ) {
for(int i=1; i<argc; i++){

	if(argv[i][0] == '-'){
		switch(argv[i][1]){
		case 'h': printf("\t-s\twindow size\n\t-a\tcolor for first channel\n\t-z\tbackground color\n\t-r\tsample rate\n\t-h\tdisplay this help and exit\n"); return 0;

		case 'a':
			sscanf(&argv[i][2], "%2hhx", &col[1][0]);
			sscanf(&argv[i][4], "%2hhx", &col[1][1]);
			sscanf(&argv[i][6], "%2hhx", &col[1][2]);
			break;
		case 'z':
			sscanf(&argv[i][2], "%2hhx", &col[0][0]);
			sscanf(&argv[i][4], "%2hhx", &col[0][1]);
			sscanf(&argv[i][6], "%2hhx", &col[0][2]);
			break;
		case 'r': sscanf(&argv[i][2], "%d", &sampler8); break;
		case 's': sscanf(&argv[i][2], "%f", &ss); break;
		}
	}
}
}

float st=ss/p;

pa_simple *s;
pa_sample_spec ssp;
ssp.format = PA_SAMPLE_U8;
ssp.channels = 2;
ssp.rate = sampler8;

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
		  "ceq",              // Stream description.
		  &ssp,               // Sample format.
		  NULL,               // Use default channel map
		  &bset,              // Buffering attributes.
		  NULL                // Ignore error code.
		  );

unsigned char data[(int)p];
 float x=0, y=0;
 int cpx, cpy;


    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(ss, ss, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;
	    SDL_SetWindowResizable(window, SDL_TRUE);
	    SDL_SetWindowTitle(window, "CEQ");
	    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, col[0][0], col[0][1], col[0][2], SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, col[1][0], col[1][1], col[1][2], 255);
		pa_simple_read(s, data, p, NULL);
		for(int i=0; i<p; i+=2){
			if(i!=p-1){
				SDL_RenderDrawPoint(renderer, (data[i]/255.0f)*ss, ss-(data[i+1]/255.0f)*ss);
			}
		}
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }else if (event.type == SDL_WINDOWEVENT) { 
			if(event.window.event == SDL_WINDOWEVENT_RESIZED){
		    		ss=((event.window.data1<event.window.data2)? event.window.data1 : event.window.data2);
				SDL_SetWindowSize(window, ss, ss);
		    	}
		    }
                }
		//printf("latency: %lu\n",pa_simple_get_latency(s, NULL));
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

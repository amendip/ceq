# ceq
Audio visualizer for pulseaudio using the SDL2 library.
## Flavours
### circular
![circ](https://raw.githubusercontent.com/amendip/ceq/main/scrots/circ.png)
### oscilloscope line
![lin](https://raw.githubusercontent.com/amendip/ceq/main/scrots/lin.png)
### vectroscope
![2d](https://raw.githubusercontent.com/amendip/ceq/main/scrots/2d.png)
## Compiling
Install SDL2 and pulseaudio if you haven't already.

Compile with:
```
gcc -lSDL2 -lpulse-simple ceq.c -o whatevernameyoudliketogivetoyourexecutable
```

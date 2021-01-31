# ceq
Audio visualizer for pulseaudio using the SDL2 library. Works in a single thread and is a bit stupid but I'm more likely to rewrite it using other libraries than I am to fix it.
## Flavours
### circular
![circ](https://raw.githubusercontent.com/amendip/ceq/main/scrots/circ.png)
### oscilloscope
![lin](https://raw.githubusercontent.com/amendip/ceq/main/scrots/lin.png)
### vectorscope
![2d](https://raw.githubusercontent.com/amendip/ceq/main/scrots/2d.png)
## Compiling
There are precompiled binaries included but if you'd like you can compile from the source. Install SDL2 and pulseaudio if you haven't already.

Compile with:
```
gcc -lSDL2 -lpulse-simple ceq.c -o whatevernameyoudliketogivetoyourexecutable
```

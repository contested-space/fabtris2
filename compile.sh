CFLAGS="$CFLAGS -Wall -Wextra"
CFLAGS="$CFLAGS -Wundef"
CFLAGS="$CFLAGS -Wcast-align"
CFLAGS="$CFLAGS -Wwrite-strings"
CFLAGS="$CFLAGS -Wunreachable-code"
CFLAGS="$CFLAGS -Wformat=2"
CFLAGS="$CFLAGS -Wswitch-enum"
CFLAGS="$CFLAGS -Winit-self"
CFLAGS="$CFLAGS -Wno-implicit-fallthrough"

CFLAGS="$CFLAGS $(sdl2-config --cflags)"

gcc src/fabtris2.c src/fabtrimino.c src/square.c src/matrix.c src/next.c src/grid.c src/hold.c $CFLAGS -I./src -lSDL2 -lSDL2_image -g -std=gnu11 -o fabtris2

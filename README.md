# fabtris2

A clone of a popular block stacking game.

## Installation

### Install dependencies

#### On Ubuntu

```shell
sudo apt-get install -y gcc libsdl2-dev libsdl2-image-dev
```

#### On Fedora

```shell
sudo dnf install -y gcc SDL2-devel SDL2-image-devel
```

#### On Alpine

```shell
sudo apk add -U alpine-sdk sdl2-dev sdl2_image-dev bash
```

### Compile

You can build the way the developer intended.

```shell
./compile.sh
```

Or you can build it with fancy tools.

```shell
meson setup build && ninja -C build
```

## Usage

```shell
./fabtris2       # if you built with the shell script

# or

./build/fabtris2 # if you built with ninja
```

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## Credits

[@contested-space][flf] — Fabien Lamarche-Filion

[flf]: https://github.com/contested-space

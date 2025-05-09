# psychic-potato

### a 3d render built in c using only stdlib + sdl(for multiple system suppourt)

## here's how it looks.
![pic](/assets/1.png)


### Install nix then:
```
    cd psychic-potato
    nix develop

```
### Using bear in nixos to define paths:

```
 bear -- gcc -I/nix/store/pz6jgrfqyl2il2785a75vmwdp9vypv0j-glibc-2.40-66-dev/include -c -o main.o main.c
```
```compile_json
      "/nix/store/f0m6caffiykyvsjim9376a3hx2yj2ghj-gcc-wrapper-14.2.1.20250322/bin/gcc",
      "-I/nix/store/pz6jgrfqyl2il2785a75vmwdp9vypv0j-glibc-2.40-66-dev/include",
      "-I/nix/store/vjrb1ayf85l4b7xr21fi9kkdgxrk4fnc-sdl2-compat-2.32.52-dev/include",
```|
   |
   --> **Specific to me , found your own self **

choose nix files wisely...

# A learning project ..............

# 🥔 psychic-potato

A lightweight 3D software renderer built in **C** using only `stdlib` and `SDL2`.
Designed to be cross-platform, educational, and deeply hackable.

---

## 🌀 Render Previews
  <img src="/assets/1.png" width="300"/>
  <img src="/assets/2.png" width="300"/>
  <img src="/assets/3.png" width="300"/>
  <img src="/assets/4.png" width="300"/>
  <img src="/assets/5.png" width="300"/>
  
---

## ⚙️ Getting Started with Nix

Install [Nix](https://nixos.org/download.html), then enter the dev environment:

```bash
git clone https://your-repo-url/psychic-potato
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

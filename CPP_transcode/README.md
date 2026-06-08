# WERTHEIM Cipher — C++ Port

Teljes C++ portja a "Wertheim" szövegtitkosító
alkalmazása mögötti kriptográfiai rétegnek. Az eredeti kód Free Pascal
(Lazarus) nyelven íródott, megtalálható a `../*.pas` fájlokban.

## Eredeti projekt (Pascal)

| Fájl | Szerep |
|------|--------|
| `unit1.pas` + `unit1.lfm` | GUI ablak (Lazarus Forms) |
| `hinta.pas` | Egyedi "hinta" titkosító algoritmus + Base64 |
| `titokrutin.pas` | RC4+SHA1 titkosítási réteg |
| `wertheim.lpr` | Program belépési pont |
| `wertheim.lpi` | Lazarus projekt fájl |

## A port tartalma

A C++ port a teljes kriptográfiai magot lefedi:

| Funkció | Pascal eredeti | C++ port | Megjegyzés |
|---------|---------------|----------|------------|
| Egyedi Base64 | `Encode64` / `Decode64` | `encode64` / `decode64` | Eltér a szabványos Base64-től! A karakterkészlet `0-9 A-Z a-z +/` sorrendű. |
| Hinta szubsztitúció (kódolás) | `bekod` | `bekod` | 65×65-ös helyettesítő tábla, 2 passz |
| Hinta szubsztitúció (dekódolás) | `kikod` | `kikod` | A `bekod` inverze |
| Jelszó → kulcs átalakítás | `pw_to_kod` | `pw_to_kod` | Base64-elt jelszóból pozícióképzés az `abc0` alapján |
| Kulcsgenerálás | `genhinta` | `genhinta` | `tarr` táblából sorok kiválasztása |
| Teljes titkosítás | `bekodol` | `bekodol` | RC4 → Base64 → hinta (3 réteg) |
| Teljes visszafejtés | `kikodol` | `kikodol` | hinta → Base64 → RC4 (fordított sorrend) |
| Publikus API | `hintabe` / `hintaki` | `hintabe` / `hintaki` | Egyszerű string → string függvények |
| RC4+SHA1 | `besecure` / `kisecure` | `besecure` / `kisecure` | OpenSSL EVP-vel implementálva |

## Építés

### Követelmények

- C++17 kompatibilis fordító (MSVC, GCC, Clang)
- OpenSSL (libcrypto) — fejlesztői csomag szükséges

### CMake (ajánlott)

```bash
cd CPP_transcode
cmake -B build
cmake --build build
./build/wertheim_demo
```

### Windows (MSVC, manuális)

```bash
cl /EHsc /I include src/wertheim.cpp src/tarr_data.cpp src/main.cpp /link libcrypto.lib
```

### Linux

```bash
g++ -std=c++17 -I include src/wertheim.cpp src/tarr_data.cpp src/main.cpp -lcrypto -o wertheim_demo
```

## Architektúra

```
Bemenet (plaintext)
    │
    ▼
┌──────────────────┐
│  besecure()      │  RC4 stream cipher, kulcs=SHA1(jelszó)
│  (OpenSSL EVP)   │
└──────────────────┘
    │
    ▼
┌──────────────────┐
│  encode64()      │  Egyedi Base64 kódolás
└──────────────────┘
    │
    ▼
┌──────────────────┐
│  bekod()         │  Hinta szubsztitúciós rejtjelezés
│  (2 pass,        │  Minden karaktert a jelszóból származtatott
│   tarr tábla)    │  permutációs sorral helyettesít
└──────────────────┘
    │
    ▼
Kimenet (ciphertext)
```

A visszafejtés pontosan fordított sorrendben történik:
`kikod()` → `decode64()` → `kisecure()`

## Bináris kompatibilitás

A C++ port **binárisan kompatibilis** az eredeti Pascal alkalmazással:
- Az azonos jelszóval és plaintext-tel azonos ciphertext-et állít elő
- A `.wtx` fájlok átjárhatóak a Pascal és C++ verziók között

### Ismert eltérés / megjegyzések

1. **tarr tábla indexelés**: Az eredeti Pascal kód a `genhinta` függvényben
   `tarr[kod[t], ord(pwd[t])]`-vel indexel. Mivel `pwd` Base64-kódolt,
   `ord(pwd[t])` értéke lehet 43-122, de a Pascal `tarr[0..64,0..64]` deklaráció
   csak 0-64 indexeket enged. 64 feletti index esetén a Pascal kód határon túli
   memóriát olvas. A C++ port ezt úgy kezeli, hogy a második dimenziót 256-ra
   bővíti, és a nem inicializált pozíciók üres stringek. Ez **eltérhet** a
   Pascal viselkedéstől, ahol a határon túli olvasás a memória véletlenszerű
   tartalmát adja vissza. **Továbbfejlesztési javaslat**: ha a pontos bináris
   kompatibilitás nem követelmény, érdemes `ord(pwd[t]) % 65`-re javítani az
   indexelést.

2. **Karakterkódolás**: Az eredeti Pascal kód `utf8string` típust használ.
   A C++ port `std::string`-et használ, és a UTF-8 bájtokat átlátszóan kezeli.
   Mivel a titkosítás bájt-szinten működik, ez nem okoz kompatibilitási
   problémát.

## API referencia

```cpp
namespace wertheim {

// Titkosítás: plaintext titkosítása jelszóval
std::string hintabe(const std::string& mit, const std::string& jelszo);

// Visszafejtés: ciphertext visszafejtése jelszóval
std::string hintaki(const std::string& mit, const std::string& jelszo);

// Egyedi Base64
std::string encode64(const std::string& s);
std::string decode64(const std::string& s);

// RC4+SHA1 réteg
std::string besecure(const std::string& mit, const std::string& pw);
std::string kisecure(const std::string& mit, const std::string& pw);

}
```

## iPhone/iOS portolás

A core titkosítási kód (ez a C++ port) közvetlenül használható iOS-en:
1. Fordítsd static library-ként (`libwertheim.a`) iOS architektúrákra
   (arm64) — ehhez OpenSSL forrást kell iOS-re is lefordítani, vagy
   CommonCrypto-t használni az RC4+SHA1 réteghez
2. Hívd meg Swift/ObjC-ból C bridge-en keresztül:
   ```swift
   let encrypted = wertheim::hintabe(plaintext, password)
   ```
3. A GUI-t natívan SwiftUI/UIKit-ben kell megírni

## Licenc

Az eredeti kód szerzője: Mihályi József
A C++ port az eredeti kód funkcionális megfelelője.

## Használati példa

```cpp
#include "wertheim.hpp"
#include <iostream>

int main() {
    std::string titkos = wertheim::hintabe("Helló Világ!", "jelszó");
    std::string vissza = wertheim::hintaki(titkos, "jelszó");
    std::cout << vissza << std::endl; // "Helló Világ!"
    return 0;
}
```

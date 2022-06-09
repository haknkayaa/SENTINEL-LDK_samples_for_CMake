
# SENTINEL LDK SAMPLES for CMAKE
[English](./README.md), [Turkish](./README.tr.md)

Bu çalışma lisans koruması için geliştirilen Thales firmasının SENTINEL LDK örneklerini
C ve C++ yazılım diliyle CMake tabanlı revizyonlarını içerir.

## Neden?
Orjinal örnekler gcc veya makefile dosyalarıyla çok ilkel bir şekilde
yapılmıştır. Modern Qt tabanlı projemde CMake kullandığımdan dolayı
SENTINEL LDK API kullanımını CMake için tekrardan
revize etmem gerekti.

SENTINEL LDK ürünleri için uzmanlığımın olmadığını ve yaşanacak herhangi bir
sorun için sorumluluk kabul etmediğimi beyan ederim. Amatör iseniz daha fazla
profesyonel destek için lütfen firma ile iletişime geçiniz.

## Test edilen geliştirme ortamı:
- Ubuntu 20.04 LTS
- cmake version 3.16.3
- gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
- g++ (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
- SENTINEL LDK 8.42 for Linux

## Nasıl kullanılır?

Terminal açın ve şu şekilde yapın:

- İlgili klasöre gidin.

  `cd ./Samples/Runtime/C-CMake/`

- Build için bir klasör oluşturun ve içine gidin.

  `mkdir build && cd build`

- CMake ile build dosyası oluşturun işlemi yapın.

  `cmake ..`

- Build için `make` komutunu çalıştırın.

  `make -jN (N is core count)`



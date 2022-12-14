# Компајлирање
Задатак је могуће компајлирати:

- директно из командне линије или
- коришћењем cmake алата

У првом случају, додатне опције компајлирања је потребно додати у формату ``-опција``. У другом случају је потребно
додати опцију у линију ``set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MPI_C_COMPILE_FLAGS}")`` у ``CMakeLists.txt`` датотеци.

## Компајлирање из терминала
Уколико желите да користите изгенерисане улазне податке у ``hdf5`` формату неопходно је имати инсталирану подршку за овај
формат података. На Убунту оперативним системина, hdf5 пакет можете инсталирати покретањем следећих команди:
```sh
sudo apt install libhdf5-dev 
```
Затим се позицинонирати у коренски директоријум задатка и покренути:
```sh
h5cc utils/*.h utils/*.c main.c
```
Уколико не желите да користите улазне податке у hdf5 формату, задатак можетекомпајлирати на следећи начин:
```sh
gcc main.c -DDISABLE_HDF5
```
Уколико искључите подршку за учитавање генерисаних улазних података, потребно је да 
модификујете изворни код тако да на неки други начин обезбедите учитавање улазних података, али је обавезно користити
вредности дате у датотекама m3x3.h5 и m5x5.h5. Да би се постигло множење две матрице, сваку од претходно наведених
матрица помножити саму собом, али решење направити тако да буде независно од података који се налазе
у матрицама (дакле не треба да буде специфичности које користе чињеницу да се множе две исте матрице).

## Компајлирање ``cmake`` алатом
Уколико немате инсталиране ``cmake`` и ``make`` пакете нећете моћи овако да компајлирате задатак.

Инсталација на Убунту оперативним системима:
```sh
sudo apt install cmake make -y
```
Након успешне инсталације, потребно је да се позиционирате у коренски директоријум задатка и покренете следеће наредбе:

```sh
mkdir build
cd build
cmake ..
make -j4
```
Уколико хоћете да искомпајлирате програм без подршке за ``hdf5`` пакет, линију ``cmake ..`` треба заменити са 
``cmake -DENABLE_HDF5=OFF ..``. Уколико искључите подршку за учитавање генерисаних улазних података, потребно је да 
модификујете изворни код тако да на неки други начин обезбедите учитавање улазних података.

# Покретање програма
Позиционирати се у директоријум у којем се налази извршна датотека и покренути ``mpiexec -np <N> ./a.out``, или други 
назив уколико је другачије специфицирано током компајлирања. ``<N>`` заменити конкретним бројем који представља број 
процеса које хоћете да створите.

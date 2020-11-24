# IECAI Testing Branch - November 2020
### November 2020 - bsc_dat_ent.h (OLD, COMBINED JSON)

#### :heavy_check_mark: 8 November 2020, Ubuntu 20
|     Build item      |  Current status  |
|---------------------|---------------|
|`meson wrap install jsoncpp`|  Operation successful :heavy_check_mark: |
| `cmake` and `make`  jsoncpp 1.8.4   |   Compile Passed  :heavy_check_mark:  |
| `dist/json/json.h` inclusion| Seems to be passing :heavy_check_mark:  |
| `base_entity.h` inclusion | Passing :heavy_check_mark: |
| `g++ -ljsoncpp` compile | Passing :heavy_check_mark: |

###### :memo:  13:27 MST - found potential workaround
Ran `amalgamate.py` after compilation and copied `dist` folder. Testing soon.
###### :memo: 14:24 MST - fixing non-jsoncpp errors
Currently on compile-run four. Comments being added for :ledger: running log.
###### :memo: :bangbang: 15:22 MST - fixed all but one non-jsoncpp error
All other issues resolved, but final is not making much sense - see issue A2 (line 162). Compile-run 7.
###### :memo: :shipit: 16:23 MST - fixed all non-jsoncpp errors!
The issue was, oddly, that the point class wanted to be set within the pre-function declaration area.
###### :memo: 10 Nov 2020  11:27 MST - creating sample CMAKE script
Will be preparing a 'cmake' script for preparing the project. Still unsure about linking errors.
###### :memo: 24 Nov 2020 14:07 MST - Found Issue with jsoncpp
It was because, foolishly, I didn't `make install`. Found that out when working with `libsdl`
#### Compile `jsoncpp` Script:
```
meson wrap install jsoncpp
meson subprojects update
meson subprojects download
cd subprojects/jsoncpp-1.8.4
cmake
make
cd seutp  # note: jsoncpp-side typo
ninja
cd ..
python3 ./amalgamate.py
cp -r ./dist ./{Location}
```
#### Lib Path:
```
/lib:/usr/lib:/usr/local/lib:/usr/local/include
```
#### Compile Command:
```
sudo g++ -ljsoncpp error-finding.cpp -o out.o
```

##### To-Do List for November 2020 branch, part 1:
- [x] Produce commands to include `jsoncpp` in compiler
- [x] Run tests on `error-finding.cpp` and log results
- [x] Produce basic compile script
- [x] Find why 2,000 `ld` errors come up - Was failing to use the proper libraries.


### November 2020 - libSDL2 operations
| Build Item | Status |
|---------|--|
|`make` and `make install` SDL2 | Passing :heavy_check_mark: |
|  Including `/usr/local/include/SDL.h` ... | Passing :heavy_check_mark: |
| `g++ -lSDL2 $((sdl2-config --cflags --libs))` | Passing :heavy_check_mark: |
| Execution | Failing :x: |

Failing notes: `libGL error` missing `raedonsi` (need to fix search path?)

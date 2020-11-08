# IECAI Testing Branch 1
### November 2020 Application

|  Target System  | Status |
|-----------------|--------|
| :penguin: Ubuntu 20.04.1 LTS | :warning: :repeat: |
| :computer: Windows 10 | :x: :no_entry: |



#### :warning: 8 November 2020 @ 12:14 MST
|     Build item      |  Current status  |
|---------------------|---------------|
|`meson wrap install jsoncpp`|  Operation successful :heavy_check_mark: |
| `cmake` and `make`  jsoncpp 1.8.4   |   Compile Passed  :heavy_check_mark:  |
|`gcc` inclusion jsoncpp| 1100+ lines of errors :x: :bangbang:  |
|`gcc error-finding.cpp`| ONE MORE ERROR IN `base_entity.h` :warning:|

###### :memo:  13:27 MST - found potential workaround
Ran `amalgamate.py` after compilation and copied 'dist' folder. Testing soon.
###### :memo: 14:24 MST - fixing non-jsoncpp errors
Currently on compile-run four. Comments being added for :ledger: running log.
###### :memo: :bangbang: 15:22 MST - fixed all but one non-jsoncpp error
All other issues resolved, but final is not making much sense - see issue A2 (line 162). 

### Compile commands:
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

##### To-Do List for November 2020 branch, part 1:
- [x] Produce commands to include `jsoncpp` in compiler
- [x] Run tests on `error-finding.cpp` and log results
- [x] Produce basic compile script

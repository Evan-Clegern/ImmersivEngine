# IECAI Testing Branch 1
## November 2020 Application

|  Target System  | Status |
|-----------------|--------|
| :penguin: Ubuntu 20.04.1 LTS | :x: :repeat: |
| :computer: Windows 10 | :x: :no_entry: |



#### :x: 8 November 2020 @ 12:14 MST
|     Build item      |  Current status  |
|---------------------|---------------|
|`meson wrap install jsoncpp`|  Operation successful :heavy_check_mark: |
| `cmake` and `make`  jsoncpp 1.8.4   |   Compile Passed  :heavy_check_mark:  |
|`gcc` inclusion jsoncpp| Unknown :interrobang:  |
|`gcc error-finding.cpp`| Can't use jsoncpp :x:|

##### Test file information
```
fatal error: json/json.h: No such file or directory
#include <json/json.h>
```
:bangbang: Working to fix as of 12:35 MST
##### To-Do List for November 2020 branch, part 1:
- [ ] Produce commands to include `jsoncpp` in compiler
- [ ] Run tests on `error-finding.cpp` and log results
- [ ] Produce basic compile script

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
|`gcc` inclusion jsoncpp| Amalgamated Success :heavy_check_mark:  |
|`gcc error-finding.cpp`| *Multiple* Point-Operation Problems. :x:|
##### OLD DATA
###### Test file information
```
fatal error: json/json.h: No such file or directory
#include <json/json.h>
```
:bangbang: Working to fix as of 12:35 MST
###### 13:27 MST - found potential workaround
:memo: Ran `amalgamate.py` after compilation and copied 'dist' folder. Testing soon.

##### To-Do List for November 2020 branch, part 1:
- [x] Produce commands to include `jsoncpp` in compiler
- [ ] Run tests on `error-finding.cpp` and log results
- [ ] Produce basic compile script

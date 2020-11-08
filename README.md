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
|`gcc` inclusion jsoncpp| 1100+ lines of errors :x: :bangbang:  |
|`gcc error-finding.cpp`| ONE MORE ERROR. :warning:|

###### :memo:  13:27 MST - found potential workaround
Ran `amalgamate.py` after compilation and copied 'dist' folder. Testing soon.
###### :memo: 14:24 MST - fixing non-jsoncpp errors
Currently on compile-run four. Comments being added for :ledger: running log.
##### To-Do List for November 2020 branch, part 1:
- [x] Produce commands to include `jsoncpp` in compiler
- [x] Run tests on `error-finding.cpp` and log results
- [ ] Produce basic compile script

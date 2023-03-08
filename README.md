# destructive_func
C++ Self-destructing functions testing

## examples

```cpp
#include "destructive_func.hpp"
void myFunction1(int num) {
    if ((num % 10) < 5)
        goto myFunction1_Exit;
    
    for (int i < 0; i < num; i++)
        std::cout << num << std::endl;
    
    // we are required to use labels to ensure correct returning procedure
myFunction1_Exit:
    get_eip();
}
int main() {
    using fntype = void(*)(int);
    destructive_func<fntype> fn1(myFunction1);
    fn1(time(0)); // zeroes myFunction1 after call
    return 0;
}
```

## notes and warnings
- breaks when the function declares STL classes sometimes
- (almost) traceless, as the deleter itself does not get destroyed
- the deleter leaves out the last few bytes of the function
  - The actual ret instruction, sometimes push instruction
- more testing required, optimizations may or may not break it
  - Tested with /O2

## todo
- fix above issues
- c implementation for driver code maybe

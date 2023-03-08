#include <iostream>
#include "destructive_func.hpp"

void asdf(const char* thestring) {
    if ((time(0) % 10) < 5)
        goto END;

    printf(thestring);
    for (int i = 0; i < 3; i++)
        printf("%i\n", i);

    // using a label ensures that get_eip will be at the end of the function
    // otherwise, the return's eip will not span the entire function
END:
    get_eip();
}

int main() {
    using fntype = void(*)(const char*);
    destructive_func<fntype> obj(asdf);
    obj.preserving_call("I love bananas!\n"); // call without destroying

    std::cout << "Start: 0x" << std::hex << &asdf << std::endl;
    std::cout << "End:   0x" << std::hex << reinterpret_cast<void*>(obj.exit_eip) << std::endl;

    std::cout << std::dec << "size: " << obj.exit_eip - reinterpret_cast<unsigned long>(asdf) << std::endl;

    //__debugbreak();

    obj("I hate bananas!\n"); // destroy after call

    //__debugbreak();

    obj("I hate bananas!\n");

    return 0;
}

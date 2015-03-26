#include <bitset>
#include <iostream>



template <size_t N>
std::bitset<N> increment ( std::bitset<N> bit) {
    for ( size_t i = 0; i < N; ++i ) {
	bit[i] = bit[i] ^ 1;
        if ( bit[i] == 0 ) {  //No carry
            bit[i] = 0;
            break;
            }
        //bit[i] = 0;  //Carry 
        }
    return bit;
}

int main () {
    std::bitset<32> foo;
	foo[0] = 1;
    std::cout << foo << "to long is: " << foo.to_ulong () << ' ';
    foo = increment ( foo );
    std::cout << foo << "to long is: " << foo.to_ulong () << ' ';
    foo = increment ( foo );
    std::cout << foo.to_ulong () << ' ';
    foo = increment ( foo );
    std::cout << foo.to_ulong () << std::endl;

    return 0;
}

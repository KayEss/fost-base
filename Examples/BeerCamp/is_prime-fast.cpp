#include <cmath>

typedef unsigned int integer_type;

// A simple prime number test which is fairly efficient
bool is_prime(integer_type v) {
    if (v % 2 == 0)
        return v == 2;
    else {
        integer_type u = integer_type(std::sqrt(double(v)));
        for (integer_type i = 3; i <= u; i += 2)
            if (v % i == 0) return false;
        return true;
    }
}

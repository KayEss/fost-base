function(prime name src1 src2)
    add_executable(${name} ${src1} ${src2})
    target_link_libraries(${name} fost-cli)
endfunction()

prime(primes-single is_prime-fast.cpp primes-single.cpp)
prime(primes-single-slow is_prime-slow.cpp primes-single.cpp)

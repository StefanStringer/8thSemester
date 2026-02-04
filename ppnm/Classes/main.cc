#include<iostream>
#include<vector>
#include<cstdio>
#include"hello.h"

int main(){
    hello();
    double a = 1.0;
    double b = a;
    if(a==b) {std::printf"a==b\n";}
    else {std::printf"a!=b\n";}
    std::vector<double> v {1,2,3};
    for(size_t i=0;i<v.size();i++)std::printf("%g ",v[i]);
    std::printf("\n");
    for(auto vi : v) std::printf("%g ",vi); //can say auto or double, it is the same
    std::printf("\n");
    auto u = v;
    auto& w=v; // placing & is a reference
return 0;
}
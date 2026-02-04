#include<iostream>
#include<vector>
#include<cstdio>
#include"hello.h"

int main(){
    hello();
    double a = 1.0;
    double b = a;
    std::vector<double> v {1,2,3};
    auto u = v;
    auto& w=v; // placing & is a reference
return 0;
}
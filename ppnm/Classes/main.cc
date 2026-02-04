#include<iostream>
#include<vector>
#include<cstdio>
//#include<hello.h>

int main(){
   // hello();
    double a = 1.0;
    double b = a;

    if(a==b) {
        std::cout << "a==b \n" << std::endl;
    }
    else {
        std::cout << "a!=b\n" << std::endl;
    }
    std::vector<double> v {1,2,3};
    for(size_t i=0;i<v.size();i++) {
    std::cout << ("\n") << std::endl;
    }
    //for(auto vi : v) std::cout << ("%g ",vi) << std::endl; //can say auto or double, it is the same
    //std::cout << ("\n") << std::endl;
    //for(auto vi : v) vi=6;
    //for(auto vi : v) std::cout << ("%g ",vi) << std::endl;
    // auto u = v;
    // auto& w=v; // placing & is a reference
return 0;
}
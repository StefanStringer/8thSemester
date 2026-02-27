#include <iostream>
#include <iomanip>
#include <vector>
#include "vec.h"  
//#include<hello.h>

int main(){
   // hello();
    double a = 1.0;
    double b = a;

    if(a==b) {std::cout << "a==b \n" << std::endl;}
    else {std::cout << "a!=b\n" << std::endl;}
    std::vector<double> v {1,2,3};
    for(size_t i=0;i<v.size();i++) {std::cout << ("\n") << std::endl;}
    //for(auto vi : v) std::cout << ("%g ",vi) << std::endl; //can say auto or double, it is the same
    //std::cout << ("\n") << std::endl;
    //for(auto vi : v) vi=6;
    //for(auto vi : v) std::cout << ("%g ",vi) << std::endl;
    // auto u = v;
    // auto& w=v; // placing & is a reference
    size_t i=0;
    while(i<v.size()){
        std::cout << v[i] << std::endl;
        i+=1;
    }
    i=0;
    do{std::cout << v[i] << std::endl;
    i+=1;
    }while(i<v.size());
    std::cout << "\n" << std::endl;

    pp::vec j{1,2,3};
    j.x = 6;
    std::cout << j.x <<" "<< j.y <<" "<< j.z << "\n";
    // b=std::ref(j); // fuck ????

return 0;
}
// vec.h
#pragma once

#include <cstddef>               // for size_t if you need it
#include <string> 
// #include<string>
// #include<iostream>
// #include<cstdio>
namespace pp{
	struct vec{
		double x,y,z;
		//ctors
		vec(double a,double b,double c){ // parm. ctor
			std::cout << "paramerterised constructor called\n" << std::endl;
			x=a; y=b; z=c;
		}
		vec() : vec(0,0,0) { // default ctor calls parm ctor
			std::cout << "default constructor called\n" << std::endl;
			}
		vec(const vec&)=default; // copy ctor: vec a =b;
		vec(vec&&)=default; // move ctor: vec a = b+c; 
		
		//dtor
		~vec(){
			std::cout << "destructor called\n" << std::endl;
		}

		//assignments
		vec& operator=(const vec&); // copy= : a=b;
		vec& operator=(vec&&); // move= : a=b+c;
		
		// member operatos
		vec& operator+=(double);
		vec& operator-=(double);
		vec& operator*=(double);
		vec& operator/=(double);

		void print(const std::string& s ="");

		//steam output (friend is non memeber function, no access or instance (so no self))
		friend std::ostream& operator<<(std::ostream&, const vec&);
};
// non-members operators
vec operator-(const vec&);
vec operator+(const vec&,const vec&);
vec operator-(const vec&,const vec&);
vec operator*(const vec&,double);
vec operator*(double,const vec&);
vec operator/(const vec&,double);

//approximare equality
bool operator==(const vec&,const vec&);
}

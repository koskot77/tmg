#include <iostream>
#include <stdlib.h>
#include <math.h>
// Write a program that converts a number to a print form (like check writer)
// Example: input: 561 => output: Five Hundred Sixty One 
//          input: 12340982 => Output: Twelve million three hundred forty thousand nine hundred eighty two 
using namespace std;

const char* one(unsigned int a){
    const char *digit[10] = {0, "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    return digit[a%10]; 
}

const char* teen(unsigned int a){
    const char *tenish[10] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
    if( a >= 10 && a < 20 ) return tenish[a-10];
    return 0;
}

const char* ty(unsigned int a){
    const char *tens[10]  = {"ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
    if( a<10 || a>99 ) return 0;
    return tens[(a/10)-1];
}

string upTo100(unsigned int a){
    string retval;
    if( a>100 || a==0 ) return retval;

    if( a<10 ){
        retval = one(a);
        return retval;
    }
    if( a >= 10 && a < 20 ){
        retval = teen(a);
        return retval;
    }
    retval = ty(a);
    const char *low = one(a);
    if( low )
        retval.append(" ").append(low);

    return retval;
}

string upTo1000(unsigned int a){
    string retval;
    if( a>1000 || a==0 ) return retval;

    if( a<100 ) return upTo100(a);

    retval.append( one(a/100) ).append(" hundred");
    if( a%100 )
        retval.append(" ").append( upTo100(a%100) );

    return retval;
}

int main(int argc, char *argv[]){
    int a = strtol(argv[1],NULL,0);

    string result;

    if( a==0 ){
        result = "zero";
        cout<<result<<endl;
        return 0;
    }

    if( a<1000 ){
        result = upTo1000(a);
        cout<<result<<endl;
        return 0;
    }

    if(log10(a)>=12){
        result = "A rather big number";
        cout<<result<<endl;
        return 0;
    }

    for(unsigned int order3=log10(a)/3; order3>0; order3--){

        const char *name[4] = {"", "thousand", "million", "billion"};
        unsigned long lut[4]= {1,   1000,       1000000,   1000000000};

        if( result.length() ) result.append(" ");

        if( (a/lut[order3])%1000 ){
            result.append( upTo1000( (a/lut[order3])%1000 ) );
            result.append(" ").append( name[order3] );
        }

    }

    if( a%1000 )
        result.append(" ").append( upTo1000(a%1000) );

    cout<<result<<endl;
    return 0;
}

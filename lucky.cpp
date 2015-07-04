#include <iostream>
#include <string.h>
using namespace std;

// Compile: g++ -Wall -o lucky lucky.cpp

// "Bus tickets are numbered with a 6 digits number from 000000 to 999999.
//  Write a program that counts number of all 'lucky' bus tickets.
//  A Ticket is considered to be 'lucky' if the sum of the first 3 digit equals the sum of last 3 digits."

int main(int argc, char *argv[]){

    // 
    const unsigned int maxSum = 9+9+9;
    unsigned int counts[maxSum+1];
    bzero(counts,sizeof(unsigned int)*maxSum);

    for(unsigned int triplet=0; triplet<1000; triplet++){
        // first, second and third digits of the 3-digit number
        unsigned int digit1 = (triplet/1)  % 10;
        unsigned int digit2 = (triplet/10) % 10;
        unsigned int digit3 = (triplet/100);
        // sum the thre digits
        counts[ digit1 + digit2 + digit3 ]++;
    }

    unsigned int nLucky = 0;
    for(unsigned int sum=0; sum <= maxSum; sum++)
        nLucky += counts[sum]*counts[sum];

    cout<<"Number of lucky tickets: "<<nLucky<<endl;

    int m=0;
    // cross-check:
    for(unsigned int n=0; n<1000000; n++){
        unsigned int digit1 = (n/1)  % 10;
        unsigned int digit2 = (n/10) % 10;
        unsigned int digit3 = (n/100)% 10;

        unsigned int digit4 = (n/1000)  % 10;
        unsigned int digit5 = (n/10000) % 10;
        unsigned int digit6 = (n/100000);
        if( digit1 + digit2 + digit3 == digit4 + digit5 + digit6 ) m++;
    }

    cout<<"Number of lucky tickets 2: "<<m<<endl;

    return 0;
}

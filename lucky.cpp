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
    unsigned int counts[maxSum];
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
    for(unsigned int sum=0; sum < maxSum; sum++)
        nLucky += counts[sum]*counts[sum];

    cout<<"Number of lucky tickets: "<<nLucky<<endl;

    return 0;
}

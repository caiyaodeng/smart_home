/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat 07 May 2016 10:41:47 AM EDT
 ************************************************************************/

#include<iostream>
#include "timex.h"
using namespace std;

int main () {

    unsigned char a[20];
    tx::get_Time_E8(a);
    cout << a << endl;
    return true;
}

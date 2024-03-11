#include <stdio.h>

// static int i;
// static int i=12;
// static int i;

int func(){
    static int i=55;
    return i=i+5;
}

int main(){
    
    // static int i=8;
    int x = func();
    int y = func();
    if(x )
    printf("%d %d",x,y);
    return 0 ;
}

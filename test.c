#include <stdio.h>

int getDoomsday(int year) {
    int anchorDay = (5 * ((year / 100) % 4) + 2) % 7;
    int doomsday = (year % 100 + year % 100 / 4 + anchorDay) % 7;
    return doomsday;
}

int main(int argc, char const *argv[])
{
    int d,m, y;
    d =13;
    m = 10;
    y =2002;
    

    int weekday  = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;

    printf("%d\n",weekday);
    return 0;
}

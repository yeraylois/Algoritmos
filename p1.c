#include <sys/time.h>
#include <stdio.h>
#include <math.h>

/**
* Yeray Lois Sánchez  --> yeray.lois@udc.es
* Anxo Galdo Blasco   --> anxo.galdo.blasco@udc.es
* Sofía Oubiña Falcon --> sofia.oubina.falcon@udc.es
*/
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

/* obtiene la hora actual en microsegundos */
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int fib1(int n) {
    if (n < 2)
        return n;
    else
        return fib1(n - 1) + fib1(n - 2);
}

int fib2(int n) {
    int i, j, k;
    i = 1;
    j = 0;

    for (k = 1; k <= n; k++) {
        j = i + j;
        i = j - i;
    }
    return j;
}

int fib3(int n) {
    int i, j, k, h, t;
    i = 1;
    j = 0;
    k = 0;
    h = 1;
    while (n > 0) {
        if ((n % 2) != 0) {
            t = j * h;
            j = (i * h) + (j * k) + t;
            i = (i * k) + t;
        }
        t = h * h;
        h = (2 * k * h) + t;
        k = (k * k) + t;
        n = n / 2;
    }
    return j;
}

void test() {
    int i;
    printf("n\t\t\tFib1(n)\t\tFib2(n)\t\tFib3(n)\n");
    for (i = 0; i < 20; i++) {
        printf("%d\t\t\t%d\t\t\t%d\t\t\t%d\n", i, fib1(i), fib2(i), fib3(i));
    }
}

void tfib1() {
    int n, i, iter, lim;
    double t, t1,t2, x, y, z;
    lim = 500;
    iter = 0;
    printf("\t\t   n\t\tt\t t/(1.1^n)\tt/(1.62^n)\tt/(2^n)\n");

    for (n = 2; n <= 32; n *= 2) {
        t1 = microsegundos();
        fib1(n);
        t2 = microsegundos();
        t = t2 - t1;

        if (t < lim) {
            iter = 1000;
            t1 = microsegundos();
            for (i = 0; i < iter; i++) {
                fib1(n);
            }
            t2 = microsegundos();
            t = (t2 - t1) / iter;
            printf("(*)");
        }

        x = t / pow(1.1, n);
        y = t / pow(((1 + sqrt(5))/2),n);
        z = t / pow(2, n);

        if(iter == 0)
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
        else
            printf("\t%12d%15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
    }
}


void tfib2 () {
    int n, i, iter, lim;
    double t, t1,t2, x, y, z;
    lim = 500;
    iter = 0;
    printf("\t\t   n\t\tt\t  t/(n^0.8)\t t/(n)\t\tt/(nlog(n))\n");

    for (n = 1000; n <= 10000000; n *= 10) {
        t1 = microsegundos();
        fib2(n);
        t2 = microsegundos();
        t = t2 - t1;

        if (t < lim) {
            iter = 1000;
            t1 = microsegundos();
            for (i = 0; i < iter; i++) {
                fib2(n);
            }
            t2 = microsegundos();
            t = (t2 - t1) / iter;
            printf("(*)");
        }

        x = t / pow(n,0.8);
        y = t / n;
        z = t / n * log(n);

        if (iter == 0)
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
        else
            printf("\t%12d%15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
    }
}


void tfib3() {
    int n, i, iter, lim;
    double t1, t2, t, x, y, z;
    lim = 500;
    iter = 0;
    printf("\t\t   n\t\tt\tt/(sqrt(log(n))) t/(log(n))\tt/(n^0.5)\n");
    for (n = 1000; n <= 10000000; n *= 10) {
        t1 = microsegundos();
        fib3(n);
        t2 = microsegundos();
        t = t2 - t1;

        if (t < lim) {
            iter = 1000;
            t1 = microsegundos();
            for (i = 0; i < iter; i++) {
                fib3(n);
            }
            t2 = microsegundos();
            t = (t2 - t1) / iter;
            printf("(*)");
        }

        x = t / sqrt(log(n));
        y = t / log(n);
        z = t / pow(n, 0.5);

        if (iter == 0)
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
        else
            printf("\t%12d%15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
    }
}


int main() {
    printf("========================= fib1 =========================\n\n");
    tfib1();

    printf("\n========================= fib2 =========================\n\n");
    tfib2();

    printf("\n========================= fib3 =========================\n\n");
    tfib3();

    printf("\n\n(*) Medición repetida por tiempo inicial bajo el límite.");
}

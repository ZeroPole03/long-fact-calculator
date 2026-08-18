#include <stdio.h>
#include <time.h>


struct longfact{
    double coef; int n, exp;
};

double fact(int n){
    if(n <= 1){
        return 1;
    }else{
        double actual = n, anterior;
        for(int k = 1; k < n; k++){
            anterior = n - k;
            actual *= anterior;
        }
        return actual;
    }
}

struct longfact detLF(int n){
    struct longfact lf; lf.n = n;
    if(n <= 1){
        lf.exp = 0; lf.coef = 1;
        return lf;
    }else{
        double actual = n, anterior; int cont = 0;
        for(int k = 1; k < n; k++){
            anterior = n - k;
            actual *= anterior;
            while(actual >= 10){
                actual *= 0.1f;
                cont ++;
            };
        }
        lf.exp = cont;
        lf.coef = actual;
        return lf;
    }
}

void printfLF(struct longfact lf){
    printf("%d! ~= %lf x 10^%d\n", lf.n, lf.coef, lf.exp);
}

int main(){
    struct longfact factor;
    int N;
    clock_t t; 
    printf("de que numero desea obtener el factorial\n");
    scanf("%d", &N);
    t = clock();
    factor = detLF(N);
    //printf("%lf\n", fact(N));
    t = clock() - t;
    printfLF(factor);
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("fact() tardo %f segundos en ejecutarse \n", time_taken);
    return 0;
}
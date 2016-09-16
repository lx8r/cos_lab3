#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdbool.h>
#include <cmath>

#define NUMBER_IS_2_POW_K(x) ((!((x)&((x)-1)))&&((x)>1)) // =1 if x is pow(2, k), k=1,2, ...
#define Pi 3.1415926535897932384626433832795 //число Пи

using namespace std;

struct complex {
    double re;
    double im;
};

void print_complex(complex *a);
void fft(complex *mass, int N);
void sort(int *mass, int N);
void wtof(complex *arr, int N);
complex t_coeff(int k, int N);

int main(int argc, char** argv) {

    int N = 256;
    cin >> N;

    if(!NUMBER_IS_2_POW_K(N)) {
        cout << "ERROR: N=" << N << " isn&#39;t pow(2, k), k=1,2, ...\n";
        return false;
    }

    complex *arr;
    arr = new complex [N]; // Входной массив

    for(int i = 0; i < N; i++){
        (arr+i)->re = sin(2*Pi*2*i/8/N);
        (arr+i)->im = 0;
        print_complex(arr+i);
    }

    cout << "\n";
    fft(arr, N);

    for(int i = 0; i < N; i++){
        print_complex(arr+i);
    }

    wtof(arr, N);
    delete [] arr;
    return 0;
}

void fft(complex *mass, int N){
    int P = N;
    complex *mass1; // дополнительный массив для помощи в двоично-инверсной перестановке
    mass1 = new complex [N];

    //Двоично-инверсная перестановка-- -- -

    mass1->re = mass->re;
    mass1->im = mass->im;
    (mass1+N-1)->re = (mass+N-1)->re;
    (mass1+N-1)->im = (mass+N-1)->im;
    for(int i=1, j=1; i<N-1; i++){

        if(!(i%2)){
            (mass1+j)->re = (mass+i)->re;
            (mass1+j)->im = (mass+i)->im;
            j++;
        }
        else{
            (mass1+(N/2-1)+j)->re = (mass+i)->re;
            (mass1+(N/2-1)+j)->im = (mass+i)->im;
        }
    }

    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    if(N>2){
        fft(mass1, N/2);
        fft(mass1+N/2, N/2);
    }

    complex Wn;

    for(int i=0; i < N/2; i++){
        Wn = t_coeff(i,N);
        (mass+i)->re = (mass1+i)->re + Wn.re*(mass1+i+N/2)->re - Wn.im*(mass1+i+N/2)->im;
        (mass+i)->im = (mass1+i)->im + Wn.re*(mass1+i+N/2)->im + Wn.im*(mass1+i+N/2)->re;
        (mass+i+N/2)->re = (mass1+i)->re - (Wn.re*(mass1+i+N/2)->re - Wn.im*(mass1+i+N/2)->im);
        (mass+i+N/2)->im = (mass1+i)->im - (Wn.re*(mass1+i+N/2)->im + Wn.im*(mass1+i+N/2)->re);
    }

    delete [] mass1;
}

void sort(int *mass, int N){

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(*(mass+j) > *(mass+j+1)){
                swap(*(mass+j), *(mass+j+1));
            }
        }
    }

}

//Counting of turn coefficients

complex t_coeff(int k, int N){

    double arg = 2.0*Pi*(double)k/(double)N;
    complex W_coeff;

    W_coeff.re = cos(arg);
    W_coeff.im = -sin(arg);

    return W_coeff;

}

//printing complex number

void print_complex(complex *a){

    cout << round(a->re*10)/10 << "+" << round(a->im*10)/10 << "j\t";

}

//write to m-file

void wtof(complex *arr, int N){
    string fileName;

    cin >> fileName;
    fileName = "D:\\COS\\lab3\\fft\\" + fileName + ".m";
    ofstream fout(fileName);

    fout << "clear;\nN = " << N << ";\n\nRe = zeros(1,N);\nIm = zeros(1,N);\n\n";

    for(int i=0; i<N; i++){
        fout << "Re(1," << i+1 << ") = " << (arr+i)->re << ";\n";
        fout << "Im(1," << i+1 << ") = " << (arr+i)->im << ";\n\n";
    }

    fout << "figure(1)\nstem(Re),grid on\n\nfigure(2)\nstem(Im),grid on";
    fout.close();
}

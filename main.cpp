#include <iostream>
#include <fstream>
#include <cmath>


#define Pi 3.141592653589

using namespace std;


class Complex{
private:
    double re;
    double im;
public:
    Complex(){re=im=0;};
    Complex(double inRe){re = inRe;im = 0;};
    Complex(double inRe, double inIm) {re = inRe;im = inIm;};
    
    void print();
    double real(){return re;};
    double imag(){return im;};


    friend Complex operator+(Complex c1, Complex c2);
    friend Complex operator-(Complex c1, Complex c2);
    friend Complex operator*(Complex c1, Complex c2);
    friend Complex operator/(Complex c1, Complex c2);

};

void fft(Complex *mass, int N);
void writeToFile(Complex *arr, int N);
int toPow2(int n);

int main(int argc, char** argv) {
    int n = 256,  N = 256;
    cin >> n;
    N = toPow2(n);
    cout << N;


    Complex *arr = new Complex[N];

    for(int i = 0; i < n; i++){
        double func = sin(2*Pi*25*i/n)+7*sin(2*Pi*50*i/n)+5*sin(2*Pi*75*i/n);
        arr[i] = Complex(func);
        arr[i].print();

    }

    for(int i = n; i < N; i++){
        arr[i] = Complex();
        arr[i].print();
        cout << i;
    }

    fft(arr, N);

    for(int i = 0; i < N; i++){
        arr[i].print();
        cout << endl;
    }


    writeToFile(arr, N);
    delete [] arr;
    return 0;
}

int toPow2(int n){
    int i=0, N=2;

    while(n=n/2)
        i++;

    for(int j=0; j<i; j++)
        N *= 2;

    if(N<n)
        return N*2;
    else
        return N;
}

void fft(Complex *mass, int N){

    Complex *mass1 = new Complex [N];

    for(int i=0, j=0; i<N; i++){

        if(!(i%2))
            mass1[j++] = mass[i];
        else
            mass1[(N/2-1)+j] = mass[i];

    }

    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    if(N>2){
        fft(mass1, N/2);
        fft(mass1+N/2, N/2);
    }



    for(int i=0; i < N/2; i++){
        double arg = 2.0*Pi*(double)i/(double)N;
        Complex weightCoeff = Complex(cos(arg), -sin(arg));

        mass[i] = mass1[i] + mass1[N/2+i]*weightCoeff;
        mass[N/2+i] = mass1[i] - mass1[N/2 +i]*weightCoeff;
    }

    delete [] mass1;
}

void writeToFile(Complex *arr, int N){
    string fileName;

    cin >> fileName;
    fileName = "D:\\COS\\lab3\\fft\\" + fileName + ".m";
    ofstream fout(fileName);

    fout << "clear;\nN = " << N << ";\n\nRe = zeros(1,N);\nIm = zeros(1,N);\n\n";

    for(int i=0; i<N; i++){
        fout << "Re(1," << i+1 << ") = " << arr[i].real() << ";\n";
        fout << "Im(1," << i+1 << ") = " << arr[i].imag() << ";\n\n";
    }

    fout << "figure(1)\nstem(Re),grid on\n\nfigure(2)\nstem(Im),grid on";
    fout.close();
}


void Complex::print() {
    cout << re;
    if(im>=0)
        cout << "+";

    cout << im <<"j\t";
}

Complex operator+(Complex c1, Complex c2) {

    return Complex(c1.re+c2.re, c1.im+c2.im);
}

Complex operator-(Complex c1, Complex c2) {

    return Complex(c1.re-c2.re, c1.im-c2.im);
}

Complex operator*(Complex c1, Complex c2) {

    return Complex(c1.re*c2.re - c1.im*c2.im, c1.re*c2.im + c1.im*c2.re);
}

Complex operator/(Complex c1, Complex c2) {
    if(c2.re == 0|| c2.im == 0){
        cout << "Can\'t (";
        c1.print();
        cout << ")/(";
        c2.print();
        cout << ")\n";
        return c1;
    }

    return Complex((c1.re*c2.re+c1.im*c2.im)/(c2.re*c2.re + c2.im*c2.im), (c1.im*c2.re-c1.re*c2.im)/(c2.re*c2.re + c2.im*c2.im));
}

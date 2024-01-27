#include <iostream>
#include <cmath>
#include <complex>
#include <vector>

using namespace std;

using ld = long double;
using cpx = complex<ld>;
using cpxvec = vector<cpx>;

cpxvec recurse_fft(cpxvec *p, bool inv = false, int stack = 0, cpx w = complex(0.0, 0.0));
cpxvec for_fft(cpxvec *p);
string fft_multiply(string *a, string *b);
cpxvec bignumber_to_cpxvec(string *s);
void padding(cpxvec *v, int min);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string a;
    string b;
    cin >> a >> b;

    string s = fft_multiply(&a, &b);
    cout << s << endl;

    return 0;
}

cpxvec recurse_fft(cpxvec *p, bool inv, int stack, cpx w) {
    int n = p->size(); //size should be 2^n
    if (n == 1) return *p;

    //calculate omega (fft->e^(2pi/n), ifft->e^(-2pi/n))
    if(stack == 0) w = inv ? exp(complex(0.0, -2 * M_PI / n)) : exp(complex(0.0, 2 * M_PI / n));

    //pe for even elements, po for odd elements
    cpxvec pe;
    cpxvec po;
    pe.reserve(n / 2 + 1);
    po.reserve(n / 2 + 1);

    for (int i = 0; i<<1 < n; i ++) {
        pe.push_back((*p)[i<<1]);
        po.push_back((*p)[(i<<1) + 1]);
    }

    //divide and conquer
    cpx w2 = w*w;
    const cpxvec ye = recurse_fft(&pe, inv, stack + 1, w2);
    const cpxvec yo = recurse_fft(&po, inv, stack + 1, w2);

    cpxvec result(n, complex(0.0, 0.0));

    //combine pe, po
    const int t = n / 2;
    cpx wj = complex(1.0, 0.0);
    for (int j = 0; j < t; j++) {
        const cpx q = wj * yo[j];
        result[j] = ye[j] + q;
        result[j + t] = ye[j] - q;
        wj *= w;
    }

    //ifft postprocess
    if (inv && stack == 0) {
        for (auto &i: result) { i /= n; }
    }
    return result;
}

string fft_multiply(string *a, string *b) {//a, b의 크기가 2^n으로 같아야함
    //number(string) to cpxvec
    cpxvec va = bignumber_to_cpxvec(a);
    cpxvec vb = bignumber_to_cpxvec(b);

    //resize to 2^n

    int min_size = va.size() + vb.size();
    padding(&va, min_size);
    padding(&vb, min_size);

    //fft
    cpxvec ffta = recurse_fft(&va);
    cpxvec fftb = recurse_fft(&vb);

    //multiply fft values
    for (int i = 0; i < ffta.size(); i++) { ffta[i] *= fftb[i]; }

    //cpx multiply result
    cpxvec result = recurse_fft(&ffta, true);
    //ifft
    vector<int> intres;
    intres.reserve(result.size());
    intres.push_back(0);

    //cpx to int vector (single int digit)
    for (int i = 0; i < result.size(); i++) {
        int real = round(result[i].real());
        intres[i] += real;
        int up = intres[i] / 10;
        intres[i] %= 10;
        intres.push_back(up);
    }

    //remove front zeros
    for (int i = intres.size() - 1; i > 0; i--) {
        if (intres[i] == 0) {
            intres.pop_back();
        } else break;
    }

    //final number
    string s;
    s.reserve(result.size());

    //int vector to string (reversing order)
    for (int i = intres.size() - 1; i >= 0; i--) {
        s.push_back(intres[i] + '0');
    }
    return s;
}

cpxvec bignumber_to_cpxvec(string* s) {
    cpxvec v;
    v.reserve(s->size());
    //number(string) to cpxvec (reversing order)
    for (int i = s->size() - 1; i >= 0; i--) {
        v.emplace_back(complex(double((*s)[i] - '0'), 0.0));
    }
    return v;
}

void padding(cpxvec* v, int min) {
    //get appropriate size of 2^n
    int size = 1 << (int) ceil(log2(max((int) v->size(), min)));
    v->resize(size);
}
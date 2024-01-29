#include <iostream>
#include <cmath>
#include <complex>
#include <vector>

using namespace std;

using cpx = complex<double>;
using cpxvec = vector<cpx>;

cpxvec for_fft(cpxvec &p, int min, int inv = 1);
string fft_multiply(string &a, string &b);
cpxvec bignumber_to_cpxvec(string &s);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string a;
    string b;
    cin >> a >> b;

    string s = fft_multiply(a, b);
    cout << s << endl;

    return 0;
}

string fft_multiply(string &a, string &b) {//a, b의 크기가 2^n으로 같아야함
    //number(string) to cpxvec
    cpxvec va = bignumber_to_cpxvec(a);
    cpxvec vb = bignumber_to_cpxvec(b);

    //fft
    int min = va.size() + vb.size();
    cpxvec ffta = for_fft(va, min);
    cpxvec fftb = for_fft(vb, min);

    //multiply fft values
    for (int i = 0; i < ffta.size(); i++) { ffta[i] *= fftb[i]; }

    //ifft
    cpxvec res = for_fft(ffta, ffta.size(), -1);

    vector<int> intres;
    intres.reserve(res.size());
    intres.push_back(0);

    //cpx to int vector (single int digit)
    for (int i = 0; i < res.size(); i++) {
        int real = round(res[i].real());
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
    s.reserve(intres.size());

    //int vector to string (reversing order)
    for (int i = intres.size() - 1; i >= 0; i--) {
        s.push_back(intres[i] + '0');
    }
    return s;
}

cpxvec bignumber_to_cpxvec(string& s) {
    cpxvec v;
    v.reserve(s.size());
    //number(string) to cpxvec (reversing order)
    for (int i = s.size() - 1; i >= 0; i--) {
        v.push_back(complex(double(s[i] - '0'), 0.0));
    }
    return v;
}

cpxvec for_fft(cpxvec &p, int min, int inv) {
    int psize = p.size();
    int size = max(psize, min);
    int n=1;
    while(n<size){
        n<<=1;
    }

    cpxvec v(n);
    v[0] = p[0];
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        while (j >= bit) {
            j -= bit;
            bit >>= 1;
        }
        j += bit;
        v[j] = i<psize ? p[i] : 0;
    }

    for (int i = 2; i <= n; i <<= 1) {
        cpx w1 = exp(complex(0.0, inv * 2 * M_PI / i));
        for (int j = 0; j < n; j += i) {
            cpx w = 1;
            for (int k = j; k < j + (i >> 1); ++k, w *= w1) {
                int k2 = k + (i >> 1);

                cpx t1 = v[k];
                cpx t2 = v[k2] * w;

                v[k] = t1 + t2;
                v[k2] = t1 - t2;
            }
        }
    }

    if (inv == -1) for (int i = 0; i < n; i++) v[i] /= n;
    return v;
}

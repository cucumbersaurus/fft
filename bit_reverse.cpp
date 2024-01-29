#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace chrono;

vector<int> reverseBits(int length)
{
    int size = 1<<length;
    vector<int> res;
    res.reserve(size);

    for(int i=0;i<size;i++) {
        int rev = 0;
        int n=i;
        for (int j = 0; j < length; j++) {
            rev <<= 1;
            rev += n % 2;
            n >>= 1;
        }
        res.push_back(rev);
    }
    return res;
}

vector<int> bit_reversal_permutation(int n){
    int max = 1<<n;
    vector<int> res;
    res.reserve(max);
    res.push_back(0);
    res.push_back(max>>1);

    int firstidx = 0;
    int secidx = 1;
    int newnum = max>>2;
    int n2 = res[1];

    for(int i=2;i<max;i++) {
        firstidx++;
        if (firstidx == secidx) {
            res.push_back(newnum);
            newnum>>=1;
            secidx <<= 1;
            n2=res[secidx];
            firstidx = 0;
        } else {
            res.push_back(res[firstidx] + n2);
        }
    }
    return res;
}

vector<int> another_bit_reverse(int l) {
    int S = 1 << l;
    vector<int> res;
    res.reserve(S);
    for (int i = 1, j=0; i < S; i++) {
        int bit = S>>1;
        while(j>=bit){
            j-=bit;
            bit>>=1;
        }
        j+=bit;
        res.push_back(j);
    }
    return res;
}

void single_test(){
    int length;
    cin >> length;

    system_clock::time_point t1s = system_clock::now();
    vector<int> reverse1 = bit_reversal_permutation(length);
    system_clock::time_point t1e = system_clock::now();
    nanoseconds nano1 = t1e - t1s;
    for(auto i : reverse1){
        cout<<i<<" ";
    }
    cout<<endl<<"내 알고리즘  :  "<<nano1.count()<<endl;


    system_clock::time_point t2s = system_clock::now();
    vector<int> reverse2 = reverseBits(length);
    system_clock::time_point t2e = system_clock::now();
    nanoseconds nano2 = t2e - t2s;
    for(auto i : reverse2){
        cout<<i<<" ";
    }
    cout<<endl<<"단순 뒤집기  :  "<<nano2.count()<<endl;


    system_clock::time_point t3s = system_clock::now();
    vector<int> reverse3 =another_bit_reverse(length);
    system_clock::time_point t3e = system_clock::now();
    nanoseconds nano3 = t3e - t3s;
    for(auto i : reverse3){
        cout<<i<<" ";
    }
    cout<<endl<<"티스토리 코드  :  "<<nano3.count()<<endl;
}

void accumulate_test() {
    int n;
    cin >> n;
    int repeating_time = 100;

    cout << endl << "내 알고리즘" << endl;
    for (int i = 1; i <= n; i++) {
        long long total = 0;
        for (int j = 0; j < repeating_time; j++) {
            system_clock::time_point ts = system_clock::now();

            bit_reversal_permutation(i);

            system_clock::time_point te = system_clock::now();
            nanoseconds nano = te - ts;
            total += nano.count();
        }
        cout << i << " : " << total / repeating_time << " \t";
    }

    cout << endl << "티스토리 코드" << endl;
    for (int i = 1; i <= n; i++) {
        long long total = 0;
        for (int j = 0; j < repeating_time; j++) {
            system_clock::time_point ts = system_clock::now();

            another_bit_reverse(i);

            system_clock::time_point te = system_clock::now();
            nanoseconds nano = te - ts;

            total += nano.count();
        }
        cout << i << " : " << total / repeating_time << " \t";
    }

    cout << endl << "단순 뒤집기" << endl;
    for (int i = 1; i <= n; i++) {
        long long total = 0;
        for (int j = 0; j < repeating_time; j++) {

            system_clock::time_point ts = system_clock::now();

            reverseBits(i);

            system_clock::time_point te = system_clock::now();
            nanoseconds nano = te - ts;

            total += nano.count();
        }
        cout << i << " : " << total / repeating_time << " \t";
    }
}

int main() {

    accumulate_test();
    //single_test();

    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

string add(string a, string b);
string mult_by_two(string num);
void bigfactorial(int n);


int str_to_int(char *argument) {
    int num = 0;
    for (int i = 0; argument[i] != '\0'; i++) {
        num *= 10;
        num += argument[i] - '0';
    }
    return num;
}

int main(int argc, char ** argv)
{
    for (int i = 1; i < argc; i++) {
        auto start = chrono::high_resolution_clock::now();
        int num = str_to_int(argv[i]);
        bigfactorial(num);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "**********************************************\nTime Taken for !" << num << ": " << duration.count() << " microseconds" << "\n**********************************************\n";
    }

    if (argc == 0) {
        cout << "usage: run this program with numerical arguments to get their factorials" << endl;
    }

    return 0;
}


void bigfactorial(int n) 
{
    
    
    // multiply current n by each product 
    // if product > 1/2 max product, bit shift and add result to next product down the line
    // e.g. (on an 8 bit example, max multiplicand is 15)
    // [12, 4, 1] * 13 -> [156, 52, 13] --> [(9,12),52,13] -> [9,15,4,13]
    // conversion to a string goes like the following
    // only consider the right half of the products
    // for each bit, find the value of the bit by its power of 2, and add to a sum
    // [12, 4, 1] -> 1100, 0100, 0001 -> 1 + 64 + 1024 + 2048 = 3137
    // verify 3137 * 13 = 40781
    // [9,15,4,13] = 1001 1111 0100 1101 (correct)
    // on the real vector, data reads from left to right
    // [13,4,15,9] but which ultimately does not change the value
    
    vector<unsigned long long> product = {1ULL};
    
    // machine-independent ull half-width all bits turned on in the second half
    const int halfwidth = sizeof(unsigned long long) * 4;
    const unsigned long long MAX_PRODUCT = (~0ULL) >> halfwidth;
    unsigned long long temp = 0;
    
    for (int i = 2; i < n+1; i++) {
        
        for (int j = 0; j < product.size(); j++) {
            product[j] *= i; 
        }
        for (int j = 0; j < product.size(); j++) {
            product[j] += temp;
            if (product[j] > MAX_PRODUCT) {
                temp = product[j] >> halfwidth;
                product[j] = product[j] & MAX_PRODUCT;
            } else {
                temp = 0;
            }
        }
        if (temp > 0) {
            // overflow, must add new element in product vector
            product.push_back(temp);
        }
        temp = 0;
    }
    
    // convert to string and add along the way
    string str_product = "0", pow2 = "1";
    for (int i = 0; i < product.size(); i++) {
        for (int j = 0; j < halfwidth; j++) {
            if (product[i] & (1 << j)) {
                str_product = add(str_product, pow2);
            }
            pow2 = mult_by_two(pow2);
        }
    }
    
    cout << str_product << endl;
}


string mult_by_two(string num) 
{
    int carry = 0;
    for (int i = num.size() - 1; i >= 0; i--) {
        switch(num[i]) {
            case '0': num[i] = '0' + carry; carry = 0; break;
            case '1': num[i] = '2' + carry; carry = 0; break;
            case '2': num[i] = '4' + carry; carry = 0; break;
            case '3': num[i] = '6' + carry; carry = 0; break;
            case '4': num[i] = '8' + carry; carry = 0; break;
            case '5': num[i] = '0' + carry; carry = 1; break;
            case '6': num[i] = '2' + carry; carry = 1; break;
            case '7': num[i] = '4' + carry; carry = 1; break;
            case '8': num[i] = '6' + carry; carry = 1; break;
            case '9': num[i] = '8' + carry; carry = 1; break;
        }
    }
    return carry == 1 ? "1" + num : num;
}

string add(string a, string b) 
{
    int carry = 0, i, sum;
    if (b.size() > a.size())
        swap(a,b);
    
    
    // cout << "a is " << a << endl;
    // cout << "b is " << b << endl;
    i  = a.size() - 1;
    for (int j = b.size() - 1; j >= 0;) {
        // cout << "a[i] = " << a[i] << endl;
        // cout << "b[j] = " << b[j] << endl;
        // cout << "carry is " << carry << endl;
        sum = (a[i] - '0') + (b[j] - '0') + carry;
        // cout << "sum is " << sum << endl;
        if (sum > 9) {
            carry = 1;
            sum -= 10;   
        } else {
            carry = 0;
        }
        // cout << "carry is now " << carry << " and sum is " << sum;
        a[i] = sum + '0';
        // cout << " and a[i] is " << a[i] << endl;
        
        i--;
        j--;
    }
    while (i >= 0 && carry > 0) {
        sum = a[i] - '0' + carry;
        // cout << "2nd loop: sum is " << sum << " and a[i] is " << a[i] << endl;
        if (sum > 9) {
            carry = 1;
            sum -= 10;
        } else {
            carry = 0;
        }
        
        a[i] = sum + '0'; 
        // cout << "now a[i] is " << a[i] << endl;
        i--;
    }
    
    if (carry > 0) {
        return "1" + a;
    }
    return a;
}

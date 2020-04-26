// my first program in C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

vector<long long> A;
vector<int> random_P(int, int);
long long karmarkarKarp(vector<long long>);
long long a_karmarkar_karp(vector<long long>);
long long repeated_random(vector<int> );
long long hill_climbing(vector<int> );
long long sim_anneal(vector<int> );
long long prepar_repeated_random(vector<int> );
long long prepar_hill_climbing(vector<int> );
long long prepar_sim_anneal(vector<int> );
int max_iter = 25000;

int main(int argc, char *argv[]) {
    srand((unsigned) time(0));

    int flag = atoi(argv[1]);
    int algorithm = atoi(argv[2]);

    // Reading file into vector A
    string fileName = argv[3];
    std::ifstream file(fileName);

    // store into A
    string k;
    while (std::getline(file, k)){
        long long i = stoll(k);
        A.push_back(i);
    }
    file.close();

    // printf("Printing the matrix A: \n");
    // for(int i = 0; i < A.size(); i++) {
    //     printf("i is: %i and val: %lld \n", i,  A[i]);
    // }

    // vector for initial solution
    vector<int>  p;

    switch (algorithm) {
      // standard kk
      case 0:
        std::cout << "\nresidual: " << karmarkarKarp(A) << "\n";
        break;
      // repeated random
      case 1:
        // use range 2 to create two subsets
        p = random_P(A.size(),2);

        std::cout << "\n residual for rr: " << repeated_random(p) << "\n";
        break;
      // hill climbing
      case 2:
        p = random_P(A.size(),2);

        std::cout << "\n residual for hc: " << hill_climbing(p) << "\n";
        break;
      // sim anneal
      case 3:
        p = random_P(A.size(),2);

        std::cout << "\n residual for sa: " << sim_anneal(p) << "\n";
        break;

      //pp repeated random
      case 11:
        p = random_P(A.size(), A.size());

        std::cout << "\n residual for prr: " << prepar_repeated_random(p) << "\n";
        break;
      //pp hill climb
      case 12:
        p = random_P(A.size(), A.size());

        std::cout << "\n residual for phc: " << prepar_hill_climbing(p) << "\n";
        break;
      //pp sim anneal
      case 13:
        p = random_P(A.size(), A.size());

        std::cout << "\n residual for psa: " << prepar_sim_anneal(p) << "\n";
        break;
    }
}

// Generates a random solution sequence P with size n, values from 1 to n
vector<int> random_P(int n, int range) {
    vector<int> vect(n, -1);

    for (int i = 0; i < n; i++) {
        vect[i] = (rand() % range + 1);
    }
    // printf("Printing the rand P: \n");
    // for(int i = 0; i < vect.size(); i++) {
    //     printf("i is: %i and val: %d \n", i,  vect[i]);
    // }

    return vect;
}

// Generates a random neighbor to sequence P
vector<int> random_neighbor(vector<int> p, int range) {
  vector<int> vect;
  vect = p;

  int j = rand() % range + 1;

  while(true) {
      int i = rand() % p.size();

      if (vect[i] != j) { // make a swap if the index values are not the same
        vect[i] = j;
        break;
      }
  }
  return vect;
}

// Karmarkar Karp with P and A' - has A' and uses P?
long long a_karmarkar_karp (vector<int>  p) {
    // create A' based on P and A
    vector<long long>  ap(p.size(), 0);
    for(int i = 0; i < p.size();  i++) {
        ap[p[i] - 1]+=A[i];
    }

    // printf("AP is: \n");
    // for(int i = 0; i < ap.size(); i++) {
    //     printf("i is: %i and val: %lld \n", i, ap[i]);
    // }

    return(karmarkarKarp(ap));
}

// Pure Karmarkar Karp, with input just A and no P
long long karmarkarKarp(vector<long long> a) {

    // Karmarkar Karp
    while(a.size()>1) {
        // Sort input
        sort(a.begin(), a.end(), greater<long long>());

        //delete all zeroes, keeping the 0 if it is the only element in the array
        while(a.back() == 0 && a.size() > 1){
            a.pop_back();
        }

        if(a.size() <= 1) {
            return a[0];
        }
        //subtract second largest from largest
        a[0]-= a[1];
        //replace second largest with 0
        a[1] = 0;
    }

    return a[0];
}

long long prepar_repeated_random(vector<int>  p) {
    vector<int>  s;
    s = p;
    long long sk = a_karmarkar_karp(s);

    for (int i = 0; i < max_iter; i++) {
        // Generate random solution
        vector<int>  sp = random_P(p.size(), p.size());
        long long spk = a_karmarkar_karp(sp);
        if (spk < sk) {
            s = sp;
            sk = spk;
        }
    }
    return(sk);
}

long long prepar_hill_climbing(vector<int>  p) {
    vector<int>  s;
    s = p;
    long long sk = a_karmarkar_karp(s);

    for (int i = 0; i < max_iter; i++) {
        // Generate random neighbor
        vector<int>  sp = random_neighbor(s, s.size());
        long long spk = a_karmarkar_karp(sp);
        if (spk < sk) {
            s = sp;
            sk = spk;
        }
    }
    return(sk);
}

long long prepar_sim_anneal(vector<int>  p) {
    vector<int>  s;
    s = p;
    long long sk = a_karmarkar_karp(s);

    vector<int>  spp;
    spp = s;
    long long sppk = a_karmarkar_karp(spp);

    for (int i = 0; i < max_iter; i++) {
        // Generate random neighbor
        vector<int>  sp = random_neighbor(s, s.size());
        long long spk = a_karmarkar_karp(sp);
        if (spk < sk) {
            s = sp;
            sk = spk;
        } else {
// probablity stuff
        }

        if (sk < sppk) {
          spp = s;
        }
    }

    return(sppk);
}

// All of the below functions use range 2, to simulate a swap between two subsets
long long repeated_random(vector<int>  p) {
    vector<int>  s;
    s = p;
    long long sk = a_karmarkar_karp(s);

    for (int i = 0; i < max_iter; i++) {
        // Generate random solution
        vector<int>  sp = random_P(p.size(), 2);
        double spk = a_karmarkar_karp(sp);
        if (spk < sk) {
            s = sp;
            sk = spk;
        }
    }
    return(sk);
}


long long hill_climbing(vector<int>  p) {
    vector<int>  s;
    s = p;
    long long sk = a_karmarkar_karp(s);

    for (int i = 0; i < max_iter; i++) {
        // Generate random neighboring solution
        vector<int>  sp = random_neighbor(s, 2);
        long long spk = a_karmarkar_karp(sp);
        if (spk < sk) {
            s = sp;
            sk = spk;
        }
    }
    return(sk);
}

long long sim_anneal(vector<int>  p) {
    vector<int>  s;
    s = p;
    long long sk = a_karmarkar_karp(s);

    vector<int>  spp;
    spp = s;
    long long sppk = a_karmarkar_karp(spp);

    for (int i = 0; i < max_iter; i++) {
        // Generate random neighbor
        vector<int>  sp = random_neighbor(s, 2);
        long long spk = a_karmarkar_karp(sp);
        if (spk < sk) {
            s = sp;
            sk = spk;
        } else {
// probablity stuff
        }

        if (sk < sppk) {
          spp = s;
        }
    }

    return(sppk);
}

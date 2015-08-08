  ****************************************************
  Find total ways to provide changes
   of Rs. n
   when m types of denominations are available
   where s[0]..s[m-1] contains value of each type
   **************************************************/

#include <iostream>

using namespace std;

int CountChange(int s[], int n, int m) {

  if (n == 0) {  // changes found for amount
    return 1;
  }

  if (n < 0) {  // exceeding the amount
    return 0;
  }


  if (m <= 0  &&  n > 0) { // changes exhausted
    return 0;
  }

  // count excluding s[m-1] and including s[m-1]
  return CountChange(s, n, m-1) + CountChange(s, n-s[m-1], m);

}


int CountChangeDP (int s[], int n, int m) {

  // creation and intialization of table
  // to store values in bottom-up manner
  int** table = new int* [n+1];
  for (int k=0; k<=n; k++)
    table[k] = new int[m];

  for(int k=0; k<m; k++)
    table[0][k] = 1;


  for (int i=1; i<=n; i++) {
    for (int j=0; j<m; j++) {

      //include s[j]
      int x = (i >= s[j]) ? table [i-s[j]] [j] : 0;
      
      //exclude s[j]
      int y = (j >= 1) ? table [i][j-1] : 0;

      // total ways to give change for "i"
      // using dominations of s[0] to s[j]
      table[i][j] = x + y;
    }
  }

  return table[n][m-1];
}

int main() {

  int m = 3; // different denominations count
  int n = 5; // total amount

  int S[] = {1, 2, 3}; // denominations

  cout << "Total ways to give changes: " << CountChange(S, n, m) << endl;
  cout << "Total ways to give changes: " << CountChangeDP(S, n, m) << endl;

  return 0;
}

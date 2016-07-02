#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iterator>

using namespace std;


void read_nums(std::string fname, vector<int>& numbers) {

    std::ifstream ifs (fname, std::ifstream::in);

    int num;
    ifs >> num;

    while (ifs.good()) {
      std::cout << num << endl;
      numbers.push_back(num);
      ifs >> num;
    }

    ifs.close();

}

int main(int argc, char* argv[]) {

  vector<int> first, second;
  read_nums(argv[1], first);
  read_nums(argv[2], second);

  vector<int> merged(first.size()+second.size());
  std::merge (first.begin(), first.end(), second.begin(), second.end(), merged.begin());

  for (const auto& itr : merged) {
    cout << itr << " ";
  }
  cout << endl;

  std::ofstream output_file(argv[3]);
  std::ostream_iterator<int> output_iterator(output_file, " ");
  std::copy(merged.begin(), merged.end(), output_iterator);
}

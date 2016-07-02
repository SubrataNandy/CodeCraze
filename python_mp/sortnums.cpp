#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

int main(int argc, char *argv[])
{

    vector<int> numbers;

    std::ifstream ifs (argv[1], std::ifstream::in);

    int num;
    ifs >> num;

    while (ifs.good()) {
      std::cout << num << endl;
      numbers.push_back(num);
      ifs >> num;
    }

    ifs.close();

    std::sort(numbers.begin(), numbers.end());

    for (const auto& itr : numbers) {
      cout << itr << " ";
    }
    cout << endl;

    std::ofstream output_file(argv[2]);
    std::ostream_iterator<int> output_iterator(output_file, " ");
    std::copy(numbers.begin(), numbers.end(), output_iterator);
}

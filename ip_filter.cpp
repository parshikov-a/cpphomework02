#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using IPAddress = std::vector<int>;
using IPPool = std::vector<IPAddress>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
  std::vector<std::string> r;

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(d);
  while(stop != std::string::npos)
  {
    r.push_back(str.substr(start, stop - start));

    start = stop + 1;
    stop = str.find_first_of(d, start);
  }

  r.push_back(str.substr(start));

  return r;
}

void print_address(const IPAddress &ip_adr){
  for(auto ip_part = ip_adr.cbegin(); ip_part != ip_adr.cend(); ++ip_part) {
   if (ip_part != ip_adr.cbegin()) {
      std::cout << ".";
    }
    std::cout << *ip_part;
  }   
}

void print_pool(const IPPool &ip_pool){
	for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
		print_address(*ip);
    std::cout << std::endl; 
  }
}

void r_lex_sort(IPPool &ip_pool)
{
  std::sort(ip_pool.rbegin(), ip_pool.rend(),
            [](const IPAddress& a1,
               const IPAddress& a2) -> bool {
    if(a1.at(0) < a2.at(0)) return true;
    if(a1.at(0) > a2.at(0)) return false;
    if(a1.at(1) < a2.at(1)) return true;
    if(a1.at(1) > a2.at(1)) return false;
    if(a1.at(2) < a2.at(2)) return true;
    if(a1.at(2) > a2.at(2)) return false;
    if(a1.at(3) < a2.at(3)) return true;
    if(a1.at(3) > a2.at(3)) return false;
    return false;
  });
}

template<typename Addr, typename T>
bool predicate(const Addr& a, const T& t) {
  if (a.size() == 0) return false;
  return a.at(0) == t;
}

template<typename Addr, typename T, typename... Args>
bool predicate(const Addr& a, const T& t, Args... args) {
  return a.at(0) == t &&
         predicate(IPAddress(++a.cbegin(), a.cend()), args...);
}

template<typename Pool, typename... Args>
void filter2(const Pool& pool, Args... args) {    
  for (auto ip = pool.cbegin(); ip != pool.cend(); ++ip) {
    if(predicate(*ip, args...)) {
        print_address(*ip);
        std::cout << std::endl;
    }
  }
}

int main()
{
  try
  {
    IPPool ip_pool;

    for(std::string line; std::getline(std::cin, line);)
    {
      std::vector<std::string> v = split(line, '\t');
      std::vector<std::string> addr = split(v.at(0), '.');
      ip_pool.push_back(IPAddress{std::stoi(addr.at(0)),
                                     std::stoi(addr.at(1)),
                                     std::stoi(addr.at(2)),
                                     std::stoi(addr.at(3))}
        );
    }

	//////////////////////////////////////
	// 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

    r_lex_sort(ip_pool);
    print_pool(ip_pool);

	///////////////////////////////////////
        // filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

    [&ip_pool](){
      for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
        if(ip->at(0) == 1) {
          print_address(*ip);
          std::cout << std::endl;
        }
      }
    }();

	///////////////////////////////////////////
        // filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
    
    filter2(ip_pool, 46, 70);

/////////////////////////////////////////////////
        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46

    [&ip_pool](){
      for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
        if(any_of(ip->cbegin(), ip->cend(),
              [](const int& ip_part){
              return ip_part == 46;})) {
          print_address(*ip);
          std::cout << std::endl;
        }    
      }
    }();
  }
  catch(const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

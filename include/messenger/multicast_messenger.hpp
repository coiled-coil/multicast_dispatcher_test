#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <iostream>

namespace messenger {

using handler_t = std::function< void(int error_code, int token) >;

class multicast_messenger
{
    // using table_type = std::multimap<int, handler_t>;
    using table_type = std::unordered_multimap<int, handler_t>;

public:
    multicast_messenger()
    {}

    void register_handler(int token, handler_t handler)
    {
        using namespace std;
        table_.insert(make_pair(token, handler));
//cout << "INSERT : token = " << token << " size = " << table_.size() << endl;
    }

    void invoke(int error_code, int token)
    {
        using namespace std;

        auto ret = table_.equal_range(token);
        temp_buffer_.assign(ret.first, ret.second);
        table_.erase(token);

//int count = 0;
        for (auto& ret : temp_buffer_) {
//cout << "INVOKE[" << (count++) << "] : token = " << token << endl;
            (ret.second)(error_code, token);
        }
    }

private:
    table_type table_;
    std::vector<std::pair<int, handler_t>> temp_buffer_;
};


} // namespace messenger

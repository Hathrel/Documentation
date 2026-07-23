#include <algorithm>
#include <charconv>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

std::optional<std::size_t> parse_count(std::string_view text) {
    std::size_t value{};
    const char* const end = text.data() + text.size();
    const auto result = std::from_chars(text.data(), end, value);
    if (result.ec != std::errc{} || result.ptr != end || value == 0) {
        return std::nullopt;
    }
    return value;
}

bool is_word_character(char value) {
    return std::isalnum(static_cast<unsigned char>(value)) != 0;
}

char lower_ascii(char value) {
    const auto byte = static_cast<unsigned char>(value);
    return static_cast<char>(std::tolower(byte));
}

void count_line(std::string_view line,
                std::unordered_map<std::string, std::size_t>& counts) {
    std::string word;
    for (const char character : line) {
        if (is_word_character(character)) {
            word.push_back(lower_ascii(character));
        } else if (!word.empty()) {
            ++counts[word];
            word.clear();
        }
    }
    if (!word.empty()) {
        ++counts[word];
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "usage: " << argv[0] << " <input-file> [result-count]\n";
        return 2;
    }

    std::size_t result_count{20};
    if (argc == 3) {
        const auto parsed = parse_count(argv[2]);
        if (!parsed) {
            std::cerr << "result-count must be a positive integer\n";
            return 2;
        }
        result_count = *parsed;
    }

    std::ifstream input{argv[1]};
    if (!input) {
        std::cerr << "could not open: " << argv[1] << '\n';
        return 1;
    }

    std::unordered_map<std::string, std::size_t> counts;
    for (std::string line; std::getline(input, line);) {
        count_line(line, counts);
    }
    if (input.bad()) {
        std::cerr << "error while reading: " << argv[1] << '\n';
        return 1;
    }

    std::vector<std::pair<std::string, std::size_t>> sorted{counts.begin(), counts.end()};
    std::ranges::sort(sorted, [](const auto& left, const auto& right) {
        if (left.second != right.second) {
            return left.second > right.second;
        }
        return left.first < right.first;
    });

    result_count = std::min(result_count, sorted.size());
    for (std::size_t index{}; index < result_count; ++index) {
        std::cout << sorted[index].second << '\t' << sorted[index].first << '\n';
    }
}


#pragma once
#include <cstdint>
#include <fmt/core.h>
#include <memory>
#include <vector>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;
std::nullptr_t constexpr null = nullptr;
template <typename T>
using Vec = std::vector<T>;
template <typename T>
using Box = std::unique_ptr<T>;
template <typename T>
auto box = std::make_unique<T>;

void ensure(auto x, char const* msg = "Unexpected null") {
	if (!x) {
		fmt::print(stderr, "<Error>{}</Error>\n", msg);
		std::exit(-1);
	}
}

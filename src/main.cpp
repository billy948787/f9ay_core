﻿#include <array>
#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <print>
#include <queue>
#include <ranges>
#include <source_location>

#include "bmp.hpp"
#include "dct.hpp"
#include "huffman_coding.hpp"
#include "jpeg.hpp"
#include "lz77_compress.hpp"
#include "matrix.hpp"
#include "matrix_view.hpp"
#include "png.hpp"

#ifdef WIN32
#include "platform/windows_show.hpp"
#endif

using namespace f9ay;

int main(int argc, char** argv) {
    std::filesystem::path path = std::source_location::current().file_name();
    path = path.parent_path().parent_path() / "test_data" / "swiss.bmp";
    std::cout << path << std::endl;
    std::ifstream fs(path, std::ios::binary);
    if (!fs.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    const auto file = readFile(fs);
    auto result = Bmp::importFromByte(file.get());
    std::unique_ptr<std::byte[]> buffer;
    size_t size;
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::visit(
        [&]<typename T>(T&& mtx) {
            start = std::chrono::high_resolution_clock::now();
            std::tie(buffer, size) = Jpeg<Jpeg_sampling::ds_4_4_4>::exportToByte(mtx);
            end = std::chrono::high_resolution_clock::now();
        },
        result);

    std::println("{}", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

    std::ofstream out(path.parent_path() / "current.jpeg", std::ios::binary);
    out.write(reinterpret_cast<const char*>(buffer.get()), size);
    // Matrix<colors::RGB> mtx(res.row(), res.col());
    // for (auto i = 0; i < res.row(); i++) {
    //     for (auto j = 0; j < res.col(); j++) {
    //         mtx[i, j].r = res[i, j].r;
    //         mtx[i, j].g = res[i, j].g;
    //         mtx[i, j].b = res[i, j].b;
    //     }
    // }
    std::visit(
        [&path](auto&& arg) {
            std::ofstream out(path.parent_path() / "test.png", std::ios::binary);
            // calculate duration
            auto start = std::chrono::high_resolution_clock::now();
            auto [buffer, size] = PNG::exportToByte(arg);
            auto end = std::chrono::high_resolution_clock::now();
            std::println("{}", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
            out.write(reinterpret_cast<const char*>(buffer.get()), size);
        },
        result);
    //
    // std::string test = "aaabbaaa";
    //
    // auto vec = LZ77::lz77EncodeSlow(test);
    // for (auto [offset, length, value] : vec) {
    //     std::cout << "Offset: " << offset << ", Length: " << length << ", Value: " << (value.has_value() ? *value
    //     : '
    //     ')
    //               << "\n";
    // }
    // std::visit(
    //     [&path](auto&& arg) {
    //         std::ofstream out(path.parent_path() / "test.png", std::ios::binary);
    //
    //         auto rgbMtx = arg.trans_convert([](const auto& color) {
    //             return colors::color_cast<colors::RGB>(color);
    //         });
    //
    //         auto [buffer, size] = PNG::exportToByte(rgbMtx, FilterType::Sub);
    //         out.write(reinterpret_cast<const char*>(buffer.get()), size);
    //     },
    //     result);
    //
    // std::string test = "aaabbaaa";
    //
    // auto vec = LZ77::lz77EncodeSlow(test);
    // for (auto [offset, length, value] : vec) {
    //     std::cout << "Offset: " << offset << ", Length: " << length << ", Value: " << (value.has_value() ? *value
    //     : '
    //     ')
    //               << "\n";
    // }
    // auto start = std::chrono::high_resolution_clock::now();
    // auto [buffer, size] = Jpeg::write(mtx);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::println("{}", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

    // std::ofstream out(path.parent_path() / "current.jpeg", std::ios::binary);
    // out.write(reinterpret_cast<const char*>(buffer.get()), size);
}

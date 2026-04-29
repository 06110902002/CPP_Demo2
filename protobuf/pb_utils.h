//
// Created by rambo.liu on 2026/4/28.
// 基础类型的序列化类

#ifndef CPP_TEST1_PB_UTILS_H
#define CPP_TEST1_PB_UTILS_H

#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cstdint>

// ---------------------------------------------------------
// 1. 序列化器 (用于写入数据)
// ---------------------------------------------------------
class Serializer {
public:
    // 写入任意基本类型 (int, long, float, double, char, bool 等)
    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, Serializer&>::type
    operator<<(const T& value) {
        size_t currentSize = buffer_.size();
        buffer_.resize(currentSize + sizeof(T));
        // 直接内存拷贝，极致高效
        std::memcpy(buffer_.data() + currentSize, &value, sizeof(T));
        return *this;
    }

    // 特化：处理 std::string
    // 策略：先写入长度(uint32_t)，再写入字符数据
    Serializer& operator<<(const std::string& str) {
        uint32_t length = static_cast<uint32_t>(str.size());
        *this << length; // 递归调用上面的模板写入长度
        if (length > 0) {
            size_t currentSize = buffer_.size();
            buffer_.resize(currentSize + length);
            std::memcpy(buffer_.data() + currentSize, str.c_str(), length);
        }
        return *this;
    }

    // 获取底层数据缓冲区（用于发送或存储）
    const std::vector<char>& getBuffer() const { return buffer_; }

    // 清空缓冲区
    void clear() { buffer_.clear(); }

private:
    std::vector<char> buffer_;
};

// ---------------------------------------------------------
// 2. 反序列化器 (用于读取数据)
// ---------------------------------------------------------
class Deserializer {
public:
    Deserializer(const char* data, size_t size)
        : data_(data), size_(size), offset_(0) {}

    // 读取任意基本类型
    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, Deserializer&>::type
    operator>>(T& value) {
        checkSize(sizeof(T));
        std::memcpy(&value, data_ + offset_, sizeof(T));
        offset_ += sizeof(T);
        return *this;
    }

    // 特化：处理 std::string
    Deserializer& operator>>(std::string& str) {
        uint32_t length = 0;
        *this >> length; // 先读出长度
        if (length > 0) {
            checkSize(length);
            str.resize(length);
            std::memcpy(&str[0], data_ + offset_, length);
            offset_ += length;
        } else {
            str.clear();
        }
        return *this;
    }

    // 检查是否还有足够的数据
    bool isFinished() const { return offset_ >= size_; }

private:
    void checkSize(size_t required) {
        if (offset_ + required > size_) {
            throw std::runtime_error("反序列化错误：数据流已损坏或长度不足");
        }
    }

    const char* data_;
    size_t size_;
    size_t offset_;
};


#endif //CPP_TEST1_PB_UTILS_H
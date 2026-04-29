//
// Created by rambo.liu on 2026/4/28.
//
/** ------------------------本demo 支持以下功能------------------------ */
// 支持 Varint/ZigZag 编码（int32/uint32/sint32/int64 等）
// 支持定长编码（fixed32/fixed64/float/double）
// 支持长度前缀编码（string/bytes/ 嵌套消息）
// 支持重复字段（repeated）
// 零拷贝、高性能、无内存冗余
// 纯 C++11 标准，跨平台无依赖
/** ------------------------本demo 支持以下功能------------------------ */
// 核心设计思路：
// 严格遵循 Protobuf wire protocol：保证与官方 Protobuf 二进制兼容
// 零拷贝设计：序列化直接写入缓冲区，反序列化直接读取内存
// 通用模板：一套代码支持所有数据类型，无需为每个消息生成代码
// 无动态分配（可选）：可使用固定大小缓冲区，避免性能损耗
// 异常安全：使用错误码返回，无 C++ 异常

#ifndef LITEPB_H
#define LITEPB_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

// 轻量级 Protobuf 编解码库命名空间
namespace litepb {

// Protobuf 线类型 (Wire Type)
// 决定字段如何编码
enum WireType : uint8_t {
    kVarint = 0,      // 可变长整数 (int32/uint32/int64等)
    kFixed64 = 1,     // 64位定长 (double/fixed64)
    kLengthDelim = 2, // 带长度前缀 (string/bytes/嵌套消息/repeated打包)
    kFixed32 = 5,     // 32位定长 (float/fixed32)
};

// 错误码
enum ErrorCode : int {
    kSuccess = 0,     // 成功
    kOutOfBuffer = -1,// 缓冲区不足
    kInvalidData = -2,// 数据格式非法
    kTagMismatch = -3,// 字段标签不匹配
};

// 包含 inline 函数实现
// 这些函数必须放在头文件才能正常内联优化
#include "litepb.inl"

// 输出流：负责 Protobuf 序列化（写入内存）
class OutputStream {
public:
    // 构造：绑定缓冲区
    OutputStream(uint8_t* buf, uint32_t capacity);

    // 获取已写入字节数
    uint32_t Size() const;

    // 获取原始数据指针
    const uint8_t* Data() const;

    // 写入 Varint 类型字段
    ErrorCode WriteVarint(uint32_t field_num, uint64_t value);

    // 写入 ZigZag 编码的 32 位有符号整数
    ErrorCode WriteSInt32(uint32_t field_num, int32_t value);

    // 写入 ZigZag 编码的 64 位有符号整数
    ErrorCode WriteSInt64(uint32_t field_num, int64_t value);

    // 写入 32 位定长字段
    ErrorCode WriteFixed32(uint32_t field_num, uint32_t value);

    // 写入 64 位定长字段
    ErrorCode WriteFixed64(uint32_t field_num, uint64_t value);

    // 写入字符串
    ErrorCode WriteString(uint32_t field_num, const std::string& value);

    // 写入二进制字节
    ErrorCode WriteBytes(uint32_t field_num, const uint8_t* data, uint32_t len);

    // 写入嵌套消息
    ErrorCode WriteMessage(uint32_t field_num, const OutputStream& nested);

    // 写入打包的 repeated Varint 数组（高性能）
    ErrorCode WriteRepeatedVarint(uint32_t field_num, const std::vector<uint64_t>& values);

private:
    // 写入原始字节（内部使用）
    ErrorCode WriteRaw(const uint8_t* data, uint32_t len);

    uint8_t* buf_;       // 输出缓冲区
    uint32_t capacity_;  // 缓冲区总容量
    uint32_t pos_;       // 当前写入位置
};

// 输入流：负责 Protobuf 反序列化（读取内存）
class InputStream {
public:
    // 默认构造（用于嵌套消息临时对象）
    InputStream();

    // 从缓冲区构造
    InputStream(const uint8_t* buf, uint32_t len);

    // 是否还有未读数据
    bool HasMore() const;

    // 获取剩余可读字节数
    uint32_t Remaining() const;

    // 读取字段标签 (field_num + wire_type)
    ErrorCode ReadTag(uint32_t* field_num, WireType* type);

    // 读取 Varint 整数
    ErrorCode ReadVarint(uint64_t* out_value);

    // 读取 ZigZag 编码的 sint32
    ErrorCode ReadSInt32(int32_t* out_value);

    // 读取 ZigZag 编码的 sint64
    ErrorCode ReadSInt64(int64_t* out_value);

    // 读取 32 位定长
    ErrorCode ReadFixed32(uint32_t* out_value);

    // 读取 64 位定长
    ErrorCode ReadFixed64(uint64_t* out_value);

    // 读取长度前缀
    ErrorCode ReadLength(uint32_t* out_len);

    // 读取字符串
    ErrorCode ReadString(std::string* out_value);

    // 读取嵌套消息，生成子输入流
    ErrorCode ReadMessage(InputStream* out_stream);

    // 跳过当前字段（用于兼容未知字段）
    ErrorCode SkipField(WireType type);

private:
    const uint8_t* buf_;  // 数据起始
    const uint8_t* end_;  // 数据结束
    const uint8_t* pos_;  // 当前读取位置
};

// Protobuf 消息基类
// 所有自定义消息都必须继承此类
class Message {
public:
    virtual ~Message() = default;

    // 序列化：写入输出流
    virtual ErrorCode Serialize(OutputStream& os) const = 0;

    // 反序列化：从输入流读取
    virtual ErrorCode Deserialize(InputStream& is) = 0;

    // 便捷接口：序列化到数组
    uint32_t SerializeToArray(uint8_t* buf, uint32_t capacity) const;

    // 便捷接口：从数组反序列化
    ErrorCode ParseFromArray(const uint8_t* buf, uint32_t len);
};

} // namespace litepb

#endif
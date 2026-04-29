//
// Created by rambo.liu on 2026/4/28.
//

#include "litepb.h"

namespace litepb {

// ==================== OutputStream 实现 ====================

// 构造函数：初始化输出流
OutputStream::OutputStream(uint8_t* buf, uint32_t capacity)
    : buf_(buf), capacity_(capacity), pos_(0) {}

// 获取已写入长度
uint32_t OutputStream::Size() const { return pos_; }

// 获取数据指针
const uint8_t* OutputStream::Data() const { return buf_; }

// 写入 Varint 字段（tag + value）
ErrorCode OutputStream::WriteVarint(uint32_t field_num, uint64_t value) {
    uint8_t temp[10];
    uint32_t tag_len = EncodeTag(field_num, kVarint, temp);
    uint32_t val_len = EncodeVarint(value, temp + tag_len);
    return WriteRaw(temp, tag_len + val_len);
}

// 写入 sint32（ZigZag + Varint）
ErrorCode OutputStream::WriteSInt32(uint32_t field_num, int32_t value) {
    return WriteVarint(field_num, ZigZagEncode32(value));
}

// 写入 sint64
ErrorCode OutputStream::WriteSInt64(uint32_t field_num, int64_t value) {
    return WriteVarint(field_num, ZigZagEncode64(value));
}

// 写入 fixed32（定长 4 字节）
ErrorCode OutputStream::WriteFixed32(uint32_t field_num, uint32_t value) {
    uint8_t temp[10];
    uint32_t tag_len = EncodeTag(field_num, kFixed32, temp);
    if (pos_ + tag_len + 4 > capacity_) return kOutOfBuffer;
    WriteRaw(temp, tag_len);
    memcpy(buf_ + pos_, &value, 4);
    pos_ += 4;
    return kSuccess;
}

// 写入 fixed64（定长 8 字节）
ErrorCode OutputStream::WriteFixed64(uint32_t field_num, uint64_t value) {
    uint8_t temp[10];
    uint32_t tag_len = EncodeTag(field_num, kFixed64, temp);
    if (pos_ + tag_len + 8 > capacity_) return kOutOfBuffer;
    WriteRaw(temp, tag_len);
    memcpy(buf_ + pos_, &value, 8);
    pos_ += 8;
    return kSuccess;
}

// 写入字符串
ErrorCode OutputStream::WriteString(uint32_t field_num, const std::string& value) {
    return WriteBytes(field_num, reinterpret_cast<const uint8_t*>(value.data()), (uint32_t)value.size());
}

// 写入二进制数据（带长度前缀）
ErrorCode OutputStream::WriteBytes(uint32_t field_num, const uint8_t* data, uint32_t len) {
    uint8_t temp[20];
    uint32_t tag_len = EncodeTag(field_num, kLengthDelim, temp);
    uint32_t len_len = EncodeVarint(len, temp + tag_len);
    ErrorCode err = WriteRaw(temp, tag_len + len_len);
    if (err != kSuccess) return err;
    return WriteRaw(data, len);
}

// 写入嵌套消息
ErrorCode OutputStream::WriteMessage(uint32_t field_num, const OutputStream& nested) {
    return WriteBytes(field_num, nested.Data(), nested.Size());
}

// 写入 repeated varint 数组（打包格式，高性能）
ErrorCode OutputStream::WriteRepeatedVarint(uint32_t field_num, const std::vector<uint64_t>& values) {
    if (values.empty()) return kSuccess;

    // 计算所有元素编码后的总长度
    uint32_t total_len = 0;
    uint8_t tmp[10];
    for (uint64_t v : values) {
        total_len += EncodeVarint(v, tmp);
    }

    // 写入 tag + 总长度
    uint8_t tag_buf[10];
    uint32_t tag_len = EncodeTag(field_num, kLengthDelim, tag_buf);
    uint32_t len_len = EncodeVarint(total_len, tag_buf + tag_len);
    ErrorCode err = WriteRaw(tag_buf, tag_len + len_len);
    if (err) return err;

    // 写入所有数据
    for (uint64_t v : values) {
        uint32_t n = EncodeVarint(v, tmp);
        err = WriteRaw(tmp, n);
        if (err) return err;
    }
    return kSuccess;
}

// 写入原始数据（内部核心方法）
ErrorCode OutputStream::WriteRaw(const uint8_t* data, uint32_t len) {
    if (pos_ + len > capacity_) return kOutOfBuffer;
    memcpy(buf_ + pos_, data, len);
    pos_ += len;
    return kSuccess;
}

// ==================== InputStream 实现 ====================

// 默认构造
InputStream::InputStream() : buf_(nullptr), end_(nullptr), pos_(nullptr) {}

// 从缓冲区构造
InputStream::InputStream(const uint8_t* buf, uint32_t len)
    : buf_(buf), end_(buf + len), pos_(buf) {}

// 是否还有数据
bool InputStream::HasMore() const { return pos_ < end_; }

// 剩余长度
uint32_t InputStream::Remaining() const { return static_cast<uint32_t>(end_ - pos_); }

// 读取字段 tag
ErrorCode InputStream::ReadTag(uint32_t* field_num, WireType* type) {
    uint32_t n = DecodeTag(pos_, Remaining(), field_num, type);
    if (!n) return kInvalidData;
    pos_ += n;
    return kSuccess;
}

// 读取 Varint
ErrorCode InputStream::ReadVarint(uint64_t* out_value) {
    uint32_t n = DecodeVarint(pos_, Remaining(), out_value);
    if (!n) return kInvalidData;
    pos_ += n;
    return kSuccess;
}

// 读取 sint32
ErrorCode InputStream::ReadSInt32(int32_t* out_value) {
    uint64_t v;
    ErrorCode err = ReadVarint(&v);
    if (err) return err;
    *out_value = ZigZagDecode32(v);
    return kSuccess;
}

// 读取 sint64
ErrorCode InputStream::ReadSInt64(int64_t* out_value) {
    uint64_t v;
    ErrorCode err = ReadVarint(&v);
    if (err) return err;
    *out_value = ZigZagDecode64(v);
    return kSuccess;
}

// 读取 fixed32
ErrorCode InputStream::ReadFixed32(uint32_t* out_value) {
    if (Remaining() < 4) return kOutOfBuffer;
    memcpy(out_value, pos_, 4);
    pos_ += 4;
    return kSuccess;
}

// 读取 fixed64
ErrorCode InputStream::ReadFixed64(uint64_t* out_value) {
    if (Remaining() < 8) return kOutOfBuffer;
    memcpy(out_value, pos_, 8);
    pos_ += 8;
    return kSuccess;
}

// 读取长度前缀
ErrorCode InputStream::ReadLength(uint32_t* out_len) {
    uint64_t v;
    ErrorCode err = ReadVarint(&v);
    if (err) return err;
    *out_len = static_cast<uint32_t>(v);
    return kSuccess;
}

// 读取字符串
ErrorCode InputStream::ReadString(std::string* out_value) {
    uint32_t len;
    ErrorCode err = ReadLength(&len);
    if (err) return err;
    if (Remaining() < len) return kOutOfBuffer;
    out_value->assign(reinterpret_cast<const char*>(pos_), len);
    pos_ += len;
    return kSuccess;
}

// 读取嵌套消息，创建子流
ErrorCode InputStream::ReadMessage(InputStream* out_stream) {
    uint32_t len;
    ErrorCode err = ReadLength(&len);
    if (err) return err;
    if (Remaining() < len) return kOutOfBuffer;
    *out_stream = InputStream(pos_, len);
    pos_ += len;
    return kSuccess;
}

// 根据 wire type 跳过字段（兼容协议升级）
ErrorCode InputStream::SkipField(WireType type) {
    switch (type) {
        case kVarint: {
            uint64_t dummy;
            return ReadVarint(&dummy);
        }
        case kFixed64:
            pos_ += 8;
            return kSuccess;
        case kLengthDelim: {
            uint32_t len;
            ErrorCode err = ReadLength(&len);
            if (err) return err;
            pos_ += len;
            return kSuccess;
        }
        case kFixed32:
            pos_ += 4;
            return kSuccess;
        default:
            return kInvalidData;
    }
}

// ==================== Message 实现 ====================

// 序列化到数组
uint32_t Message::SerializeToArray(uint8_t* buf, uint32_t capacity) const {
    OutputStream os(buf, capacity);
    if (Serialize(os) != kSuccess) return 0;
    return os.Size();
}

// 从数组解析
ErrorCode Message::ParseFromArray(const uint8_t* buf, uint32_t len) {
    InputStream is(buf, len);
    return Deserialize(is);
}

} // namespace litepb
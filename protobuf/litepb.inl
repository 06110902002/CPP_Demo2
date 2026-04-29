//内联实现文件：litepb.inl（给头文件用，不参与编译）

#ifndef LITEPB_INL
#define LITEPB_INL

// 编码无符号 Varint
// value：要编码的数字
// buf：输出缓冲区
// 返回：编码后占用的字节数
/**实现思路：
用最少的字节存整数。
小数字（0~127）→ 只用 1 个字节
大数字 → 用多个字节
比起固定 4 字节 / 8 字节，省流量、省空间、更快
--------------------------------------------------------
规则只有 2 条（死记硬背都能记住）：
规则①：每个字节只使用低 7 位存数据
规则②：最高位（第 8 位）是标志位
1 = 后面还有字节
0 = 这是最后一个字节
--------------------------------------------------------
我用一个数字带你走一遍算法
我们用 数字 300 来演示编码过程：
300 的二进制：
300 = 100101100 （一共9位）
规则（再强调一遍）
每个字节只用低 7 位存数据
最高位（第 8 位）是标志位
1 = 还有后续字节
0 = 最后一个字节
从低位往高位，每 7 位切一组
--------------------------------------------------------
第一步：把 300 写成二进制
300 = 1 0010 1100 （共9位）
--------------------------------------------------------
第二步：从低位开始，每 7 位分一组
9 位 → 分成两组：

高位组：1（剩下的2位）
低位组：0010 1100（低7位）
正确分组是：

组1（低7位）： 00101100
组2（高位）：        10
--------------------------------------------------------
第三步：给每组加标志位
不是最后一组 → 最高位 = 1
最后一组 → 最高位 = 0
组 1（低 7 位：00101100）
不是最后一组，所以最高位置 1：
1 010 1100
即：
0xAC

组 2（高位：1）
是最后一组，最高位保持 0：
0 0000 0010
即：
0x02
最终 Varint 编码结果
0x94, 0x02
*/
inline uint32_t EncodeVarint(uint64_t value, uint8_t* buf) {
    uint32_t n = 0;

    // 循环：只要数字 > 127（0x7F），说明需要多个字节
    while (value > 0x7F) {
        // 取低 7 位：value & 0x7F
        // 最高位置 1：| 0x80
        buf[n++] = static_cast<uint8_t>((value & 0x7F) | 0x80); //取低7位 + 标志位1

        // 右移 7 位，处理下一组
        value >>= 7;
    }

    // 最后一组：最高位保持 0
    buf[n++] = static_cast<uint8_t>(value & 0x7F);  //处理最后一组（标志位0）
    return n;
}

// 解码无符号 Varint
// buf：数据源
// max_len：最大可读长度
// out_value：解码结果
// 返回：读取的字节数，0 表示失败
inline uint32_t DecodeVarint(const uint8_t* buf, uint32_t max_len, uint64_t* out_value) {
    *out_value = 0;
    uint32_t n = 0;
    uint8_t byte;
    do {
        if (n >= max_len) return 0;
        byte = buf[n];
        *out_value |= (static_cast<uint64_t>(byte & 0x7F)) << (7 * n);
        n++;
    } while (byte & 0x80); // 最高位为1表示还有后续字节
    return n;
}

// ZigZag 编码：把负数映射为正数，使小负数也能高效使用 Varint
inline uint64_t ZigZagEncode32(int32_t value) {
    return (static_cast<uint32_t>(value) << 1) ^ (value >> 31);
}

inline uint64_t ZigZagEncode64(int64_t value) {
    return (static_cast<uint64_t>(value) << 1) ^ (value >> 63);
}

// ZigZag 解码：恢复原始有符号整数
inline int32_t ZigZagDecode32(uint64_t value) {
    return static_cast<int32_t>((value >> 1) ^ (~(value & 1) + 1));
}

inline int64_t ZigZagDecode64(uint64_t value) {
    return static_cast<int64_t>((value >> 1) ^ (~(value & 1) + 1));
}

// 编码字段标签：tag = (field_num << 3) | wire_type
inline uint32_t EncodeTag(uint32_t field_num, WireType type, uint8_t* buf) {
    return EncodeVarint((static_cast<uint64_t>(field_num) << 3) | type, buf);
}

// 解码字段标签
inline uint32_t DecodeTag(const uint8_t* buf, uint32_t max_len, uint32_t* field_num, WireType* type) {
    uint64_t tag;
    uint32_t n = DecodeVarint(buf, max_len, &tag);
    if (n == 0) return 0;
    *field_num = static_cast<uint32_t>(tag >> 3);
    *type = static_cast<WireType>(tag & 0x07);
    return n;
}

#endif
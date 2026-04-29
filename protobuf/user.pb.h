//
// Created by rambo.liu on 2026/4/28.
//

#ifndef USER_PB_H
#define USER_PB_H

#include "litepb.h"

// 地址消息（嵌套消息）
class Address : public litepb::Message {
public:
    std::string city;     // 城市
    uint32_t zipcode = 0; // 邮编

    // 序列化：将成员编码为 Protobuf 格式
    litepb::ErrorCode Serialize(litepb::OutputStream& os) const override {
        litepb::ErrorCode err;

        // 字段 1: string city
        if (!city.empty()) {
            err = os.WriteString(1, city);
            if (err) return err;
        }

        // 字段 2: uint32 zipcode
        if (zipcode != 0) {
            err = os.WriteVarint(2, zipcode);
            if (err) return err;
        }

        return litepb::kSuccess;
    }

    // 反序列化：从 Protobuf 数据解析成员
    litepb::ErrorCode Deserialize(litepb::InputStream& is) override {
        uint32_t field_num;
        litepb::WireType type;

        // 循环读取所有字段
        while (is.HasMore()) {
            litepb::ErrorCode err = is.ReadTag(&field_num, &type);
            if (err) return err;

            switch (field_num) {
                case 1: // city
                    err = is.ReadString(&city);
                    break;
                case 2: // zipcode
                    err = is.ReadVarint(reinterpret_cast<uint64_t*>(&zipcode));
                    break;
                default: // 跳过不认识的字段（兼容协议扩展）
                    err = is.SkipField(type);
                    break;
            }
            if (err) return err;
        }
        return litepb::kSuccess;
    }
};

// 用户消息（主消息）
class User : public litepb::Message {
public:
    uint32_t id = 0;              // 用户ID
    std::string name;             // 姓名
    int32_t age = 0;              // 年龄
    Address address;              // 地址（嵌套消息）
    std::vector<uint64_t> scores; // 分数列表（repeated）

    litepb::ErrorCode Serialize(litepb::OutputStream& os) const override {
        litepb::ErrorCode err;

        // 字段1：uint32 id
        if (id != 0) {
            err = os.WriteVarint(1, id);
            if (err) return err;
        }

        // 字段2：string name
        if (!name.empty()) {
            err = os.WriteString(2, name);
            if (err) return err;
        }

        // 字段3：sint32 age
        if (age != 0) {
            err = os.WriteSInt32(3, age);
            if (err) return err;
        }

        // 字段4：嵌套消息 address
        {
            uint8_t temp_buf[256];
            litepb::OutputStream nested_os(temp_buf, sizeof(temp_buf));
            address.Serialize(nested_os);
            err = os.WriteMessage(4, nested_os);
            if (err) return err;
        }

        // 字段5：repeated uint64 scores
        if (!scores.empty()) {
            err = os.WriteRepeatedVarint(5, scores);
            if (err) return err;
        }

        return litepb::kSuccess;
    }

    litepb::ErrorCode Deserialize(litepb::InputStream& is) override {
        uint32_t field_num;
        litepb::WireType type;

        while (is.HasMore()) {
            litepb::ErrorCode err = is.ReadTag(&field_num, &type);
            if (err) return err;

            switch (field_num) {
                case 1: // id
                    err = is.ReadVarint(reinterpret_cast<uint64_t*>(&id));
                    break;
                case 2: // name
                    err = is.ReadString(&name);
                    break;
                case 3: // age
                    err = is.ReadSInt32(&age);
                    break;
                case 4: { // address
                    litepb::InputStream nested_is;
                    err = is.ReadMessage(&nested_is);
                    if (!err) {
                        address.Deserialize(nested_is);
                    }
                    break;
                }
                case 5: {
                    // repeated uint64 scores（打包 LengthDelim 格式）
                    litepb::InputStream data_stream;
                    // 用 ReadMessage 接口安全获取子流（正确做法）
                    err = is.ReadMessage(&data_stream);
                    if (err != litepb::kSuccess) break;

                    // 循环解析所有 varint
                    while (data_stream.HasMore()) {
                        uint64_t val;
                        err = data_stream.ReadVarint(&val);
                        if (err != litepb::kSuccess) break;
                        scores.push_back(val);
                    }
                    break;
                }
                default:
                    err = is.SkipField(type);
                    break;
            }
            if (err) return err;
        }
        return litepb::kSuccess;
    }
};

#endif
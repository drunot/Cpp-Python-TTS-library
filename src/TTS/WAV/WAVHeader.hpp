#include <cstdint>
#include <cstring>

#ifndef ED6A7809_D773_416A_9451_D0CA4C02A9FE
#define ED6A7809_D773_416A_9451_D0CA4C02A9FE

namespace WAV {
    enum class type_format_e : std::uint16_t {
        PCM = 1
    };
#pragma pack(push, 1)
    struct WAVHeader {
        const std::uint8_t         identifier[4] = {'R', 'I', 'F', 'F'}; // Cannot use string because of terminator.
        const std::uint32_t        file_size;
        const std::uint8_t         file_type[4] = {'W', 'A', 'V', 'E'}; // Cannot use string because of terminator.
        const std::uint8_t         chunk_marker[4] = {'f', 'm', 't', ' '}; // Cannot use string because of terminator.
        const std::uint32_t        chunk_length = 16;
        const type_format_e        type_format = type_format_e::PCM;
        const std::uint16_t        channels;
        const std::uint32_t        sample_rate;
        const std::uint32_t        bytes_pr_second;
        const std::uint16_t        bytes_per_sample_for_all_channels;
        const std::uint16_t        bits_pr_sample;
        const std::uint8_t         data_marker[4] = {'d', 'a', 't', 'a'}; // Cannot use string because of terminator.
        const std::uint32_t        data_size;
        
        WAVHeader(std::uint32_t Data_size,
                  std::uint32_t Sample_rate,
                  type_format_e Type_format,
                  std::uint32_t Bits_pr_sample,
                  std::uint16_t Channels);
                  
        static constexpr inline std::size_t size() {
            return sizeof(WAVHeader);
        }
        
        void insert_header(void* buffer);
    };
#pragma pack(pop)
}

#endif /* ED6A7809_D773_416A_9451_D0CA4C02A9FE */

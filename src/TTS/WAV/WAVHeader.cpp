#include "WAVHeader.hpp"

namespace WAV {

    WAVHeader::WAVHeader(std::uint32_t Data_size,
                         std::uint32_t Sample_rate,
                         type_format_e Type_format,
                         std::uint32_t Bits_pr_sample,
                         std::uint16_t Channels) :
        file_size(Data_size + sizeof(WAVHeader)),
        data_size(Data_size),
        sample_rate(Sample_rate),
        bits_pr_sample(Bits_pr_sample),
        bytes_per_sample_for_all_channels((Bits_pr_sample * Channels) / 8),
        channels(Channels),
        bytes_pr_second((Sample_rate * Bits_pr_sample * Channels) / 8) {}
        
        
    void WAVHeader::insert_header(void* buffer) {
        std::memcpy(buffer, this, sizeof(WAVHeader));
    }
}
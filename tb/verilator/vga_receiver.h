#ifndef ZENITH_VGA_RECEIVER_H
#define ZENITH_VGA_RECEIVER_H

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct VgaReceiverOptions {
    bool enabled = false;
    bool live = false;
    unsigned scale = 1;
    std::string dump_path;
};

/*
 * Decode the physical ZenithSoC VGA pins.  The receiver deliberately knows
 * nothing about the RTL framebuffer: it reconstructs the image from HSYNC,
 * VSYNC and RGB444 in the same way as an external display.
 */
class VgaReceiver {
public:
    explicit VgaReceiver(VgaReceiverOptions options);
    ~VgaReceiver();

    VgaReceiver(const VgaReceiver&) = delete;
    VgaReceiver& operator=(const VgaReceiver&) = delete;

    void sample(bool rst_n,
                bool hsync,
                bool vsync,
                uint8_t red,
                uint8_t green,
                uint8_t blue);
    void finish();

    bool quit_requested() const { return quit_requested_; }
    uint64_t frame_count() const { return frame_count_; }

private:
    static constexpr unsigned kWidth = 640;
    static constexpr unsigned kHeight = 480;
    static constexpr unsigned kHorizontalTotal = 800;
    static constexpr unsigned kVerticalTotal = 525;
    static constexpr unsigned kHsyncStart = 656;
    static constexpr unsigned kHsyncEnd = 752;
    static constexpr unsigned kVsyncStart = 490;
    static constexpr unsigned kVsyncEnd = 492;
    static constexpr unsigned kSystemClocksPerPixel = 4;

    struct Viewer;

    void reset_decoder();
    void consume_pixel(uint8_t red, uint8_t green, uint8_t blue);
    void complete_frame();
    void poll_viewer();
    bool write_png(const std::string& path) const;

    VgaReceiverOptions options_;
    std::unique_ptr<Viewer> viewer_;
    std::vector<uint32_t> frame_;

    bool have_previous_sync_ = false;
    bool previous_hsync_ = true;
    bool previous_vsync_ = true;
    bool horizontal_locked_ = false;
    bool vertical_locked_ = false;
    bool collecting_frame_ = false;
    bool have_complete_frame_ = false;
    bool finished_ = false;
    bool quit_requested_ = false;

    unsigned x_ = 0;
    unsigned y_ = 0;
    unsigned phase_ = 0;
    unsigned visible_pixels_ = 0;
    uint64_t samples_ = 0;
    uint64_t frame_count_ = 0;
    uint64_t timing_errors_ = 0;
    uint64_t previous_frame_sample_ = 0;
    uint64_t previous_content_sample_ = 0;
    uint64_t previous_frame_hash_ = 0;
    uint64_t host_window_start_frame_ = 0;
    std::chrono::steady_clock::time_point host_window_start_;
    bool have_frame_hash_ = false;
    double signal_fps_ = 0.0;
    double content_fps_ = 0.0;
    double host_fps_ = 0.0;
};

#endif

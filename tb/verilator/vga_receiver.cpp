#include "vga_receiver.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
#include <zlib.h>

#ifdef ZTB_HAVE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#endif

struct VgaReceiver::Viewer {
#ifdef ZTB_HAVE_X11
    Display* display = nullptr;
    Window window = 0;
    GC gc = nullptr;
    Atom wm_delete = 0;
    XImage* image = nullptr;
    unsigned scale = 1;

    ~Viewer() {
        if (image)
            XDestroyImage(image);
        if (display && gc)
            XFreeGC(display, gc);
        if (display && window)
            XDestroyWindow(display, window);
        if (display)
            XCloseDisplay(display);
    }

    static unsigned long channel(uint8_t value, unsigned long mask) {
        if (mask == 0)
            return 0;

        unsigned shift = 0;
        while (((mask >> shift) & 1UL) == 0)
            ++shift;

        const unsigned long maximum = mask >> shift;
        return ((static_cast<unsigned long>(value) * maximum + 127) / 255)
               << shift;
    }

    bool open(unsigned requested_scale) {
        scale = std::clamp(requested_scale, 1U, 4U);
        display = XOpenDisplay(nullptr);
        if (!display)
            return false;

        const int screen = DefaultScreen(display);
        const unsigned width = VgaReceiver::kWidth * scale;
        const unsigned height = VgaReceiver::kHeight * scale;

        window = XCreateSimpleWindow(display,
                                     RootWindow(display, screen),
                                     0,
                                     0,
                                     width,
                                     height,
                                     0,
                                     BlackPixel(display, screen),
                                     BlackPixel(display, screen));
        if (!window)
            return false;

        XStoreName(display, window, "ZenithSoC VGA");
        XSelectInput(display, window,
                     ExposureMask | KeyPressMask | StructureNotifyMask);
        wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(display, window, &wm_delete, 1);
        XMapWindow(display, window);

        gc = XCreateGC(display, window, 0, nullptr);
        image = XCreateImage(display,
                             DefaultVisual(display, screen),
                             DefaultDepth(display, screen),
                             ZPixmap,
                             0,
                             nullptr,
                             width,
                             height,
                             32,
                             0);
        if (!gc || !image)
            return false;

        image->data = static_cast<char*>(
            std::calloc(image->bytes_per_line, image->height));
        return image->data != nullptr;
    }

    void render(const std::vector<uint32_t>& pixels) {
        if (!display || !image)
            return;

        for (unsigned y = 0; y < VgaReceiver::kHeight; ++y) {
            for (unsigned x = 0; x < VgaReceiver::kWidth; ++x) {
                const uint32_t rgb = pixels[y * VgaReceiver::kWidth + x];
                const uint8_t red = (rgb >> 16) & 0xff;
                const uint8_t green = (rgb >> 8) & 0xff;
                const uint8_t blue = rgb & 0xff;
                const unsigned long native =
                    channel(red, image->red_mask) |
                    channel(green, image->green_mask) |
                    channel(blue, image->blue_mask);

                for (unsigned dy = 0; dy < scale; ++dy)
                    for (unsigned dx = 0; dx < scale; ++dx)
                        XPutPixel(image, x * scale + dx, y * scale + dy, native);
            }
        }

        XPutImage(display,
                  window,
                  gc,
                  image,
                  0,
                  0,
                  0,
                  0,
                  image->width,
                  image->height);
        XFlush(display);
    }

    void set_title(uint64_t frame,
                   double signal_fps,
                   double content_fps,
                   double host_fps) {
        if (!display || !window)
            return;

        std::ostringstream title;
        title << "ZenithSoC VGA | frame " << frame;
        if (signal_fps > 0.0)
            title << " | signal " << std::fixed << std::setprecision(2)
                  << signal_fps << " FPS";
        if (content_fps > 0.0)
            title << " | content " << std::fixed << std::setprecision(2)
                  << content_fps << " FPS";
        if (host_fps > 0.0)
            title << " | host " << std::fixed << std::setprecision(2)
                  << host_fps << " FPS";
        XStoreName(display, window, title.str().c_str());
    }

    bool poll(bool& redraw) {
        redraw = false;
        while (display && XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);

            if (event.type == Expose) {
                redraw = true;
            } else if (event.type == ClientMessage &&
                       static_cast<Atom>(event.xclient.data.l[0]) == wm_delete) {
                return true;
            } else if (event.type == KeyPress) {
                const KeySym key = XLookupKeysym(&event.xkey, 0);
                if (key == XK_Escape || key == XK_q || key == XK_Q)
                    return true;
            }
        }
        return false;
    }
#else
    bool open(unsigned) { return false; }
    void render(const std::vector<uint32_t>&) {}
    void set_title(uint64_t, double, double, double) {}
    bool poll(bool&) { return false; }
#endif
};

VgaReceiver::VgaReceiver(VgaReceiverOptions options)
    : options_(std::move(options)),
      frame_(kWidth * kHeight, 0xff000000U),
      host_window_start_(std::chrono::steady_clock::now()) {
    options_.scale = std::clamp(options_.scale, 1U, 4U);

    if (!options_.enabled)
        return;

    std::cout << "[VGA] receiver enabled: 640x480 RGB444, 25 MHz pixel stream\n";

    if (options_.live) {
        viewer_ = std::make_unique<Viewer>();
        if (viewer_->open(options_.scale)) {
            std::cout << "[VGA] live X11 window opened (scale="
                      << options_.scale << ")\n";
        } else {
            viewer_.reset();
            std::cerr << "[VGA] WARN: X11 display unavailable; continuing headless\n";
            if (options_.dump_path.empty()) {
                options_.dump_path = "out/vga_latest.png";
                std::cerr << "[VGA] latest complete frame will be written to "
                          << options_.dump_path << "\n";
            }
        }
    }
}

VgaReceiver::~VgaReceiver() {
    finish();
}

void VgaReceiver::reset_decoder() {
    have_previous_sync_ = false;
    horizontal_locked_ = false;
    vertical_locked_ = false;
    collecting_frame_ = false;
    visible_pixels_ = 0;
    phase_ = 0;
}

void VgaReceiver::sample(bool rst_n,
                         bool hsync,
                         bool vsync,
                         uint8_t red,
                         uint8_t green,
                         uint8_t blue) {
    if (!options_.enabled)
        return;

    ++samples_;
    if ((samples_ & 0xfffU) == 0)
        poll_viewer();

    if (!rst_n) {
        reset_decoder();
        return;
    }

    if (!have_previous_sync_) {
        previous_hsync_ = hsync;
        previous_vsync_ = vsync;
        have_previous_sync_ = true;
        return;
    }

    const bool hsync_falling = previous_hsync_ && !hsync;
    const bool hsync_rising = !previous_hsync_ && hsync;
    const bool vsync_falling = previous_vsync_ && !vsync;
    const bool vsync_rising = !previous_vsync_ && vsync;
    previous_hsync_ = hsync;
    previous_vsync_ = vsync;

    bool pixel_tick = false;
    const bool sync_edge = hsync_falling || hsync_rising ||
                           vsync_falling || vsync_rising;

    if (hsync_falling) {
        if (horizontal_locked_ && x_ != kHsyncStart)
            ++timing_errors_;
        horizontal_locked_ = true;
        x_ = kHsyncStart;
    } else if (hsync_rising) {
        if (horizontal_locked_ && x_ != kHsyncEnd)
            ++timing_errors_;
        horizontal_locked_ = true;
        x_ = kHsyncEnd;
    }

    if (vsync_falling) {
        if (vertical_locked_ && y_ != kVsyncStart)
            ++timing_errors_;
        vertical_locked_ = true;
        y_ = kVsyncStart;
    } else if (vsync_rising) {
        if (vertical_locked_ && y_ != kVsyncEnd)
            ++timing_errors_;
        vertical_locked_ = true;
        y_ = kVsyncEnd;
    }

    if (sync_edge) {
        phase_ = 0;
        pixel_tick = true;
    } else if (horizontal_locked_) {
        phase_ = (phase_ + 1) % kSystemClocksPerPixel;
        pixel_tick = phase_ == 0;
    }

    /* Keep horizontal phase running before VSYNC lock; otherwise every HSYNC
     * edge before the first vertical pulse would look like a short line. */
    if (pixel_tick && horizontal_locked_)
        consume_pixel(red, green, blue);
}

void VgaReceiver::consume_pixel(uint8_t red,
                                uint8_t green,
                                uint8_t blue) {
    if (vertical_locked_ && x_ == 0 && y_ == 0) {
        std::fill(frame_.begin(), frame_.end(), 0xff000000U);
        collecting_frame_ = true;
        visible_pixels_ = 0;
    }

    if (collecting_frame_ && x_ < kWidth && y_ < kHeight) {
        const uint32_t red8 = (red & 0xfU) * 17U;
        const uint32_t green8 = (green & 0xfU) * 17U;
        const uint32_t blue8 = (blue & 0xfU) * 17U;
        frame_[y_ * kWidth + x_] =
            0xff000000U | (red8 << 16) | (green8 << 8) | blue8;
        ++visible_pixels_;

        if (x_ == kWidth - 1 && y_ == kHeight - 1) {
            if (visible_pixels_ == kWidth * kHeight)
                complete_frame();
            else
                ++timing_errors_;
            collecting_frame_ = false;
        }
    }

    if (++x_ == kHorizontalTotal) {
        x_ = 0;
        if (++y_ == kVerticalTotal)
            y_ = 0;
    }
}

void VgaReceiver::complete_frame() {
    ++frame_count_;
    have_complete_frame_ = true;

    if (previous_frame_sample_ != 0) {
        const uint64_t frame_cycles = samples_ - previous_frame_sample_;
        if (frame_cycles != 0)
            signal_fps_ = 100000000.0 / static_cast<double>(frame_cycles);
    }
    previous_frame_sample_ = samples_;

    /* Count only visible image changes. This measures animation/buffer-swap
     * cadence even when VGA repeats the same framebuffer at the signal rate. */
    uint64_t frame_hash = 1469598103934665603ULL;
    for (const uint32_t pixel : frame_) {
        frame_hash ^= pixel;
        frame_hash *= 1099511628211ULL;
    }
    if (!have_frame_hash_) {
        previous_frame_hash_ = frame_hash;
        previous_content_sample_ = samples_;
        have_frame_hash_ = true;
    } else if (frame_hash != previous_frame_hash_) {
        const uint64_t content_cycles = samples_ - previous_content_sample_;
        if (content_cycles != 0)
            content_fps_ = 100000000.0 / static_cast<double>(content_cycles);
        previous_frame_hash_ = frame_hash;
        previous_content_sample_ = samples_;
    }

    const auto now = std::chrono::steady_clock::now();
    if (frame_count_ == 1) {
        host_window_start_ = now;
        host_window_start_frame_ = frame_count_;
    } else {
        const double elapsed =
            std::chrono::duration<double>(now - host_window_start_).count();
        if (elapsed > 0.0 && (host_fps_ == 0.0 || elapsed >= 0.5)) {
            host_fps_ = static_cast<double>(frame_count_ -
                                            host_window_start_frame_) /
                        elapsed;
            host_window_start_ = now;
            host_window_start_frame_ = frame_count_;
        }
    }

    if (viewer_) {
        viewer_->set_title(frame_count_, signal_fps_, content_fps_, host_fps_);
        viewer_->render(frame_);
    }

    if (!options_.dump_path.empty() && !write_png(options_.dump_path))
        std::cerr << "[VGA] WARN: cannot write " << options_.dump_path << "\n";

    if (frame_count_ == 1) {
        std::cout << "[VGA] received frame 1; FPS measurement started\n";
    } else if (frame_count_ == 2 || (frame_count_ % 10) == 0) {
        std::cout << "[VGA] received frame " << frame_count_
                  << ": signal=" << std::fixed << std::setprecision(2)
                  << signal_fps_ << " FPS, content=" << content_fps_
                  << " FPS, host=" << host_fps_
                  << " FPS\n";
    }
}

void VgaReceiver::poll_viewer() {
    if (!viewer_)
        return;

    bool redraw = false;
    if (viewer_->poll(redraw))
        quit_requested_ = true;
    if (redraw && have_complete_frame_)
        viewer_->render(frame_);
}

static void write_u32_be(std::ostream& output, uint32_t value) {
    const char bytes[4] = {
        static_cast<char>(value >> 24),
        static_cast<char>(value >> 16),
        static_cast<char>(value >> 8),
        static_cast<char>(value)
    };
    output.write(bytes, sizeof(bytes));
}

static bool write_png_chunk(std::ostream& output,
                            const char type[4],
                            const uint8_t* data,
                            uint32_t size) {
    write_u32_be(output, size);
    output.write(type, 4);
    if (size != 0)
        output.write(reinterpret_cast<const char*>(data), size);

    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(type), 4);
    if (size != 0)
        crc = crc32(crc, data, size);
    write_u32_be(output, static_cast<uint32_t>(crc));
    return static_cast<bool>(output);
}

bool VgaReceiver::write_png(const std::string& path) const {
    const std::string temporary = path + ".tmp";
    std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
    if (!output)
        return false;

    std::vector<uint8_t> scanlines(kHeight * (1 + kWidth * 3));
    for (unsigned y = 0; y < kHeight; ++y) {
        uint8_t* row = &scanlines[y * (1 + kWidth * 3)];
        row[0] = 0;
        for (unsigned x = 0; x < kWidth; ++x) {
            const uint32_t pixel = frame_[y * kWidth + x];
            row[1 + x * 3 + 0] = (pixel >> 16) & 0xff;
            row[1 + x * 3 + 1] = (pixel >> 8) & 0xff;
            row[1 + x * 3 + 2] = pixel & 0xff;
        }
    }

    uLongf compressed_size = compressBound(scanlines.size());
    std::vector<uint8_t> compressed(compressed_size);
    if (compress2(compressed.data(),
                  &compressed_size,
                  scanlines.data(),
                  scanlines.size(),
                  Z_BEST_SPEED) != Z_OK)
        return false;
    compressed.resize(compressed_size);

    static constexpr uint8_t signature[8] = {
        0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a
    };
    output.write(reinterpret_cast<const char*>(signature), sizeof(signature));

    uint8_t ihdr[13] = {
        static_cast<uint8_t>(kWidth >> 24),
        static_cast<uint8_t>(kWidth >> 16),
        static_cast<uint8_t>(kWidth >> 8),
        static_cast<uint8_t>(kWidth),
        static_cast<uint8_t>(kHeight >> 24),
        static_cast<uint8_t>(kHeight >> 16),
        static_cast<uint8_t>(kHeight >> 8),
        static_cast<uint8_t>(kHeight),
        8, 2, 0, 0, 0
    };

    if (!write_png_chunk(output, "IHDR", ihdr, sizeof(ihdr)) ||
        !write_png_chunk(output,
                         "IDAT",
                         compressed.data(),
                         compressed.size()) ||
        !write_png_chunk(output, "IEND", nullptr, 0))
        return false;

    output.close();

    if (!output)
        return false;
    if (std::rename(temporary.c_str(), path.c_str()) != 0) {
        std::remove(temporary.c_str());
        return false;
    }
    return true;
}

void VgaReceiver::finish() {
    if (finished_ || !options_.enabled)
        return;
    finished_ = true;

    poll_viewer();
    std::cout << "[VGA] summary: " << frame_count_
              << " complete frame(s), " << timing_errors_
              << " timing resynchronization(s)\n";
    if (signal_fps_ > 0.0) {
        std::cout << "[VGA] latest FPS: signal=" << std::fixed
                  << std::setprecision(2) << signal_fps_
                  << ", content=" << content_fps_
                  << ", host=" << host_fps_ << "\n";
    }

    if (!options_.dump_path.empty()) {
        if (have_complete_frame_)
            std::cout << "[VGA] latest frame: " << options_.dump_path << "\n";
        else
            std::cerr << "[VGA] WARN: no complete frame available for "
                      << options_.dump_path << "\n";
    }
}

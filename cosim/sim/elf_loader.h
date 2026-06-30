// ============================================================================
// Extracts 32-bit words from PT_LOAD segments to preload them into the
// DUT DDR/ROM through DPI. The entry point is used to align Spike.
// Intentionally minimal and self-contained, with no libfesvr dependency.
// ============================================================================

#ifndef COSIM_ELF_LOADER_H
#define COSIM_ELF_LOADER_H

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <utility>

struct ElfImage {
    uint32_t entry = 0;

    // Pairs: absolute byte address and 32-bit word.
    // Only 4-byte aligned words are emitted.
    std::vector<std::pair<uint32_t, uint32_t>> words;

    // HTIF "tohost" symbol address, or 0 if missing.
    uint32_t tohost = 0;

    // Generator's data_area[] symbol
    uint32_t data_area = 0;
    uint32_t data_area_size = 0;
};

inline bool load_elf(const std::string& path, ElfImage& out) {
    FILE* f = std::fopen(path.c_str(), "rb");

    if (!f) return false;

    auto rd = [&](void* dst, size_t n, long off) -> bool {
        if (std::fseek(f, off, SEEK_SET) != 0) return false;
        return std::fread(dst, 1, n, f) == n;
    };


    uint8_t ehdr[52];

    if (!rd(ehdr, sizeof(ehdr), 0)) {
        std::fclose(f);
        return false;
    }

    // Magic + class(1=32-bit) + data(1=little-endian)
    if (!(ehdr[0] == 0x7f && ehdr[1] == 'E' && ehdr[2] == 'L' && ehdr[3] == 'F')) {
        std::fclose(f);
        return false;
    }

    if (ehdr[4] != 1 || ehdr[5] != 1) {
        std::fclose(f);
        return false;
    } // ELF32 little-endian

    auto u16 = [](const uint8_t* p) {
        return (uint32_t) p[0] | ((uint32_t) p[1] << 8);
    };

    auto u32 = [](const uint8_t* p) {
        return (uint32_t)  p[0]
             | ((uint32_t) p[1] << 8)
             | ((uint32_t) p[2] << 16)
             | ((uint32_t) p[3] << 24);
    };

    out.entry          = u32(ehdr + 24);
    uint32_t phoff     = u32(ehdr + 28);
    uint32_t shoff     = u32(ehdr + 32);
    uint32_t phentsize = u16(ehdr + 42);
    uint32_t phnum     = u16(ehdr + 44);
    uint32_t shentsize = u16(ehdr + 46);
    uint32_t shnum     = u16(ehdr + 48);

    for (uint32_t i = 0; i < phnum; i++) {
        uint8_t ph[32];

        if (!rd(ph, sizeof(ph), phoff + (long)i * phentsize)) {
            std::fclose(f);
            return false;
        }

        uint32_t p_type   = u32(ph + 0);
        uint32_t p_offset = u32(ph + 4);
        uint32_t p_paddr  = u32(ph + 12);
        uint32_t p_filesz = u32(ph + 16);

        if (p_type != 1 || p_filesz == 0) continue;

        std::vector<uint8_t> buf(p_filesz);

        if (!rd(buf.data(), p_filesz, p_offset)) {
            std::fclose(f);
            return false;
        }

        // Emit 32-bit words. The last partial word is zero-padded.
        for (uint32_t off = 0; off < p_filesz; off += 4) {
            uint32_t w = 0;

            for (uint32_t b = 0; b < 4 && (off + b) < p_filesz; b++) {
                w |= (uint32_t)buf[off + b] << (8 * b);
            }

            out.words.emplace_back(p_paddr + off, w);
        }
    }

    // Search for the "tohost" symbol inside the symbol table (.symtab).
    if (shoff && shnum) {
        // Find .symtab (type=2) and its associated .strtab through sh_link.
        for (uint32_t i = 0; i < shnum; i++) {
            uint8_t sh[40];

            if (!rd(sh, sizeof(sh), shoff + (long)i * shentsize)) {
                break;
            }

            uint32_t sh_type = u32(sh + 4);

            if (sh_type != 2) {
                continue;
            }

            uint32_t sym_off  = u32(sh + 16);
            uint32_t sym_size = u32(sh + 20);
            uint32_t sym_link = u32(sh + 24);   // Index of the related string table
            uint32_t sym_ent  = u32(sh + 36);

            if (sym_ent == 0) sym_ent = 16;

            // Header of the associated string table
            uint8_t sth[40];

            if (!rd(sth, sizeof(sth), shoff + (long)sym_link * shentsize)) {
                break;
            }

            uint32_t str_off  = u32(sth + 16);
            uint32_t str_size = u32(sth + 20);

            std::vector<char> strtab(str_size);

            if (!rd(strtab.data(), str_size, str_off)) {
                break;
            }

            uint32_t nsyms = sym_size / sym_ent;

            for (uint32_t s = 0; s < nsyms; s++) {
                uint8_t syme[16];

                if (!rd(syme, sizeof(syme), sym_off + (long)s * sym_ent)) {
                    break;
                }

                uint32_t st_name  = u32(syme + 0);
                uint32_t st_value = u32(syme + 4);
                uint32_t st_size  = u32(syme + 8);

                if (st_name < str_size) {
                    const char* nm = &strtab[st_name];

                    if (std::string(nm) == "tohost") {
                        out.tohost = st_value;
                    } else {
                        out.data_area      = st_value;
                        out.data_area_size = st_size;
                    }
                }

                if (out.tohost && out.data_area) {
                    break;
                }
            }

            if (out.tohost && out.data_area) {
                break;
            }
        }
    }

    std::fclose(f);
    return true;
}

#endif 
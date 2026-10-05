#ifndef COSIM_COVERAGE_H
#define COSIM_COVERAGE_H

#include <cstdint>
#include <fstream>
#include <map>
#include <string>

// Counts only successfully compared instructions inside the generated stream.
// These are observed architectural events, not proof of RTL toggle coverage.
class Coverage {
public:
    std::map<std::string, uint64_t> bins;
    uint32_t line_bytes = 16;

    void instruction(uint32_t pc, uint32_t bits, unsigned length,
                     uint32_t next_pc, const std::string& mnemonic,
                     unsigned rd, uint32_t value) {
        ++bins["retired"];
        ++bins["opcode." + mnemonic];
        ++bins["length." + std::to_string(length)];
        if (length == 4 && (pc & 3) == 2) ++bins["fetch.cross_word"];
        if (rd) {
            ++bins["rd.x" + std::to_string(rd)];
            ++bins[value == 0 ? "result.zero" : value == 0xffffffff ? "result.ones" :
                   value == 0x80000000 ? "result.int_min" : "result.other"];
        }
        if (length != 4) {
            if (rd) last_write[rd] = bins["retired"];
            return;
        }
        const unsigned op = bits & 0x7f;
        const unsigned rs1 = (bits >> 15) & 31, rs2 = (bits >> 20) & 31;
        const bool read1 = op == 0x33 || op == 0x13 || op == 0x03 || op == 0x23 || op == 0x63 || op == 0x67 || op == 0x53;
        const bool read2 = op == 0x33 || op == 0x23 || op == 0x63 ||
                           (op == 0x53 && (bits >> 27) < 0x18);
        if (read1) source(rs1);
        if (read2) source(rs2);
        if (rd && read1 && rd == rs1) ++bins["alias.rd_rs1"];
        if (rd && read2 && rd == rs2) ++bins["alias.rd_rs2"];
        if (rd) last_write[rd] = bins["retired"];
        if (op == 0x63) {
            const std::string branch = "branch.funct3." + std::to_string((bits >> 12) & 7);
            ++bins[branch + (next_pc == pc + length ? ".not_taken" : ".taken")];
            if (next_pc < pc) ++bins["branch.backward"];
        }
        if (op == 0x53) ++bins["float.funct3." + std::to_string((bits >> 12) & 7)];
        if (op == 0x67) ++bins["jump.indirect"];
        if ((op == 0x33 || op == 0x13 || op == 0x03 || op == 0x53) && !rd)
            ++bins["destination.x0"];
    }

    void memory(bool store, uint32_t address, unsigned size, uint32_t base, uint32_t bytes) {
        const std::string kind = store ? "store" : "load";
        ++bins[kind + ".bytes." + std::to_string(size) + ".lane." + std::to_string(address & 3)];
        if (address >= base && address - base < bytes)
            ++bins["data.page." + std::to_string((address - base) / 4096)];
        if ((address % line_bytes) + size == line_bytes) ++bins[kind + ".line_end"];
        if (store) {
            last_store = address;
            last_store_size = size;
        } else if (last_store_size && address < uint64_t(last_store) + last_store_size &&
                   last_store < uint64_t(address) + size) {
            ++bins["load.overlaps_latest_store"];
        }
    }

    bool write(const std::string& path) const {
        if (path.empty()) return true;
        std::ofstream out(path);
        for (const auto& [key, value] : bins) out << key << ' ' << value << '\n';
        return bool(out);
    }

private:
    std::map<unsigned, uint64_t> last_write;
    uint32_t last_store = 0;
    unsigned last_store_size = 0;

    void source(unsigned reg) {
        if (!reg) ++bins["source.x0"];
        if (reg && last_write.count(reg)) {
            const auto distance = bins["retired"] - last_write[reg];
            ++bins["raw.distance." + (distance <= 4 ? std::to_string(distance) : "5plus")];
        }
    }
};
#endif

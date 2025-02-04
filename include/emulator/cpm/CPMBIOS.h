#pragma once

#include "RegistersBase.h"
#include "MemoryBase.h"
#include "TerminalBase.h"
#include "DiskControllerBase.h"

#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>
#include <utility>

#define CCP         0xD400
#define BDOS        0xDC00
#define CBIOS       0xEA00
#define HDDISKS     2
#define STACK       0xEF00

enum class CPMVersion {
    v1,
    v2,
    v3,
    UNKNOWN
};

enum BIOSCALL : uint16_t {
    BOOT       =  0,
    WBOOT      =  1,
    CONST      =  2,
    CONIN      =  3,
    CONOUT     =  4,
    LIST       =  5,
    PUNCH      =  6,
    READER     =  7,
    HOME       =  8,
    SELDSK     =  9,
    SETTRK     = 10,
    SETSEC     = 11,
    SETDMA     = 12,
    READ       = 13,
    WRITE      = 14,
    // CP/M 2
    LISTST     = 15,
    SECTRAN    = 16,
    // CP/M 3
    CONOST     = 17,
    AUXIST     = 18,
    AUXOST     = 19,
    DEVTBL     = 20,
    DEVINI     = 21,
    DRVTBL     = 22,
    MULTIO     = 23,
    MOVE       = 24,
    TIME       = 25,
    SELMEM     = 26,
    SETBNK     = 27,
    XMOVE      = 28,
    USERF      = 29,
    RESERV1    = 30,
    RESERV2    = 31
};

#define HAS_BIOS_FUNCTION(functionName, signature) \
template <typename T, typename = void> \
struct has_##functionName##_available : std::false_type {}; \
 \
template <typename T> \
struct has_##functionName##_available<T, std::void_t< \
    decltype(std::declval<T>().functionName signature)>> \
        : std::true_type {};

HAS_BIOS_FUNCTION(BOOT, ())
HAS_BIOS_FUNCTION(WBOOT, ())
HAS_BIOS_FUNCTION(CONST, ())
HAS_BIOS_FUNCTION(CONIN, ())
HAS_BIOS_FUNCTION(CONOUT, ())
HAS_BIOS_FUNCTION(LIST, ())
HAS_BIOS_FUNCTION(PUNCH, ())
HAS_BIOS_FUNCTION(READER, ())
HAS_BIOS_FUNCTION(HOME, ())
HAS_BIOS_FUNCTION(SELDSK, ())
HAS_BIOS_FUNCTION(SETTRK, ())
HAS_BIOS_FUNCTION(SETSEC, ())
HAS_BIOS_FUNCTION(SETDMA, ())
HAS_BIOS_FUNCTION(READ, ())
HAS_BIOS_FUNCTION(WRITE, ())
HAS_BIOS_FUNCTION(LISTST, ())
HAS_BIOS_FUNCTION(SECTRAN, ())
HAS_BIOS_FUNCTION(CONOST, ())
HAS_BIOS_FUNCTION(AUXIST, ())
HAS_BIOS_FUNCTION(AUXOST, ())
HAS_BIOS_FUNCTION(DEVTBL, ())
HAS_BIOS_FUNCTION(DEVINI, ())
HAS_BIOS_FUNCTION(DRVTBL, ())
HAS_BIOS_FUNCTION(MULTIO, ())
HAS_BIOS_FUNCTION(MOVE, ())
HAS_BIOS_FUNCTION(TIME, ())
HAS_BIOS_FUNCTION(SELMEM, ())
HAS_BIOS_FUNCTION(SETBNK, ())
HAS_BIOS_FUNCTION(XMOVE, ())
HAS_BIOS_FUNCTION(USERF, ())
HAS_BIOS_FUNCTION(RESERV1, ())
HAS_BIOS_FUNCTION(RESERV2, ())

#define BIOS_CALL_FUNCTION(functionName) \
    if constexpr(has_##functionName##_available<Derived>::value) { \
        static_cast<Derived*>(this)->functionName(); \
    } else { \
        functionName(); \
    }

template <typename RegistersType, typename MemoryType, typename TerminalType, typename DiskControllerType>
class CPMBIOS : public BIOSBase<CPMBIOS<RegistersType, MemoryType, TerminalType, DiskControllerType>> {
    public:
        CPMBIOS(RegistersType* registers, MemoryType* memory, TerminalType* terminal, DiskControllerType* diskController, CPMVersion version = CPMVersion::UNKNOWN)
            : registers(registers), memory(memory), terminal(terminal), diskController(diskController), version(version) {}

        void call(BIOSCALL callNumber) {
            switch(callNumber) {
                case BIOSCALL::BOOT: BIOS_CALL_FUNCTION(BOOT); break;
                case BIOSCALL::WBOOT: BIOS_CALL_FUNCTION(WBOOT); break;
                case BIOSCALL::CONST: BIOS_CALL_FUNCTION(CONST); break;
                case BIOSCALL::CONIN: BIOS_CALL_FUNCTION(CONIN); break;
                case BIOSCALL::CONOUT: BIOS_CALL_FUNCTION(CONOUT); break;
                case BIOSCALL::LIST: BIOS_CALL_FUNCTION(LIST); break;
                case BIOSCALL::PUNCH: BIOS_CALL_FUNCTION(PUNCH); break;
                case BIOSCALL::READER: BIOS_CALL_FUNCTION(READER); break;
                case BIOSCALL::HOME: BIOS_CALL_FUNCTION(HOME); break;
                case BIOSCALL::SELDSK: BIOS_CALL_FUNCTION(SELDSK); break;
                case BIOSCALL::SETTRK: BIOS_CALL_FUNCTION(SETTRK); break;
                case BIOSCALL::SETSEC: BIOS_CALL_FUNCTION(SETSEC); break;
                case BIOSCALL::SETDMA: BIOS_CALL_FUNCTION(SETDMA); break;
                case BIOSCALL::READ: BIOS_CALL_FUNCTION(READ); break;
                case BIOSCALL::WRITE: BIOS_CALL_FUNCTION(WRITE); break;
                case BIOSCALL::LISTST: BIOS_CALL_FUNCTION(LISTST); break;
                case BIOSCALL::SECTRAN: BIOS_CALL_FUNCTION(SECTRAN); break;
                case BIOSCALL::CONOST: BIOS_CALL_FUNCTION(CONOST); break;
                case BIOSCALL::AUXIST: BIOS_CALL_FUNCTION(AUXIST); break;
                case BIOSCALL::AUXOST: BIOS_CALL_FUNCTION(AUXOST); break;
                case BIOSCALL::DEVTBL: BIOS_CALL_FUNCTION(DEVTBL); break;
                case BIOSCALL::DEVINI: BIOS_CALL_FUNCTION(DEVINI); break;
                case BIOSCALL::DRVTBL: BIOS_CALL_FUNCTION(DRVTBL); break;
                case BIOSCALL::MULTIO: BIOS_CALL_FUNCTION(MULTIO); break;
                case BIOSCALL::MOVE: BIOS_CALL_FUNCTION(MOVE); break;
                case BIOSCALL::TIME: BIOS_CALL_FUNCTION(TIME); break;
                case BIOSCALL::SELMEM: BIOS_CALL_FUNCTION(SELMEM); break;
                case BIOSCALL::SETBNK: BIOS_CALL_FUNCTION(SETBNK); break;
                case BIOSCALL::XMOVE: BIOS_CALL_FUNCTION(XMOVE); break;
                case BIOSCALL::USERF: BIOS_CALL_FUNCTION(USERF); break;
                case BIOSCALL::RESERV1: BIOS_CALL_FUNCTION(RESERV1); break;
                case BIOSCALL::RESERV2: BIOS_CALL_FUNCTION(RESERV2); break;
                default: throw std::runtime_error("Invalid BIOS call number");
            }
        }

        void setCPMVersion(CPMVersion version) { this->version = version; }
        CPMVersion getCPMVersion() const { return version; }

        void setResetCPUCallback(std::function<void()> callback) { resetCPUCallback = callback; }
        bool hasResetCPUCallback() const { return resetCPUCallback != nullptr; }

        void BOOT() {
            if(hasResetCPUCallback()) {
                resetCPUCallback();
            }
        }

        void WBOOT() {
            if(hasResetCPUCallback()) {
                resetCPUCallback();
            }
        }
        
        void CONST() {
            registers->A = terminal->getStatus() ? 1 : 0;
        }

        void CONIN() {
            registers->A = terminal->getLastChar();
        }

        void CONOUT() {
            terminal->printChar(registers->C);
        }

        void LIST() {
            terminal->printChar(registers->C);
        }

        void PUNCH() {
            terminal->printChar(registers->C);
        }

        void READER() {
            registers->A = 26; // ^Z character, device not present
        }

        void HOME() {
            diskController->setCurrentDrive(0);
            diskController->setCurrentSector(1);
        }

        void SELDSK() {
            diskController->setCurrentDrive(registers->C);
            if(registers->C < HDDISKS) {
                throw std::runtime_error("Invalid drive number");
            }
            if(isBitSet(registers->E, 0)) {
                // new disk loaded, need to reload disk controller
            } else {
                // disk already loaded, return DPH in HL, if disk is not present, return 0 in HL
            }
        }

        void SETTRK() {
            if(version == CPMVersion::v1) {
                diskController->setCurrentTrack(registers->C());
            } else {
                diskController->setCurrentTrack(registers->BC());
            }
        }

        void SETSEC() {
            if(version == CPMVersion::v1) {
                diskController->setCurrentSector(registers->C());
            } else {
                diskController->setCurrentSector(registers->BC());
            }
        }

        void SETDMA() {
            dmaAddress = registers->BC();
        }

        void READ() {
            std::vector<uint8_t> sectorData = diskController->readSector();
            if(sectorData.size() == 0) {
                registers->A = 0x01;
                // there needs to also be 0xFF if media changed
            } else {
                registers->A = 0x00;
            }
            memory->write(dmaAddress, diskController->read());
        }

        void WRITE() {
            bool result = diskController->writeSector(memory->read(dmaAddress));
            if(result) {
                registers->A = 0x00;
            } else {
                registers->A = 0x01;
                // there needs to also be 0xFF if media changed
                // and 0x02 if read only media
            }
        }

        void LISTST() {
            registers->A = 0x00;
        }

        void SECTRAN() {
            uint16_t sector = registers->BC();
            if(diskController->getCurrentDrive() < HDDISKS) {
                registers->HL(sectorTranslation[sector + 1]);
            } else {
                registers->HL(sectorTranslation[sector]);
            }
        }

        void CONOST() {
            registers->A = 0xFF;
        }

        void AUXIST() {
            registers->A = 0xFF;
        }

        void AUXOST() {}
    protected:
        RegistersType* registers;
        MemoryType* memory;
        TerminalType* terminal;
        DiskControllerType* diskController;
        CPMVersion version;
        uint16_t dmaAddress;

        const uint8_t sectorTranslation[] = {
            1, 7, 13, 19, 25, 5, 11, 17, 23, 3, 9, 15, 21, 
            2, 8, 14, 20, 26, 6, 12, 18, 24, 4, 10, 16, 22
        };

        std::function<void()> resetCPUCallback;

        void bootSystem() {};
        void readClock() {};
};

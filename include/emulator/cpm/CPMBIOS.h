#pragma once

#include "RegistersBase.h"
#include "BIOSBase.h"
#include "MemoryBase.h"
#include "CPMDeviceTable.h"
#include "IOBase.h"
#include "BitOps.h"

#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>
#include <utility>
#include <functional>

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
    static_cast<std::decay_t<decltype(*this)>*>(this)->functionName();

class CPMBIOS : public BIOSBase {
    public:
        CPMBIOS(RegistersBase* registers, MemoryBase* memory, IOBase* io) : registers(registers), memory(memory), io(io) {}

        void call(CPMBIOSCALL callNumber) override {
            switch(callNumber) {
                case CPMBIOSCALL::BOOT: BIOS_CALL_FUNCTION(BOOT); break;
                case CPMBIOSCALL::WBOOT: BIOS_CALL_FUNCTION(WBOOT); break;
                case CPMBIOSCALL::CONST: BIOS_CALL_FUNCTION(CONST); break;
                case CPMBIOSCALL::CONIN: BIOS_CALL_FUNCTION(CONIN); break;
                case CPMBIOSCALL::CONOUT: BIOS_CALL_FUNCTION(CONOUT); break;
                case CPMBIOSCALL::LIST: BIOS_CALL_FUNCTION(LIST); break;
                case CPMBIOSCALL::PUNCH: BIOS_CALL_FUNCTION(PUNCH); break;
                case CPMBIOSCALL::READER: BIOS_CALL_FUNCTION(READER); break;
                case CPMBIOSCALL::HOME: BIOS_CALL_FUNCTION(HOME); break;
                case CPMBIOSCALL::SELDSK: BIOS_CALL_FUNCTION(SELDSK); break;
                case CPMBIOSCALL::SETTRK: BIOS_CALL_FUNCTION(SETTRK); break;
                case CPMBIOSCALL::SETSEC: BIOS_CALL_FUNCTION(SETSEC); break;
                case CPMBIOSCALL::SETDMA: BIOS_CALL_FUNCTION(SETDMA); break;
                case CPMBIOSCALL::READ: BIOS_CALL_FUNCTION(READ); break;
                case CPMBIOSCALL::WRITE: BIOS_CALL_FUNCTION(WRITE); break;
                case CPMBIOSCALL::LISTST: BIOS_CALL_FUNCTION(LISTST); break;
                case CPMBIOSCALL::SECTRAN: BIOS_CALL_FUNCTION(SECTRAN); break;
                case CPMBIOSCALL::CONOST: BIOS_CALL_FUNCTION(CONOST); break;
                case CPMBIOSCALL::AUXIST: BIOS_CALL_FUNCTION(AUXIST); break;
                case CPMBIOSCALL::AUXOST: BIOS_CALL_FUNCTION(AUXOST); break;
                case CPMBIOSCALL::DEVTBL: BIOS_CALL_FUNCTION(DEVTBL); break;
                case CPMBIOSCALL::DEVINI: BIOS_CALL_FUNCTION(DEVINI); break;
                case CPMBIOSCALL::DRVTBL: BIOS_CALL_FUNCTION(DRVTBL); break;
                case CPMBIOSCALL::MULTIO: BIOS_CALL_FUNCTION(MULTIO); break;
                case CPMBIOSCALL::MOVE: BIOS_CALL_FUNCTION(MOVE); break;
                case CPMBIOSCALL::TIME: BIOS_CALL_FUNCTION(TIME); break;
                case CPMBIOSCALL::SELMEM: BIOS_CALL_FUNCTION(SELMEM); break;
                case CPMBIOSCALL::SETBNK: BIOS_CALL_FUNCTION(SETBNK); break;
                case CPMBIOSCALL::XMOVE: BIOS_CALL_FUNCTION(XMOVE); break;
                case CPMBIOSCALL::USERF: BIOS_CALL_FUNCTION(USERF); break;
                case CPMBIOSCALL::RESERV1: BIOS_CALL_FUNCTION(RESERV1); break;
                case CPMBIOSCALL::RESERV2: BIOS_CALL_FUNCTION(RESERV2); break;
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
                deviceTable->createDeviceTable();
            }
        }

        void WBOOT() {
            if(hasResetCPUCallback()) {
                resetCPUCallback();
                deviceTable->createDeviceTable();
            }
        }
        
        void CONST() {
            registers->A = io->getTerminal().getStatus() ? 1 : 0;
        }

        void CONIN() {
            registers->A = io->getTerminal().getLastChar();
        }

        void CONOUT() {
            io->getTerminal().printChar(registers->C);
        }

        void LIST() {
            io->getTerminal().printChar(registers->C);
        }

        void PUNCH() {
            io->getTerminal().printChar(registers->C);
        }

        void READER() {
            registers->A = 26; // ^Z character, device not present
        }

        void HOME() {
            io->getDiskController().setCurrentDrive(0);
            io->getDiskController().setCurrentSector(1);
        }

        void SELDSK() {
            io->getDiskController().setCurrentDrive(registers->C);
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
                io->getDiskController().setCurrentTrack(registers->C());
            } else {
                io->getDiskController().setCurrentTrack(registers->BC());
            }
        }

        void SETSEC() {
            if(version == CPMVersion::v1) {
                io->getDiskController().setCurrentSector(registers->C());
            } else {
                io->getDiskController().setCurrentSector(registers->BC());
            }
        }

        void SETDMA() {
            dmaAddress = registers->BC();
        }

        void READ() {
            std::vector<uint8_t> sectorData = io->getDiskController().readSector();
            if(sectorData.size() == 0) {
                registers->A = 0x01;
                // there needs to also be 0xFF if media changed
            } else {
                registers->A = 0x00;
            }
            memory->write(dmaAddress, io->getDiskController().read());
        }

        void WRITE() {
            bool result = io->getDiskController().writeSector(memory->read(dmaAddress));
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
            if(io->getDiskController().getCurrentDrive() < HDDISKS) {
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

        void AUXOST() {
            registers->A = 0xFF;
        }

        void DEVTBL() {
            registers->A = 0xFF;
        }

        void DEVINI() {
            registers->A = 0xFF;
        }

        void DRVTBL() {
            registers->A = 0xFF;
        }

        void MULTIO() {
            registers->A = 0xFF;
        }

        void MOVE() {
            registers->A = 0xFF;
        }

        void TIME() {
            registers->A = 0xFF;
        }

        void SELMEM() {
            registers->A = 0xFF;
        }

        void SETBNK() {
            registers->A = 0xFF;
        }

        void XMOVE() {
            registers->A = 0xFF;
        }

        void USERF() {
            registers->A = 0xFF;
        }

        void RESERV1() {
            registers->A = 0xFF;
        }

        void RESERV2() {
            registers->A = 0xFF;
        }
        
    protected:
        RegistersBase* registers;
        MemoryBase* memory;
        IOBase* io;
        CPMVersion version;
        uint16_t dmaAddress;
        std::shared_ptr<CPMDeviceTable> deviceTable;

        std::array<uint8_t, 26> sectorTranslation = {
            1, 7, 13, 19, 25, 5, 11, 17, 23, 3,  9, 15, 21, 
            2, 8, 14, 20, 26, 6, 12, 18, 24, 4, 10, 16, 22
        };

        std::function<void()> resetCPUCallback;

        void bootSystem() {};
        void readClock() {};
};

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <unistd.h>
#include "dosbox.h"
#include "debug.h"
#include "cpu.h"
#include "video.h"
#include "pic.h"
#include "cpu.h"
#include "ide.h"
#include "callback.h"
#include "inout.h"
#include "mixer.h"
#include "timer.h"
#include "dos_inc.h"
#include "setup.h"
#include "control.h"
#include "cross.h"
#include "programs.h"
#include "support.h"
#include "mapper.h"
#include "ints/int10.h"
#include "menu.h"
#include "render.h"
#include "pci_bus.h"
#include "parport.h"
#include "clockdomain.h"
#include "zip.h"
#include "unzip.h"
#include "ioapi.h"
#include "shell.h"
#include "build_timestamp.h"
#include "DBXMemoryDomains.h"
#include "mem.h"
#include "cpu.h"
#include "include/vga.h"
#include "voodoo.h"
//#include "Core/PowerPC/Jit64Common/Jit64PowerPCState.h"

//using namespace cli;
using namespace System;
using namespace RTCV;
using namespace NetCore;
using namespace Vanguard;
using namespace Runtime::InteropServices;
using namespace Diagnostics;

#using <System.dll>
#define MEM_SIZE MEM_TotalPages()
#define CPU_SIZE 21108393
#define VGA_SIZE VGA_Memory_t::memsize;
#define VOODOO_SIZE 

#define MEM_OFFSET
#define CPU_OFFSET 
#define VGA_OFFSET 
#define VOODOO_OFFSET 
#define WORD_SIZE 1
#define BIG_ENDIAN false

// MEMORY OFFSETS ARE ABSOLUTE
// THIS IS CURRENTLY BEING HANDLED ON THE DOLPHIN SIDE
// SRAM = 0x80000000-0x817FFFFF
// EXRAM = 0x90000000-0x93FFFFFF

// ARAM = 0x80000000-0x81000000
// ARAM HAS TO BE WRITTEN TO WITH A DIFFERENT FUNCTION THAN THE OTHERS

// Memory::Write_U8 and Memory::Read_U8 for SRAM and EXRAM
// DSP::ReadARAM and DSP::WriteAram for ARAM

//void MessageDelegate(Object*);

std::string* Memory::Name::get()
{
    return "Memory";
}

long long SRAM::Size::get()
{
    return SRAM_SIZE;
}

int SRAM::WordSize::get()
{
    return WORD_SIZE;
}

bool SRAM::BigEndian::get()
{
    return BIG_ENDIAN;
}

unsigned char SRAM::PeekByte(long long addr)
{
    if(addr < SRAM_SIZE)
    {
        // Convert the address
        addr += SRAM_OFFSET;
        return Memory::Read_U8(static_cast<u32>(addr));
    }
    return 0;
}

array<unsigned char>^ SRAM::PeekBytes(long long address, int length)
{
    array<unsigned char>^ bytes = gcnew array<unsigned char>(length);
    for(int i = 0; i < length; i++)
    {
        bytes[i] = PeekByte(address + i);
    }
    return bytes;
}

void SRAM::PokeByte(long long addr, unsigned char val)
{
    if(addr < SRAM_SIZE)
    {
        // Convert the address
        addr += SRAM_OFFSET;
        Memory::Write_U8(val, static_cast<u32>(addr));
        PowerPC::ppcState.iCache.Invalidate(addr);
    }
}

String^ EXRAM::Name::get()
{
    return "EXRAM";
}

long long EXRAM::Size::get()
{
    return EXRAM_SIZE;
}

int EXRAM::WordSize::get()
{
    return WORD_SIZE;
}

bool EXRAM::BigEndian::get()
{
    return BIG_ENDIAN;
}

unsigned char EXRAM::PeekByte(long long addr)
{
    if(addr < EXRAM_SIZE)
    {
        // Convert the address
        addr += EXRAM_OFFSET;
        return Memory::Read_U8(static_cast<u32>(addr));
    }
    return 0;
}

array<unsigned char>^ EXRAM::PeekBytes(long long address, int length)
{
    array<unsigned char>^ bytes = gcnew array<unsigned char>(length);
    for(int i = 0; i < length; i++)
    {
        bytes[i] = PeekByte(address + i);
    }
    return bytes;
}

void EXRAM::PokeByte(long long addr, unsigned char val)
{
    if(addr < EXRAM_SIZE)
    {
        // Convert the address
        addr += EXRAM_OFFSET;
        Memory::Write_U8(val, static_cast<u32>(addr));
        PowerPC::ppcState.iCache.Invalidate(addr);
    }
}

String^ ARAM::Name::get()
{
    return "ARAM";
}

long long ARAM::Size::get()
{
    return ARAM_SIZE;
}

int ARAM::WordSize::get()
{
    return WORD_SIZE;
}

bool ARAM::BigEndian::get()
{
    return BIG_ENDIAN;
}

unsigned char ARAM::PeekByte(long long addr)
{
    if(addr < ARAM_SIZE)
    {
        // Convert the address
        addr += ARAM_OFFSET;
        return DSP::ReadARAM(static_cast<u32>(addr));
    }
    return 0;
}

array<unsigned char>^ ARAM::PeekBytes(long long address, int length)
{
    array<unsigned char>^ bytes = gcnew array<unsigned char>(length);
    for(int i = 0; i < length; i++)
    {
        bytes[i] = PeekByte(address + i);
    }
    return bytes;
}

void ARAM::PokeByte(long long addr, unsigned char val)
{
    if(addr < ARAM_SIZE)
    {
        // Convert the address
        addr += ARAM_OFFSET;
        DSP::WriteARAM(val, static_cast<u32>(addr));
    }
}

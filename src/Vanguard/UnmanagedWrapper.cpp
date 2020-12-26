#pragma once

#include "UnmanagedWrapper.h"
#include "include/dos_system.h"
#include "dos_inc.h"
#include "Vanguard/VanguardClient.h"
#include <chrono>
#include "mem.h"

#include <cpu.h>
#include <mixer.h>
#include <vga.h>
#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "debug.h"
#include "src/debug/debug_inc.h"
#include "src/debug/disasm_tables.h"
#include "paging.h"
#include <src/hardware/mame/emu.h>
//#include <src/dos/dos_execute.cpp>

//C++/CLI has various restrictions (no std::mutex for example), so we can't actually include certain headers directly
//What we CAN do is wrap those functions

bool UnmanagedWrapper::savestate_done = false;

void UnmanagedWrapper::VANGUARD_EXIT() {
    System::Environment::Exit(0);
    return;
}

std::string GetSaveStatePath() {
    return "";
}


bool UnmanagedWrapper::IS_N3DS() {
    return false;
}

std::string UnmanagedWrapper::VANGUARD_GETGAMENAME() {
    std::string title;
    //title = RunningProgram;
    DOS_MCB mcb(dos.psp()-1);
    static char name[9];
    mcb.GetFileName(name);
    name[8] = 0;
    if(!strlen(name)) strcpy(name, "DOSBOX-X");
    for(Bitu i = 0; i < 8; i++) { //Don't put garbage in the title bar. Mac OS X doesn't like it
        if(name[i] == 0) break;
        if(!isprint(*reinterpret_cast<unsigned char*>(&name[i]))) name[i] = '?';
    }
    title = name;
    return title;
}

void UnmanagedWrapper::VANGUARD_LOADSTATE(const std::string& file) {
    ////u64 title_id;
    //if (Core::System::GetInstance().GetAppLoader().ReadProgramId(title_id) == Loader::ResultStatus::
    //    Success) {

    //    if (!FileUtil::Copy(file, GetSaveStatePath(title_id, 9))) {
    //        LOG_ERROR(Core, "Could not copy file for loadstate{}", file);
    //        return;
    //    }
    //}
    //Core::System::GetInstance().SendSignal(Core::System::Signal::LoadVanguard, 9);
    //SaveState::save(SaveState::SLOT_COUNT);
    //SaveState::instance().load(1);
    SaveState::instance().load(SaveState::SLOT_COUNT * SaveState::MAX_PAGE - 1, file);
    return;
}

std::string UnmanagedWrapper::VANGUARD_SAVESTATE(const std::string& file) {

    //Core::System::GetInstance().SendSignal(Core::System::Signal::Save, 9);

    //u64 title_id;
    //if (Core::System::GetInstance().GetAppLoader().ReadProgramId(title_id) ==
    //    Loader::ResultStatus::Success) {

    //    /*if (!FileUtil::Copy(GetSaveStatePath(title_id, 9), file)) {
    //        LOG_ERROR(Core, "Could not copy file for savestate{}", file);
    //        return;
    //    }*/
    //    return GetSaveStatePath(title_id, 9);
    //}

    std::string path = SaveState::instance().save(SaveState::SLOT_COUNT * SaveState::MAX_PAGE - 1);
    const std::string& filetemp = file + ".temp";
    //System::IO::FileStream ^ fs = System::IO::File::OpenWrite(gcnew System::String(filetemp.c_str()));
    //fs->Close();
    return path;

}

void UnmanagedWrapper::VANGUARD_LOADSTATE_DONE() {

    VanguardClientUnmanaged::LOAD_STATE_DONE();
}
void UnmanagedWrapper::VANGUARD_SAVESTATE_DONE() {

    VanguardClientUnmanaged::SAVE_STATE_DONE();
}

void UnmanagedWrapper::PADDR_POKEBYTE(long long addr, unsigned char val, long offset) {
    /*u8* ptr = Core::System::GetInstance().Memory().GetPhysicalRef(offset).GetPtr();*//*
    u8* ptr = (u8*)mem_readb(PAGING_GetPhysicalAddress((PhysPt)offset));*/
    //u8* ptr = (u8)phys_readb((PhysPt)offset);
    //unsigned char* dst = ptr + addr;
    phys_writeb((PhysPt)addr, val);
    /*Core::System::GetInstance().InvalidateCacheRange(reinterpret_cast<u32>(dst), 1);*/
    /*if (VideoCore::g_renderer != nullptr) {
        VideoCore::g_renderer->Rasterizer()->InvalidateRegion(reinterpret_cast<PAddr>(dst), 1);
    }*/
    return;
}

unsigned char UnmanagedWrapper::PADDR_PEEKBYTE(long long addr, long offset) {/*
    u8* ptr = (u8*)mem_readb(PAGING_GetPhysicalAddress((PhysPt)addr));*/
    //u8* ptr = (u8*)phys_readb((PhysPt)offset);
    u8 value;
    //std::memcpy(&value, ptr + addr, sizeof(u8));
    value = (u8)phys_readb((PhysPt)addr);
    return value;
}

std::string UnmanagedWrapper::VANGUARD_SAVECONFIG() {
    return "";
}

void UnmanagedWrapper::VANGUARD_LOADCONFIG(std::string cfg) {
}

void UnmanagedWrapper::VANGUARD_CORESTEP() {
    VanguardClientUnmanaged::CORE_STEP();
}

void UnmanagedWrapper::LOAD_STATE_DONE() {

    VanguardClientUnmanaged::LOAD_STATE_DONE();
}

//static GMainWindow* GetMainWindow() {
//    for (QWidget* w : qApp->topLevelWidgets()) {
//        if (GMainWindow* main = qobject_cast<GMainWindow*>(w)) {
//            return main;
//        }
//    }
//    return nullptr;
//}

void UnmanagedWrapper::VANGUARD_PAUSEEMULATION() {
    ;
}
void UnmanagedWrapper::VANGUARD_RESUMEEMULATION() {
    ;
}

void UnmanagedWrapper::VANGUARD_STOPGAME() {
    ;
}
void UnmanagedWrapper::VANGUARD_LOADGAME(const std::string& file) {
    ;
}

VanguardSettingsUnmanaged UnmanagedWrapper::nSettings{};
void UnmanagedWrapper::GetSettingsFromDOSBox() {
    /*nSettings.is_new_3ds = Settings::values.is_new_3ds;
    nSettings.region_value = Settings::values.region_value;
    nSettings.init_clock = (u32)Settings::InitClock::FixedTime;
    nSettings.init_time = Settings::values.init_time;
    nSettings.shaders_accurate_mul = Settings::values.shaders_accurate_mul;
    nSettings.upright_screen = Settings::values.upright_screen;
    nSettings.enable_dsp_lle = Settings::values.enable_dsp_lle;
    nSettings.enable_dsp_lle_multithread = Settings::values.enable_dsp_lle_multithread;*/
}
void UnmanagedWrapper::SetSettingsFromUnmanagedWrapper() {
    /*Settings::values.is_new_3ds = nSettings.is_new_3ds;
    Settings::values.region_value = nSettings.region_value;
    Settings::values.init_clock = Settings::InitClock::FixedTime;
    Settings::values.init_time = nSettings.init_time;
    Settings::values.shaders_accurate_mul = nSettings.shaders_accurate_mul;
    Settings::values.upright_screen = nSettings.upright_screen;
    Settings::values.enable_dsp_lle = nSettings.enable_dsp_lle;
    Settings::values.enable_dsp_lle_multithread = nSettings.enable_dsp_lle_multithread;*/
}

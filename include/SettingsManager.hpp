#ifndef UE4SS_SETTINGSMANAGER_HPP
#define UE4SS_SETTINGSMANAGER_HPP

#include <cstdint>
#include <filesystem>

#include <File/File.hpp>

namespace RC
{
    class SettingsManager
    {
    public:
        struct SectionOverrides
        {
            File::StringType ModsFolderPath{};
        } Overrides;

        struct SectionGeneral
        {
            bool EnableHotReloadSystem{};
            bool InvalidateCacheIfDLLDiffers{true};
            bool EnableDebugKeyBindings{false};
            int64_t MaxScanAttemptsNormal{20};
            int64_t MaxScanAttemptsModular{2000};
        } General;

        struct SectionEngineVersionOverride
        {
            int64_t MajorVersion{-1};
            int64_t MinorVersion{-1};
        } EngineVersionOverride;

        struct SectionObjectDumper
        {
            bool LoadAllAssetsBeforeDumpingObjects{};
        } ObjectDumper;

        struct SectionCXXHeaderGenerator
        {
            bool DumpOffsetsAndSizes{};
            bool KeepMemoryLayout{};
            bool LoadAllAssetsBeforeGeneratingCXXHeaders{};
        } CXXHeaderGenerator;

        struct SectionUHTHeaderGenerator
        {
            bool IgnoreAllCoreEngineModules{};
            bool IgnoreEngineAndCoreUObject{true};
            bool MakeAllFunctionsBlueprintCallable{};
            bool MakeAllPropertyBlueprintsReadWrite{};
            bool MakeEnumClassesBlueprintType{};
        } UHTHeaderGenerator;

        struct SectionDebug
        {
            bool ConsoleEnabled{true};
        } Debug;

        struct SectionThreads
        {
            int64_t SigScannerNumThreads{8};
            int64_t SigScannerMultithreadingModuleSizeThreshold{16777216};
        } Threads;

    public:
        SettingsManager() = default;

    public:
        auto deserialize(std::filesystem::path& file_name) -> void;
    };
}

#endif //UE4SS_SETTINGSMANAGER_HPP

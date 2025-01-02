-- premake5.lua
workspace "XpObjectMaster"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "XpObjectMaster"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "XpObjectMaster"
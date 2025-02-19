cmake_minimum_required(VERSION 3.28.1)

project(VUTD)

add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/Main.c
    ${PROJECT_SOURCE_DIR}/Reflection.h
    ${PROJECT_SOURCE_DIR}/Reflection.c
    ${PROJECT_SOURCE_DIR}/TPair.h
    ${PROJECT_SOURCE_DIR}/Utility.h
    ${PROJECT_SOURCE_DIR}/Utility.c
    ${PROJECT_SOURCE_DIR}/TGeneric.h
    ${PROJECT_SOURCE_DIR}/TGeneric.c
    ${PROJECT_SOURCE_DIR}/TObject.h
    ${PROJECT_SOURCE_DIR}/TObject.c
    ${PROJECT_SOURCE_DIR}/Vutd_Macros.h
    ${PROJECT_SOURCE_DIR}/TString.h
    ${PROJECT_SOURCE_DIR}/TString.c
    ${PROJECT_SOURCE_DIR}/Containers/TIterator.h
    ${PROJECT_SOURCE_DIR}/Containers/TIterator.c
    ${PROJECT_SOURCE_DIR}/Containers/It_Array.h
    ${PROJECT_SOURCE_DIR}/Containers/It_Array.c
    ${PROJECT_SOURCE_DIR}/Containers/TC_Allocator.h
    ${PROJECT_SOURCE_DIR}/Containers/TC_Allocator.c
    ${PROJECT_SOURCE_DIR}/Containers/TContainer.h
    ${PROJECT_SOURCE_DIR}/Containers/TContainer.c
    ${PROJECT_SOURCE_DIR}/Containers/TArray.h
    ${PROJECT_SOURCE_DIR}/Containers/TArray.c
    ${PROJECT_SOURCE_DIR}/Containers/TTuple.h
    ${PROJECT_SOURCE_DIR}/Containers/TTuple.c
    ${PROJECT_SOURCE_DIR}/Containers/TVector.h
    ${PROJECT_SOURCE_DIR}/Containers/TVector.c
    ${PROJECT_SOURCE_DIR}/Containers/TDoubly_Linked_List.h
    ${PROJECT_SOURCE_DIR}/Containers/TDoubly_Linked_List.c
    ${PROJECT_SOURCE_DIR}/Containers/TLinked_List.h
    ${PROJECT_SOURCE_DIR}/Containers/TLinked_List.c
    ${PROJECT_SOURCE_DIR}/Containers/TMap.h
    ${PROJECT_SOURCE_DIR}/Containers/TMap.c
    ${PROJECT_SOURCE_DIR}/VMemory.h
    ${PROJECT_SOURCE_DIR}/VMemory.c
)

target_compile_options(${PROJECT_NAME} PRIVATE -fms-extensions)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 23)
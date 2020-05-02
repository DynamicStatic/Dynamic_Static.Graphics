
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#include "dynamic_static/core/string.hpp"
#include "dynamic_static/cpp-generator.hpp"
#include "dynamic_static/vk-xml-parser.hpp"
#include "utilities.hpp"

#include <array>

namespace dst {
namespace vk {
namespace cppgen {

/**
TODO : Documentation
*/
class EquailtyOperatorsGenerator final
{
public:
    /**
    TODO : Documentation
    */
    inline EquailtyOperatorsGenerator(const xml::Manifest& xmlManifest)
    {
        using namespace dst::cppgen;
        CppFile headerFile(std::filesystem::path(DYNAMIC_STATIC_GRAPHICS_VULKAN_GENERATED_INCLUDE_PATH) / "equality-operators.hpp");
        headerFile << CppInclude { CppInclude::Type::Internal, "dynamic_static/graphics/vulkan/defines.hpp" };
        headerFile << std::endl;
        CppFile sourceFile(std::filesystem::path(DYNAMIC_STATIC_GRAPHICS_VULKAN_GENERATED_SOURCE_PATH) / "equality-operators.cpp");
        sourceFile << CppInclude { CppInclude::Type::Internal, "dynamic_static/graphics/vulkan/generated/equality-operators.hpp" };
        sourceFile << CppInclude { CppInclude::Type::Internal, "dynamic_static/graphics/vulkan/detail/comparison-operators-utilities.hpp" };
        sourceFile << CppInclude { CppInclude::Type::Internal, "dynamic_static/graphics/vulkan/generated/structure-to-tuple.hpp" };
        sourceFile << std::endl;
        CppFunction::Collection equalityOperatorFunctions;
        for (const auto& structureitr : xmlManifest.structures) {
            const auto& structure = structureitr.second;
            if (structure.alias.empty()) {
                CppParameter lhsParameter;
                lhsParameter.type = "const " + structure.name + "&";
                lhsParameter.name = "lhs";
                CppParameter rhsParameter;
                rhsParameter.type = "const " + structure.name + "&";
                rhsParameter.name = "rhs";
                CppFunction equalityOperatorFunction;
                equalityOperatorFunction.cppCompileGuards = { structure.compileGuard };
                equalityOperatorFunction.cppReturn = "bool";
                equalityOperatorFunction.name = "operator==";
                equalityOperatorFunction.cppParameters = { lhsParameter, rhsParameter };
                equalityOperatorFunction.cppSourceBlock.add_snippet(R"(
                    using namespace dst::gfx::vk::detail;
                    return structure_to_tuple(lhs) == structure_to_tuple(rhs);
                )");
                CppFunction inequalityOperatorFunction;
                inequalityOperatorFunction.cppCompileGuards = { structure.compileGuard };
                inequalityOperatorFunction.cppReturn = "bool";
                inequalityOperatorFunction.name = "operator!=";
                inequalityOperatorFunction.cppParameters = { lhsParameter, rhsParameter };
                inequalityOperatorFunction.cppSourceBlock.add_snippet(R"(
                    return !(lhs == rhs);
                )");
                equalityOperatorFunctions.push_back(equalityOperatorFunction);
                equalityOperatorFunctions.push_back(inequalityOperatorFunction);
            }
        }
        headerFile << equalityOperatorFunctions.generate_declaration();
        sourceFile << equalityOperatorFunctions.generate_definition();
    }

private:
    inline dst::cppgen::CppFunction create_pnext_handler_function(const vk::xml::Manifest& vkXmlManifest)
    {
        using namespace dst::cppgen;
        CppFunction createPNextCopyFunction;
        createPNextCopyFunction.cppReturn = "void*";
        createPNextCopyFunction.name = "create_pnext_copy";
        CppParameter pNextParameter;
        pNextParameter.type = "const void*";
        pNextParameter.name = "pNext";
        CppParameter pAllocationCallbacksParameter;
        pAllocationCallbacksParameter.type = "const VkAllocationCallbacks*";
        pAllocationCallbacksParameter.name = "pAllocationCallbacks";
        createPNextCopyFunction.cppParameters = {
            pNextParameter,
            pAllocationCallbacksParameter,
        };
        createPNextCopyFunction.cppSourceBlock.add_snippet(R"(
            if (pNext) {
        )");
        CppSwitch vkStructureTypeSwitch;
        vkStructureTypeSwitch.cppCondition = "*(VkStructureType*)pNext";
        auto vkStructureTypeEnumerationItr = vkXmlManifest.enumerations.find("VkStructureType");
        if (vkStructureTypeEnumerationItr != vkXmlManifest.enumerations.end()) {
            const auto& vkStructureTypeEnumeration = vkStructureTypeEnumerationItr->second;
            for (const auto& vkStructureTypeEnumerator : vkStructureTypeEnumeration.enumerators) {
                if (vkStructureTypeEnumerator.alias.empty()) {
                    auto vkStructureTypeItr = vkXmlManifest.structureTypes.find(vkStructureTypeEnumerator.name);
                    if (vkStructureTypeItr != vkXmlManifest.structureTypes.end()) {
                        auto vkStructureItr = vkXmlManifest.structures.find(vkStructureTypeItr->second);
                        if (vkStructureItr != vkXmlManifest.structures.end()) {
                            CppSwitch::CppCase vkStructureTypeCase;
                            vkStructureTypeCase.cppCompileGuards = { vkStructureTypeEnumerator.compileGuard };
                            vkStructureTypeCase.name = vkStructureTypeEnumerator.name;
                            vkStructureTypeCase.cppSourceBlock.add_snippet(
                                R"(
                                    return create_dynamic_array_copy(1, (const ${VK_STRUCTURE_TYPE}*)pNext, pAllocationCallbacks);
                                )", {
                                    { "${VK_STRUCTURE_TYPE}", vkStructureItr->first },
                                }
                            );
                            vkStructureTypeSwitch.cppCases.push_back(vkStructureTypeCase);
                        }
                    }
                }
            }
        }
        createPNextCopyFunction.cppSourceBlock.add_snippet(Tab { 1 }, vkStructureTypeSwitch.generate_inline_definition());
        createPNextCopyFunction.cppSourceBlock.add_snippet(R"(
            };
            return nullptr;
        )");
        return createPNextCopyFunction;
    }
};

} // namespace cppgen
} // namespace vk
} // namespace dst

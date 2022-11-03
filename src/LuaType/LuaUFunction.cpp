#include <format>

#include <LuaType/LuaUFunction.hpp>
#include <Helpers/Integer.hpp>
#pragma warning(disable: 4005)
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>
#pragma warning(default: 4005)

namespace RC::LuaType
{
    auto DynamicUnrealFunctionOutParameters::add(const PropertyAndLuaRefPair param_ref_pair) -> void
    {
        m_params[m_last] = param_ref_pair;
        ++m_last;
    }

    auto DynamicUnrealFunctionOutParameters::get(size_t index) -> const PropertyAndLuaRefPair&
    {
        return m_params[index];
    }

    auto DynamicUnrealFunctionOutParameters::get() -> const DynamicUnrealFunctionOutParameters::ParamsArray&
    {
        return m_params;
    }

    UFunction::UFunction(Unreal::UObject* base, Unreal::UFunction* function) : UObjectBase<Unreal::UFunction, UFunctionName>(function), m_base(base) {}

    auto UFunction::construct(const LuaMadeSimple::Lua& lua, Unreal::UObject* owning_object, Unreal::UFunction* function) -> const LuaMadeSimple::Lua::Table
    {
        LuaType::UFunction lua_object{owning_object, function};

        LuaMadeSimple::Lua::Table table = LuaType::UObject::construct(lua, lua_object);

        setup_member_functions<LuaMadeSimple::Type::IsFinal::Yes>(table);

        setup_metamethods(lua_object);

        // Create object & surrender ownership to Lua
        lua.transfer_stack_object(std::move(lua_object), ClassName::ToString(), lua_object.get_metamethods());

        return table;
    }

    auto UFunction::construct(const LuaMadeSimple::Lua& lua, BaseObject& construct_to) -> const LuaMadeSimple::Lua::Table
    {
        LuaMadeSimple::Lua::Table table = UObject::construct(lua, construct_to);

        setup_member_functions<LuaMadeSimple::Type::IsFinal::No>(table);

        setup_metamethods(construct_to);

        return table;
    }

    auto UFunction::setup_metamethods(BaseObject& base_object) -> void
    {
        base_object.get_metamethods().create(LuaMadeSimple::Lua::MetaMethod::Call, [](const LuaMadeSimple::Lua& lua) -> int {
            return call_ufunction_from_lua(lua);
        });
    }

    template<LuaMadeSimple::Type::IsFinal is_final>
    auto UFunction::setup_member_functions(const LuaMadeSimple::Lua::Table& table) -> void
    {
        if constexpr (is_final == LuaMadeSimple::Type::IsFinal::Yes)
        {
            table.add_pair("type", [](const LuaMadeSimple::Lua& lua) -> int {
                lua.set_string(ClassName::ToString());
                return 1;
            });

            // If this is the final object then we also want to finalize creating the table
            // If not then it's the responsibility of the overriding object to call 'make_global()'
            table.make_global(ClassName::ToString());
        }
    }
}

// Copyright (c) 2022-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//
#pragma once

#include "../BindingsPythonUtils.h"
#include "IVariant.h"
#include "VariantUtils.h"

#include "../extras/EnvironmentVariable.h"

namespace carb
{
namespace variant
{

// PyObjectVTable for python variant types
struct PyObjectVTable
{
    static_assert(sizeof(py::object) == sizeof(void*), "Bad assumption");

    static void Destructor(VariantData* self) noexcept
    {
        try
        {
            py::object* p = reinterpret_cast<py::object*>(&self->data);
            py::gil_scoped_acquire gil;
            p->~object();
        }
        catch (...)
        {
        }
    }
    static VariantData Copy(const VariantData* self) noexcept
    {
        const py::object* p = reinterpret_cast<const py::object*>(&self->data);
        VariantData d{ self->vtable, nullptr };
        try
        {
            py::gil_scoped_acquire gil;
            new (&d.data) py::object(*p);
        }
        catch (...)
        {
        }
        return d;
    }
    static bool Equals(const VariantData* self, const VariantData* other) noexcept
    {
        if (self->vtable == other->vtable)
        {
            CARB_ASSERT(self->vtable == get());
            const py::object* pself = reinterpret_cast<const py::object*>(&self->data);
            const py::object* pother = reinterpret_cast<const py::object*>(&other->data);
            try
            {
                py::gil_scoped_acquire gil;
                return pself->is(*pother);
            }
            catch (...)
            {
                return false;
            }
        }

        // Try to convert us into the other type since it's not a python type
        VariantData temp;
        if (traits::convertTo(*self, other->vtable, temp))
        {
            bool b = traits::equals(temp, *other);
            traits::destruct(temp);
            return b;
        }

        return false;
    }
    static omni::string ToString(const VariantData* self) noexcept
    {
        const py::object* pself = reinterpret_cast<const py::object*>(&self->data);
        try
        {
            py::gil_scoped_acquire gil;
            auto str = pself->cast<std::string>();
            return omni::string(str.begin(), str.end());
        }
        catch (...)
        {
            return omni::string(omni::formatted, "py::object:%p", pself->ptr());
        }
    }
    template <class T>
    static bool Convert(const py::object& val, void*& out) noexcept
    {
        Translator<T> t{};
        try
        {
            out = t.data(val.cast<T>());
            return true;
        }
        catch (...)
        {
        }
        return false;
    }
    static bool ConvertTo(const VariantData* self, const VTable* newtype, VariantData* target) noexcept
    {
        const py::object* pself = reinterpret_cast<const py::object*>(&self->data);
        try
        {
            static std::unordered_map<RString, bool (*)(const py::object&, void*&)> converters{
                { eBool, Convert<bool> },
                { eUInt8, Convert<uint8_t> },
                { eUInt16, Convert<uint16_t> },
                { eUInt32, Convert<uint32_t> },
                { eUInt64, Convert<uint64_t> },
                { eInt8, Convert<int8_t> },
                { eInt16, Convert<int16_t> },
                { eInt32, Convert<int32_t> },
                { eInt64, Convert<int64_t> },
                { eFloat, Convert<float> },
                { eDouble, Convert<double> },
                { eString,
                  [](const py::object& val, void*& out) {
                      Translator<omni::string> t{};
                      try
                      {
                          py::gil_scoped_acquire gil;
                          auto str = val.cast<std::string>();
                          out = t.data(omni::string(str.begin(), str.end()));
                          return true;
                      }
                      catch (...)
                      {
                      }
                      return false;
                  } },
            };
            auto iter = converters.find(newtype->typeName);
            if (iter != converters.end() && iter->second(*pself, target->data))
            {
                auto iface = getCachedInterface<IVariant>();
                CARB_ASSERT(iface, "Failed to acquire interface IVariant");
                target->vtable = iface->getVTable(iter->first);
                return true;
            }
            return false;
        }
        catch (...)
        {
        }
        return false;
    }
    static size_t Hash(const VariantData* self) noexcept
    {
        const py::object* pself = reinterpret_cast<const py::object*>(&self->data);
        try
        {
            py::gil_scoped_acquire gil;
            return (size_t)py::hash(*pself);
        }
        catch (...)
        {
            return size_t(pself);
        }
    }
    static const VTable* get() noexcept
    {
        static const VTable v{
            sizeof(VTable), RString("py::object"), Destructor, Copy, Equals, ToString, ConvertTo, Hash
        };
        return &v;
    }
};

// Translator for python
template <>
struct Translator<py::object, void>
{
    static_assert(sizeof(py::object) == sizeof(void*), "Bad assumption");

    RString type() const noexcept
    {
        static const RString t("py::object");
        return t;
    }
    void* data(py::object o) const noexcept
    {
        void* d{};
        py::gil_scoped_acquire gil;
        new (&d) py::object(std::move(o));
        return d;
    }
    py::object value(void* d) const noexcept
    {
        py::object* p = reinterpret_cast<py::object*>(&d);
        py::gil_scoped_acquire gil;
        return *p;
    }
};

inline void definePythonModule(py::module& m)
{
    static bool typeRegistered = false;
    static LoadHookHandle loadHook = kInvalidLoadHook;

    // We need to register our PyObject variant type, so either do it immediately if IVariant is available, or defer
    // until IVariant is loaded.
    IVariant* v = getFramework()->tryAcquireExistingInterface<IVariant>();
    if (v)
    {
        v->registerType(PyObjectVTable::get());
        typeRegistered = true;

        // If we see `v` unload, we reset our typeRegistered flag
        getFramework()->addReleaseHook(v, [](void*, void*) { typeRegistered = false; }, nullptr);
    }
    else if (loadHook == kInvalidLoadHook)
    {
        // IVariant isn't available yet, so we'll register our type if/when it loads in the future
        loadHook = getFramework()->addLoadHook<IVariant>(
            nullptr,
            [](const PluginDesc&, void*) {
                if (auto v = getFramework()->tryAcquireInterface<IVariant>())
                {
                    if (!typeRegistered)
                    {
                        v->registerType(PyObjectVTable::get());
                        typeRegistered = true;
                        getFramework()->removeLoadHook(std::exchange(loadHook, kInvalidLoadHook));
                        // If we see `v` unload, we reset our typeRegistered flag
                        getFramework()->addReleaseHook(v, [](void*, void*) { typeRegistered = false; }, nullptr);
                    }
                }
            },
            nullptr);
    }

    defineInterfaceClass<IVariant>(m, "IVariant", "acquire_variant_interface");
}

} // namespace variant
} // namespace carb

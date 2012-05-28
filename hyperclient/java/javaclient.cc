// Copyright (c) 2012, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// STL
#include <map>
#include <string>

// e
#include <e/guard.h>

// HyperType's and HyperClient
#include "hyperclient/hyperclient.h"
#include "hyperclient/java/javaclient.h"

#include <iostream>
#include <sstream>

HyperType :: HyperType()
{
    std::cout << "HyperType Constructed" << std::endl;
}

HyperType :: ~HyperType() throw ()
{
    std::cout << "HyperType Destroyed" << std::endl;
}

hyperdatatype 
HyperType :: type() const
{
    return HYPERDATATYPE_GARBAGE;
}

void 
HyperType :: serialize(std::string& value) const
{
    std::cout << "HyperType Serialized" << std::endl;
}

HyperType* 
HyperType :: deserialize(const hyperclient_attribute& attr)
{
    if ( attr.datatype == HYPERDATATYPE_STRING )
    {
        return new HyperString(attr.value, attr.value_sz);
    }
    else if ( attr.datatype == HYPERDATATYPE_INT64 )
    {
        return new HyperInteger(le64toh(*reinterpret_cast<const int64_t*>(attr.value)));
    }

    std::string ex_str;
    ex_str += "Type '";
    ex_str += attr.datatype;
    ex_str += "' not supported.";

    throw ex_str;
}

std::string
HyperType :: toString()
{
    return "";
}

HyperString :: HyperString(const std::string& attr_value)
                                   : m_attr_value(attr_value)
{
    std::cout << "HyperString Constructed" << std::endl;
}

HyperString :: HyperString(const char *s, size_t n)
                                   : m_attr_value(s,n)
{
    std::cout << "HyperString Constructed" << std::endl;
}

HyperString :: ~HyperString() throw ()
{
    std::cout << "HyperString Destroyed" << std::endl;
}

hyperdatatype 
HyperString :: type() const
{
    return HYPERDATATYPE_STRING;
}

void
HyperString :: serialize(std::string& value) const
{
    std::cout << "HyperString Serialized" << std::endl;
    value.assign(std::string(m_attr_value));
}

size_t
HyperString :: read(char *bytes, int len, size_t pos)
{
    size_t size = m_attr_value.size();
    size_t available = size-pos;

    if ( len == 0 ) return available;

    return m_attr_value.copy(bytes,len,pos);
}

std::string
HyperString :: toString()
{
    return m_attr_value;
}

HyperInteger :: HyperInteger(int64_t attr_value)
                                     : m_attr_value(attr_value)
{
   std::cout << "HyperInteger Constructed" << std::endl;
}

HyperInteger :: ~HyperInteger() throw ()
{
   std::cout << "HyperInteger Destroyed" << std::endl;
}

hyperdatatype
HyperInteger :: type() const
{
    return HYPERDATATYPE_INT64;
}

void
HyperInteger :: serialize(std::string& value) const
{
    std::cout << "HyperInteger Serialized" << std::endl;
    value.assign(
        reinterpret_cast<const char *>(&htole64(m_attr_value)),
        sizeof(int64_t));
}

std::string
HyperInteger :: toString()
{
    std::stringstream ss;
    ss << m_attr_value;
    return ss.str();
}

HyperMap :: HyperMap()
{
    m_type = HYPERDATATYPE_GARBAGE;
    std::cout << "HyperMap Constructed" << std::endl;
}

HyperMap :: HyperMap(const HyperMap& hypermap)
                                   : m_map(hypermap.m_map)
{
    m_type = hypermap.type();
    std::cout << "HyperMap Copy Constructed" << std::endl;
}

HyperMap :: ~HyperMap() throw ()
{
    std::cout << "HyperMap Destroyed" << std::endl;
}

hyperdatatype 
HyperMap :: type() const
{
    return m_type;
}

void
HyperMap :: serialize(std::string& value) const
{
    /*
    std::cout << "HyperMap Serialized" << std::endl;
    value.assign(std::string(m_attr_value));
    */
}

std::string
HyperMap :: toString()
{
    std::stringstream ss;
    ss << "HyperMap@" << this;
    return ss.str();
}

size_t
HyperMap :: size() const
{
    return m_map.size();
}

bool
HyperMap :: empty() const
{
    return m_map.empty();
}

void
HyperMap :: destr_del(const std::string& key) throw (std::out_of_range)
{
    std::cout << "destr_del was called!" << std::endl;
    std::map<std::string,HyperType*>::iterator i = m_map.find(key);
    if (i != m_map.end())
    {
        delete m_map[key];
        m_map.erase(i);
    }
    else
        throw std::out_of_range("key not found");
}

void
HyperMap :: destr_clear() throw()
{
    std::cout << "destr_clear was called!" << std::endl;
    for (std::map<std::string,HyperType*>::iterator i = m_map.begin();
            i != m_map.end(); i++)
    {
        delete (*i).second;
        m_map.erase(i);
    }
}

void
HyperMap :: clear()
{
    destr_clear();
}

void
HyperMap :: set(const std::string& key, HyperType* value)
{
    insert(key,value);
}

void
HyperMap :: del(const std::string& key) throw (std::out_of_range)
{
    destr_del(key);
}

const HyperType*
HyperMap :: get(const std::string& key) throw (std::out_of_range)
{
    return m_map[key];
}

bool
HyperMap :: has_key(const std::string& key)
{
    std::map<std::string,HyperType*>::iterator i = m_map.find(key);
    return i != m_map.end();
}

void
HyperMap :: insert(const std::string& key, HyperType* value)
{
    m_map.insert(std::make_pair(std::string(key),value));
}

std::map<std::string,HyperType*>::const_iterator
HyperMap :: begin() const
{
    return m_map.begin();
}

std::map<std::string,HyperType*>::const_iterator
HyperMap :: end() const
{
    return m_map.end();
}

HyperVector :: HyperVector()
{
    m_type = HYPERDATATYPE_GARBAGE;
    std::cout << "HyperVector Constructed" << std::endl;
}

HyperVector :: HyperVector(const HyperVector& hypervector)
                                   : m_vector(hypervector.m_vector)
{
    m_type = hypervector.type();
    std::cout << "HyperVector Copy Constructed" << std::endl;
}

HyperVector :: ~HyperVector() throw ()
{
    std::cout << "HyperVector Destroyed" << std::endl;
}

hyperdatatype 
HyperVector :: type() const
{
    return m_type;
}

void
HyperVector :: serialize(std::string& value) const
{
    /*
    std::cout << "HyperVector Serialized" << std::endl;
    value.assign(std::string(m_attr_value));
    */
}

std::string
HyperVector :: toString()
{
    std::stringstream ss;
    ss << "HyperVector@" << this;
    return ss.str();
}

size_t
HyperVector :: size() const
{
    return m_vector.size();
}

bool
HyperVector :: empty() const
{
    return m_vector.empty();
}

void
HyperVector :: destr_clear() throw()
{
    std::cout << "destr_clear was called!" << std::endl;

    for (std::vector<HyperType*>::iterator i = m_vector.begin();
            i != m_vector.end(); i++)
    {
        delete *i;
        m_vector.erase(i);
    }
}

void
HyperVector :: clear()
{
    destr_clear();
}

void
HyperVector :: put(HyperType* value)
{
    m_vector.push_back(value);
}

HyperType*
HyperVector :: back()
{
    return m_vector.back();
}

const HyperType*
HyperVector :: get(size_t i) throw (std::out_of_range)
{
    return m_vector[i];
}

HyperClient :: HyperClient(const char* coordinator, in_port_t port)
    : m_client(coordinator, port)
{
}

HyperClient :: ~HyperClient() throw ()
{
}

hyperclient_returncode
HyperClient :: get(const std::string& space,
                   const std::string& key,
                   HyperMap *values)
{
    int64_t id;
    hyperclient_returncode stat1 = HYPERCLIENT_A;
    hyperclient_returncode stat2 = HYPERCLIENT_B;
    hyperclient_attribute* attrs = NULL;
    size_t attrs_sz = 0;

    id = m_client.get(space.c_str(),
                      key.data(),
                      key.size(),
                      &stat1,
                      &attrs,
                      &attrs_sz);

    if (id < 0)
    {
        assert(static_cast<unsigned>(stat1) >= 8448);
        assert(static_cast<unsigned>(stat1) < 8576);
        return stat1;
    }

    int64_t lid = m_client.loop(-1, &stat2);

    if (lid < 0)
    {
        assert(static_cast<unsigned>(stat2) >= 8448);
        assert(static_cast<unsigned>(stat2) < 8576);
        return stat2;
    }

    assert(id == lid);
    e::guard g = e::makeguard(free, attrs); g.use_variable();

    for (size_t i = 0; i < attrs_sz; ++i)
    {
        /*
        values->insert(std::make_pair(std::string(attrs[i].attr),
                                      HyperType::deserialize(attrs[i])));
        */

        values->insert(attrs[i].attr,HyperType::deserialize(attrs[i]));
    }

    assert(static_cast<unsigned>(stat1) >= 8448);
    assert(static_cast<unsigned>(stat1) < 8576);
    return stat1;
}

hyperclient_returncode
HyperClient :: put(const std::string& space,
                   const std::string& key,
                   const HyperMap& attributes)
{
    int64_t id;
    hyperclient_returncode stat1 = HYPERCLIENT_A;
    hyperclient_returncode stat2 = HYPERCLIENT_B;
    std::vector<hyperclient_attribute> attrs;
    std::vector<std::string> values;
    values.reserve(attributes.size());

    for (std::map<std::string, HyperType*>::const_iterator ci = attributes.begin();
            ci != attributes.end(); ++ci)
    {
        hyperclient_attribute at;
        at.attr = ci->first.c_str();
        std::string value;
        ci->second->serialize(value);
        values.push_back(value);
        at.value = values.back().data();
        at.value_sz = value.size();
        at.datatype = ci->second->type();
        attrs.push_back(at);
        std::cout << "value type = " << ci->second->type() << std::endl;
        std::cout << "value = " << value << std::endl;
        std::cout << "value size = " << value.size() << std::endl;
    }

    id = m_client.put(space.c_str(),
                      key.data(),
                      key.size(),
                      &attrs.front(),
                      attrs.size(),
                      &stat1);

    if (id < 0)
    {
        assert(static_cast<unsigned>(stat1) >= 8448);
        assert(static_cast<unsigned>(stat1) < 8576);
        return stat1;
    }

    int64_t lid = m_client.loop(-1, &stat2);

    if (lid < 0)
    {
        assert(static_cast<unsigned>(stat2) >= 8448);
        assert(static_cast<unsigned>(stat2) < 8576);
        return stat2;
    }

    assert(lid == id);
    assert(static_cast<unsigned>(stat1) >= 8448);
    assert(static_cast<unsigned>(stat1) < 8576);
    return stat1;
}

hyperclient_returncode
HyperClient :: del(const std::string& space,
                   const std::string& key)
{
    int64_t id;
    hyperclient_returncode stat1 = HYPERCLIENT_A;
    hyperclient_returncode stat2 = HYPERCLIENT_B;

    id = m_client.del(space.c_str(), key.data(), key.size(), &stat1);

    if (id < 0)
    {
        assert(static_cast<unsigned>(stat1) >= 8448);
        assert(static_cast<unsigned>(stat1) < 8576);
        return stat1;
    }

    int64_t lid = m_client.loop(-1, &stat2);

    if (lid < 0)
    {
        assert(static_cast<unsigned>(stat2) >= 8448);
        assert(static_cast<unsigned>(stat2) < 8576);
        return stat2;
    }

    assert(lid == id);
    assert(static_cast<unsigned>(stat1) >= 8448);
    assert(static_cast<unsigned>(stat1) < 8576);
    return stat1;
}

hyperclient_returncode
HyperClient :: range_search(const std::string& space,
                            const std::string& attr,
                            int64_t lower,
                            int64_t upper,
                            HyperVector *results)
{
    hyperclient_range_query rn;
    rn.attr = attr.c_str();
    rn.lower = lower;
    rn.upper = upper;

    int64_t id;
    hyperclient_returncode status = HYPERCLIENT_A;
    hyperclient_attribute* attrs = NULL;
    size_t attrs_sz = 0;

    id = m_client.search(space.c_str(), NULL, 0, &rn, 1, &status, &attrs, &attrs_sz);

    if (id < 0)
    {
        return status;
    }

    int64_t lid;
    hyperclient_returncode lstatus = HYPERCLIENT_B;

    while ((lid = m_client.loop(-1, &lstatus)) == id)
    {
        if (lstatus == HYPERCLIENT_SEARCHDONE)
        {
            return HYPERCLIENT_SUCCESS;
        }

        if (status != HYPERCLIENT_SUCCESS)
        {
            break;
        }

        std::cout << "push_back start" << std::endl;
        results->put(new HyperMap());
        std::cout << "push_back end" << std::endl;

        for (size_t i = 0; i < attrs_sz; ++i)
        {
            ((HyperMap*)(results->back()))->insert(attrs[i].attr,HyperType::deserialize(attrs[i]));
        }

        if (attrs)
        {
            hyperclient_destroy_attrs(attrs, attrs_sz);
        }

        attrs = NULL;
        attrs_sz = 0;
    }

    if (lid < 0)
    {
        assert(static_cast<unsigned>(lstatus) >= 8448);
        assert(static_cast<unsigned>(lstatus) < 8576);
        return lstatus;
    }

    assert(lid == id);
    assert(static_cast<unsigned>(status) >= 8448);
    assert(static_cast<unsigned>(status) < 8576);
    return status;
}
